# Atlassian System Design Interview

> **轮次**：Senior/Staff onsite 必有 1 轮 System Design（45-60 分钟）
> **真题来源**：Glassdoor、Blind、LeetCode Discuss、Hello Interview、prepfully（2024-2025）

---

## 题目列表

| # | 文件 | 题目 | 真题确认 | 业务关联 | 难度 |
|---|------|------|---------|---------|------|
| 1 | [01_ConfluenceRealTimeCollab.md](01_ConfluenceRealTimeCollab.md) | Confluence Real-Time Collaborative Editing | ✅ 高频真题 | Confluence 核心 | ★★★★★ |
| 2 | [02_JiraIssueTracking.md](02_JiraIssueTracking.md) | Jira-style Issue Tracking System | ✅ 真题 | Jira 核心 | ★★★★ |
| 3 | [03_TaggingSystem.md](03_TaggingSystem.md) | Unified Tagging System (Cross-Product) | ✅ 真题原题 | 跨 Atlassian 产品 | ★★★★ |
| 4 | [04_NotificationKMergeSystem.md](04_NotificationKMergeSystem.md) | Cross-Product Notification (K-way Merge) | ✅ 真题原题 | 通知中心 | ★★★★ |
| 5 | [05_BitbucketCICDPipeline.md](05_BitbucketCICDPipeline.md) | Bitbucket CI/CD Pipeline | ✅ 真题 | Bitbucket Pipelines | ★★★★ |

**参见 LLD/coding 中相关组件**：
- [Rate Limiter (Sliding Window)](../RateLimiter_SlidingWindow.cpp) — API gateway 限流子组件
- [LRU Cache](../LC146_LRUCache.cpp) — 缓存层
- [Customer Support Rating](../CustomerSupportRating_LLD.cpp) — JSM rating 系统

---

## Atlassian System Design 面试特点

### 与传统 FAANG 的区别

| 维度 | FAANG | Atlassian |
|------|-------|-----------|
| 重点话题 | 大规模 scale | **协作 + 一致性 + 多产品集成** |
| 必谈技术 | sharding, CDN | **OT/CRDT, WebSocket, sticky session** |
| 业务术语 | 通用 | Jira/Confluence/Bitbucket 强相关 |
| 面试官期待 | 系统能力 | 系统能力 + 对 Atlassian 产品的理解 |

### 高频考察主题（按出现频率）

1. **实时协作**（Confluence-style）— OT vs CRDT trade-off
2. **多产品统一**（搜索、通知、tag）— 跨 Jira/Confluence/Bitbucket
3. **可扩展配置**（custom fields、workflows）— hybrid schema
4. **Notification fan-out** — push vs pull
5. **CI/CD pipeline** — 多租户隔离 + DAG 调度
6. **Rate limiting** — API gateway 防 abuse

---

## 评分维度（Senior/Staff 期望）

| 维度 | Senior (P50) | Staff (P60) |
|------|------|-------|
| 需求澄清 | 5-10 个问题 | 主动引导，提出隐含需求 |
| Capacity estimation | 主动算 | + 多假设 trade-off |
| High-level design | 清晰组件 + 数据流 | + 多方案对比 |
| 深入细节 | 1-2 个核心组件 | 主动选 + 在追问中深入 |
| Trade-off | 知道每个选择的代价 | 引用行业 pattern + 失败案例 |
| 故障处理 | 至少 3 种故障 | 设计本身考虑 partial failure |
| 业务理解 | 知道是 Jira/Confluence | 能讲产品架构演进、技术栈细节 |

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

### 必谈关键词（Atlassian-specific）

按重要性排序，**任何题目都至少提 3-5 个**：

1. **Tenant isolation / Multi-tenancy** — 所有 Atlassian 服务都是多租户
2. **OT / CRDT** — 协作类题必谈
3. **WebSocket + sticky session** — 实时类必谈
4. **Hybrid schema / JSONB** — Jira-like 灵活字段
5. **Permission inheritance** — 跨产品权限传播
6. **Webhook + retry** — Atlassian Connect 集成模式
7. **Audit log / immutable history** — 企业级要求
8. **Rate limiting + back-pressure** — API gateway 必备
9. **Push vs pull (fan-out)** — 通知/Feed 类
10. **Dead letter queue** — 失败兜底

---

## 备考策略

### 优先级排序

**必看（保过线）**：
- #1 Confluence Real-Time Collab — 高频且最难
- #2 Jira Issue Tracking — 业务核心
- #3 Tagging System — 真题原题

**重点准备**：
- #4 Notification K-way Merge — 真题原题
- #5 Bitbucket CI/CD — 系统编排

**面试前过一遍**：
- 现有 LLD 题（[code/](.../) 目录）— 子组件知识

### 训练方法

1. **第一遍**：通读每题，理解架构图
2. **第二遍**：合上文档，自己画架构 + 写关键决策
3. **第三遍**：白板 mock，45 分钟限时
4. **找朋友 mock**：或用 ChatGPT 当面试官

### 常见踩坑

- ❌ 一上来就画图，没问需求
- ❌ Capacity 不算，光"用 Redis 就行了"
- ❌ 没把 OT/CRDT 提一嘴（协作类必扣分）
- ❌ 不谈故障（"Postgres 挂了怎么办？" 答不上来）
- ❌ 不联系 Atlassian 实际产品（面试官期待你了解他们）

---

## 与 Coding Round 的关联

System Design 中提到的某些组件，coding round 可能直接考实现：

| System Design | 对应 Coding 题 |
|--------------|---------------|
| Cache layer | [LC146 LRU Cache](../LC146_LRUCache.cpp) |
| Rate limiter | [RateLimiter SlidingWindow](../RateLimiter_SlidingWindow.cpp) |
| Notification merge | LC23 Merge K Sorted Lists（k-way merge） |
| Workflow / DAG | LC207/210 Course Schedule（topo sort） |
| Hierarchy | [CompanyHierarchy ClosestDept](../CompanyHierarchy_ClosestDepartment.cpp) |
| Rating system | [CustomerSupportRating LLD](../CustomerSupportRating_LLD.cpp) |
| Snake game / Tic-Tac-Toe | [LC353](../LC353_DesignSnakeGame.cpp), [LC348](../LC348_DesignTicTacToe.cpp) |

面试时如果 SD 中讲到这些组件，**可以提一句"这其实就是 LeetCode XXX 的工业版"** —— 显示你打通了 algo 与 system 的边界。

---

## 资源链接（真题来源）

- [Atlassian Engineering Interview Guide (Official)](https://www.atlassian.com/company/careers/resources/interviewing/engineering)
- [Atlassian P50 Interview Experience | LeetCode (Jan 2025)](https://leetcode.com/discuss/interview-experience/6399510/Atlassian-P50-Interview-Experienceor-Jan-2025/)
- [Atlassian P60 Sept 2025 | LeetCode](https://leetcode.com/discuss/post/7246118/)
- [Hello Interview: Tagging System for Atlassian](https://www.hellointerview.com/community/questions/jira-confluence-bitbucket-tags/cmacerxv900gtad08ibj10s1o)
- [System Design Handbook: Atlassian Guide](https://www.systemdesignhandbook.com/guides/atlassian-system-design-interview/)
- [Atlassian LLD Questions (Medium)](https://medium.com/@prashant558908/atlassian-low-level-design-questions-from-recent-interviews-d09c463c0889)
- [Glassdoor: Atlassian Senior Software Engineer](https://www.glassdoor.com/Interview/Atlassian-Senior-Software-Engineer-Interview-Questions-EI_IE115699.0,9_KO10,34.htm)
