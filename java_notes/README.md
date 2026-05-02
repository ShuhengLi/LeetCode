# Java LeetCode 速查册（C++ 转 Java 专用）

> 给有 C++ 刷题基础的人快速上手 Java 面试编程。

---

## 文件目录

| 文件 | 内容 | 何时看 |
|-----|------|-------|
| [01_data_structures.md](01_data_structures.md) | 所有 Java 集合（List/Set/Map/Queue/PQ）+ 复杂度 + 决策树 | 第一遍学 / 写代码遇到不知道用什么 |
| [02_arrays_strings.md](02_arrays_strings.md) | 数组、字符串、StringBuilder、二维网格 | 字符串题 / 数组题之前 |
| [03_cpp_to_java.md](03_cpp_to_java.md) | C++ 到 Java 1:1 速查映射 | 知道 C++ 写法，想找 Java 等价 |
| [04_gotchas.md](04_gotchas.md) | 17 个高频坑（必看） | **第一周必读 + 面试前再过一遍** |
| [05_patterns_templates.md](05_patterns_templates.md) | 17 个算法模板（BFS/DFS/DP/UF/Trie...） | 写代码时直接抄 |

---

## 强烈推荐阅读顺序

```
1. 04_gotchas.md      ← 先看坑（避免低级错误）
2. 01_data_structures ← 掌握工具
3. 03_cpp_to_java     ← 对照转换
4. 02_arrays_strings  ← 字符串基本功
5. 05_patterns        ← 模板背诵
```

---

## 五大核心心智转换（C++ → Java）

### 1. 容器类型不一样
```
C++ vector<int>      →  int[] (固定) 或 ArrayList<Integer> (可变)
C++ unordered_map    →  HashMap (默认无序，对应 Java 默认！)
C++ map              →  TreeMap (有序的要主动选 Tree)
C++ priority_queue   →  PriorityQueue (但默认是小顶堆，相反！)
C++ pair<A,B>        →  没有！用 int[] 或 record
```

### 2. 比较是 `equals` 不是 `==`
```java
String a = "hello", b = "hello";
a == b              // 不靠谱
a.equals(b)         // ✅
```
基本类型用 `==`，引用类型用 `.equals()`。**永远记住。**

### 3. Integer 不是 int
```java
List<Integer> list;          // 不能 List<int>
int x = map.get(5);          // null → NPE
int x = map.getOrDefault(5, 0);  // ✅
```

### 4. String 不可变
```java
String s = "abc";
s += "d";                    // 创建新对象
// 循环里这么写 → O(n²)，必须 StringBuilder
```

### 5. PriorityQueue 默认小顶堆（相反）
```java
PriorityQueue<Integer> pq = new PriorityQueue<>();   // 小顶堆
PriorityQueue<Integer> max = new PriorityQueue<>(Comparator.reverseOrder());
```

---

## 速查表：C++ → Java 一行对照

| 操作 | C++ | Java |
|------|-----|------|
| 数组长度 | `v.size()` / `arr.size()` | `arr.length` (无括号) |
| 字符串长度 | `s.size()` / `s.length()` | `s.length()` (有括号) |
| 字符串字符 | `s[i]` | `s.charAt(i)` |
| 排序 | `sort(v.begin(), v.end())` | `Arrays.sort(arr)` |
| 反转 | `reverse(v.begin(), v.end())` | `Collections.reverse(list)` |
| 二分查找 | `lower_bound(v, x)` | `Collections.binarySearch(list, x)` 或 `TreeSet.ceiling(x)` |
| 入栈/入队 | `st.push(x)` / `q.push(x)` | `deque.push(x)` / `deque.offer(x)` |
| 取栈/队头 | `st.top()` / `q.front()` | `deque.peek()` |
| 出栈/出队 | `st.pop()` / `q.pop()` | `deque.pop()` / `deque.poll()` |
| 优先队列 | `pq.top()` / `pq.pop()` | `pq.peek()` / `pq.poll()` |
| Map 取值 | `m[k]` / `m.at(k)` | `map.get(k)` (不存在 null) |
| Map 默认值 | `m.count(k) ? m[k] : 0` | `map.getOrDefault(k, 0)` |
| Set 是否包含 | `s.count(x)` | `set.contains(x)` |
| 频次累加 | `m[k]++` | `map.merge(k, 1, Integer::sum)` |
| pair 创建 | `{1, 2}` 或 `make_pair(1,2)` | `new int[]{1, 2}` |
| 子串 | `s.substr(2, 3)` (start, len) | `s.substring(2, 5)` (start, end) |
| 字符串拼接（循环） | `s += c` | `StringBuilder sb; sb.append(c)` |

---

