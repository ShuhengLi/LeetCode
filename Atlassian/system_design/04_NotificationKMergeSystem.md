# Atlassian SD #4 — Cross-Product Notification System (K-way Merge)

> **真题来源**：Atlassian onsite 报告（2024-2025）
> **原题**：*"How would you merge K sorted lists of notifications from multiple Confluence pages? Design a system to deliver them efficiently."*
> **业务关联**：Confluence / Jira / Bitbucket 统一通知中心
> **时间**：45-60 分钟

---

## 1. 需求澄清

### Functional Requirements
- 用户在 Atlassian 各产品中订阅了多个对象（Jira project, Confluence page, Bitbucket repo）
- 系统聚合所有源的通知，按时间倒序展示**统一 inbox**
- 用户可标记 read/unread、归档、订阅/取消订阅
- 多渠道送达：in-app, email, push, Slack
- 支持**频次合并**（10 个连续编辑事件 → 一条 "5 changes in last hour"）
- 支持 quiet hours / 用户偏好

### Non-Functional Requirements
- **规模**：10M users, 100M sources, 1B notifications/day
- **延迟**：event → user 看到 < 5s（in-app）
- **可用性**：99.95%
- **历史**：保留 90 天 in-app 历史

### 关键澄清
> "Why K-way merge? Why does it come up here?"
> → 每个源（每个 page / project / repo）有自己的事件 stream，按时间排序；
>    用户 inbox 是 K 个流的归并，需高效合并。

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| Users | 10M (DAU 3M) |
| Sources | 100M |
| Avg subscriptions per user | 50 |
| Notifications/day | 1B (avg 100/user) |
| TPS（peak） | 50K |
| Per-event size | 500 B |
| Daily data | 500 GB |
| 90-day retention | 45 TB |

---

## 3. High-Level Architecture

```
   ┌──────────────────────────────────────┐
   │   Producer Services                   │
   │  (Jira, Confluence, Bitbucket)        │
   └──────────────────┬───────────────────┘
                      │ events
                      ▼
              ┌───────────────┐
              │     Kafka     │
              │ events.{prod} │
              └───────┬───────┘
                      │
                      ▼
              ┌─────────────────────┐
              │  Subscription Resolver│
              │  - Lookup who subscribed
              │  - Fan-out per recipient
              └────────┬────────────┘
                       │
                       ▼
              ┌─────────────────────┐
              │     Kafka           │
              │ user-notifications  │ (partitioned by userId)
              └────────┬────────────┘
                       │
        ┌──────────────┼──────────────┐
        ▼              ▼              ▼
┌──────────────┐ ┌─────────────┐ ┌────────────────┐
│ Persistor    │ │ Aggregator  │ │ Channel        │
│ (Cassandra)  │ │ (Flink)     │ │ Dispatcher     │
│ user inbox   │ │ frequency   │ │ (push/email/   │
│              │ │ collapse    │ │  slack/in-app) │
└──────────────┘ └─────────────┘ └────────────────┘
```

---

## 4. K-way Merge — 核心算法

用户打开 inbox，看到来自 50 个订阅源的通知按时间倒序排列。

### 暴力做法（不可行）
```
拉取所有 50 个源最近 N 条 → 合并排序 → 返回前 100
O(N × K log(N × K))，数据传输大
```

### 优化 1：min-heap K-way merge
```cpp
struct StreamCursor {
    string sourceId;
    Notification current;
    Iterator next;
};

// Min-heap by timestamp
priority_queue<StreamCursor> minHeap;
for (auto& source : userSubscriptions) {
    minHeap.push(source.firstNotification());
}

vector<Notification> result;
while (!minHeap.empty() && result.size() < pageSize) {
    auto cur = minHeap.top(); minHeap.pop();
    result.push_back(cur.current);
    if (cur.hasNext()) {
        cur.advance();
        minHeap.push(cur);
    }
}
```
Time: `O(N log K)` where K = subscriptions, N = items returned

### 优化 2：物化的 inbox（推荐生产方案）
- 每个 user 一个 inbox（Cassandra 按 (userId, ts) 排序）
- Producer 写事件 → Subscription Resolver 找出受影响 users → 写入每个 user 的 inbox
- Read = 直接 query user inbox（O(pageSize)，无需合并）

**Trade-off**：write fan-out 大（一个事件可能写到 10K 用户的 inbox），但 read 极快。

### 何时选哪个？
- **Pull-based K-way merge**：少数高活跃用户、源数量适中
- **Push-based fan-out**：用户多、事件量大、读多写少（典型 social feed 模型）
- Atlassian 选 **fan-out**：notification 是 read-heavy，且 sources 不会无限增长

**热点处理**：某 source 订阅者百万 → fan-out 风暴。解决：
- 大 source 走 pull 模式（不写 inbox，read 时合并）
- 临界值动态切换（subscriber count > 1K 切 pull）

---

## 5. 数据存储

### Cassandra schema（user inbox）
```cql
CREATE TABLE user_inbox (
  user_id BIGINT,
  ts TIMEUUID,
  notification_id UUID,
  source_type TEXT,
  source_id TEXT,
  event_type TEXT,
  payload TEXT,
  read_at TIMESTAMP,
  PRIMARY KEY (user_id, ts)
) WITH CLUSTERING ORDER BY (ts DESC);
```
- Partition by `user_id` → 单 user 数据集中
- Sorted by ts DESC → query latest 100 是 O(1)
- TTL 90 天

