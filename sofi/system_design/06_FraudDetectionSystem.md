# SoFi System Design #6 — Real-Time Fraud Detection System

> **业务背景**：SoFi 是 fintech，反欺诈是核心防线。监管要求 + 业务损失双重压力。
> **真题相关**：System Design 中"design fraud detection"是 fintech 公司高频题
> **时间**：45-60 分钟

---

## 1. 需求澄清

### Functional Requirements
- 对**每笔交易**实时打分（risk score 0-100）
- 高分 → block；中分 → step-up（额外验证 / 人工 review）；低分 → allow
- 支持多种欺诈类型：
  - Account takeover (ATO)
  - Synthetic identity
  - Card-not-present fraud
  - Money laundering / mule accounts
- 支持**规则引擎**（人工配置）+ **ML 模型**（学习历史欺诈模式）
- 反馈循环：人工 review 结果回流训练数据

### Non-Functional Requirements
- **延迟**：每次评分 P99 < 100ms（不能拖慢交易）
- **吞吐**：5K TPS 峰值
- **可用性**：99.99%（fraud service 挂 → 必须降级，不能拒所有交易）
- **准确性**：False positive rate < 1%（拒错单太多影响用户）；recall > 95%（漏的欺诈少）
- **合规**：SAR (Suspicious Activity Report) 报告、BSA/AML、模型可解释性

### 关键澄清
> "Sync (block before交易) or async (post-transaction review)?"
> → 两者都要。Sync for blocking + async for monitoring + investigation。

> "What's the cost of a false positive vs false negative?"
> → False negative（漏的欺诈）成本远高 (avg $500 loss)，但 false positive (用户体验差) 也不能太多。

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| TPS 峰值 | 5K（与 #2 Payment 一致） |
| 每次评分特征数 | ~200 |
| 每次评分计算开销 | < 50ms（含特征拉取 + 模型推理） |
| 模型大小 | XGBoost ~50 MB；DNN ~500 MB |
| 历史数据 | 7 年训练数据 ~10 TB |
| Per-user behavior history | 90天活跃记录，~5 KB/user |

---

## 3. High-Level Architecture

```
                                       ┌─────────────────────┐
                                       │ Payment Service     │
                                       │ (#2 system)         │
                                       └──────────┬──────────┘
                                                  │ gRPC sync (P99 < 100ms)
                                                  ▼
                                       ┌─────────────────────┐
                                       │ Fraud Decision API  │
                                       └──┬───────┬─────┬────┘
                                          │       │     │
                          ┌───────────────┘       │     │
                          ▼                       ▼     ▼
                   ┌──────────────┐       ┌─────────────────┐
                   │ Rules Engine │       │ ML Inference    │
                   │ (Drools)     │       │ (XGBoost/DNN)   │
                   └──────┬───────┘       └────────┬────────┘
                          │                        │
                          └────────┬───────────────┘
                                   ▼
                          ┌──────────────────┐
                          │  Feature Store   │
                          │  (Redis + DB)    │
                          └────────┬─────────┘
                                   │
              ┌────────────────────┼─────────────────────┐
              ▼                    ▼                     ▼
       ┌──────────────┐    ┌──────────────┐    ┌────────────────┐
       │ Real-time    │    │ User Profile │    │ Device / IP    │
       │ Aggregates   │    │ (KYC, hist.) │    │ Reputation     │
       │ (Flink)      │    │              │    │ (3rd-party)    │
       └──────┬───────┘    └──────────────┘    └────────────────┘
              ▲
              │
       ┌──────────────┐
       │   Kafka      │
       │ events       │
       └──────────────┘
              ▲
              │
       ┌──────────────────────────────────────┐
       │ Producer Services                    │
       │ (Login, Transaction, Profile change) │
       └──────────────────────────────────────┘

  ────── Async Path ──────

       Decisions → Kafka → ┬→ Postgres (audit)
                           ├→ Case Management UI (analyst review)
                           ├→ Feedback loop (label → training set)
                           └→ Anomaly Detection (drift, attacks)
```

---

## 4. 核心组件详解

### 4.1 Fraud Decision API
**Endpoint**:
```
POST /fraud/score
{
  "transaction_id": "txn_abc",
  "user_id": "u_123",
  "amount": 500.00,
  "merchant": "Amazon",
  "device_id": "d_xyz",
  "ip": "1.2.3.4",
  ...
}
↓ < 100ms
{
  "decision": "ALLOW" | "BLOCK" | "STEP_UP",
  "score": 23,
  "reason_codes": ["high_velocity", "new_device"]
}
```

