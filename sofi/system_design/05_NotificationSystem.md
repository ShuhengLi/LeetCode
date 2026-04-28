# SoFi System Design #5 — Notification System

> **业务背景**：所有 SoFi 产品都需要通知（loan status, payment due, fraud alert, marketing）
> **频率**：System Design 高频中题（fintech / 大平台必考）
> **时间**：45-60 分钟

---

## 1. 需求澄清

### Functional Requirements
- 多渠道：**Push (FCM/APNs)**, **SMS**, **Email**, **In-app**
- 多场景：
  - Transactional（payment confirm, fraud alert）— 必送达
  - Status updates（loan approved, statement ready）
  - Marketing（promotions, news）— 可丢
- 用户偏好：opt-in/out per channel per category
- Quiet hours / 时区
- Templating（多语言、个性化）
- Scheduling（延后发送）
- **Idempotency**（同一通知不重复）

### Non-Functional Requirements
- **吞吐**：peak 100K notif/s（marketing campaign）
- **延迟**：transactional P99 < 5s；marketing 可分钟级
- **送达率**：> 99% transactional
- **可用性**：99.95%
- **合规**：CAN-SPAM, TCPA（SMS opt-in 强制）, GDPR

### 关键澄清
> "Send to one user or broadcast to many?" → 都要。Single-user (transactional) + segment (marketing campaigns)
> "Sender retry on failure?" → Yes，但要避免无限重试 spam 用户

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| 用户数 | 10M |
| 平均 notif/user/day | 5 → 50M/day |
| 峰值 TPS | 100K（marketing burst） |
| Transactional/day | 5M |
| Avg payload size | 2 KB |
| Daily data | 100 GB |

---

## 3. High-Level Architecture

```
   ┌────────────────────────────────────────────┐
   │   Producer Services                         │
   │  (Loan, Payment, Fraud, Marketing, etc.)    │
   └──────────────────────┬─────────────────────┘
                          │ HTTP / gRPC
                          ▼
                ┌───────────────────────┐
                │ Notification API      │
                │  (validation, dedup)  │
                └──────────┬────────────┘
                           │
                           ▼
                ┌───────────────────────┐
                │   Kafka               │
                │  notif.{priority}     │
                └─┬───────┬──────┬──────┘
                  │       │       │
            (high) │  (med)│  (low)│
                  ▼       ▼       ▼
            ┌─────────────────────────┐
            │  Notification Workers   │
            │  - Pref check           │
            │  - Template render      │
            │  - Channel routing      │
            └────┬──────┬──────┬──────┘
                 │      │      │
                 ▼      ▼      ▼
            ┌──────┐┌──────┐┌──────┐
            │ Push ││ SMS  ││Email │ Adapters
            │FCM/  ││Twilio││ SES  │
            │APNs  ││      ││      │
            └──┬───┘└──┬───┘└──┬───┘
               │       │       │
               ▼       ▼       ▼
            (User devices / phones / inboxes)

         ┌──────────────────────────┐
         │ Status Tracker (DB)      │
         │ - delivered/failed/read  │
         └──────────────────────────┘
```

---

## 4. 核心组件详解

### 4.1 Notification API
**Endpoint**：
```
POST /notifications
{
  "notification_id": "uuid",        // idempotency
  "user_id": "u_123",
  "category": "PAYMENT_CONFIRM",    // 决定优先级 / 模板
  "priority": "HIGH",
  "channels": ["PUSH", "EMAIL"],    // 可省略，默认按用户偏好
  "data": {
    "amount": 100.50,
    "merchant": "Starbucks"
  },
  "send_at": "2026-04-27T10:00:00Z" // 可选 schedule
}
```

**职责**：
1. Schema 验证
2. Idempotency check（Redis SETNX on `notification_id`）
3. 路由到对应 priority Kafka topic
4. 写 status DB（PENDING）
5. 立即返回 ack

### 4.2 Kafka Topics 设计
**按优先级分 topic**：
- `notif.transactional` — 高优，多 partition、多 consumer
- `notif.status` — 中优
- `notif.marketing` — 低优，可被节流 / 限速

**为什么分 topic？**
- 防止 marketing 大量消息阻塞 transactional
- 不同 SLA（高优要求秒级，低优分钟级）
- Consumer 资源独立分配

**Partition 策略**：
- Transactional：by `user_id`（保证同用户消息顺序）
- Marketing：随机（最大并行度）

