# Atlassian SD #3 — Unified Tagging System (Jira/Confluence/Bitbucket)

> **真题来源**：Hello Interview / Atlassian onsite 报告（2024-2025）— 真题原题
> **原题**：*"Design a unified tagging system that allows users to add/remove/update tags on any content (Jira tickets, Confluence pages, Bitbucket PRs)"*
> **时间**：45-60 分钟

---

## 1. 需求澄清

### Functional Requirements
- 用户可以**给任何内容打 tag**（Jira issue、Confluence page、Bitbucket PR）
- 同一个 tag 可以跨产品共享（"release-2026-Q2" 在多个 issue + page 上）
- **Search by tag**：返回所有带此 tag 的内容（across products）
- **Tag autocomplete**：输入时建议已有 tag
- **Tag management**：rename、merge、delete
- **Permission-aware**：用户只能看到自己有权访问的内容

### Non-Functional Requirements
- **规模**：100M items, 10M users, 50M unique tags
- **写入**：1K tag-ops/s
- **读**：10K tag queries/s（含 autocomplete）
- **延迟**：search by tag P99 < 200ms；autocomplete < 50ms
- **可用性**：99.9%
- **一致性**：最终一致（标签变化 < 5s 全局可见）

### 关键澄清
> "Are tags global (cross-tenant) or per-tenant?"
> → 通常 per-tenant（per-org），不同公司的 tag 不共享

> "Can tags have hierarchies / colors / metadata?"
> → 起步：纯字符串 + 颜色；MVP 不做层级

> "How are tags scoped — by space/project/repo or org-wide?"
> → 推荐 org-wide（用户在该 org 内可复用），但带 source product hint

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| Items 总数 | 100M（all products） |
| Unique tags | 50M |
| 平均 tags per item | 5 |
| 总 tag-item 关系 | 500M |
| Tag op TPS（write） | 1K |
| Tag query TPS（read） | 10K |
| Autocomplete QPS | 5K |
| Storage（关系） | 500M × 30B = 15 GB |

---

## 3. High-Level Architecture

```
                       ┌───────────────────┐
                       │   Web / Mobile    │
                       └─────────┬─────────┘
                                 ▼
                       ┌───────────────────┐
                       │   API Gateway     │
                       └─────────┬─────────┘
                                 ▼
                       ┌───────────────────┐
                       │  Tagging Service  │
                       │  (Spring Boot)    │
                       └──┬───────┬────┬───┘
                          │       │    │
              ┌───────────┘       │    └────────────┐
              ▼                   ▼                 ▼
        ┌──────────┐       ┌──────────┐      ┌──────────────┐
        │PostgreSQL│       │  Kafka   │      │   Redis      │
        │(canonical│       │ (events) │      │ (autocomplete│
        │  data)   │       │          │      │  + hot cache)│
        └──────────┘       └────┬─────┘      └──────────────┘
                                │
                  ┌─────────────┼──────────────┐
                  ▼             ▼              ▼
           ┌──────────┐  ┌──────────┐  ┌──────────────────┐
           │  ES      │  │ Counters │  │ Permission       │
           │(reverse  │  │ (Redis)  │  │ Cache Invalidator│
           │ index by │  │  tag freq│  │                  │
           │  tag)    │  │          │  │                  │
           └──────────┘  └──────────┘  └──────────────────┘

  Item content lives in Jira/Confluence/Bitbucket — tagging service only
  stores metadata (productType, itemId, tag) and joins on read.
```

---

## 4. Data Model

### Canonical schema (Postgres)
```sql
-- Tag entity (per-tenant)
CREATE TABLE tags (
  id BIGSERIAL PRIMARY KEY,
  tenant_id BIGINT NOT NULL,
  name VARCHAR(100) NOT NULL,
  color VARCHAR(8),                       -- hex
  created_by BIGINT,
  created_at TIMESTAMP DEFAULT NOW(),
  UNIQUE (tenant_id, name)                -- per-tenant uniqueness
);

CREATE INDEX idx_tags_tenant_name ON tags (tenant_id, name);

-- Many-to-many: which items have which tags
CREATE TABLE item_tags (
  tenant_id BIGINT NOT NULL,
  product VARCHAR(20) NOT NULL,           -- 'JIRA' | 'CONFLUENCE' | 'BITBUCKET'
  item_id VARCHAR(64) NOT NULL,           -- product-specific id
  tag_id BIGINT NOT NULL REFERENCES tags(id),
  added_by BIGINT,
  added_at TIMESTAMP DEFAULT NOW(),
  PRIMARY KEY (tenant_id, product, item_id, tag_id)
);

-- Reverse index: query "give me all items with tag X"
CREATE INDEX idx_item_tags_tag ON item_tags (tag_id, product, item_id);
-- Forward: "what tags does this item have"
CREATE INDEX idx_item_tags_item ON item_tags (product, item_id);
```

### Sharding
- By `tenant_id`（一致性 hash）
- Hot tenants 单独 shard
- 跨 shard 查询很少（tag 是 tenant-internal）

---

## 5. 核心 API 设计

