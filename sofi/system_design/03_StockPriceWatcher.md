# SoFi System Design #3 — Stock Price Watcher / Alert App

> **真题来源**：Glassdoor Senior Engineer onsite 报告（2024-2025）
> **原题**：*"Design a price watcher app for stock prices"*
> **业务关联**：SoFi Invest 产品线
> **时间**：45-60 分钟

---

## 1. 需求澄清

### Functional Requirements
- 用户可以**订阅多支股票**的价格更新（real-time）
- 用户可以设置**价格告警**（>=, <=, % change）
- 触发告警时通过 **push notification / SMS / email** 通知
- 用户查看**实时价格** + **历史 K 线**（1m/5m/1h/1d）
- 支持自选股 watchlist（最多 50 支）

### Non-Functional Requirements
- **规模**：5M 用户，每人订阅 20 支股票 → 100M subscriptions
- **股票数**：~10K 美股
- **价格更新频率**：每股票每秒 1-10 次更新（peak）
- **延迟**：从交易所到用户屏幕 P99 < 500ms
- **告警延迟**：触发条件命中后 < 1s 推送
- **可用性**：99.9%（市场开盘时段优先）

### 关键澄清
> "Real-time = how real?" → 1秒级满足散户需求；HFT 不在范围

> "What's the source of truth for prices?" → 第三方 market data provider (Polygon, IEX, Bloomberg)

> "Do we charge for the data?" → Tier system: free（15min delayed） / premium（real-time）

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| 用户数 | 5M, DAU = 1M |
| 股票数 | 10K |
| 总价格更新/秒 | 50K (10K 股票 × 5 updates/s avg) |
| 总订阅数 | 100M |
| 推送 fan-out 峰值 | 10K stocks × avg 100K subs each ≈ very high |
| 告警 evaluation/秒 | 50K updates × ~10 alerts hit/update = 500K/s |

---

## 3. High-Level Architecture

```
                    ┌─────────────────────────┐
                    │ Market Data Providers   │
                    │ (Polygon, IEX, NASDAQ)  │
                    └────────────┬────────────┘
                                 │ WebSocket / FIX
                                 ▼
                    ┌─────────────────────────┐
                    │ Price Ingestion Layer   │
                    │ (normalizer + dedup)    │
                    └────────────┬────────────┘
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │      Kafka              │
                    │ topics: prices.{ticker} │
                    └──┬──────┬──────────┬────┘
                       │      │          │
                       ▼      ▼          ▼
              ┌────────┐  ┌────────┐  ┌──────────┐
              │ Price  │  │ Alert  │  │ Time-    │
              │ Cache  │  │ Engine │  │ series   │
              │(Redis) │  │ (Flink)│  │ DB       │
              └───┬────┘  └───┬────┘  │(Influx)  │
                  │           │        └──────────┘
                  │           ▼
                  │    ┌────────────┐
                  │    │Notification│
                  │    │  Service   │
                  │    └─────┬──────┘
                  │          │
                  ▼          ▼
        ┌─────────────────────────┐
        │  Subscription Gateway   │ (WebSocket fan-out)
        │   (Sticky session)      │
        └────────────┬────────────┘
                     │
                     ▼
                  ┌──────┐
                  │ Users│
                  └──────┘
```

---

## 4. 核心组件详解

### 4.1 Price Ingestion Layer
**职责**：
- 维持与 market data 提供商的 WebSocket / FIX 连接
- 反序列化 + normalize（统一 schema）
- Dedup（同一股票同一时刻多源数据，取 newest）
- 投递到 Kafka

**关键设计**：
- 多 provider 冗余（IEX 主，Polygon 备）
- 心跳监控，超时切换
- Symbol 映射表（不同 provider 可能用 different ticker）

### 4.2 Kafka 设计
- Topic: `prices.{exchange}` 或 hash by ticker
- Partition by `ticker`（同股票同顺序）
- Retention：1 小时（流处理够了，长期数据走 TSDB）
- Compaction：可选，保留每个 ticker 最新值

