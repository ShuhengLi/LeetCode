# SoFi System Design #1 — Real-Time Cloud Billing System (AWS-style)

> **真题来源**：Glassdoor / Blind 多份 Senior Engineer onsite 报告（2024-2025）
> **原题**：*"You own a competitor to AWS and want to accurately show customers their current running bill. Customers incur charges when their virtual machines are ON. Design the system to show real-time billing amounts."*
> **时间**：45-60 分钟

---

## 1. 需求澄清（前 5 分钟，主动问）

### Functional Requirements
- 用户可以查看 **当前实时账单**（running bill，精确到分钟级）
- 用户可以查看 **历史账单**（按月/天/小时聚合）
- 系统支持多种产品计费：
  - VM（按运行时长 + 实例规格计费）
  - Storage（按 GB-月）
  - Network egress（按 GB）
- 计费精度要求：分钟级，最终结账精度到小数点后 6 位

### Non-Functional Requirements
- **规模**：1000万用户，每用户 100 个 VM 实例 → 10亿活跃资源
- **吞吐**：每秒 100K 计费事件（VM 启停、读写存储等）
- **延迟**：用户查询账单 P99 < 200ms；账单刷新延迟 < 1 分钟
- **一致性**：账单金额是钱，**强最终一致**（eventually consistent 但保证不丢不重）
- **可用性**：99.99%（账单查询）
- **审计**：所有计费事件可追溯（compliance 要求）

### 关键澄清问题
> "What's the freshness requirement for the displayed bill?"
> → 1分钟级延迟可接受？（影响是否需要纯实时流处理）

> "Do customers ever pre-pay or have credits?"
> → 涉及账户余额扣减还是单纯展示？

> "What's the SLA for billing accuracy?"
> → fintech 通常是 0.001% 错误率以内

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| 用户数 | 10M |
| 活跃 VM | 1B（10M × 100） |
| 计费事件/秒 | 100K |
| 每事件大小 | 200 bytes |
| 每秒数据 | 20 MB/s ≈ 1.7 TB/天 |
| 历史保留 | 7年（compliance）→ ~4 PB |
| 查询 QPS | 50K（10M DAU / 200s 平均访问间隔） |

---

## 3. High-Level Architecture

```
┌─────────────┐     ┌──────────────┐     ┌─────────────────┐
│   Clients   │────▶│ API Gateway  │────▶│ Billing Service │
│ (Web/Mobile)│     │ (Auth, RL)   │     │   (Read API)    │
└─────────────┘     └──────────────┘     └────────┬────────┘
                                                   │
                          ┌────────────────────────┴───────────┐
                          ▼                                     ▼
                   ┌──────────────┐                    ┌────────────────┐
                   │ Redis Cache  │                    │ Aggregated DB  │
                   │ (running bill)│                    │ (PostgreSQL)   │
                   └──────────────┘                    └────────────────┘
                          ▲                                     ▲
                          │                                     │
                   ┌──────┴─────────────────────────────────────┘
                   │
            ┌──────────────┐
            │ Aggregator   │ (Flink/Kafka Streams, 每分钟 flush)
            │ (Stream Job) │
            └──────┬───────┘
                   ▲
                   │
            ┌──────────────┐     ┌────────────────────────┐
            │    Kafka     │◀────│ Metering Collectors    │
            │ (events)     │     │ (sidecars on each VM)  │
            └──────┬───────┘     └────────────────────────┘
                   │
                   ▼
            ┌──────────────┐
            │  S3 / Cold   │  (raw events, 7yr retention, audit)
            │   Storage    │
            └──────────────┘
```

---

## 4. 核心组件详解

### 4.1 Metering Collectors（事件采集层）
- 每个 VM/Storage/Network 服务上部署 sidecar
- 周期性发送计费事件到 Kafka
- 事件格式：
  ```json
  {
    "event_id": "uuid",          // 幂等性 key
    "user_id": "u_123",
    "resource_id": "vm_abc",
    "resource_type": "vm.t3.medium",
    "metric": "instance_hour",
    "value": 60,                 // 60秒
    "timestamp": 1714000000,
    "region": "us-west-2"
  }
  ```

**关键设计**：
- `event_id` 是幂等性主键，防止重复计费
- 事件本地落盘 + 异步发送（避免网络中断丢数据）

### 4.2 Kafka（消息总线）
- Topic: `billing-events`
- Partition by `user_id`（保证同一用户事件有序）
- Replication factor = 3
- Retention: 7 天（足够 reprocessing）

**为什么 Kafka 而不是 SQS/Kinesis？**
- 高吞吐（百万 msg/s）
- 持久化 + 可重放（监管审计 + 故障恢复）
- 分区有序保证

### 4.3 Aggregator（流处理层）
- **技术选型**：Apache Flink（exactly-once 语义）或 Kafka Streams
- 每分钟一个窗口（tumbling window）
- 输出两种数据：
  1. 写入 Redis：`bill:{user_id}` → 当前小时累计金额（用于实时展示）
  2. 写入 PostgreSQL：每分钟聚合（用于历史查询）

**计费公式**：
```
hourly_cost = instance_hours × price_per_hour
```
价格表用 Redis 缓存，定时从 Pricing Service 拉取。

### 4.4 Storage 分层

| 数据 | 存储 | TTL | 用途 |
|------|------|-----|------|
| 实时账单 | Redis | 24h | 用户 dashboard 查询 |
| 分钟聚合 | PostgreSQL（按月分表） | 90天 | 当月账单详情 |
| 小时/日聚合 | PostgreSQL | 7年 | 历史报表 |
| Raw events | S3 (Parquet) | 7年 | 审计/重算 |