```
POST /tags
  Body: { name, color }
  Response: { tagId }

POST /items/{product}/{itemId}/tags
  Body: { tagIds: [1, 2] }
  Response: 204

DELETE /items/{product}/{itemId}/tags/{tagId}
  Response: 204

GET /items/{product}/{itemId}/tags
  Response: [{ id, name, color }, ...]

GET /tags/{tagId}/items?product=JIRA&page=1&size=50
  Response: [{ product, itemId, snippet }, ...]
  // ★ Permission-filtered

GET /tags/autocomplete?prefix=re&limit=10
  Response: [{ id, name }, ...]
  // ★ tenant-scoped, sorted by usage frequency

PUT /tags/{tagId}/rename
  Body: { newName }

POST /tags/merge
  Body: { sourceIds: [1, 2], targetId: 3 }
```

---

## 6. 关键问题与权衡

### Q1: 为什么 ES 而不是只用 Postgres？
- Postgres 能撑日常查询（reverse index 已建）
- ES 优势：
  - 复杂查询（多 tag AND/OR）
  - 全文搜索 + tag 联合搜索（"找带 'urgent' 标签且包含 'login' 的 issue"）
  - Autocomplete（更高效的 prefix search）
- **取舍**：起步只用 Postgres；扩展到大规模 + 复杂 search 时引入 ES

### Q2: Permission 怎么处理？
**关键陷阱**：tagging service 不是权限源头！
- Tag DB 不存权限
- Search by tag 返回 itemIds → 再调对应产品的 permission API 过滤
- 性能：批量调用 + cache

**优化**：
- 在 tag service 缓存 (userId, itemId) → can_view（短 TTL）
- Permission 变化通过 Kafka event 失效 cache

### Q3: Autocomplete 怎么做？
- Postgres prefix query: `WHERE name LIKE 'pre%'` + GIN index `gin_trgm_ops`
- 更优：Redis Sorted Set，score = usage frequency
  - Key: `tag-prefix:{tenantId}:{prefix}`
  - 不存所有 prefix（爆炸）；用 Trie 或 Elasticsearch completion suggester

### Q4: 同时多人加 tag 冲突？
- 主键 `(tenant, product, itemId, tagId)` UNIQUE → INSERT ON CONFLICT DO NOTHING
- 幂等 ✅

### Q5: 跨产品搜索（e.g., 给我所有 Q2-release tag 的所有内容）？
- ES 单 index 存所有 product 的 item-tag 关系
- 查询：`{ "term": {"tag": "Q2-release"}, "filter": [permission_filter] }`
- 分页 + 多产品聚合返回

### Q6: Tag rename / merge 一致性？
**Rename**: 简单 — UPDATE tags SET name = ?；item-tag 关系不变（用 tag_id）
**Merge**: 复杂
1. UPDATE item_tags SET tag_id = target WHERE tag_id IN (sources)
2. ON CONFLICT 时 DO NOTHING（已有 target 不重复）
3. DELETE source tags
4. Publish events to invalidate caches

包在 transaction 里（单 tenant 单 shard 可行）。

### Q7: 50M unique tags 是否过多？
- 通常 long-tail（少数热门 + 大量低频）
- 老的低频 tag 可定期回收（无引用 + > 90 天 → soft delete）
- 防止 user 创造垃圾 tag → rate limit + minimum-length

---

## 7. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Postgres 挂 | 写中断 | failover；ES 提供降级 read |
| ES 挂 | 搜索差 | 降级到 Postgres reverse index |
| Permission service 慢 | search by tag 慢 | 缓存 hot 权限；超时降级（保守只显示安全集） |
| Kafka 挂 | 索引/失效不及时 | 几秒内恢复；用户感知 stale |

---

## 8. 监控

- **Tag write/read TPS**
- **Search latency** P50/P99
- **Autocomplete latency** P99
- **Permission filter cost**（每个 search 平均多少 perm check）
- **Tag count growth**（异常激增可能是 abuse）
- **Hot tag**（被频繁查询的 → 加缓存）

---

## 9. 进阶讨论

### Hierarchy / Categories
- 父子关系：parent_tag_id
- "release/Q2-2026" 自动包含 "release"
- 实现：closure table 或 nested set

### AI / 自动 tag
- ML 模型基于内容自动建议 tag
- 用户接受 → 实际打 tag；记反馈用于训练

### Multi-region
- Per-region Postgres shard
- 跨 region search：fan-out 到所有 region，聚合
- ES global index（write fan-out + read merge）

### 与 Atlassian Connect 集成
- 第三方 app 可读写 tag（OAuth scope）
- Webhook："item tagged" event

### Semantic search
- Tag embedding（vector）→ "similar tags"
- Pinecone / FAISS 加速

---

## 10. 5分钟版本

```
API Gateway → Tagging Service
                  ├→ Postgres (canonical: tags + item_tags, sharded by tenant)
                  ├→ Redis (autocomplete: ZSET by usage freq)
                  ├→ ES (reverse index for cross-product search)
                  ├→ Kafka (events: tag added/removed)
                  └→ Permission Cache (hot decisions, TTL'd)

Read by tag → ES query → permission filter (call product APIs) → return items
```

**核心三句话**：
- "Tagging is metadata about content owned elsewhere — service stores (product, itemId, tagId), never the content."
- "Permission lives in source products — tag service post-filters; never store auth state here."
- "Three indices (Postgres canonical, Redis autocomplete, ES cross-product) — each chosen for one query type."
