# Atlassian SD #1 — Confluence Real-Time Collaborative Editing

> **真题来源**：Atlassian Senior/Staff onsite 报告（2024-2025），最高频 SD 题
> **业务关联**：Confluence 实际产品（核心场景）
> **时间**：45-60 分钟

---

## 1. 需求澄清（前 5 分钟必问）

### Functional Requirements
- 多人**同时编辑**同一文档（页面）
- 实时看到他人光标位置 + 编辑内容（< 100ms）
- 支持**离线编辑**，恢复连接后同步
- 历史版本（page history）+ 任意版本回滚
- 评论 / @mention（次要）

### Non-Functional Requirements
- **规模**：100M docs, 10M DAU；单文档峰值并发 50 人编辑
- **延迟**：键入 → 协作者看到 < 100ms（同地域）
- **一致性**：最终一致，但冲突必须正确解决
- **可用性**：99.9%
- **持久性**：编辑不丢（绝不容忍）

### 关键澄清
> "Document size — small wiki page or huge collaborative spreadsheet?"
> → Confluence 是结构化文档（rich text），单页一般 < 100KB

> "Hard real-time (Google Docs-style) or soft (autosave every few seconds)?"
> → Confluence 走 hard real-time，类似 Google Docs

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| 文档数 | 100M |
| DAU | 10M |
| 并发编辑会话 | 50K |
| 编辑事件/秒（peak） | 200K (avg 4 ops/user/sec) |
| 单文档大小 | ~50 KB (avg) |
| 历史版本（粒度） | 每秒一个 snapshot 太多；每分钟或每显著变化 |
| 操作日志 | 100M docs × ~1000 ops × 50B = 5 TB |

---

## 3. Core Algorithm: OT vs CRDT — **必谈的 trade-off**

| | **OT (Operational Transform)** | **CRDT (Conflict-free Replicated Data Type)** |
|---|---|---|
| 原理 | 中央服务器对每个 op 做转换以适配新基线 | 每个 op 携带元数据（id, vector clock）；本地合并即正确 |
| Source of truth | **必须有中央服务器** | 完全去中心化即可工作 |
| 复杂度 | 数学复杂（操作转换矩阵），bug 多 | 数据结构本身保证正确，操作简单 |
| 网络 | 需要在线 + 顺序保证 | 离线友好，最终合并 |
| 内存 | 低（只存当前状态） | 高（每个 op 的 metadata 永久保留 → tombstones） |
| 实现难度 | 高 | 中 |
| 典型用户 | Google Docs（OT）、Confluence（OT） | Figma、Notion、Linear、Yjs/Automerge |

**Confluence 选 OT** — 原因：
- 中央 server 已有（不是去中心化产品）
- 文档规模较小，CRDT metadata overhead 不划算
- "lighter than CRDT, suitable for short-document collaboration"（来自 Atlassian 工程博客）

如果面试官问"现在重做你选哪个"：CRDT 更现代，离线/移动场景更强；但 OT 在 single-region 高并发场景延迟更低。

---

## 4. High-Level Architecture

```
┌──────────┐  WebSocket  ┌──────────────────┐
│ Client A │◀───────────▶│ Edge Gateway     │
└──────────┘             │ (sticky session) │
                         └────────┬─────────┘
┌──────────┐                      │
│ Client B │◀────────────────────▶│
└──────────┘                      ▼
                         ┌─────────────────────┐
                         │ Collab Service      │
                         │ - OT engine         │
                         │ - per-doc actor     │
                         │ - applies & broadcasts ops │
                         └────────┬──────┬─────┘
                                  │      │
                         ┌────────▼───┐ ┌▼─────────────┐
                         │ Op Log     │ │ Snapshot DB  │
                         │ (Kafka /   │ │ (PostgreSQL  │
                         │  append)   │ │  + S3 docs)  │
                         └────────────┘ └──────────────┘
                                  │
                         ┌────────▼───┐
                         │ Replay /   │
                         │ Snapshotter│
                         │ (every N   │
                         │  ops)      │
                         └────────────┘
```

---

## 5. 核心组件详解

### 5.1 Edge Gateway (WebSocket)
- Sticky session：同一个文档的所有客户端路由到同一个 Collab Service 实例
- TLS 终结、auth、rate limit
- 心跳每 30s，断线即推 user-presence-leave

### 5.2 Collab Service — 核心
**Per-document actor model**（每个被编辑的文档由一个进程内 actor 串行处理 ops）：
- 单线程顺序处理 → 不需要锁
- 内存中保存：当前文档状态 + 操作历史（最近 N ops）
- 接收客户端 op → OT transform → apply → broadcast

**OT 流程**（client-server 模型）：
```
Client                          Server
  |                                |
  | --- op (basis = v=10) -------> |
  |                                | Server is at v=12 (ops 11, 12 from others)
  |                                | Transform op against ops 11..12
  |                                | Apply transformed op → v=13
  | <--- ack(v=13) --------------- |
  | <--- broadcast op12, op13 ---> |  (other clients)
  |
  | (client locally updates basis to v=13)
```