### 4.3 Notification Workers
**核心流程**：
```
1. Consume from Kafka
2. Load user preferences (Redis cache, fallback Postgres)
3. Apply filters:
   - Quiet hours? → reschedule
   - Opted-out for this category? → drop + log
   - Rate limit (no more than 5 marketing/day)? → drop or queue
4. Render template (Handlebars / Freemarker)
5. Route to channel adapter
6. Update status DB
```

**伪代码**：
```java
public void process(NotificationEvent event) {
    var prefs = userPrefService.get(event.userId);

    if (!prefs.allowsCategory(event.category)) {
        statusDb.update(event.id, "FILTERED_OPTOUT");
        return;
    }

    if (prefs.isQuietHours() && !event.isUrgent()) {
        scheduler.scheduleFor(event, prefs.nextActiveTime());
        return;
    }

    var rendered = templateEngine.render(event.template, event.data, prefs.locale);

    for (var channel : event.channels) {
        adapter(channel).send(prefs.contactInfo(channel), rendered);
        statusDb.update(event.id, channel, "SENT");
    }
}
```

### 4.4 Channel Adapters
**Push（FCM / APNs）**
- 维护 device token 表
- Token 过期处理（FCM 返回 invalid → 删 token）
- 批量 send（最大 500/请求）

**SMS（Twilio）**
- 按 phone number 国家路由（SoFi 主要 US）
- TCPA 合规：必须有 opt-in 记录
- 成本敏感（每条 $0.0075） → 严控触发

**Email（SES / Sendgrid）**
- HTML + plain text 双版
- Bounce / complaint 处理（webhook）
- DKIM/SPF 配置

**In-app**
- 写入 Postgres `inbox` 表
- 用户开 app 时拉取
- 用 WebSocket 实时推送（in-app banner）

### 4.5 User Preference Service
**Schema**：
```sql
CREATE TABLE user_notif_prefs (
  user_id BIGINT,
  category VARCHAR(40),
  channel VARCHAR(20),  -- PUSH/SMS/EMAIL/INAPP
  enabled BOOLEAN,
  PRIMARY KEY (user_id, category, channel)
);

CREATE TABLE user_notif_settings (
  user_id BIGINT PRIMARY KEY,
  quiet_hours_start TIME,
  quiet_hours_end TIME,
  timezone VARCHAR(40),
  locale VARCHAR(8),
  email VARCHAR(255),
  phone VARCHAR(20),
  ...
);

CREATE TABLE device_tokens (
  user_id BIGINT,
  device_id VARCHAR(64),
  fcm_token VARCHAR(255),
  apns_token VARCHAR(255),
  last_seen TIMESTAMP,
  PRIMARY KEY (user_id, device_id)
);
```

**Cache**：Redis `user_prefs:{user_id}` JSON，TTL 1h，用户改设置时主动 invalidate

### 4.6 Status Tracker
**Schema**：
```sql
CREATE TABLE notification_status (
  notification_id UUID PRIMARY KEY,
  user_id BIGINT NOT NULL,
  category VARCHAR(40),
  status VARCHAR(20),  -- PENDING/SENT/DELIVERED/FAILED/READ/FILTERED
  channels JSONB,      -- per-channel status
  sent_at TIMESTAMP,
  delivered_at TIMESTAMP,
  read_at TIMESTAMP,
  failure_reason TEXT
);

CREATE INDEX idx_user_time ON notification_status (user_id, sent_at DESC);
```

- Webhook 回调更新 delivered/read（FCM/APNs/SES 都有）
- 7-30 天后归档到 cold storage

### 4.7 Scheduler
- 用户偏好 quiet hours / 时区
- Marketing 按用户活跃时间发（提高打开率）
- 实现：Redis Sorted Set，score = unix timestamp，定时 worker 拉取
  - `ZRANGEBYSCORE` + `ZREMRANGEBYSCORE` 原子操作

---

## 5. 关键问题与权衡

### Q1: 如何防止重复发送？
**多层 dedup**：
1. API 层：`notification_id` 是 idempotency key（Redis SETNX）
2. Worker 层：处理前查 status DB，已 SENT 则跳过
3. 消息层：Kafka exactly-once（producer + consumer）

### Q2: Marketing campaign 100K msg/s 怎么扛？
- Producer 端：批量 publish（每批 1K msg）
- Kafka：marketing topic 200 个 partition
- Workers：水平扩展，每 worker 5K msg/s
- Adapter：批量 API（FCM/SES 支持）

### Q3: 如何 fan-out 给 1M 用户的 segment？
- 不要在 API 层展开！
- 用专门的 **Campaign Service**：
  - 接收 segment query
  - 异步从用户 DB 流式拉取
  - 分批投递到 Kafka

