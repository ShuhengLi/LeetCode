# SoFi System Design #4 — Loan Origination & Approval System

> **业务背景**：SoFi 核心业务是 personal loan / student loan refi / mortgage
> **真题相关**：System Design 轮经常问"design SoFi's core product"
> **时间**：45-60 分钟

---

## 1. 需求澄清

### Functional Requirements
- 用户**申请贷款**（personal / student refi / mortgage）
- 系统执行**信用评估**（credit check + underwriting）
- 多步审批流程（document upload → KYC → credit pull → underwriting → offer → e-sign → fund）
- 用户在 dashboard **查看申请状态**
- 审批后**资金发放**（disburse to bank account）
- 还款 schedule 生成 + 月度还款（与 #2 Payment 系统集成）

### Non-Functional Requirements
- **吞吐**：peak 1K 申请/分钟（推广季）
- **流程时长**：从申请到资金 1 小时 - 7 天（视产品）
- **延迟**：每个 API call P99 < 500ms；审批工作流总时长可异步
- **合规**：FCRA、ECOA、TILA、SOX、SOC2
- **数据保留**：7-25 年（取决于 loan type）
- **可用性**：99.9%

### 关键澄清
> "Sync or async approval?" → 大部分 async。Personal loan 可能 instant pre-approval（< 30s），final approval 需人工 review。

> "Multi-step workflow with potential days of waiting?" → Yes，所以是 long-running workflow。

> "Are partial completions saved?" → 必须，用户可能多日内分步完成。

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| 日申请数 | 50K |
| 每申请数据量 | ~20 MB（含 docs / pay stubs / IDs） |
| 总活跃 loans | 5M |
| 月度还款交易 | 5M/月 ≈ 165K/天 |
| 文档存储 | 1 PB（含历史） |

---

## 3. High-Level Architecture

```
                    ┌──────────────────────────┐
                    │   Web / Mobile App       │
                    └──────────────┬───────────┘
                                   │
                                   ▼
                    ┌──────────────────────────┐
                    │   API Gateway + Auth     │
                    └──────────────┬───────────┘
                                   │
                                   ▼
                    ┌──────────────────────────┐
                    │ Loan Application Service │
                    └──────┬───────────────────┘
                           │
                ┌──────────┼──────────────────────┐
                ▼          ▼                      ▼
         ┌──────────┐ ┌─────────────┐     ┌────────────────┐
         │  Postgres│ │  S3 (docs)  │     │  Workflow      │
         │ (loans,  │ │  encrypted  │     │  Orchestrator  │
         │  apps)   │ │             │     │ (Temporal/     │
         └──────────┘ └─────────────┘     │  AWS Step Fn)  │
                                          └───────┬────────┘
                                                  │
                ┌─────────────────────────────────┼───────────────────────┐
                ▼                                 ▼                       ▼
         ┌──────────────┐                 ┌──────────────┐         ┌──────────────┐
         │ KYC Service  │                 │  Credit      │         │ Underwriting │
         │ (Plaid/Jumio)│                 │  Bureau      │         │  Engine      │
         └──────────────┘                 │ (Equifax,    │         │ (rules + ML) │
                                          │  Experian)   │         └──────────────┘
                                          └──────────────┘                  │
                                                                            │
                                                  ┌─────────────────────────┘
                                                  ▼
                                          ┌──────────────┐
                                          │ E-Sign       │
                                          │ (DocuSign)   │
                                          └──────┬───────┘
                                                 │
                                                 ▼
                                          ┌──────────────┐
                                          │ Disbursement │ → 调用 Payment 系统
                                          │  Service     │
                                          └──────────────┘
```

---

## 4. 核心组件详解

### 4.1 Loan Application Service
- REST API（Spring Boot）
- Endpoints:
  - `POST /applications` — 创建草稿
  - `PATCH /applications/{id}` — 增量保存（用户分步填表）
  - `POST /applications/{id}/submit` — 提交进入 workflow
  - `GET /applications/{id}` — 查状态
  - `POST /applications/{id}/documents` — pre-signed S3 upload URL

**State Machine**：
```
DRAFT → SUBMITTED → KYC_PENDING → CREDIT_PENDING →
UNDERWRITING → OFFER_GENERATED → OFFER_ACCEPTED →
ESIGN_PENDING → ESIGN_COMPLETED → FUNDING → FUNDED
                                          ↓
                                      ACTIVE_LOAN
```

任何步骤可能 → `REJECTED` / `CANCELED`

### 4.2 Workflow Orchestrator (Temporal)
**为什么需要 workflow engine？**
- 流程跨多天/周
- 涉及外部服务调用（可能失败 / 慢）
- 需要 retry、timeout、人工干预
- 必须可恢复（服务重启不丢状态）

**为什么不用 Kafka 单独搞？**
- Kafka 适合事件流，不适合"长流程编排 + 等待外部回调"
- Temporal 提供 workflow as code（durable execution）

