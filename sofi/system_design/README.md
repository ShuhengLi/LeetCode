# SoFi System Design Interview — Staff / Senior Engineer

> **轮次**：Senior/Staff Onsite 必有 1 轮 System Design（45-60 分钟）
> **真题来源**：Glassdoor、Blind、Programhelp、候选人公开报告（2024-2025）

---

## 题目列表

| # | 文件 | 题目 | 真题确认 | 业务关联 | 难度 |
|---|------|------|---------|---------|------|
| 1 | [01_RealTimeCloudBillingSystem.md](01_RealTimeCloudBillingSystem.md) | Real-Time Cloud Billing (AWS-style) | ✅ 真题原题 | 通用，类比 SoFi 利息计算 | ★★★★ |
| 2 | [02_PaymentProcessingSystem.md](02_PaymentProcessingSystem.md) | Payment Processing System (millions TPS) | ✅ 真题原题 | SoFi Money / Banking 核心 | ★★★★★ |
| 3 | [03_StockPriceWatcher.md](03_StockPriceWatcher.md) | Stock Price Watcher / Alert App | ✅ 真题原题 | SoFi Invest | ★★★★ |
| 4 | [04_LoanOriginationSystem.md](04_LoanOriginationSystem.md) | Loan Origination & Approval Workflow | ⭐ 业务核心 | SoFi 起家产品 | ★★★★★ |
| 5 | [05_NotificationSystem.md](05_NotificationSystem.md) | Multi-Channel Notification System | ⭐ 高频通用 | 所有产品依赖 | ★★★ |
| 6 | [06_FraudDetectionSystem.md](06_FraudDetectionSystem.md) | Real-Time Fraud Detection | ⭐ Fintech 必考 | 反欺诈 / AML | ★★★★ |
| 7 | [07_RateLimiter.md](07_RateLimiter.md) | Distributed Rate Limiter | ⭐ 经典中题 | API Gateway 子组件 | ★★★ |

**图例：** ✅ = 公开报告确认 SoFi 真题；⭐ = 业务高度相关 + 同类公司高频题

---

## SoFi System Design 面试特点

### 与传统 FAANG 的区别

| 维度 | FAANG | SoFi（Fintech） |
|------|-------|----------------|
| 重点 | Scale（亿级用户） | **正确性 + 合规** |
| 一致性 | 通常最终一致即可 | 钱相关必须强一致 |
| 数据保留 | 业务需要决定 | 监管强制 7+ 年 |
| 故障时 | 降级 | 优先保不出错（金额） |
| 必谈话题 | CDN, sharding | **idempotency, audit, double-entry, KYC/AML** |

### 评分维度（Senior/Staff 期望）

| 维度 | Senior | Staff |
|------|--------|-------|
| 需求澄清 | 主动问 5-10 个问题 | 引导面试官，提出他没想到的需求 |
| Capacity estimation | 主动算（QPS、storage） | 同 + 提出多种 scale 假设的 trade-off |
| High-level design | 画清楚组件 + 数据流 | 同 + 多种方案对比 |
| 深入细节 | 选 1-2 个组件深挖 | 主动选 + 在追问中深入 |
| Trade-off | 知道每个选择的代价 | 能讲行业通用 pattern + 反例 |
| 故障处理 | 至少讨论 3 种故障 | 设计本身就考虑 partial failure |
| 数据/合规 | 知道关键合规点 | 把合规作为设计 driver |

---

## 通用解题框架（45 分钟时间盒）

```
0-5 min:    需求澄清（FR + NFR + 关键约束）
5-10 min:   Capacity estimation（QPS, storage, bandwidth）
10-20 min:  High-level architecture（画图 + 主要组件）
20-35 min:  Deep dive 1-2 个核心组件
35-40 min:  故障处理 + 监控 + 取舍讨论
40-45 min:  进阶 / 面试官 follow-up
```

### 必谈的 fintech 关键词

按重要性排序，**任何题目都至少提 3-5 个**：

1. **Idempotency**（幂等）— 钱不能扣两次
2. **Audit log / Immutable history** — 合规
3. **Strong consistency on money, eventual elsewhere** — 合理取舍
4. **Saga / Compensation** — 跨服务事务
5. **PII / PCI-DSS / KYC / AML** — 数据保护合规
6. **Double-entry bookkeeping** — Ledger 最佳实践
7. **Reconciliation** — 对账机制
8. **Dead letter queue** — 失败兜底
9. **Circuit breaker** — 防 cascade failure
10. **Outbox pattern** — DB+消息一致