### Subscription DB (Postgres)
```sql
CREATE TABLE subscriptions (
  user_id BIGINT NOT NULL,
  source_type VARCHAR(20) NOT NULL,
  source_id VARCHAR(64) NOT NULL,
  level VARCHAR(20),  -- ALL / MENTIONS / NONE
  created_at TIMESTAMP,
  PRIMARY KEY (user_id, source_type, source_id)
);
CREATE INDEX idx_subs_source ON subscriptions(source_type, source_id);
```
- Forward index：用户的 subs（read by user）
- Reverse index：某 source 的 subscribers（read by source — fan-out 时用）

---

## 6. Subscription Resolver（fan-out 关键）

```java
public void onProductEvent(Event e) {
    List<Long> subscribers = subscriptionRepo.findBySource(e.sourceType, e.sourceId);

    // Optimization: skip noisy notifications based on user prefs
    List<Long> filtered = preferenceService.filter(subscribers, e);

    // Batch write to user inboxes
    for (List<Long> batch : Lists.partition(filtered, 100)) {
        kafkaProducer.send("user-notifications",
            batch.stream().map(uid -> new UserNotification(uid, e)).toList());
    }
}
```

**Optimizations**:
- 大 source（subscribers > 10K）走异步 batch + flow control
- Subscription cache (Redis) 减少 DB 查询
- "Important users"（CEO follows everyone）单独处理

---

## 7. Aggregator（频次合并）

防止用户被刷屏：
- 同源 + 同事件类型在 1 小时内 > 5 条 → 合并为 "5 changes in <source>"
- Flink stateful job, keyed by (userId, sourceId, eventType)
- Tumbling 1-hour window
- 输出：合并通知（替换或新增 with collapse）

```
(userId=u1, sourceId=p1, eventType=PAGE_EDITED) →
   - 10:01: edit by alice
   - 10:05: edit by alice
   - 10:15: edit by bob
   ...
   →  Single notification: "3 people edited p1 in last hour"
```

---

## 8. Channel Dispatcher（多渠道）

按用户偏好分发：
- **In-app**：写 inbox + WebSocket push
- **Email**：批量 digest（每天 9am, 3pm）
- **Push (mobile)**：FCM/APNs
- **Slack**：webhook 到 user 配置的 channel

每渠道独立 worker pool；失败 → DLQ + 重试。

---

## 9. 关键问题与权衡

### Q1: Push vs Pull (fan-out vs read-time merge)?
**Push (fan-out at write)**:
- ✅ Read 简单快
- ❌ Write 放大（×N subscribers）；存储重复
- 适合：热门内容少、订阅者分布均匀

**Pull (merge at read)**:
- ✅ 不放大
- ❌ Read 重，K 越大越慢
- 适合：超大 fan-out（Twitter Justin Bieber 模型）

**混合**：默认 push；对超大 source 切 pull。

### Q2: 如何避免重复（同事件多渠道送达）？
- Notification ID 全局 unique
- 每个 channel 维护"已送达"状态
- Dedupe key = (userId, eventId, channel)

### Q3: Read consistency（标记 read 后立即看到）？
- Cassandra LWW（last-write-wins）通常足够
- Critical update（read receipt）写后 immediately read-back 校验

### Q4: 历史 query（"找上周的 Bitbucket PR 通知"）？
- Query Cassandra by (userId, ts range)
- 加 secondary index 或维护额外 view: `(userId, source_type, ts)`

### Q5: Spam / abuse 防护？
- 单 user 一小时通知数 cap
- 异常源（spam bot）被 throttle
- 用户可 mute / unsubscribe / report

---

## 10. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Cassandra 节点挂 | 部分用户 inbox 不可读 | 副本 RF=3，自动 routing |
| Kafka 挂 | event lag | 短期内 producer 缓冲，恢复后追赶 |
| Subscription DB 挂 | fan-out 不能进行 | producer 暂存到 retry queue；P0 alert |
| Email provider 挂 | email 通知延迟 | 多 provider 切换；降级到 push |
| 大 fan-out 风暴（CEO bulk action） | 系统卡 | rate limit + back-pressure；优雅降级 |

---

## 11. 监控

- **End-to-end latency** (event → user device) P99
- **Inbox write success rate**
- **Fan-out factor**（avg subscribers per event）
- **Channel delivery rate** by channel
- **Read ratio**（多少通知被实际打开）
- **Quiet-hour violations**（应该静默时发了）

---

## 12. 进阶讨论

### Personalized ranking
- 不只是时间倒序，按重要性 ML rank
- Features：source 重要性（user owns it？）、event type、historical engagement
- A/B test 不同 ranker

### Smart digest
- AI 摘要："今天你的项目有 5 个重要变化"
- 用 LLM 生成自然语言摘要

### Cross-Atlassian unification
- 同一 user 在 Jira / Confluence / Bitbucket 见到统一 inbox
- 共享 user identity（Atlassian Account）

### Real-time WebSocket
- 用户在 web 时 → 实时推送（不等下次 refresh）
- 见 Confluence collab 设计中的 Edge Gateway

---

## 13. 5分钟版本

```
Producers → Kafka → Subscription Resolver
                       │
              fan-out by user
                       │
                       ▼
                User Inbox Kafka topic
                       │
        ┌──────────────┼──────────────┐
        ▼              ▼              ▼
    Cassandra       Aggregator     Channel
    inbox          (Flink, freq    Dispatcher
    (per-user)      collapse)     (push/email/etc)

Read inbox = direct Cassandra query (no merge needed!)
For pull-mode big sources = K-way heap merge at query time.
```

**核心三句话**：
- "Default to push (fan-out at write) for fast reads; switch to pull for hot sources to avoid write amplification."
- "Cassandra partitioned by userId, sorted by ts — read latest 100 is O(1), no K-way merge needed in steady state."
- "K-way merge appears for pull-mode subscribers + at the unified cross-source query layer."
```
