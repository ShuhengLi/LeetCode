# SoFi System Design #2 — Payment Processing System

> **真题来源**：Programhelp / Glassdoor，多份 Senior Engineer 报告（2024-2025）
> **原题**：*"Design SoFi's payment processing system to support millions of transactions with high concurrency."*
> **时间**：45-60 分钟
> **核心考点**：money movement 的正确性、幂等、防双花、合规

---

## 1. 需求澄清

### Functional Requirements
- 用户从 SoFi 账户向外部账户**转账**（ACH / Wire / Internal）
- 支持**信用卡支付**、**ACH debit**、**P2P 转账**（如 Zelle 风格）
- 处理 **退款**（refund）和**对账**（reconciliation）
- 提供**幂等 API**（同一请求多次重试只扣一次）

### Non-Functional Requirements
- **吞吐**：5K TPS 峰值（黑五、发薪日）
- **延迟**：用户感知 P99 < 2s（实际外部清算可能 1-3 天）
- **可用性**：99.99%
- **一致性**：**强一致**（钱不能错），ACID required
- **正确性**：double-spend 必须为 0
- **合规**：PCI-DSS、SOC2、SOX、KYC/AML
- **审计**：所有交易 immutable 7+ 年

### 关键澄清问题
> "Sync or async response to user?"
> → 必须 async（外部清算慢），同步返回 "pending"，最终用 webhook/notification 告知

> "Internal vs external transfer?"
> → 走两条不同路径，内部可即时，外部走 ACH/Wire

> "What's our idempotency window?"
> → 通常 24 小时

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| 用户数 | 10M |
| 日交易数 | 50M |
| 峰值 TPS | 5K（10x 平均） |
| 每交易记录大小 | 2 KB（含元数据、合规字段） |
| 日数据量 | 100 GB |
| 7 年存储 | ~250 TB |

---

## 3. High-Level Architecture

```
                                        ┌──────────────────┐
                                        │   Fraud Service  │
                                        │  (real-time ML)  │
                                        └────────▲─────────┘
                                                 │
┌──────────┐   HTTPS    ┌──────────────┐   sync  │
│  Mobile/ │───────────▶│ API Gateway  │─────────┤
│   Web    │            │ (Auth, RL)   │         ▼
└──────────┘            └──────┬───────┘  ┌──────────────┐
                               │          │ Payment API  │
                               └─────────▶│  Service     │
                                          │ (Spring Boot)│
                                          └──┬─────────┬─┘
                              ┌───── async ──┘         │
                              ▼                        │ sync (idempotency
                       ┌──────────────┐                │  + balance check)
                       │ Kafka:       │                ▼
                       │ payments-cmd │         ┌──────────────┐
                       └──────┬───────┘         │  Postgres    │
                              │                 │ (Accounts +  │
                              ▼                 │ Transactions)│
                       ┌──────────────┐         │   ACID       │
                       │  Payment     │         └──────────────┘
                       │  Workers     │              ▲
                       │ (Saga Orch.) │              │
                       └──────┬───────┘              │
                              │                      │
                ┌─────────────┼──────────────┐       │
                ▼             ▼              ▼       │
         ┌──────────┐  ┌──────────┐  ┌──────────────┐│
         │ ACH/Wire │  │  Card    │  │  Ledger      ││
         │ Adapter  │  │  Adapter │  │   Service    │┘
         │  (Fed)   │  │ (Stripe) │  │  (double     │
         └──────────┘  └──────────┘  │   entry)     │
                                     └──────────────┘
```

---

## 4. 核心组件详解

### 4.1 Payment API Service（接收请求）
**职责**：
1. 校验请求合法性（schema、amount > 0、account 存在）
2. **幂等性检查**：`Idempotency-Key` header → Redis 查重
3. KYC/AML 状态校验（用户被冻结？）
4. 同步：余额检查 + 软扣款（balance hold）
5. 投递 Kafka，立即返回 `transaction_id` + `status=PENDING`

**幂等实现**：
```java
@PostMapping("/payments")
public PaymentResponse create(
    @RequestHeader("Idempotency-Key") String idempotencyKey,
    @RequestBody PaymentRequest req
) {
    // SETNX in Redis with 24h TTL
    if (!redis.setIfAbsent("idem:" + idempotencyKey, "processing", 24h)) {
        // 已存在，返回之前结果
        return getCachedResult(idempotencyKey);
    }
    // ... 处理新请求
}
```

