# SoFi System Design #7 — Distributed Rate Limiter

> **业务背景**：API Gateway 防 abuse / DDoS / fair use；fintech 对接外部 partner 时的 quota 管理
> **频率**：System Design 中题，常作为 #2 Payment / #6 Fraud 的子组件
> **时间**：30-45 分钟（中等深度）

---

## 1. 需求澄清

### Functional Requirements
- 限制每个 user / IP / API key 在时间窗口内的请求数
- 支持多种规则：
  - Per-user：1000 req/hour
  - Per-IP：100 req/min（防 DDoS）
  - Per-API：specific endpoint 严格限流（如 `/login` 5 req/min 防 brute force）
  - Per-tier：free 用户 vs premium 用户不同 quota
- 支持多种算法：fixed window / sliding window / token bucket / leaky bucket
- 限流命中时：返回 **429 Too Many Requests** + `Retry-After` header

### Non-Functional Requirements
- **延迟**：每次检查 P99 < 5ms（每个 API call 都要过）
- **吞吐**：100K req/s 全局
- **准确性**：偶尔多放过几个可接受（不是钱），但不能严重超限
- **分布式一致**：多个 API gateway 实例共享 limit
- **可用性**：99.99%（rate limiter 挂不能阻塞所有 API）

### 关键澄清
> "Strict count or approximate?"
> → 通常 approximate 即可。Strict 要 distributed lock，性能差。

> "Rate limiter 挂了怎么办？"
> → Fail open（放行）vs fail closed（全拒）？大多数选 fail open，可用性优先。

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| API 总 QPS | 100K |
| Unique users 同时活跃 | 1M |
| Rate limiter 决策/s | 100K |
| Redis ops/s | 100K（每 decision 1-2 ops） |
| 内存 (per user counter) | ~100 bytes × 1M = 100 MB |

---

## 3. 算法选型对比

### 3.1 Fixed Window Counter
**思路**：每个时间窗口（如 1 分钟）一个计数器，超过即拒。

**优点**：实现简单，O(1) 内存
**缺点**：边界 burst 问题（窗口切换瞬间允许 2x 流量）

```python
key = f"rl:{user}:{minute_bucket}"
count = redis.incr(key)
if count == 1: redis.expire(key, 60)
if count > LIMIT: return DENY
return ALLOW
```

### 3.2 Sliding Window Log
**思路**：保存所有请求时间戳，删过期的，统计窗口内数量。

**优点**：精确
**缺点**：内存大（每请求 1 条记录），适合低 QPS

```
key = "rl:user_123"
ts = now()
redis.zadd(key, ts, ts)              # ZSet: score = ts, value = ts
redis.zremrangebyscore(key, 0, ts - 60)  # 移除 60s 前的
count = redis.zcard(key)
if count > LIMIT: return DENY
```

### 3.3 Sliding Window Counter（推荐）
**思路**：当前 window 计数 + 上一 window 加权外推。

```
total = current_count + previous_count × (1 - elapsed_in_current / window_size)
```

**优点**：精度好 + 内存 O(1)
**缺点**：稍复杂，仍是近似

### 3.4 Token Bucket（推荐 for API quota）
**思路**：桶以固定速率加 token；每请求消耗 1 个；桶空则拒。

**优点**：
- 允许 burst（用积累的 token）
- 平滑，业务友好
- 适合"每秒 X 次平均，但允许短时间超出"

**实现**（Redis Lua 原子）：
```lua
-- KEYS[1] = "rl:user_123"
-- ARGV[1] = capacity (e.g., 100)
-- ARGV[2] = refill_rate (tokens/sec)
-- ARGV[3] = now (timestamp)

local data = redis.call('HMGET', KEYS[1], 'tokens', 'last')
local tokens = tonumber(data[1]) or capacity
local last = tonumber(data[2]) or now

-- 补充 token
local elapsed = now - last
tokens = math.min(capacity, tokens + elapsed * refill_rate)

if tokens >= 1 then
  tokens = tokens - 1
  redis.call('HMSET', KEYS[1], 'tokens', tokens, 'last', now)
  redis.call('EXPIRE', KEYS[1], 3600)
  return 1  -- ALLOW
else
  return 0  -- DENY
end
```