## 面试前 30 分钟必查清单

打开本目录，重点看这些章节：

- [ ] `04_gotchas.md` 第 1, 2, 3 节（== vs equals, autoboxing, 溢出）
- [ ] `01_data_structures.md` 复杂度表（口头表达 trade-off 用）
- [ ] `05_patterns_templates.md` 你今天要考的题型（BFS / DP / 二分 等）
- [ ] `02_arrays_strings.md` StringBuilder 用法（字符串题必用）

---

## 一些"应该这么写"的潜规则

1. **能用 `int[]` 就别用 `ArrayList<Integer>`** — 性能差 2-5 倍
2. **能用 `int[26]` 频次桶就别用 `HashMap<Character, Integer>`** — 同上
3. **`StringBuilder` 永远是字符串拼接的默认选择**
4. **`ArrayDeque` 替代 `Stack` 和 `LinkedList`**
5. **`Comparator.reverseOrder()` 比 `(a,b) -> b - a` 安全**（无溢出风险）
6. **用 `Integer.compare(a, b)` 写比较器**（避免 `a - b` 溢出）
7. **频次累加用 `map.merge(k, 1, Integer::sum)`** 一行搞定
8. **图的邻接表用 `computeIfAbsent`**：
   ```java
   graph.computeIfAbsent(u, k -> new ArrayList<>()).add(v);
   ```

---

## 与 SoFi 真题的关联

如果在准备 SoFi（fintech）面试，本目录与 [`../sofi/code/`](../sofi/code/) 配套：

| Java 知识点 | 对应 SoFi 真题 |
|-----------|---------------|
| 频次桶 / Anagram | [01 MinChangesToMakeAnagrams](../sofi/code/01_MinChangesToMakeAnagrams.java) |
| Sliding Window | [02 SmallestCleanSubarray](../sofi/code/02_SmallestCleanSubarray.java) |
| Tree BFS/DFS | [03 BinaryTreeRightSideView](../sofi/code/03_BinaryTreeRightSideView.java) |
| OOP 设计 | [04 ChessKnightValidator](../sofi/code/04_ChessKnightValidator.java) |
| HashMap + Comparator | [05 TransactionFrequencyOrder](../sofi/code/05_TransactionFrequencyOrder.java) |
| HashMap + 双向链表 | [06 LRUCache](../sofi/code/06_LRUCache.java), [07 KV Store](../sofi/code/07_InMemoryKeyValueStore.java) |
| Stack | [08 AsteroidCollision](../sofi/code/08_AsteroidCollision.java) |
| Topological Sort | [12 CourseSchedule](../sofi/code/12_CourseSchedule.java) |
| BFS Shortest Path | [13 WordLadder](../sofi/code/13_WordLadder.java) |
| PriorityQueue | [14 TopKFrequentWords](../sofi/code/14_TopKFrequentWords.java) |

---

## 推荐学习曲线

### Week 1: 基础（每天 1-2 小时）
- 通读 `04_gotchas.md`
- 通读 `01_data_structures.md`
- 用 Java 重做 5-10 道做过的简单题（two sum、reverse linked list 等）

### Week 2: 巩固
- `03_cpp_to_java.md` 快速对照
- `02_arrays_strings.md` + 写 5 道字符串题
- 每个数据结构（List/Map/Set/Stack/Queue/PQ）至少 2 道题

### Week 3: 模板内化
- `05_patterns_templates.md` 每个模板默写一遍
- 按 pattern 刷题：BFS 5 道、DFS 5 道、DP 5 道、二分 5 道

### Week 4: 实战
- 真题模拟（SoFi / 目标公司）
- 重点关注：能 30 分钟写完 + 边写边讲

---

## 工具建议

- **本地 IDE**：IntelliJ IDEA Community（免费，Java 神器）
- **Java 版本**：JDK 17 LTS（LeetCode 默认）/ 21（最新 LTS）
- **快速验证**：JShell（命令行 REPL）— 测语法很方便
  ```bash
  jshell
  jshell> Map<String, Integer> m = new HashMap<>();
  jshell> m.merge("a", 1, Integer::sum);
  ```

---

## 一句话总结

> **C++ 的 STL 概念都能在 Java 找到对应，但有 5 处反直觉**：
> 1. `Integer` 不是 `int`（autoboxing）
> 2. `==` 不是 `equals`
> 3. `String` 不能改（用 `StringBuilder`）
> 4. `PriorityQueue` 默认小顶堆（C++ 默认大顶堆）
> 5. `HashMap.get()` 不存在返回 null（C++ `[]` 创建默认值）
>
> 记住这 5 条 + 本目录的模板，面试 Java 编程不会有大问题。