**为什么 Kafka 而不是 Redis Pub/Sub？**
- Persistence + replay
- 横向扩展（百万 msg/s）
- 多消费者独立 offset

### 4.3 Price Cache (Redis)
- Key: `price:{ticker}` → JSON `{price, ts, change_pct, volume}`
- TTL: 24h（市场闭市后清理）
- 查询：用户打开 app → 一次性 mget watchlist
- 写入：Kafka consumer 实时更新

### 4.4 Alert Engine（核心难点）
**问题规模**：
- 50K price updates/s
- 每股可能有 10-100K alerts 订阅
- 朴素：每 update × 每 alert = 50K × 100K = 5B/s ❌

**优化思路**：按 ticker 索引 alerts
```
alert_index:
  AAPL → [
    {user: u1, type: ABOVE, threshold: 200},
    {user: u2, type: BELOW, threshold: 180},
    ...
  ]
```

每次价格更新：
1. 拿到 ticker 的 alert list（pre-loaded in memory）
2. 检查每个 alert 是否触发（O(N) where N = alerts on this ticker）
3. 触发的发送到 Notification Service

**进一步优化**：用 **Sorted Set** 按 threshold 排序
- ABOVE alerts 排序 ascending：当 price = X，触发所有 threshold ≤ X 的（log N + K）
- BELOW alerts 排序 descending：触发 threshold ≥ X 的

**实现**：Apache Flink stateful job
- KeyBy(ticker)
- 状态：当前 alerts 列表（RocksDB backed）
- 处理函数：对每个新 price 评估 + 触发

**Alert 状态管理**：
- 已触发 alert 是 one-shot 还是持续？通常 one-shot，触发后变 INACTIVE
- 用户可设置 "snooze 1 hour" 等

### 4.5 Subscription Gateway（实时推送给用户）
**WebSocket fan-out 是难点**：
- 单机 WebSocket 上限 ~100K connections
- 5M DAU 在线 → 50 台机器（保留 buffer 用 100 台）

**架构**：
```
User → WebSocket Gateway (sticky) → 订阅 Redis Pub/Sub channel
       Gateway 收到 push → forward to user's socket
```

**Subscription 路由**：
- 用户连接时上报 watchlist
- Gateway 订阅 Redis channel `price.{ticker}` 列表
- Kafka Price consumer 同时写入 Redis Pub/Sub
- 这样每条 price update 只发到关心它的 gateway

**优化 fan-out 风暴**：
- AAPL 价格更新一次 → 1M 用户订阅 → 1M push messages
- 解决：每用户每股票限频（client-side throttle）
  - 上行：每秒最多 1 次推送给同一 user
  - Backend 在 gateway 侧聚合（last-write-wins within window）

### 4.6 Time-series DB（历史 K 线）
**选型**：InfluxDB / TimescaleDB / ClickHouse
- Schema：(ticker, timestamp, open, high, low, close, volume)
- 预聚合：raw → 1m → 5m → 1h → 1d（continuous aggregation）
- 查询：用户打开图表 → 拉对应粒度数据
- 冷数据归档到 S3（Parquet）

### 4.7 Notification Service
- 多渠道：FCM (Android) / APNs (iOS) / SMS / Email
- 优先级队列：price alert 高优，营销低优
- 用户偏好：免打扰时段、quiet hours
- Idempotency：同 alert ID 短时间内不重复

---

## 5. 关键问题与权衡

### Q1: 如何 scale alert evaluation？
- 按 ticker 分片（Flink keyBy）
- 热门股（AAPL/TSLA）单独 partition，多个 task 处理
- 状态分散在多机，不集中

### Q2: 同一价格触发的多个用户告警如何并发推送？
- Notification Service 是无状态 + 横向扩展
- 用 Kafka topic `alerts.fired` 解耦
- 多个 worker 并行消费