**示例 Workflow（Temporal Java SDK）**：
```java
@WorkflowInterface
public interface LoanApplicationWorkflow {
    @WorkflowMethod
    LoanResult process(ApplicationRequest req);
}

public class LoanApplicationWorkflowImpl implements LoanApplicationWorkflow {
    public LoanResult process(ApplicationRequest req) {
        // 自动重试 + 持久化状态
        var kycResult = activities.runKYC(req);
        if (!kycResult.passed) return reject("KYC failed");

        var credit = activities.pullCredit(req);   // 调 Equifax
        if (credit.score < 600) return reject("Credit too low");

        var decision = activities.underwrite(req, credit);
        if (!decision.approved) return reject(decision.reason);

        var offer = activities.generateOffer(decision);
        // wait for user to accept (signal from external)
        Workflow.await(() -> applicationAccepted);

        activities.requestEsign(offer);
        Workflow.await(() -> esignCompleted);   // 等 DocuSign webhook

        activities.disburseFunds(offer);
        return LoanResult.success(offer);
    }
}
```

**关键特性**：
- `Workflow.await()` 可等数天，pod 重启不丢状态
- 每个 activity 自动重试（exponential backoff）
- 所有历史可查（debugging easy）

### 4.3 Postgres（核心业务数据）
**Schema**：
```sql
CREATE TABLE applications (
  app_id UUID PRIMARY KEY,
  user_id BIGINT NOT NULL,
  product_type VARCHAR(20),  -- PERSONAL/STUDENT/MORTGAGE
  status VARCHAR(30) NOT NULL,
  amount_requested DECIMAL(15,2),
  workflow_id VARCHAR(64),   -- Temporal workflow handle
  created_at TIMESTAMP,
  ...
);

CREATE TABLE applications_audit (
  -- append-only, immutable
  audit_id BIGSERIAL,
  app_id UUID,
  prev_status VARCHAR(30),
  new_status VARCHAR(30),
  actor VARCHAR(64),  -- user / system / underwriter
  reason TEXT,
  ts TIMESTAMP
);

CREATE TABLE loans (
  loan_id UUID PRIMARY KEY,
  app_id UUID,
  principal DECIMAL(15,2),
  apr DECIMAL(8,4),
  term_months INT,
  status VARCHAR(20),  -- ACTIVE/PAID_OFF/DEFAULT
  ...
);

CREATE TABLE payment_schedule (
  loan_id UUID,
  installment_no INT,
  due_date DATE,
  principal DECIMAL,
  interest DECIMAL,
  status VARCHAR(20),
  PRIMARY KEY (loan_id, installment_no)
);
```

### 4.4 Document Storage (S3)
- 用户上传走 **pre-signed URL**（前端直接 PUT 到 S3，不经过 backend）
- Server-side encryption (KMS) 强制
- 元数据存 Postgres，文档存 S3
- 文档版本化（用户可重传）
- Compliance：7 年保留 + WORM (object lock)

### 4.5 KYC Service
- 集成 Plaid（链接银行）+ Jumio（ID verification）
- 异步：用户上传 ID → 调 Jumio → webhook 返回结果
- Fallback：人工审核队列（fraud team）

### 4.6 Credit Bureau Adapter
- 调用 Equifax / Experian / TransUnion
- **Hard pull vs soft pull**：申请时 hard（影响分数），pre-qual 时 soft
- 缓存 90 天（监管允许）
- Rate limiting（外部 API 贵 + 限流）
- Circuit breaker：bureau 挂时进入排队，30 分钟后重试

### 4.7 Underwriting Engine
**两层**：
1. **规则引擎**（Drools / 自研 DSL）：
   - DTI < 50%
   - Credit score >= product threshold
   - Income >= min
   - 黑名单 / OFAC 检查
2. **ML 模型**（risk score）：
   - 输入：credit history, employment, education
   - 输出：违约概率 → 决定 APR 利率
   - 模型训练：离线 batch，特征 store (Feast)

**决策结果**：
- APPROVED with offer (amount, APR, term)
- DECLINED with reason (FCRA 要求 adverse action notice)
- MANUAL_REVIEW（边缘案例 → 人工队列）

### 4.8 Disbursement Service
- 调用 Payment 系统（#2 设计的）的 ACH adapter
- 资金到用户绑定的外部银行账户
- Idempotent（用 application_id 做幂等 key）
- 失败处理：重试 / 告警 / 人工介入

---

## 5. 关键问题与权衡

### Q1: 为什么用 Temporal 而不是 Step Functions / Airflow？
| 工具 | 强项 | 弱项 |
|------|------|------|
| Temporal | Code-as-workflow, durable, 长运行 | 自托管复杂 |
| Step Functions | AWS managed, 可视化 | Vendor lock-in，复杂逻辑难写 |
| Airflow | ETL/batch 强 | 不适合事件驱动 long-running |
| Kafka 自研 | 灵活 | 状态/重试/补偿全要自己写 |