### Q4: 如何确保 transactional 不被 marketing 阻塞？
- 物理隔离：不同 topic + 不同 consumer group + 不同 worker pool
- 优先级队列：worker 内优先消费 high priority
- 资源 quota：marketing worker CPU/内存上限

### Q5: 用户偏好 cache 一致性？
- 改设置 → 写 DB → 删 Redis → 发 invalidation event
- Worker 读时 cache miss → 从 DB 加载 → 写回 cache
- 容忍 1 秒级延迟（用户改完设置不会立即触发通知）

### Q6: 如何处理跨时区？
- 用户偏好存 timezone（IANA）
- Quiet hours 用本地时间
- Scheduler 转 UTC 排队，发送前再转用户时区检查

### Q7: 如何防止 spam abuse？
- Per-user rate limit：marketing < 5/day
- Per-category 频次：fraud alert 不限，promotional 限
- Engagement-based：用户连续不开 → 降权
- Unsubscribe 一键退订（CAN-SPAM 强制）

### Q8: Failure handling
**Channel failure 处理**：
- FCM 暂时挂：Kafka offset 不提交，自动重试
- 用户 token 失效：标记 invalid，不再发
- SMS 失败：切 fallback channel（push 替代）
- 持续失败 > 3 次 → 写 dead letter queue 人工处理

---

## 6. 数据一致性

| 数据 | 一致性 |
|------|--------|
| Notification request | 强（idempotency_key UNIQUE） |
| User preferences | 最终（cache invalidation 几秒） |
| Status tracking | 最终（webhook 异步） |
| Device tokens | 最终（FCM 反馈） |

---

## 7. 监控

### 关键指标
- **TPS** by category & channel
- **End-to-end latency** P99 (API → 用户设备)
- **Delivery rate** per channel（< 95% 触发告警）
- **Bounce / complaint rate** for email
- **Cost per notification**（SMS 贵）
- **CTR** for marketing（业务指标）

### 关键告警
- Transactional success rate < 99% → P0
- Worker lag > 30s → P1
- Email bounce rate > 5% → P2（IP reputation 风险）
- Single-user > 100 notif/hour → P2（abuse / bug）

---

## 8. 进阶讨论

### Smart batching for push
- 短时间多个通知合并（"You have 3 new transactions"）
- 减少打扰，提高用户体验
- 实现：window 30s，buffer per user，window 结束 flush

### A/B testing
- 同一通知多个模板，按用户分组
- 收集 CTR / 转化率
- 优胜模板逐步 rollout

### Personalization with ML
- 最佳发送时间预测（用户活跃模式）
- 通道偏好预测（哪个渠道开率高）
- Content 个性化（哪类内容用户更可能点）

### Email reputation 管理
- 多 sending IP，按 reputation 分流
- Warmup 新 IP（缓慢增加发送量）
- Engagement-based filtering（cold list 不发，避免 spam 标记）

### Compliance dashboard
- TCPA：所有 SMS 必须有 opt-in 时间戳 + IP 记录
- CAN-SPAM：unsubscribe 24h 内生效
- GDPR：用户 right-to-be-forgotten → 7 天内删除所有 PII

### Notification 与其他系统集成
- Fraud alert → 优先级飙升 + 多渠道送达
- Loan approved → 触发 #4 系统的 esign 流程
- Payment failed → 触发 retry + customer support 工单

---

## 9. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| FCM 全挂 | Push 不可达 | 自动 fallback 到 in-app + email |
| Twilio 限流 | SMS 延迟 | 多 provider（Twilio + Bandwidth）切换 |
| Kafka 挂 | 全停 | Multi-AZ；本地缓冲 1 小时 |
| Worker OOM | 延迟堆积 | Auto-scale + DLQ for poison messages |
| 用户偏好 DB 挂 | 全发出（默认开） vs 全不发？ | 选保守：只发 transactional（避免 spam） |

---

## 10. 5分钟版本

```
Producer → Notif API (idempotent) → Kafka (priority topics)
                                         ↓
                                   Workers (pref filter + render)
                                         ↓
                              ┌──────────┼──────────┐
                              ▼          ▼          ▼
                            Push       SMS       Email
                              ↓          ↓          ↓
                           User devices / phones / email
                                      ↓
                              Status Tracker (delivered/read)
```

**核心三句话**：
- "Idempotency at API layer is non-negotiable — duplicates are a worse bug than missed sends in fintech."
- "Separate priority topics — never let marketing block transactional."
- "User preferences + quiet hours + opt-out are first-class citizens — compliance failure is fatal."