### 4.2 Postgres（账户与交易）
**为什么 Postgres 而不是 NoSQL？**
- ACID 事务（钱必须强一致）
- Row-level locking 防双花

**Schema**：
```sql
-- 账户表
CREATE TABLE accounts (
  account_id BIGINT PRIMARY KEY,
  user_id BIGINT NOT NULL,
  balance DECIMAL(20,4) NOT NULL,
  hold_amount DECIMAL(20,4) NOT NULL DEFAULT 0,  -- 软扣款
  version BIGINT NOT NULL,                        -- 乐观锁
  status VARCHAR(20) NOT NULL                     -- ACTIVE/FROZEN
);

-- 交易表（append-only）
CREATE TABLE transactions (
  txn_id UUID PRIMARY KEY,
  idempotency_key VARCHAR(64) UNIQUE NOT NULL,
  from_account BIGINT NOT NULL,
  to_account BIGINT NOT NULL,
  amount DECIMAL(20,4) NOT NULL,
  currency CHAR(3) NOT NULL,
  status VARCHAR(20) NOT NULL,  -- PENDING/POSTED/FAILED/REVERSED
  created_at TIMESTAMP NOT NULL,
  -- ... 合规字段
);

CREATE INDEX idx_txn_user_time ON transactions (from_account, created_at DESC);
```

**双花防护（Pessimistic Locking）**：
```sql
BEGIN;
SELECT balance, hold_amount FROM accounts
  WHERE account_id = ? FOR UPDATE;  -- 行锁

-- check balance >= amount
UPDATE accounts SET hold_amount = hold_amount + ?
  WHERE account_id = ?;

INSERT INTO transactions (...) VALUES (...);
COMMIT;
```

### 4.3 Kafka（异步处理）
- Topic: `payments-cmd`（命令）, `payments-event`（事件）
- Partition by `from_account` 保证同账户串行
- **不要**只用 Kafka 持久化交易！Postgres 仍是 source of truth

### 4.4 Payment Workers（Saga 编排）
**Saga 模式**：长流程拆成多个本地事务 + 补偿。

例：External transfer
```
Step 1: Hold funds (Postgres)
Step 2: Submit to ACH Adapter (Fed network)
Step 3: Wait for ACH ack (异步, T+1)
Step 4a: Success → Post (release hold, debit, ledger)
Step 4b: Failure → Compensate (release hold, no debit, notify user)
```

**关键**：每步状态写 DB，崩溃后可从中断处恢复。

### 4.5 Ledger Service（复式记账）
**为什么要单独 Ledger？**
- 合规要求 immutable accounting trail
- 与 Account balance 解耦（Ledger 是事实，Balance 是缓存）

**Double-entry 原则**：每笔交易必须有 debit 和 credit，**总和为零**：
```
转账 $100 from A to B:
  +$100 to B (credit)
  -$100 from A (debit)
  Sum = 0 ✓
```

定期 reconciliation：
- Sum(all ledger entries for account) == accounts.balance？
- 不一致 → 告警 + 暂停账户

### 4.6 Fraud Service
- 实时 ML scoring（gRPC 调用，<100ms）
- 高风险交易 → block 或人工 review
- 低风险 → pass through
- **Failure mode**：fraud service 挂了，规则引擎兜底（block 大额、跨境）

### 4.7 External Adapters
- **ACH Adapter**：批量提交到 Fed (NACHA file format)，T+1 settlement
- **Wire Adapter**：FedWire / SWIFT，real-time but 高费率
- **Card Adapter**：Stripe / Adyen 收单
- 每个 adapter 独立部署，circuit breaker 隔离

---

## 5. 关键问题与权衡

### Q1: 如何保证 exactly-once 扣款？
**多重保证**：
1. **API 层幂等**：`Idempotency-Key` + Redis SETNX
2. **DB 层幂等**：transactions 表 idempotency_key UNIQUE 约束
3. **Worker 层幂等**：每步检查当前状态，已处理则跳过
4. **下游幂等**：调外部 ACH 时带 client reference id

### Q2: Distributed transaction 跨 service 怎么办？
**不用 2PC**（性能差、协调复杂），用 **Saga + 补偿事务**。
- 每个本地步骤是 ACID
- 补偿用反向操作（如 release hold）
- 缺点：中间态用户可能看到不一致 → UI 显示 "pending"

### Q3: 5K TPS 单 Postgres 撑得住吗？
- 单实例 Postgres 写入约 10K TPS（SSD），读 50K+
- 5K 完全可撑，但需要：
  - 主从分离（read 走 replica）
  - 按 user_id 分库（horizontal sharding，到 100K TPS）
  - Connection pool（PgBouncer）

