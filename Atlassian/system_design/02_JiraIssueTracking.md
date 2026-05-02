# Atlassian SD #2 — Jira-style Issue Tracking System

> **真题来源**：Atlassian P50/P60 onsite 报告（2024-2025）
> **业务关联**：Jira / Jira Service Management 核心
> **时间**：45-60 分钟

---

## 1. 需求澄清

### Functional Requirements
- **CRUD on issues** (ticket): title, description, status, assignee, labels, priority
- **Custom fields**：每个 project 可自定义字段（关键复杂度来源）
- **Workflow / Status transitions**：每个 issue type 有不同状态机（TODO → IN_PROGRESS → DONE 等），可配置
- **Search**：全文搜索 + JQL（field-based query language）
- **Notifications**：状态变化、@mention、assignee 变化
- **Audit trail**：完整修改历史
- **Permissions**：project-level / issue-level

### Non-Functional Requirements
- **规模**：10M users, 100K orgs, 1B issues
- **写入**：5K issue ops/s 峰值
- **查询**：50K JQL queries/s
- **延迟**：单 issue 读 P99 < 100ms；JQL P99 < 500ms
- **可用性**：99.95%
- **审计/合规**：完整变更记录 7+ 年

### 关键澄清
> "Custom fields per project — how flexible?"
> → 极度灵活，每个 project 可有 50+ 自定义字段，类型多样

> "Search latency vs freshness?"
> → < 5s newly created issue should appear in search

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| 总 issues | 1B |
| 平均 issue size | 5 KB（含描述、字段） |
| 总 storage（核心） | 5 TB |
| Attachments | 50 PB（去 S3） |
| 每 issue 平均 changelog | 20 entries → 20B append |
| Changelog total | 20 × 1B × ~200B = 4 TB |
| QPS write | 5K |
| QPS read | 100K |
| QPS search | 50K |

---

## 3. High-Level Architecture

```
                      ┌──────────────┐
                      │   Clients    │ (Web / Mobile / API)
                      └──────┬───────┘
                             ▼
                      ┌──────────────┐
                      │ API Gateway  │ (auth, rate limit)
                      └──────┬───────┘
                             ▼
              ┌────────────────────────────┐
              │  Issue Service (Spring)    │
              │ - CRUD                     │
              │ - Workflow engine           │
              │ - Permission check          │
              └─┬──────────┬──────────┬────┘
                │          │          │
                ▼          ▼          ▼
         ┌──────────┐ ┌──────────┐ ┌──────────────┐
         │PostgreSQL│ │  Kafka   │ │  Cache       │
         │(sharded) │ │ (events) │ │  (Redis)     │
         └──────────┘ └────┬─────┘ └──────────────┘
                           │
              ┌────────────┼─────────────┬─────────────┐
              ▼            ▼             ▼             ▼
        ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────────┐
        │Search    │ │Notif     │ │Audit Log │ │Webhook       │
        │Indexer   │ │Service   │ │(append)  │ │Dispatcher    │
        │(ES)      │ │          │ │S3 +      │ │              │
        │          │ │          │ │Postgres  │ │              │
        └──────────┘ └──────────┘ └──────────┘ └──────────────┘
```

---

## 4. 核心设计：Schema for Custom Fields ★

### 难点：每个 project 的 issue 有不同字段

#### 方案 1：每 project 一张表（Schema-per-tenant）
- ❌ 100K orgs × 多 projects → 几百万张表，DB 元数据爆炸
- 仅适合 < 1K tenants

#### 方案 2：EAV（Entity-Attribute-Value）
```sql
CREATE TABLE issue_field_values (
  issue_id BIGINT,
  field_id BIGINT,
  value_text TEXT,
  value_number NUMERIC,
  value_date DATE,
  PRIMARY KEY (issue_id, field_id)
);
```
- ✅ 灵活
- ❌ 查询复杂 JQL 慢（多次 join）