**Actor 的扩展**：
- 用一致性哈希按 docId 路由到 N 个 Collab Service 实例
- 实例挂了 → 由另一台从 op log 重建状态

### 5.3 Op Log (Kafka)
- 所有 op append-only 写入，永远不删
- Topic partitioned by `docId`（同一文档的 op 严格有序）
- 用于：
  - 持久化（防止 actor 崩溃丢数据）
  - 重建 actor 状态
  - 历史回放（"page history"）
  - 离线客户端追赶

### 5.4 Snapshot DB
- 每 N 个 op 或 M 秒生成一个 snapshot（base state + version）
- 加载文档 = 最近 snapshot + 后续 op replay
- 大文档存 S3，小元数据 Postgres
- "Page history" UI 显示所有 snapshot；用户 revert = 创建新 snapshot 指向旧内容

### 5.5 Presence Service
- 谁在编辑、光标位置、selection
- 不持久化（Redis Pub/Sub 即可）
- 光标位置高频 → 客户端节流（每 50ms 一次）

---

## 6. 关键设计决策

### Q1: OT 转换函数的本质？
对于文本编辑：
```
op1: insert "X" at position 5
op2: insert "Y" at position 3  (concurrent)

After op2 applied first, re-transform op1:
op1' = insert "X" at position 6  (because Y pushed everything right)
```
精确实现取决于 op 类型（insert/delete/format/structure），是工程难点。

### Q2: 客户端如何处理网络延迟？
- **Optimistic UI**：本地立即应用，等 server ack
- 收到 server transform 结果 → 用 transformed op 重写本地（可能感觉到"撤销重做"，但人类察觉不到）

### Q3: 大量 op 时如何避免 actor OOM？
- 内存只保留最近 N 个 op
- 超过 → 触发 snapshot + 清空旧 op
- 客户端 basis 太老 → 强制 reload 整个文档

### Q4: 单 doc 50 人并发瓶颈在哪？
- Actor 是单线程，op rate 受限
- 50 × 4 ops/s = 200 ops/s → 单 actor 可处理（< 1 ms/op）
- 1000 人级别才需要分片或 read-only fan-out

### Q5: Reliability — actor 挂了？
- Op 已经在 Kafka，不会丢
- 新 actor 从 Kafka 读最新 snapshot offset → replay 后续 op
- 客户端短暂断开（< 5s）后自动重连

### Q6: 如何处理 schema 操作（移动段落 / 改 heading）？
- 不仅是文本 op，文档树 op
- 用 structural OT 或 tree-CRDT
- Confluence 用 ProseMirror（有内置 schema-aware ops）

### Q7: 移动端 / 离线？
- 客户端缓存最后 snapshot + queue 本地 pending ops
- 上线后 ops 一起发给 server，server 逐个 transform 后 apply
- 长时间离线（> 7 天）→ 强制冲突解决 UI（Git-like）

---

## 7. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Collab Service crash | 该文档编辑暂停 | 新实例从 Kafka 重建（< 5s） |
| Kafka 挂 | 不能持久化 | actor 内存中继续工作；恢复后 flush |
| WebSocket gateway 挂 | 客户端断连 | sticky DNS 切到另一台；client auto-retry |
| Postgres 挂 | 不能新建 doc / 历史查询 | 编辑继续（已加载文档不受影响） |
| Op Log 损坏 | 单个文档数据丢 | 从最近 snapshot 恢复（损失最近 N ops） |

---

## 8. 监控

- **Op latency** P50/P99（client → server → broadcast）
- **Actor lag**（op queue 长度）
- **Active concurrent docs / users**
- **Snapshot 频率** & 大小
- **WebSocket connection count** / pod
- **OT transform errors**（罕见但严重）

---

## 9. 进阶讨论

### Multi-region
- 主 region 写 + 跨 region 读复制
- 文档"主"在用户主 region；跨 region 协作转发
- Trade-off：低延迟 vs 跨大洋协作

### 关键路径优化
- 客户端 OT 在本地先做 transform → 减少 server RTT
- Pending ops queue + batching

### Confluence 与 Google Docs 的差异
- Confluence 文档结构化（macros、page properties）→ 不仅是文本
- 编辑频率低于 Google Docs（更多读，少写）
- 可缓存渲染好的 HTML（push invalidation on write）

### 与 Jira 的整合
- "Inline" issues 嵌入页面 → 协作时 issue 状态实时同步
- 用同一个 op 框架，type=embed-update

---

## 10. 5分钟版本（白板核心）

```
Client ─WS─▶ Edge ─▶ Collab Service (per-doc actor, OT engine)
                          │
              ┌───────────┼──────────┐
              ▼           ▼          ▼
           Kafka      Snapshot DB  Presence (Redis)
        (op log,     (every N ops)
         source of
         truth)
```

**核心三句话**：
- "OT (chosen by Confluence) requires central server but is lower latency for online collab; CRDT is for offline-first / decentralized."
- "Per-doc single-threaded actor — sequential consistency without locks; Kafka op log is the source of truth."
- "Snapshot every N ops to bound replay cost; client uses optimistic UI with server-driven transform replay."