### 3.5 Leaky Bucket
**思路**：固定速率出队，请求入队，队满则拒。

**优点**：严格控制输出速率
**缺点**：突发流量不被允许（适合下游需要平滑流量的场景）

---

## 4. High-Level Architecture

```
┌──────────┐     ┌─────────────────┐
│ Clients  │────▶│  API Gateway    │
└──────────┘     │ (Kong/Envoy/    │
                 │  自研 Spring)   │
                 └────────┬────────┘
                          │
                ┌─────────┴─────────┐
                ▼                   ▼
       ┌─────────────────┐   ┌──────────────┐
       │ Rate Limiter    │   │  Local Cache │  ← optional fast path
       │ Middleware      │   │  (Caffeine)  │
       └────────┬────────┘   └──────────────┘
                │
                ▼
       ┌─────────────────┐
       │ Redis Cluster   │  (sharded by key)
       │ (Lua scripts)   │
       └─────────────────┘
                ▲
                │ async metrics
                │
       ┌─────────────────┐
       │ Config Service  │  ← rules / quotas
       │ (Postgres)      │
       └─────────────────┘
```

---

## 5. 核心组件详解

### 5.1 API Gateway 集成
**Spring Boot Filter / Interceptor**：
```java
@Component
public class RateLimitFilter implements Filter {
    @Autowired RateLimitService rl;

    public void doFilter(req, res, chain) {
        String userId = extractUser(req);
        String endpoint = req.getURI();

        Decision d = rl.check(userId, endpoint);
        if (d == DENY) {
            res.setStatus(429);
            res.setHeader("Retry-After", "60");
            return;
        }
        chain.doFilter(req, res);
    }
}
```

或用现成方案：
- **Kong** with `rate-limiting` plugin
- **Envoy** with `envoy.filters.http.local_ratelimit` + `global_ratelimit` (RLS)
- **Cloud-native**: AWS API Gateway throttling、Cloudflare Rate Limiting

### 5.2 Redis 设计
**Cluster mode**：
- 16384 slots，按 user_id 一致性 hash 分片
- 多个分片 horizontal scale 到 100K+ ops/s

**Key 格式**：
```
rl:user:{user_id}:{endpoint_group}     # 用户级
rl:ip:{ip_hash}                         # IP 级
rl:apikey:{key_id}                      # API key 级
```

**Atomic 保证**：用 Lua script，整个 read-modify-write 原子

### 5.3 Local Cache（性能优化）
**问题**：每个 request 走 Redis 是 1ms 网络 RTT，叠加起来不便宜。

**优化**：local in-memory token bucket（Caffeine）+ 周期同步 Redis。

**两层设计**：
- L1（local）：每个 gateway 实例本地 cache，O(1) 查询
- L2（Redis）：全局协调

**同步策略**：
- 每秒 batch 上报 local 计数到 Redis
- Redis 返回全局视角，local 校准
- 容忍短期不一致（< 1s）换取低延迟

**适用场景**：高 QPS、单 user 流量大（同一 user 多请求都打到同一 gateway pod 时收益高）

### 5.4 Config Service
**动态配置**：
```sql
CREATE TABLE rate_limit_rules (
  rule_id UUID PRIMARY KEY,
  scope VARCHAR(20),       -- USER/IP/APIKEY/ENDPOINT
  match_pattern VARCHAR(255),  -- regex or exact
  algorithm VARCHAR(20),   -- TOKEN_BUCKET/SLIDING_WINDOW
  limit_value INT,
  window_seconds INT,
  burst INT,
  priority INT,
  enabled BOOLEAN
);
```

- Gateway 启动时拉规则到内存
- 配置变更通过 Kafka pub/sub 推送（5s 内全集群生效）
- 紧急情况可以 hot patch（例：发现攻击立即下调某 IP limit）

### 5.5 Multi-tier 支持
不同用户层级不同 limit：
```
free user:    100 req/hour
premium:      10000 req/hour
internal:     unlimited
```
通过 user 属性 join 规则；user→tier 缓存在 JWT claim，避免每次查 DB。

---