#### 方案 3：JSONB hybrid（推荐 — Atlassian 公开技术博客提到）
```sql
CREATE TABLE issues (
  id BIGINT PRIMARY KEY,
  project_id BIGINT NOT NULL,
  -- 核心字段（hot path）
  title TEXT, status VARCHAR(20), priority VARCHAR(10),
  assignee_id BIGINT, reporter_id BIGINT,
  created_at TIMESTAMP, updated_at TIMESTAMP,
  -- 自定义字段
  custom_fields JSONB
);

CREATE INDEX idx_issues_project ON issues(project_id);
CREATE INDEX idx_issues_assignee ON issues(assignee_id);
-- 关键：JSONB GIN index for custom field queries
CREATE INDEX idx_issues_custom_gin ON issues USING GIN (custom_fields);
```
- ✅ 核心字段类型化（快）
- ✅ 自定义字段灵活
- ✅ Postgres GIN 索引支持 `WHERE custom_fields @> '{"epic": "PROJ-123"}'`
- ⚠️ 字段类型校验在应用层做

### Sharding 策略
- 按 `project_id` 一致性 hash 分库（同 project issues 在同一 shard，方便 JOIN）
- Shard 数量：64 起步
- 大 project（一个 shard 装不下）→ 二级分片 by issue_id

---

## 5. Workflow Engine（状态机）

每个 issue type 定义合法状态转移：
```
States: [TODO, IN_PROGRESS, IN_REVIEW, DONE, CLOSED, REOPENED]
Transitions:
  TODO        → IN_PROGRESS (any user)
  IN_PROGRESS → IN_REVIEW   (assignee only)
  IN_REVIEW   → DONE        (reviewer only, with approval)
  DONE        → CLOSED      (auto after 7 days)
  CLOSED      → REOPENED    (any user with permission)
```

**实现**：
- Workflow definition 存 DB（JSON），加载到内存做转移合法性检查
- 转移触发 hook：发邮件、更新 sprint、auto-assign 等
- 支持自定义 workflow（每个 project 不同）

```java
public class WorkflowEngine {
    public void transition(Issue issue, String newStatus, User actor) {
        Workflow wf = loadWorkflow(issue.getType());
        Transition t = wf.findTransition(issue.getStatus(), newStatus);
        if (t == null) throw new InvalidTransitionException();
        if (!t.isAllowed(actor, issue)) throw new PermissionDeniedException();

        // Run pre-conditions, validators
        for (Condition c : t.getConditions()) c.check(issue, actor);

        // Apply transition
        String oldStatus = issue.getStatus();
        issue.setStatus(newStatus);
        issueRepo.save(issue);

        // Post-functions (async)
        eventBus.publish(new IssueStatusChanged(issue, oldStatus, newStatus, actor));
    }
}
```

---

## 6. Search & JQL

### 选型：Elasticsearch
- 全文搜索 + 复杂 filter
- 每 issue 一个 ES doc，包含所有字段（含 custom）
- 同步：DB write → Kafka event → Indexer worker → ES

### JQL 翻译
JQL 例：
```
project = "ATL" AND assignee = currentUser() AND status != Done
ORDER BY priority DESC, updated DESC
```
→ 解析为 ES query DSL：
```json
{ "bool": {
    "must": [
      {"term": {"project_id": "ATL"}},
      {"term": {"assignee_id": "<userId>"}},
      {"bool": {"must_not": [{"term": {"status": "DONE"}}]}}
    ]
  },
  "sort": [{"priority": "desc"}, {"updated_at": "desc"}]
}
```

### 索引一致性
- DB write → Kafka → Indexer 写 ES
- Lag < 5s 通常可接受
- "Strong read" API：直接查 DB 不走 ES

---

## 7. Notification & Webhook

### Internal Notifications
- Status change / assignee / comment / @mention 触发
- Per-user preference（哪些事件接收 email、in-app）
- 实现见 SoFi #5 Notification System pattern