### Q4: ACH 失败 3 天后才知道，怎么处理？
- 软扣款 hold 不能马上释放（保护用户余额）
- Status：`PENDING` → 等 ACH ack
- 收到 ack 才 `POSTED`，否则 `FAILED` + 释放 hold
- UI 显示用户**可用余额** = balance - hold

### Q5: 如何防止 race condition？
**例**：用户同时发两笔超额交易
- Postgres `SELECT FOR UPDATE` 行锁
- 或乐观锁（version 列），冲突重试 3 次

### Q6: PCI-DSS 合规？
- 不存储 raw card number（用 Stripe vault）
- TLS 1.3 强制
- 网络分段（payment service 在隔离 VPC）
- 所有 access 审计日志（CloudTrail / Datadog）

### Q7: 如何处理 chargebacks（退单）？
- 反向交易 → ledger 增加 reversal entry
- 不修改原交易（immutable）
- 通知用户 + 风控记账

---

## 6. 数据一致性模型

| 数据 | 一致性级别 | 说明 |
|------|----------|------|
| Account balance | 强一致 | 单 Postgres 行，ACID |
| Transaction status | 强一致 | 同上 |
| Ledger entries | 强一致 | append-only |
| User-facing dashboard | 最终一致 | Redis 缓存，5s 内更新 |
| Analytics / Reporting | 最终一致 | Kafka → DW（小时级） |

---

## 7. 故障处理

| 场景 | 影响 | 应对 |
|------|------|------|
| Postgres primary 挂 | 写入中断 | Sync replica failover (~30s)，期间 API 返回 5xx + 客户端重试 |
| Kafka 挂 | 异步处理停滞 | 同步路径仍可（hold funds），Kafka 恢复后续跑 |
| 外部 ACH 网络挂 | 转账延迟 | Saga 状态保留，恢复后重试；用户看到 "pending" |
| Fraud service 慢 | 用户体验下降 | Timeout 100ms，超时走规则引擎兜底 |
| 双花漏洞被利用 | 资金损失 | Ledger reconciliation 每小时跑，差异立即冻结 |

---

## 8. 监控与可观测性

### 关键指标（必上 dashboard）
- **TPS** by transaction type
- **Latency** P50/P95/P99 per endpoint
- **Error rate** by failure type (insufficient funds vs network)
- **Reconciliation drift**：ledger sum - balance（应为 0）
- **Pending duration**：大量 stuck-in-pending 是 bug
- **Fraud block rate**：异常波动可能是模型问题

### 关键告警
- 任何 reconciliation diff > $0.01 → P0 page
- ACH ack rate < 95% → P1
- Fraud service P99 > 200ms → P2

---

## 9. 进阶讨论（Senior+ 加分）

### Multi-region active-active
- 用户路由到 home region（账户主 region）
- Cross-region 转账走专用 pipeline + 时区考虑
- DR：每 region 异步复制到备份 region，RPO < 5min

### Outbox pattern
**问题**：本地 DB commit 后，Kafka publish 之前进程挂了 → 事件丢失
**解决**：
- 同 transaction 写一张 `outbox` 表
- CDC（Debezium）从 outbox 读 → Kafka
- 保证"DB 提交了 = 事件一定会发出"

### CQRS for read scaling
- Write 走 Postgres
- Read 用 ElasticSearch / DynamoDB（按用户索引最近交易）
- 通过 Kafka 同步

### Token-based currency conversion
- 内部统一用最小单位（cents）存储 BIGINT，避免浮点
- 货币转换时实时取 FX rate（带 hedging buffer）

---

## 10. 5分钟版本（白板核心）

```
1. API Gateway → Payment Service:
     - Idempotency-Key check (Redis)
     - Validate + KYC
     - Hold funds in Postgres (FOR UPDATE)
     - Publish to Kafka, return PENDING

2. Worker (Saga):
     - For internal: post immediately, double-entry ledger
     - For external: call ACH adapter, await ack
     - On ack: release hold → debit → notify user

3. 三大支柱：
     - Postgres ACID for balance
     - Kafka for async + replay
     - Ledger (immutable) for audit & reconciliation
```

**记住三句话**：
- "Money is hard — strong consistency on balance, eventual everywhere else."
- "Idempotency at every layer — API, DB, Worker, Adapter."
- "Saga over 2PC — local transactions + compensations."
