# Atlassian Interview Prep

> **Language: C++ for all coding solutions.**
> **真题来源**：LeetCode Discuss, Glassdoor, Blind, Hello Interview, prepfully (2024–2025)

---

## 目录结构

```
atlassian/
├── code_design/                  ← 11 道 LLD / Code Design 真题
├── data_structure/               ← 24 道 DSA 算法真题
└── system_design/                ← 5 道真题 System Design 文档
    └── README.md
```

---

## Part 1: Coding (Code Design + DSA Round)

### code_design/ — LLD / Code Design 题目
LLD（Low-Level Design）题，"code maintainability + extensibility"，Atlassian 高频原题。

| # | 文件 | 难度 | 备注 |
|---|------|------|------|
| 353 | [Design Snake Game](code_design/LC353_DesignSnakeGame.cpp) | M | ★★★ 极高频 |
| 348 | [Design Tic-Tac-Toe](code_design/LC348_DesignTicTacToe.cpp) | M | LLD warm-up |
| 359 | [Logger Rate Limiter](code_design/LC359_LoggerRateLimiter.cpp) | E | Rate limiter 入门 |
| 362 | [Design Hit Counter](code_design/LC362_DesignHitCounter.cpp) | M | 同上 |
| - | [Rate Limiter (Sliding Window)](code_design/RateLimiter_SlidingWindow.cpp) | M | ✅ code design 真题 |
| - | [Customer Support Rating LLD](code_design/CustomerSupportRating_LLD.cpp) | M | ✅ 2025 LLD round 真题 |
| - | [Company Hierarchy / Closest Department](code_design/CompanyHierarchy_ClosestDepartment.cpp) | M | ✅ 2025 P50 真题 |
| 146 | [LRU Cache](code_design/LC146_LRUCache.cpp) | M | ✅ 2025 phone screen 确认 |
| 460 | [LFU Cache](code_design/LC460_LFUCache.cpp) | H | ✅ LRU follow-up，高频 |
| 981 | [Time Based Key-Value Store](code_design/LC981_TimeBasedKeyValueStore.cpp) | M | 高频 LLD |
| 588 | [Design In-Memory File System](code_design/LC588_InMemoryFileSystem.cpp) | H | ✅ Confluence/Bitbucket 场景 |
| 642 | [Design Search Autocomplete System](code_design/LC642_SearchAutocomplete.cpp) | H | ✅ Trie + top-k，搜索场景 |

### data_structure/ — DSA 算法题目
两道中-难 LeetCode 风格题。

| # | 文件 | 难度 | 备注 |
|---|------|------|------|
| 14 | [Longest Common Prefix](data_structure/LC14_LongestCommonPrefix.cpp) | E | |
| 15 | [3Sum](data_structure/LC15_3Sum.cpp) | M | |
| 17 | [Letter Combinations of Phone Number](data_structure/LC17_LetterCombinations.cpp) | M | |
| 18 | [4Sum](data_structure/LC18_4Sum.cpp) | M | |
| 19 | [Remove Nth Node From End](data_structure/LC19_RemoveNthNodeFromEnd.cpp) | M | |
| 20 | [Valid Parentheses](data_structure/LC20_ValidParentheses.cpp) | E | |
| 42 | [Trapping Rain Water](data_structure/LC42_TrappingRainWater.cpp) | H | |
| 49 | [Group Anagrams](data_structure/LC49_GroupAnagrams.cpp) | M | |
| 56 | [Merge Intervals](data_structure/LC56_MergeIntervals.cpp) | M | |
| 68 | [Text Justification](data_structure/LC68_TextJustification.cpp) | H | |
| 84 | [Largest Rectangle in Histogram](data_structure/LC84_LargestRectangleInHistogram.cpp) | H | |
| 133 | [Clone Graph](data_structure/LC133_CloneGraph.cpp) | M | ✅ 图遍历，新增 |
| 146 | [LRU Cache (DSA view)](data_structure/LC146_LRUCache.cpp) | M | |
| 149 | [Max Points on a Line](data_structure/LC149_MaxPointsOnALine.cpp) | H | |
| 200 | [Number of Islands](data_structure/LC200_NumberOfIslands.cpp) | M | ✅ BFS/DFS/UF，新增 |
| 208 | [Implement Trie](data_structure/LC208_ImplementTrie.cpp) | M | ✅ 搜索补全基础，新增 |
| 253 | [Meeting Rooms II](data_structure/LC253_MeetingRoomsII.cpp) | M | ★ 高频 |
| 295 | [Find Median from Data Stream](data_structure/LC295_FindMedianDataStream.cpp) | H | ✅ 双堆，新增 |
| 300 | [Longest Increasing Subsequence](data_structure/LC300_LongestIncreasingSubsequence.cpp) | M | |
| 366 | [Find Leaves of Binary Tree](data_structure/LC366_FindLeavesOfBinaryTree.cpp) | M | ★ 极高频 |
| 438 | [Find All Anagrams in a String](data_structure/LC438_FindAllAnagrams.cpp) | M | ✅ 滑动窗口，新增 |
| 1071 | [GCD of Strings](data_structure/LC1071_GCDOfStrings.cpp) | E | ★ 高频 |
| 1366 | [Rank Teams by Votes](data_structure/LC1366_RankTeamsByVotes.cpp) | M | |
| - | [File Collection / Top N](data_structure/FileCollection_TopN.cpp) | M | Atlassian 经典自定义题 |