### Webhooks（external）
- Customer 配置 URL，事件触发推 HTTP POST
- 重试 + dead letter（超过 3 次失败 → 通知管理员）
- 限流（防止下游被打挂）

---

## 8. 关键问题与权衡

### Q1: 为什么不用 NoSQL（MongoDB / DynamoDB）？
- NoSQL 处理灵活 schema 强，但**关系查询差**（assignee 多个 issue、project 包含 issues）
- Postgres + JSONB 是混合方案：核心关系强一致，灵活字段 JSONB
- 大型 enterprise 客户依赖 SQL 报表 → SQL 必需

### Q2: 1B issues 在 Postgres 怎么撑？
- 分片：64 shards × ~16M issues each
- 每 shard 用 read replica 卸压力
- 老 issue archive 到 S3（仅查时按需加载）

### Q3: Cache 策略？
- Hot issues：Redis cache by issueId（TTL 5 min）
- Permission：Redis cache (userId, projectId) → role
- ES 查询结果：可缓存少量，但 freshness 要求高

### Q4: Permissions 怎么 scale？
- Project-level：用户在 project 中的 role
- 复杂权限（field-level、issue-level）：每次 read 时检查
- 缓存 + 失效：role 变化 publish event 清缓存

### Q5: Realtime UI updates（看到他人改了 issue）？
- WebSocket → 用户订阅相关 issue 的频道
- Server 写 issue 后 publish event
- 简单实现：客户端轮询 changes API

### Q6: Audit log
- Every change → append-only log
- Schema：`(issue_id, actor, ts, field, old_value, new_value)`
- 存 Postgres + 长期归档到 S3 (Parquet)
- 监管 + 调试用

---

## 9. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Postgres 主挂 | 写中断 | 自动 failover，30s 内恢复 |
| ES 集群挂 | 搜索不可用 | UI 降级（按 ID/创建时间筛选） |
| Kafka 挂 | 索引/通知延迟 | 同步主路径不影响；积压恢复后赶上 |
| Redis 挂 | Cache miss → DB load 增加 | 配 sufficient DB capacity |
| 单 shard 挂 | 该 shard 数据不可用 | 跨 AZ 副本 + 自动 failover |

---

## 10. 监控

- **Issue write latency** P50/P99
- **Search lag**（DB → ES）
- **Workflow transition rate** by status
- **Permission cache hit rate**
- **Webhook delivery success**
- **JQL query slow log**（> 1s 报警）

---

## 11. 进阶讨论

### Multi-tenant isolation
- 每个 customer (org) 数据隔离
- Postgres row-level security
- ES index per tenant 或 single index + tenant filter

### 大客户特殊处理
- Hyper-large orgs（百万 issues）专用 shard
- Premium SLA：dedicated infra

### Sprint / Backlog 管理
- Sprint 是另一种 entity，包含 issue list
- Drag-drop reorder → CRDT-style ordering（fractional indexing）

### Atlassian Connect / Forge
- 第三方 app 通过 webhook + REST API 集成
- 沙箱化 / quota 限制 / OAuth 权限

---

## 12. 5分钟版本

```
API Gateway → Issue Service (Spring)
                  ├→ Postgres (sharded by project_id; JSONB custom fields)
                  ├→ Kafka events
                  ├→ Workflow Engine (state machine)
                  └→ Permission Cache (Redis)

Kafka → ┬→ ES Indexer (search)
        ├→ Notification Service
        ├→ Audit Log (Postgres + S3)
        └→ Webhook Dispatcher
```

**核心三句话**：
- "Hybrid schema: core fields typed, custom fields JSONB with GIN index — Atlassian's actual choice."
- "Workflow engine separates state machine config from issue data — extensible per-project."
- "ES for search, Postgres for source of truth, Kafka glues them — 5s indexing lag is the SLO."
