# SoFi Coding Interview Questions — Staff / Senior Engineer

> 真题来源：LeetCode Discuss、1point3acres、Glassdoor、Blind、FrontendLead Discuss（2024–2025）
> 面试结构：Phone Screen（2题 Medium）+ On-site（DSA Round + Logical/Maintainability Round + System Design）

---

## 题目列表（共 15 道，按真题确认度 + 频率排序）

| # | 文件 | 题目 | 类型 | 难度 | 轮次 | 真题确认 |
|---|------|------|------|------|------|---------|
| 1 | [01_MinChangesToMakeAnagrams.java](01_MinChangesToMakeAnagrams.java) | Make Pairs Anagrams | Hash Map | Medium | Phone Screen | ✅ 多源 |
| 2 | [02_SmallestCleanSubarray.java](02_SmallestCleanSubarray.java) | Smallest Clean Subarray (k distinct) | Sliding Window | Medium | Phone Screen | ✅ 多源 |
| 3 | [03_BinaryTreeRightSideView.java](03_BinaryTreeRightSideView.java) | Binary Tree Right Side View + 文件解析 | BFS/DFS | Medium | DSA Round | ✅ Glassdoor |
| 4 | [04_ChessKnightValidator.java](04_ChessKnightValidator.java) | Chess Board Move Validator (OOP) | OOP/LLD | Medium-Hard | Logical/Maintainability | ✅ LC Discuss |
| 5 | [05_TransactionFrequencyOrder.java](05_TransactionFrequencyOrder.java) | Transaction Frequency Ordering | HashMap+Sort | Medium | Phone Screen | ✅ 1point3acres |
| 6 | [06_LRUCache.java](06_LRUCache.java) | Design LRU Cache | HashMap+DLL | Medium | On-site | ✅ Blind |
| 7 | [07_InMemoryKeyValueStore.java](07_InMemoryKeyValueStore.java) | In-Memory KV Store with getLast() | Hash+DLL | Medium | On-site | ✅ Glassdoor 2025 |
| 8 | [08_AsteroidCollision.java](08_AsteroidCollision.java) | 1D Track Object Collision | Stack | Medium | DSA Round | ✅ Glassdoor |
| 9 | [09_ChocolateBarSplit.java](09_ChocolateBarSplit.java) | Chocolate Bar Splitting (Min Gold) | Math/Simulation | Medium | DSA Round | ✅ 候选人报告 |
| 10 | [10_LongestPalindromicSubstring.java](10_LongestPalindromicSubstring.java) | Longest Palindromic Substring | String/DP | Medium | DSA Round | ✅ 多源 |
| 11 | [11_MergeIntervals.java](11_MergeIntervals.java) | Merge Intervals | Sort+Greedy | Medium | DSA Round | ⭐ 高频通用 |
| 12 | [12_CourseSchedule.java](12_CourseSchedule.java) | Course Schedule (Topological Sort) | Graph/BFS/DFS | Medium | DSA Round | ✅ 候选人报告（Graph） |
| 13 | [13_WordLadder.java](13_WordLadder.java) | Word Ladder (BFS Shortest Path) | Graph/BFS | Hard | DSA Round | ✅ 候选人报告（Graph） |
| 14 | [14_TopKFrequentWords.java](14_TopKFrequentWords.java) | Top K Frequent Words | Heap/Sort | Medium | DSA Round | ⭐ 与 #5 业务关联 |
| 15 | [15_ValidateBST.java](15_ValidateBST.java) | Validate Binary Search Tree | Tree/DFS | Medium | DSA Round | ⭐ 树类高频 |

**图例：** ✅ = 有公开报告确认是 SoFi 真题；⭐ = 高频通用题，与 SoFi 真题考察点强相关

---

## 面试流程（Senior/Staff Level）

```
Recruiter Screen (30 min)
    ↓
Technical Phone Screen (60 min)
    ├── 2 × LeetCode Medium 编码题（live coding on CoderPad）
    └── 基础系统设计讨论
    ↓
Virtual Onsite (3–4 rounds)
    ├── Round 1: DSA Coding（1 Medium + 1 Hard）
    ├── Round 2: Logical & Maintainability（OOP/LLD，如棋盘、KV Store）
    ├── Round 3: System Design（fintech 专题，见 system_design/）
    └── Round 4: Behavioral（跨团队协作、production bug、STAR 格式）
```

---

## 高频考察知识点（按出现频率）

| 知识点 | 题号 | 备注 |
|--------|------|------|
| **HashMap + 频率统计** | 1, 5, 14 | 几乎必考 |
| **Sliding Window** | 2 | 子数组类问题 |
| **HashMap + 双向链表** | 6, 7 | LRU、KV Store 类 LLD 设计 |
| **Tree BFS/DFS** | 3, 15 | 右视图、BST 验证 |
| **Stack 单调栈** | 8 | 碰撞、时序处理 |
| **Sort + Greedy** | 11 | 区间合并 |
| **Graph BFS/DFS** | 12, 13 | 拓扑、最短路径 |
| **OOP / LLD 设计** | 4, 6, 7 | Logical Round 主战场 |
| **String / DP** | 10 | 字符串处理 |
| **Heap (PriorityQueue)** | 14 | Top-K 类 |
| **Math + Simulation** | 9 | 业务逻辑题 |

---

## 备考策略（来自真实候选人反馈）

### Phone Screen（60 分钟，60% 通过率）
- **重点**：第 1, 2, 5 题（数组/字符串/HashMap），尽量 30 分钟一题
- **必备**：边界条件主动说，复杂度主动报，至少跑 2 个测试用例
- **加分**：写出多种解法并对比 trade-off

### DSA On-site Round（45 分钟）
- **重点**：第 3, 8, 10, 11, 12, 13, 15 题（中等到 Hard）
- **必备**：能用纸/白板画出数据结构演化过程
- **加分**：讨论分布式扩展（百万/十亿规模）

### Logical/Maintainability Round（45 分钟）
- **重点**：第 4, 6, 7 题（OOP 设计）
- **必备**：SOLID 原则、设计模式（策略/工厂/观察者）
- **加分**：写出可单元测试的代码（依赖注入、接口隔离）

---

## 重要提醒

1. **使用 CoderPad**：不能跑代码不要紧，但要清楚 Java 标准库 API
2. **沟通至上**：边写边说，遇到 bug 主动 debug 而非沉默
3. **复杂度敏感**：Senior 必问"百万级数据如何处理"——准备好分布式答案
4. **业务相关**：SoFi 是金融科技，多用 transaction、user、balance 等业务术语包装
5. **Behavioral 同样重要**：SoFi 强调价值观，准备 5-7 个 STAR 故事

---

## 扩展阅读

- [LeetCode SoFi 公司题库](https://leetcode.com/company/sofi/)
- [Blind: SoFi Interview Discussions](https://www.teamblind.com/company/SoFi/posts/sofi-interview)
- [Glassdoor: SoFi Senior Software Engineer](https://www.glassdoor.com/Interview/SoFi-Senior-Software-Engineer-Interview-Questions-EI_IE779979.0,4_KO5,29.htm)
- [SoFi Software Engineer Interview Guide 2026](https://prachub.com/interview-guide/sofi-software-engineer-interview-guide)