### Q3: 如果用户 watchlist 有 50 支股票，每秒 5 次更新 → 250 msg/s 给一个 user
- WebSocket 推送压力：上行带宽 + client 渲染
- 解决：server 侧聚合，每 200ms 推一次（合并多 ticker 更新）

### Q4: 如何处理市场闭市？
- Ingestion 层检测 trading hours，闭市后停止 / 减少处理
- TSDB 不再写新数据
- Alert engine 仍跑（处理 after-hours 价格）

### Q5: 数据延迟 / 丢失怎么办？
- 监控 ingestion lag（Kafka end-to-end latency）
- 多 provider 冗余 + auto-failover
- UI 显示 "Last update X seconds ago"

### Q6: 如何确保不重复触发同一 alert？
- Alert state machine: ACTIVE → TRIGGERED → ACKED
- 触发后立即 mark TRIGGERED + push notification
- 用户 ack 后才回到 ACTIVE（如果是 recurring）

### Q7: 用户设置 alert "AAPL 涨 5%"，5% 是相对什么？
- 澄清：相对开盘价 / 前日收盘 / 用户购买价？
- 通常：前日收盘价（Yahoo Finance 标准）
- 实现：alert 存 baseline price + condition

---

## 6. 数据流总结

```
1. Market data → Ingestion → Kafka (prices.AAPL)
2. Kafka → Redis cache (point-in-time price)
3. Kafka → Flink Alert Engine → Kafka (alerts.fired) → Notification
4. Kafka → InfluxDB (history)
5. WebSocket Gateway → Redis Pub/Sub → User's socket
```

---

## 7. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Market data 断开 | 无新价格 | 切换 provider；UI 显示 stale 警告 |
| Kafka 挂 | 整个 pipeline 停 | Replication + multi-AZ；ingestion 本地缓冲 |
| Alert engine 挂 | 告警漏发 | Flink checkpoint 恢复，无丢失（exactly-once） |
| WebSocket Gateway 挂 | 用户断连 | Client auto-reconnect，session 通过 LB 路由到新 pod |
| Notification 慢 | 通知延迟 | 优先级队列保高优；fallback 到 SMS |

---

## 8. 进阶讨论

### Cold start: 用户打开 app
1. HTTP GET `/watchlist` → 服务返回 ticker list
2. HTTP GET `/prices?tickers=...` → Redis batch read
3. Open WebSocket → subscribe to channels
4. 后续 push only

### 历史数据回填
- 新上市股票：从 provider API 拉历史
- 离线 batch job 写入 TSDB

### Alert 执行多次问题
- 一支股票快速波动可能瞬间多次触发同一 alert
- 解决：触发后冷却期（cooldown，5 min 内不再触发同一 alert）

### Tiered service
- Free tier: 15-min delayed prices, 5 alerts max
- Premium: real-time, 100 alerts, advanced indicators

### 与 SoFi Invest 集成
- 触发 alert 后用户可一键下单（buy/sell）
- 跨服务调用 SoFi Invest 交易系统（见 #2 Payment）

---

## 9. 监控

- **Lag**：Market data → user device end-to-end < 1s P99
- **Alert miss rate**：触发条件命中但未发出（应为 0）
- **WebSocket connection count** per pod
- **Notification delivery rate** per channel

---

## 10. 5分钟版本

```
Market Data → Kafka → ┬→ Redis (current price) → User reads
                      ├→ Flink Alert Engine (per-ticker state) → Notification
                      ├→ InfluxDB (history) → Chart query
                      └→ WebSocket Gateway (Pub/Sub) → User push
```

**核心三点**：
- "Per-ticker partitioning" — 让每个 worker 只看自己关心的股票
- "Pre-indexed alerts" — 价格 update 时 O(log N) 找触发，不用扫全表
- "WebSocket sticky + Pub/Sub" — fan-out 给 5M 用户的关键