**流程**（必须在 100ms 内完成）：
1. 拿请求字段 → 异步并行拉取特征（5-10 个数据源）
2. 规则引擎评估（< 5ms，黑名单、阈值规则）
3. ML 模型推理（< 30ms）
4. 综合决策（rules 优先 BLOCK，否则按 ML score）
5. 返回 + 异步写日志

**降级策略**（必须有）：
- 特征拉取超时 → 用部分特征 + 默认值
- ML 模型挂 → 仅规则引擎
- 整个 fraud service 挂 → 默认 ALLOW（不能阻塞交易），但记录待审

### 4.2 Feature Store（性能关键）
**Online Store（实时查询）**：
- Redis 集群
- Key 模式：
  - `user_features:{user_id}` → JSON
  - `device_features:{device_id}`
  - `merchant_features:{merchant_id}`
- TTL：根据特征类型，从 1h 到 30天

**Offline Store（训练用）**：
- Parquet on S3
- Hive metastore 索引
- 用于离线训练 + 回测

**Feast / Tecton**：开源 feature store，但很多公司自研。

### 4.3 Real-Time Aggregates（特征计算）
**Flink stateful job** 计算用户/设备的滑窗统计：
- 过去 1 小时交易次数
- 过去 24 小时交易总额
- 过去 7 天平均交易额
- 过去 30 天独立 merchant 数
- 等等...

**实现**：
- Kafka topic: `transactions`
- KeyBy(user_id)
- Sliding window aggregate
- 输出写 Redis (Online Store)

**为什么 Flink 而不是 Spark Streaming？**
- True streaming（毫秒级），不是 micro-batch
- Stateful processing 强大
- Exactly-once semantics

### 4.4 ML Inference Service
**模型**：
- 主：XGBoost（推理快、可解释、SHAP values）
- 辅：Deep Learning（捕捉复杂模式，CTR/序列建模）
- Ensemble：加权投票

**部署**：
- 模型用 ONNX / TensorFlow Serving
- 多版本并存（A/B test 新模型）
- 在 fraud-decision pod 内 in-process（避免网络往返）→ < 10ms 推理

**特征工程**：
- 用户维度：avg amount, # of cards, account age
- 交易维度：amount, time of day, weekday/weekend
- 设备维度：device fingerprint, OS, app version
- 网络维度：IP geolocation, ASN, VPN/proxy detection
- 商户维度：MCC code, historical fraud rate
- 序列维度：user 过去 N 笔交易模式

### 4.5 Rules Engine
**为什么需要规则引擎？**
- 法务 / 合规规则不能纯 ML（要可审计）
- 紧急响应（新欺诈 pattern 出现，先加规则后训模型）
- 业务可控（产品 / 风控团队改规则不需重训）

**实现**：
- Drools / OPA / 自研 DSL
- 规则示例：
  ```
  IF transaction.amount > 10000 AND transaction.country != user.home_country
  THEN action = STEP_UP, reason = "large_intl_txn"
  ```
- 规则可热加载（不重启服务）

### 4.6 Async Pipeline
**目的**：决策做出后，继续监控 + 学习。

```
Decision → Kafka topic `fraud.decisions` →
   ├→ Postgres: 持久化每个决策（审计）
   ├→ Case Management: 高分但 ALLOW 的进入分析师队列
   ├→ Feedback Loop: 真实欺诈 label 回流
   └→ Drift Detection: 监控 score 分布变化
```

### 4.7 Case Management
- Web UI 给 fraud analyst 用
- Queue：可疑案例按风险排序
- 分析师标注：true fraud / false positive
- 标注 → 训练数据，下次模型更好

---

## 5. 关键问题与权衡

### Q1: 怎么保证 P99 < 100ms？
**多管齐下**：
- 特征预计算（不要 query 时 join）
- Redis hot path（mget batch）
- 模型 in-process（不走网络）
- 异步并行特征拉取（Java CompletableFuture）
- Tail latency：99% 拿不到所有特征也要返回（partial features OK）

### Q2: 模型怎么更新？
**离线训练 → A/B 部署 → 全量 rollout**：
1. Daily / weekly 训练 pipeline（Airflow + Spark）
2. 离线评估（precision/recall on holdout）
3. Shadow mode：新模型在 prod 跑但不影响决策（对比）
4. Canary：5% traffic 用新模型
5. Gradual rollout 100%
6. 旧模型保留作 fallback

### Q3: 如何防止 model drift？
- 监控特征分布（PSI - Population Stability Index）
- 监控模型输出分布（score distribution）
- 显著漂移 → 告警 + 触发再训练

### Q4: Cold start 用户（没历史）怎么办？
- 默认低风险阈值更严
- 用 demographic / KYC 特征做先验
- Behavioral biometrics（输入速度、设备使用模式）
- 前几次交易特别 review