## 6. 关键问题与权衡

### Q1: Fixed window 边界 burst 怎么解决？
**问题**：limit 100/min，用户 12:59:59 发 100 个，13:00:00 发 100 个 → 实际 1s 内 200 个

**解决**：
- 用 sliding window counter（最常见）
- 或 token bucket（天然平滑）

### Q2: Rate limiter 挂了怎么办？
**Fail open（推荐 default）**：
- Redis 不可达 → 放行（业务可用 > 限流准确）
- Circuit breaker：连续失败后跳过 rate limit
- 有上限：放行不能无限，加全局 hard cap

**Fail closed**（特殊场景）：
- `/login` 等高敏感 API → 拒绝放行（防 brute force）

### Q3: 如何处理大客户突增流量？
- **Per-customer override**：custom limit
- **Pre-warming**：客户告知大促，临时提 quota
- **Tiered queueing**：low priority 进队列等，不直接拒（throttling 而非 limiting）

### Q4: 怎么区分合法 burst 和恶意？
- Token bucket 允许合理 burst（capacity 设大一些）
- 异常 pattern：连续 burst（不像 bot 就像 bot）→ 触发 anti-bot challenge (Captcha)
- 与 Fraud Detection (#6) 联动

### Q5: 全球部署如何同步限流？
**问题**：用户在不同 region 都打 API，每 region 独立限流可能总量超 limit。

**方案**：
- 简单：每 region 独立 limit（容易实施，可能超 quota）
- 严格：global Redis（高延迟，跨 region 50ms+） → 不实用
- 折中：region 间异步对账 + global cap 软约束

### Q6: 如何 monitor 限流效果？
- 限流命中率（429 返回比例）by user/endpoint
- Top offenders（持续被限的 IP/user）
- 误杀（被限但实际是合法用户）→ 申诉率
- Performance：rate limiter latency 自身 P99

### Q7: 如何处理时钟漂移？
- 多 gateway 实例时钟可能不一致
- 用 Redis 的 TIME 命令统一时间源（lua 内 `redis.call('TIME')`）

---

## 7. 高级话题

### Distributed sliding window 优化
Cloudflare 的实现：
- 当前 window count + 前一 window count × (1 - position_in_current_window)
- 内存 O(1)，精度 > 99%

### Adaptive rate limiting
- 监控后端服务负载
- 自动调整 limit（保护下游）
- 类似 Adaptive Concurrency Limit (Netflix's library)

### 与 Circuit Breaker 配合
- Rate limit 在前（防 abuse）
- Circuit breaker 在后（防 cascade failure）
- 两者互补：limit 是 rate-based，CB 是 error-based

### 与 Quota 区别
- **Rate limit**：短窗口（秒/分钟），防 abuse
- **Quota**：长周期（天/月），账单 / 商业模型
- 经常一起做（Stripe API 同时有 RPS limit 和 monthly quota）

### Anti-DDoS layer
- L4: SYN flood, UDP flood → 用 AWS Shield / Cloudflare
- L7: Application layer → Rate limiter + bot detection
- 两层结合（不要在 L7 处理大流量 DDoS）

---

## 8. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Redis cluster 挂 | 限流失效 | Fail open + alert ops |
| Lua script bug | 行为异常 | 灰度发布，回滚机制 |
| Config service 挂 | 新规则推不下去 | 用旧规则继续运行 |
| Local cache 漂移 | 限流不准 | 缩短同步间隔（cost trade-off） |

---

## 9. 监控

- **Limited rate** by user/IP/endpoint
- **Latency** P99（不能拖慢 API）
- **Redis hit rate**
- **False positive rate**（合法用户被限）
- **Top throttled clients**（识别 abuse / 误配置）

---

## 10. 5分钟版本

```
Request → Gateway → Rate Limit Filter
                          ↓
                    Local Token Bucket (cache)
                          ↓ (miss/sync)
                    Redis Lua (atomic)
                          ↓
                    Allow / 429
```

**核心三句话**：
- "Token bucket for API quotas — allows controlled bursts."
- "Redis + Lua for atomic distributed counter — sync 是错的，atomic 是对的."
- "Fail open by default — availability > strict limiting."