---

## Part 2: System Design

见 [system_design/README.md](system_design/README.md)，5 道真题：

| # | 题目 | 真题确认 |
|---|------|---------|
| 1 | [Confluence Real-Time Collaborative Editing](system_design/01_ConfluenceRealTimeCollab.md) | ✅ 高频 |
| 2 | [Jira-style Issue Tracking System](system_design/02_JiraIssueTracking.md) | ✅ 真题 |
| 3 | [Unified Tagging System (Cross-Product)](system_design/03_TaggingSystem.md) | ✅ 真题原题 |
| 4 | [Notification System (K-way Merge)](system_design/04_NotificationKMergeSystem.md) | ✅ 真题原题 |
| 5 | [Bitbucket CI/CD Pipeline](system_design/05_BitbucketCICDPipeline.md) | ✅ 真题 |

---

## Part 3: 面试流程概览（Senior/Staff）

```
Recruiter Screen (30 min, 文化 + JD 沟通)
    ↓
Karat Live Coding (60 min, 1 medium DSA)
    ↓
Virtual Onsite (5 rounds × 60 min)
    ├── Coding 1 — DSA / Algorithm
    ├── Coding 2 — Code Design / LLD（"code maintainability"）
    ├── System Design（Senior/Staff 必有）
    ├── Values / Behavioral
    └── Manager / Cross-functional
```

### 关键 Atlassian 面试特征

1. **"Code Design" 不是普通 LeetCode** — 招聘者会主动说 "this won't be LeetCode style"
   - 重点：可维护性、可扩展性、面试官追加需求
   - 例：snake game 加 "蛇每隔 N 个 food 才长 1 节"
2. **Repeat questions** — Atlassian 是出名的"重复出题"公司
   - 见 README 的"Atlassian Classic"题，反复刷透
3. **Values round** 比较虚但**必须认真准备**
   - "Open company, no bullshit", "Build with heart and balance" 等价值观
4. **System Design** 围绕实际产品（Jira/Confluence/Bitbucket）— 了解他们的产品架构是加分项

---

## Part 4: 备考路径

### 必看 / 第一周
1. [System Design #1 Confluence Real-Time Collab](system_design/01_ConfluenceRealTimeCollab.md)
2. [LC353 Snake Game](code_design/LC353_DesignSnakeGame.cpp)
3. [LC366 Find Leaves](data_structure/LC366_FindLeavesOfBinaryTree.cpp)
4. [LC1071 GCD of Strings](data_structure/LC1071_GCDOfStrings.cpp)
5. [LC146 LRU Cache](code_design/LC146_LRUCache.cpp)

### 第二周
6. [System Design #2 Jira](system_design/02_JiraIssueTracking.md)
7. [System Design #3 Tagging](system_design/03_TaggingSystem.md)
8. [Customer Support Rating LLD](code_design/CustomerSupportRating_LLD.cpp)
9. [Rate Limiter](code_design/RateLimiter_SlidingWindow.cpp)
10. [LC42 Trapping Rain Water](data_structure/LC42_TrappingRainWater.cpp)
11. [LC200 Number of Islands](data_structure/LC200_NumberOfIslands.cpp) ← 新增
12. [LC208 Implement Trie](data_structure/LC208_ImplementTrie.cpp) ← 新增

### 第三周
13. [System Design #4 Notification](system_design/04_NotificationKMergeSystem.md)
14. [System Design #5 CI/CD](system_design/05_BitbucketCICDPipeline.md)
15. [LC253 Meeting Rooms II](data_structure/LC253_MeetingRoomsII.cpp)
16. [LC300 LIS](data_structure/LC300_LongestIncreasingSubsequence.cpp)
17. [Company Hierarchy](code_design/CompanyHierarchy_ClosestDepartment.cpp)
18. [LC460 LFU Cache](code_design/LC460_LFUCache.cpp) ← 新增
19. [LC588 In-Memory File System](code_design/LC588_InMemoryFileSystem.cpp) ← 新增
20. [LC642 Search Autocomplete](code_design/LC642_SearchAutocomplete.cpp) ← 新增

### 第四周
- 全部 mock；剩余题目刷一遍；准备 5-7 个 STAR 故事

---

## Sources（真题确认）

- [Atlassian Engineering Interview Guide (Official)](https://www.atlassian.com/company/careers/resources/interviewing/engineering)
- [Atlassian P50 Interview Jan 2025](https://leetcode.com/discuss/interview-experience/6399510/Atlassian-P50-Interview-Experienceor-Jan-2025/)
- [Atlassian P60 Sept 2025](https://leetcode.com/discuss/post/7246118/)
- [Atlassian SDE 3 P50 Backend Role](https://www.geeksforgeeks.org/atlassian-interview-experience-for-sde-3-p50-backend-role/)
- [Hello Interview: Tagging System](https://www.hellointerview.com/community/questions/jira-confluence-bitbucket-tags/cmacerxv900gtad08ibj10s1o)
- [Glassdoor: Senior Software Engineer](https://www.glassdoor.com/Interview/Atlassian-Senior-Software-Engineer-Interview-Questions-EI_IE115699.0,9_KO10,34.htm)
- [Blind: Atlassian Discussions](https://www.teamblind.com/company/Atlassian/posts/atlassian-interview)
- [Medium: Atlassian LLD Questions](https://medium.com/@prashant558908/atlassian-low-level-design-questions-from-recent-interviews-d09c463c0889)