**为什么 PostgreSQL 而不是 NoSQL？**
- 计费需要 ACID（防止重复扣款）
- 复杂查询（按时间范围 + 资源类型聚合）
- 用 Citus 或读写分离 + 分库分表撑住规模

### 4.5 Billing Service（读 API）
```
GET /v1/bill/current?user_id=u_123
  → 1) Redis 取实时金额
    2) miss 则 query Aggregated DB

GET /v1/bill/historical?user_id=u_123&from=2026-03-01&to=2026-03-31
  → query PostgreSQL aggregated table
```

---

## 5. 关键问题与权衡

### Q1: 如何保证 exactly-once（不重不丢）？
**答案：双重保证**
1. **生产端幂等**：Kafka producer enable.idempotence=true
2. **消费端幂等**：用 `event_id` 在 Aggregator 内做 dedup（窗口内 RocksDB 状态）
3. **Flink checkpoint**：失败时从 checkpoint 恢复，保证 state + offset 原子性

### Q2: VM 启停事件丢失怎么办？
**答案**：
- VM agent 本地 WAL（write-ahead log）
- 定期心跳（每分钟），server 端发现心跳缺失则**回填**最后已知状态
- 对账机制：每天用 raw events 重算一次，与展示账单对账

### Q3: 价格变动如何处理？
- 价格表带 `effective_at` 字段
- Aggregator 根据事件时间戳查找当时有效的价格
- **不能**用当前价格回算历史（会让用户账单变化，违规）

### Q4: 如何防止用户看到错误账单？
- Redis 写入采用 **Lua 脚本**保证 read-modify-write 原子
- 双写不一致检测：Redis 与 DB 不一致时，以 DB 为准（后台异步 reconcile）
- 兜底：账单展示页加 "Updated X minutes ago" 提示

### Q5: 100K events/s 的 PostgreSQL 怎么扛？
**多层缓冲 + 批量写入**：
1. Aggregator 在窗口内做 pre-aggregation，每分钟仅写 `unique users × resource_types` 行
2. 假设 1M 活跃用户每分钟产生 5M 行 → 80K rows/s（可接受）
3. 按 user_id 分库（128 个分片）
4. 用 `INSERT ... ON CONFLICT UPDATE` 做 upsert

### Q6: 如何 scale read（用户查账单）？
- Redis 集群（user_id 分片）
- PostgreSQL read replica
- CDN 缓存静态历史账单 PDF

---

## 6. 容错与恢复

| 故障 | 影响 | 恢复策略 |
|------|------|---------|
| Aggregator 宕机 | 账单延迟更新 | 从 Kafka offset 续跑（exactly-once） |
| Redis 全挂 | 实时账单不可见 | Fallback 到 DB 查询（降级，慢但可用） |
| Kafka 宕机 | 事件积压 | Collector 本地缓存 1 小时，恢复后批量上传 |
| Postgres primary 挂 | 写入失败 | 自动 failover 到 replica + 重试队列 |
| 计费逻辑 bug | 错账 | Raw events 在 S3，可任意回放重算 |

---

## 7. 监控与告警

### 关键指标
- **Lag**：Kafka consumer lag > 1 分钟告警
- **Reconciliation**：每天对账，差异 > 0.01% 告警
- **Cost SLA**：每事件计费延迟 P99 < 60s
- **Error budget**：账单读 API 错误率 > 0.01% 触发降级

### Dashboards
- Per-region 计费事件流量
- DB 写入延迟分布
- Top heavy users（潜在 abuse）

---

## 8. 进阶讨论（Senior+ 加分）

### 多地域 / 全球部署
- 每个 region 独立计费 pipeline（数据本地化合规）
- Cross-region 全局账单：用户主 region 拉所有 region 的子账单聚合
- Currency conversion：中央 FX rate service 缓存到各 region

### 实时降级到批处理
- 流处理出问题时切到 Spark batch（每小时跑）
- 用户看到的是滞后 1 小时的账单，但仍正确

### Cost optimization 自身
- Hot data 在 Redis（贵），cold data 落 S3（便宜）
- Parquet 列式存储 + ZSTD 压缩，存储成本降 80%

### 与 SoFi 业务关联
SoFi 自身做利息/费用计算时，思路完全相同：
- 每日利息 = principal × rate / 365
- 用户账户余额是"running bill"，事件流是借/贷/利息
- Compliance 要求"任意时刻可重算" = raw event log

---

## 9. 设计取舍总结

| 取舍 | 选择 | 原因 |
|------|------|------|
| Strong vs Eventual consistency | Eventual | 1分钟延迟可接受，强一致代价太高 |
| SQL vs NoSQL | PostgreSQL | ACID + 复杂聚合查询 |
| Push vs Pull metering | Push (agent) | VM 数量太多，pull 不可扩展 |
| Stream vs Batch | Stream（Flink） | 实时账单需求 |
| 实时计算 vs 预聚合 | 预聚合 | 用户查询 QPS 高，实时算不起 |

---

## 10. 5分钟版本（白板上画的核心）

```
Agent → Kafka → Flink (1-min window) → Redis (current) + Postgres (history)
                                            ↓
                                       Billing API
                                            ↓
                                          User
```

记住三个关键词反复说：**幂等**（event_id）、**最终一致**（1-min window）、**可重算**（raw events in S3）。