### Q2: 用户多次提交同一 application 怎么办？
- Application 表 `(user_id, product_type, status='DRAFT')` UNIQUE 部分索引
- 提交按钮 idempotent（client 生成 token）

### Q3: Credit pull 失败怎么办？
- Activity 自动重试 3 次（指数退避）
- 仍失败：workflow 挂起到人工队列，发邮件让 ops 看
- 用户看到 status: "We're reviewing your information..."

### Q4: 文档 OCR / 验证怎么做？
- 异步：上传 → S3 event → Lambda → OCR (Textract) → 校验（pay stub 金额匹配申报收入？）
- 不通过 → 自动要求用户重传 + 提示

### Q5: 监管审计需求？
- 所有状态变化记 `applications_audit`
- 文档 immutable（S3 object lock）
- 决策必须可解释（ML 用 SHAP，规则引擎本身可解释）
- FCRA "adverse action" letter 30 天内发出

### Q6: 如何处理 fraud（applicant 用别人身份）？
- KYC 第一道防线
- 行为分析：device fingerprint, IP geolocation
- 速度规则：同 SSN 30 天内多次申请 → flag
- ML model: synthetic identity detection

### Q7: 还款 schedule 怎么生成？
**摊还公式（Amortization）**：
```
Monthly payment M = P × [r(1+r)^n] / [(1+r)^n - 1]
  P = principal, r = monthly rate, n = months

每期：
  Interest_i = remaining_balance × r
  Principal_i = M - Interest_i
  remaining -= Principal_i
```
预生成所有期数写 `payment_schedule` 表，定时 job 触发扣款。

---

## 6. 数据流（典型用户旅程）

```
1. User: 填表 → POST /applications (DRAFT)
2. User: 上传 ID → S3 pre-signed URL
3. User: Submit → SUBMITTED
4. System: Start Temporal workflow
5. Workflow: KYC → Plaid + Jumio (async, may take minutes)
6. Workflow: Pull credit → Equifax (sync, < 5s)
7. Workflow: Underwriting → APPROVED, generate offer
8. User: View offer in dashboard, click "Accept"
9. Workflow: signal received, request DocuSign
10. User: Sign documents → DocuSign webhook
11. Workflow: signal received, disburse via ACH
12. ACH: T+1 settlement → loan status → ACTIVE
13. Schedule: payment_schedule generated, autopay enrollment
```

---

## 7. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Equifax 挂 | 申请卡住 | Workflow retry；用户看到 "in review" |
| Postgres 挂 | 全停 | Multi-AZ + automatic failover |
| Temporal cluster 挂 | 工作流停 | Multi-region cluster；recoverable from event log |
| DocuSign webhook 丢失 | 流程卡 esign | 兜底 polling + 手动操作 console |
| 文档 OCR 错误 | 错批 | 关键决策不依赖 OCR alone；human review |

---

## 8. 进阶讨论

### Pre-qualification（soft pull）
- 用户填基本信息 → soft pull → instant rate estimate
- 不影响信用分
- 转化率显著提升（用户先看价格再决定申请）

### Streaming credit data
- Plaid 链接后持续监控（user opt-in）
- 收入变化 / 失业 → 风控调整 APR
- Pre-emptive collections（早期介入风险账户）

### Multi-product pipeline
- 同一 framework 支持 personal / student refi / mortgage
- 不同 product 配置不同 workflow（template + parameters）
- A/B testing：新 underwriting 模型先跑 5% traffic

### A/B test offers
- 同一申请多个候选 offer，根据 user segment 决定显示哪个
- 影响 take rate 和 default rate（trade-off）

### 与 SoFi 全平台集成
- Loan + Banking + Invest 统一 user profile
- Cross-sell：发放 loan 后推荐高息储蓄账户
- Holistic credit decisioning：用 internal banking 数据增强 underwriting

---

## 9. 监控

| 指标 | 告警阈值 |
|------|---------|
| Workflow stuck > 24h | P1（人工 unstuck） |
| Credit bureau error rate > 5% | P2 |
| KYC pass rate drop > 10% | P2（可能是 fraud spike） |
| Approval rate drop > 5% week-over-week | P3 |
| Disbursement failure rate > 0.5% | P1 |

---

## 10. 5分钟版本

```
申请 → Postgres (state) + S3 (docs)
      ↓
   Temporal Workflow:
      → KYC → Credit Bureau → Underwriting → Offer
      → User Accept (signal)
      → DocuSign (webhook)
      → ACH Disbursement (调用 Payment 系统)
      ↓
   Active Loan + Payment Schedule
```

**核心三句话**：
- "Long-running workflow needs Temporal — Kafka alone isn't enough."
- "Compliance-first design — audit log, immutability, FCRA explanability."
- "Async by default — user fills, system runs in background, push status updates."