---

## 高频组件 / 技术栈

| 类别 | 推荐技术 | 适用场景 |
|------|---------|---------|
| API Framework | **Spring Boot** | SoFi 主要技术栈 |
| Data Store (OLTP) | PostgreSQL（ACID） | Account / Transaction |
| Data Store (KV) | Redis（cluster） | Cache / Session / Counter |
| Document Store | S3 + Postgres metadata | KYC docs, statements |
| Message Queue | **Kafka** | 事件总线 + 异步处理 |
| Stream Processing | Flink / Kafka Streams | 实时聚合、Fraud scoring |
| Workflow Engine | **Temporal** | 长流程（loan origination） |
| Time-series | InfluxDB / TimescaleDB | Metrics, prices |
| Search | Elasticsearch | Transaction search, audit query |
| ML | XGBoost (tabular) + DL | Fraud, underwriting |
| Service Mesh | Istio / Envoy | Circuit breaker, mTLS |
| API Gateway | Kong / Envoy / Spring Cloud Gateway | 限流、路由、auth |

---

## 备考策略

### 优先级排序

**必看（保过线）**：
- #2 Payment Processing — SoFi 核心业务，最有可能问
- #1 Real-Time Billing — 真题原题
- #5 Notification — 通用题

**重点准备**：
- #4 Loan Origination — 业务核心
- #6 Fraud Detection — 高级深度

**面试前过一遍**：
- #3 Stock Price Watcher — Invest 业务
- #7 Rate Limiter — 短题，可作为子组件

### 训练方法

1. **第一遍**：通读每题，理解架构图
2. **第二遍**：合上文档，自己画架构 + 写关键决策
3. **第三遍**：白板 mock，45 分钟限时
4. **找朋友 mock**：或用 ChatGPT 当面试官

### 常见踩坑

- ❌ 一上来就画图，没问需求
- ❌ Capacity 不算，光"用 Redis 就行了"
- ❌ 只提技术栈不讲取舍（"用 Kafka" vs "用 Kafka 因为我们需要 replay + 高吞吐 + Spring 生态成熟"）
- ❌ 不谈故障（"Redis 挂了怎么办？" 答不上来）
- ❌ 忽略合规（fintech 面试不谈 audit / PII 是大忌）

---

## 与 Coding Round 的关联

System Design 中提到的某些组件，coding round 可能直接考实现：

| System Design | 对应 Coding 题 |
|--------------|---------------|
| Cache layer | [LRU Cache](../code/06_LRUCache.java) |
| Event log | [In-Memory KV Store](../code/07_InMemoryKeyValueStore.java) |
| Stream aggregation | [Top K Frequent](../code/14_TopKFrequentWords.java), [Transaction Frequency](../code/05_TransactionFrequencyOrder.java) |
| Workflow / DAG | [Course Schedule](../code/12_CourseSchedule.java) |
| Path finding | [Word Ladder](../code/13_WordLadder.java) |
| Time window | [Sliding Window](../code/02_SmallestCleanSubarray.java) |

面试时如果 SD 中讲到这些组件，**可以提一句"这其实就是 LeetCode XXX 的工业版"** —— 显示你打通了 algo 与 system 的边界。

---

## 资源

- **书**：
  - "Designing Data-Intensive Applications" by Martin Kleppmann（必读）
  - "System Design Interview" by Alex Xu（速查）
- **课程**：
  - HelloInterview（专做 system design，模拟接近真实面试）
  - Educative: Grokking the System Design Interview
- **博客**：
  - High Scalability
  - Stripe Engineering（fintech 实战，强相关）
  - SoFi Tech Blog（如有，找内部分享）

---

## 真题来源链接

- [Glassdoor: SoFi Senior Software Engineer Interview](https://www.glassdoor.com/Interview/SoFi-Senior-Software-Engineer-Interview-Questions-EI_IE779979.0,4_KO5,29.htm)
- [Blind: SoFi Interview Discussions](https://www.teamblind.com/company/SoFi/posts/sofi-interview)
- [Programhelp: SoFi Interview Guide](https://programhelp.net/en/vo/sofi-interview-experience-question-types-strategy/)
- [FrontendLead Discuss: SoFi Senior SWE Onsite](https://discuss.frontendlead.com/t/sofi-senior-swe-onsite-full-stack/1962)
