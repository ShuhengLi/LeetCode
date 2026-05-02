# Atlassian SD #5 — Bitbucket CI/CD Pipeline (Bitbucket Pipelines / Pipelines as Code)

> **真题来源**：Atlassian Senior/Staff onsite 报告（2024-2025）
> **业务关联**：Bitbucket Pipelines（实际产品）
> **时间**：45-60 分钟

---

## 1. 需求澄清

### Functional Requirements
- 用户在 repo 中提交 `bitbucket-pipelines.yml` 配置
- Push / PR 触发 pipeline 执行（多 stage：build / test / deploy）
- 每 step 在 isolated container 里跑用户代码
- 实时显示 build log / status
- 支持 cache、artifacts、secret 注入
- Manual approval gate（deploy 前需手动 approve）
- 历史记录 + 可重跑

### Non-Functional Requirements
- **规模**：1M repos, 10K builds/min（peak），单 build 5-30 分钟
- **延迟**：trigger → step 开始执行 < 30s
- **隔离**：用户代码不能影响其他用户或 host
- **可用性**：99.9%
- **成本**：按用户使用量计费（minutes）

### 关键澄清
> "Self-hosted runners?" → 支持，但 MVP 是 Atlassian-hosted
> "Multi-cloud?" → 用户 deploy 阶段可调任何 cloud；runner 在 Atlassian 自己的 K8s
> "Concurrency limits?" → 每 plan 限 # parallel steps

---

## 2. Capacity Estimation

| 指标 | 数值 |
|-----|------|
| Repos | 1M |
| Builds/min（peak） | 10K |
| Avg build duration | 10 min |
| 同时运行的 step 数 | 10K × 10 = 100K（peak） |
| Build log size | 1-100 MB |
| Daily logs total | ~ 1 PB（hot 1 周 + cold 1 年） |

---

## 3. High-Level Architecture

```
┌──────────────┐
│   Bitbucket  │ Code repo
│   (Git)      │
└──────┬───────┘
       │ webhook on push/PR
       ▼
┌──────────────────────────┐
│  Pipeline Service         │
│  - Parse YAML             │
│  - Validate config        │
│  - Create Build + Steps   │
└──┬───────────────────┬───┘
   │                   │
   ▼                   ▼
┌──────────┐    ┌──────────────┐
│Postgres  │    │   Kafka      │ (build_started, step_queued, ...)
│ Builds + │    └──────┬───────┘
│ Steps    │           │
└──────────┘           ▼
                ┌──────────────┐
                │  Scheduler   │
                │  (decides    │
                │   which step │
                │   runs next  │
                │   given DAG) │
                └──────┬───────┘
                       │
                       ▼ (assign step → runner)
              ┌────────────────────┐
              │  Runner Pool       │
              │ (K8s, autoscaling) │
              │ ─────────────────  │
              │ each pod = 1 step  │
              │ Docker-in-Docker   │
              │ or sandbox (gVisor)│
              └─┬────────┬────────┘
                │        │
       step logs│        │ artifacts / cache
                ▼        ▼
         ┌──────────┐ ┌──────────┐
         │Log Stream│ │   S3     │
         │ Service  │ │ artifacts│
         │(Kafka +  │ │  + cache │
         │ S3)      │ │          │
         └──────────┘ └──────────┘
                │
                ▼
        ┌──────────────┐
        │  Web UI       │ (WebSocket → live tail)
        └──────────────┘
```

---

## 4. 核心组件详解

### 4.1 Pipeline Service
**职责**：
- 接 Bitbucket webhook（commit, PR）
- 拉取 `bitbucket-pipelines.yml`，解析 + 校验
- 用 DAG 模型表达 step 依赖
- 创建 Build entity + Step entities，写 DB

**YAML Schema 简化版**：
```yaml
pipelines:
  default:
    - step:
        name: Build
        image: maven:3.9
        script:
          - mvn clean package
        artifacts:
          - target/*.jar
    - step:
        name: Test
        script:
          - mvn test
    - step:
        name: Deploy
        deployment: production
        trigger: manual         # gate
        script:
          - ./deploy.sh
```

**DAG 校验**：
- 检测循环（拓扑排序）
- 检测无效 image / 缺失 secret
- 限制 step 数 / 资源（防滥用）

### 4.2 Scheduler
**核心问题**：决定哪个 step 在哪个 runner 上跑。

**输入**：
- Pending steps（DAG 中 ready 状态的）
- Runner pool capacity（CPU/memory available）
- Build's plan（concurrency limit）
- Affinity（artifact 在某 runner 上 → 倾向于复用）

**算法**：
```
loop:
  for each step in priority_queue (FIFO + priority):
    if step.dependencies_complete and tenant.concurrency_left > 0:
      runner = pickRunner(step.requirements)
      assign(step, runner)
      tenant.concurrency_left -= 1
```

**Priority**：付费用户 > 免费；同 build 不同 step 公平。

**Runner placement**：
- 简单：随机 round-robin
- 优化：cache affinity（同 repo 上次用过的 runner，docker layer cache 命中）
- 高级：bin-packing（节省 idle runner）

### 4.3 Runner Pool
**技术选型**：
- Kubernetes（每个 step = 一个 pod）
- 用 cluster-autoscaler，根据 queue 长度扩缩
- Pod 用 gVisor / Kata Containers 强隔离（用户代码不能逃逸到 host）