### Q5: Adversarial attack（攻击者主动改行为骗模型）？
- 特征不能全部暴露给用户
- 模型定期更新（让攻击者来不及适应）
- Multi-model ensemble（攻一个还有其他）
- 异常检测：行为 sudden change 即使不是高 score 也 flag

### Q6: 反洗钱（AML）需求？
- Transaction monitoring：跨账户资金链分析
- Graph database（Neo4j）建立 user-account-counterparty 图
- 检测可疑模式：smurfing（拆分大额）、layering（多次转账掩盖来源）
- 自动生成 SAR（Suspicious Activity Report）给监管

### Q7: 如何评估模型？
**Offline**：
- Confusion matrix
- AUC-ROC, AUC-PR
- 业务指标：金额加权 recall, FPR @ 1% blocked

**Online**：
- A/B test：新模型 vs 老模型，看 fraud loss 和 false decline rate
- Customer impact：appeals rate, reactivation rate

### Q8: GDPR / 用户解释权
- "Why was I blocked?" → 必须能解释
- SHAP values 给出 top 3 reason codes
- Adverse action notice 包含具体理由（不能只说 "ML model"）

---

## 6. 数据流（典型决策）

```
1. User 提交 $5000 transfer
2. Payment Service → Fraud API (sync gRPC)
3. Fraud API:
   a. 并行拉特征：
      - Redis: user_features (avg amount, recent activity)
      - Redis: device_features
      - 3rd-party: IP reputation
   b. Rules: amount > 1000 且 device_new → STEP_UP candidate
   c. ML: score = 73
   d. Combined decision: STEP_UP (要求 SMS OTP)
4. Return to Payment in 80ms
5. Payment requires user to confirm OTP
6. Async:
   - Decision logged to Kafka
   - Written to Postgres for audit
   - If user fails OTP → fraud confirmed → blacklist device
```

---

## 7. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Redis 挂 | 拿不到特征 | 用默认特征 + 规则引擎兜底 |
| ML 模型 OOM | 推理失败 | Fallback 到旧版模型 / 仅规则引擎 |
| 整个 fraud API 挂 | 无评分 | Payment 默认 ALLOW + flag for review |
| Flink job 落后 | 特征过期 | 监控 lag，超 5min 告警；可短时容忍 |
| 第三方（设备指纹）挂 | 部分特征缺失 | Graceful degradation |

**核心原则**：fraud service 不能成为 SPOF 阻塞交易。宁可漏一些 fraud，也不能让所有用户无法交易。

---

## 8. 监控

### 业务指标
- **Block rate** by category（突涨可能是攻击或 bug）
- **False positive rate**（用户申诉率）
- **Fraud loss $** per day
- **Detection rate**（confirmed fraud / total fraud）

### 系统指标
- API latency P50/P99
- Feature freshness (data lag in seconds)
- Model inference time
- Cache hit rate

### 告警
- Block rate > 2% baseline → P1（可能是 model bug）
- False positive complaints spike → P2
- Fraud loss > daily budget → P0
- ML score distribution shift > 3σ → P2

---

## 9. 进阶讨论

### Graph-based fraud detection
- 用户/设备/IP 形成图
- 集群分析：mule account ring
- Graph features 喂给 ML 模型（GNN）

### Federated / multi-party
- 多家银行联合训练（不共享原始数据）
- Improves overall fraud detection

### Real-time online learning
- 部分模型增量学习（不等离线 pipeline）
- 例：blacklist 实时更新（一个用户被确认欺诈 → 同设备其他账户立即提风险）

### Synthetic data for training
- Fraud 样本稀少（< 0.1%），class imbalance
- SMOTE / GAN 合成欺诈样本
- 但要小心 distribution shift

### Privacy-preserving ML
- Differential privacy（聚合统计）
- Homomorphic encryption（极端场景）
- 实际更多用 anonymization + access control

---

## 10. 5分钟版本

```
Transaction → Fraud API (P99 < 100ms)
                  ↓
       ┌──────────┼──────────┐
       ▼          ▼          ▼
  Rules        Features    ML Model
  Engine     (Redis Store) (XGBoost in-proc)
       └──────────┼──────────┘
                  ▼
              Decision
                  ↓
              ┌───┴────────────┐
              ▼                ▼
          (sync return)    Kafka → Audit/Review/Training
```

**核心三句话**：
- "Latency budget is 100ms — everything else flows from this constraint."
- "Feature store > recompute — pre-aggregate in Flink, query Redis at decision time."
- "Always have a fallback — fraud service must never block all transactions."