**Pod 启动流程**：
1. Scheduler call K8s API: create pod with image
2. Pod 启动后从 Pipeline Service 拉 step 详情（脚本、env、secrets）
3. 执行 script，stream log 到 Log Service
4. 上传 artifacts 到 S3
5. 报告状态 → Pipeline Service
6. Pod 终止

**Cold start 优化**：
- Pre-warmed pool（保持一定数量空闲 pod）
- 镜像 prefetch（常用镜像缓存到节点）
- 单 build 多 step 复用 pod（如果 spec 允许）

### 4.4 Log Streaming
**为什么单独搞个服务？**
- 日志高频率写（一秒几 KB）
- 用户实时看（WebSocket tail）
- 持久化（90 天 hot，1 年 cold）

**架构**：
```
Runner pod ──→ Log agent (sidecar) ──→ Kafka topic ──→ S3 + Live cache (Redis)
                                                       │
                                              WebSocket ←─ User browser
```

- Live tail：Redis ring buffer（最近 1MB）+ Kafka tail
- 历史 view：S3 download（presigned URL）

### 4.5 Artifacts & Cache
**Artifacts**：build 产物（如 jar）传给后续 step
- 上传 S3，metadata 入 DB
- 下游 step 启动时下载

**Cache**：跨 build 复用（如 Maven `~/.m2`）
- Key = repo + branch + cache.spec hash
- Pull at step start, push at step end (if changed)
- 大 cache 用 S3；小的可放 Redis

---

## 5. 关键问题与权衡

### Q1: 怎么保证用户代码安全（不影响其他人）？
- **强隔离**：每 step 一个 pod；pod 用 gVisor (sandboxed runtime)
- **Network policy**：默认 only outbound（不能扫描其他 pod）
- **No host filesystem mount**
- **Quota**：CPU/memory hard limits
- **Secrets**：仅注入运行时环境变量；不写日志（mask）

### Q2: 大量 build 同时来怎么办？
- Queue + back-pressure（queue 满 → reject 或排队）
- Per-tenant concurrency limit（防霸占资源）
- Auto-scale runner pool

### Q3: Manual approval 实现？
- Step state = `WAITING_APPROVAL`
- Webhook / UI 让有权限的人 approve / reject
- Approval 写入 audit log
- 超时（24h 无人 approve）→ auto-cancel

### Q4: Idempotency / retry?
- 每 step 有唯一 ID
- 重跑 = 创建新 step（不覆盖历史）
- 用户手动 retry → 同一 build 增加 retry counter

### Q5: 失败诊断？
- Logs 必须保留
- 失败 step 的环境（env, files）可 dump 到 S3 给用户下载
- "Re-run with SSH" 模式：高级用户 SSH 进 pod 调试

### Q6: 成本控制？
- 按 minute 计费（pod 运行时间）
- 用户可 timeout 单 step
- Free tier：每月 N 分钟，超额 disable

### Q7: Multi-region?
- Runner pool per region（reduce egress for deployments）
- 中央 Pipeline Service + DB 单 region（写）+ replica 多 region（读）

---

## 6. 故障处理

| 故障 | 影响 | 应对 |
|------|------|------|
| Runner pod crash | 该 step 失败 | 自动 retry（最多 3 次）；最后失败标记 |
| K8s control plane 挂 | 不能调度新 step | Pending queue 暂存；恢复后追赶 |
| Postgres 挂 | 不能创建 build | 5xx；client retry |
| Kafka 挂 | 日志丢失 | Runner 本地缓冲；恢复后 flush |
| S3 挂 | artifact 不能存 | step 失败 + retry；用户重试 |

---

## 7. 监控

- **Build queue depth**（积压预警）
- **Step latency**（pending → running） P99
- **Runner utilization**
- **Failure rate** by step type / image
- **Per-tenant quota usage**
- **Log throughput**

---

## 8. 进阶讨论

### Pipelines as Code（GitOps）
- 配置在 repo 中（version controlled）
- 修改配置走 PR review
- 与 IaC（Terraform）相同思路

### Self-hosted runners
- 用户在自己机器上跑 agent，连回 Pipeline Service
- 用例：私有网络部署、需要特殊硬件（GPU）
- 安全：双向 mTLS，agent pull jobs（不开端口）

### Caching strategy
- **Layered cache**：global → branch → build-specific
- **Smart invalidation**：只在 dependencies 变化时重建
- 类似 Bazel / Turborepo

### 与 Jira 集成
- Build 失败 → 自动创建 Jira issue
- Issue 里 link 到 build / log
- "Smart commits"：commit message 包含 `JIRA-123 #close` 自动转移 issue

### CI vs CD 分离
- CI = 每 commit 跑（fast feedback）
- CD = release pipeline（手动 / 定时）
- Atlassian Pipelines 支持两者，via `deployment` keyword

---

## 9. 5分钟版本

```
Bitbucket → webhook → Pipeline Service (parse YAML, build DAG)
                          │
                          ▼
                      Scheduler (pick step + runner)
                          │
                          ▼
                  K8s Runner Pool (gVisor sandbox per step)
                          │
              ┌───────────┼───────────┐
              ▼           ▼           ▼
           Logs       Artifacts    Cache
        (Kafka+S3)     (S3)       (S3/Redis)
              │
              ▼
           Web UI (WebSocket live tail)
```

**核心三句话**：
- "User code in sandboxed K8s pods (gVisor) — strong isolation is non-negotiable for multi-tenant CI."
- "DAG scheduler with per-tenant concurrency caps + cache affinity to maximize utilization while staying fair."
- "Logs/artifacts/cache via S3 + Kafka streaming — separate hot live-tail (Redis) from cold storage (S3)."
```
