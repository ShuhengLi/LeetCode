# Java 数据结构 LeetCode 速查

> 给 C++ 背景的刷题者：每个数据结构对照 STL，列**面试常用方法**和**复杂度**。

---

## 1. List（数组列表）

### ArrayList — 对应 `std::vector`

```java
import java.util.ArrayList;
import java.util.List;

List<Integer> list = new ArrayList<>();
list.add(1);             // push_back, O(1) 摊还
list.add(0, 5);          // insert at index 0, O(n)
list.get(0);             // [0] / .at(0), O(1)
list.set(0, 10);         // 修改, O(1)
list.remove(0);          // 删除 index 0, O(n)
list.remove(Integer.valueOf(5));  // 按值删除（注意装箱！）
list.size();             // .size(), O(1)
list.isEmpty();          // .empty()
list.contains(5);        // O(n) 线性扫描
list.indexOf(5);         // 第一个出现位置，O(n)
list.clear();            // 清空

// 遍历
for (int x : list) { ... }
for (int i = 0; i < list.size(); i++) { ... }
list.forEach(x -> { ... });

// 转数组
Integer[] arr = list.toArray(new Integer[0]);
int[] primArr = list.stream().mapToInt(Integer::intValue).toArray();

// 从数组建
List<Integer> l = Arrays.asList(1, 2, 3);  // 注意：固定大小！不能 add/remove
List<Integer> l2 = new ArrayList<>(Arrays.asList(1, 2, 3));  // 真正可变的
```

### ⚠️ 关键陷阱
- `Arrays.asList(1, 2, 3)` 返回的是 **固定大小**视图，不能 `add`/`remove`
- `list.remove(int)` 是按 **index** 删，`list.remove(Object)` 才是按值删 → 删 `int` 必须 `Integer.valueOf(x)`
- `ArrayList<int>` **不允许**！只能 `ArrayList<Integer>`（必须用 wrapper）

### LinkedList — 实际用 ArrayDeque 替代
- LeetCode 中**几乎不用** `LinkedList`，需要 deque/queue 用 `ArrayDeque`（更快）
- 如果题目要求"双端 O(1)"，用 ArrayDeque

---

## 2. Set（集合）

### HashSet — 对应 `std::unordered_set`

```java
import java.util.HashSet;
import java.util.Set;

Set<Integer> set = new HashSet<>();
set.add(1);              // O(1) 摊还
set.contains(1);         // .count() / .find(), O(1)
set.remove(1);           // O(1)
set.size();
set.isEmpty();

// 初始化
Set<Integer> s = new HashSet<>(Arrays.asList(1, 2, 3));

// 集合运算（破坏性）
set.addAll(other);       // union
set.retainAll(other);    // intersection
set.removeAll(other);    // difference
```

### TreeSet — 对应 `std::set`（红黑树，有序）

```java
TreeSet<Integer> ts = new TreeSet<>();
ts.add(5);
ts.first();              // 最小值，对应 *begin()
ts.last();               // 最大值，对应 *rbegin()
ts.floor(7);             // <= 7 的最大，对应 -- upper_bound(7)
ts.ceiling(7);           // >= 7 的最小，对应 lower_bound(7)
ts.lower(7);             // < 7 的最大（严格小于）
ts.higher(7);            // > 7 的最小（严格大于）
ts.pollFirst();          // 取出并删最小
ts.pollLast();           // 取出并删最大
ts.headSet(10);          // < 10 的子集
ts.tailSet(10);          // >= 10 的子集
ts.subSet(5, 10);        // [5, 10)
// 全部 O(log n)
```

### LinkedHashSet — 保留插入顺序的 HashSet
```java
LinkedHashSet<Integer> lhs = new LinkedHashSet<>();
// 遍历顺序 = 插入顺序，操作复杂度同 HashSet
```

---

## 3. Map（映射）

### HashMap — 对应 `std::unordered_map`

```java
import java.util.HashMap;
import java.util.Map;

Map<String, Integer> map = new HashMap<>();
map.put("a", 1);
map.get("a");                      // 不存在返回 null（不抛异常！）
map.getOrDefault("a", 0);          // ★ 高频，频次统计必备
map.containsKey("a");
map.containsValue(1);              // O(n)，少用
map.remove("a");
map.size();
map.isEmpty();

// 频次统计（最高频用法）
map.merge("a", 1, Integer::sum);   // map[a] = (existing? +1 : 1)
// 等价于：
map.put("a", map.getOrDefault("a", 0) + 1);

// computeIfAbsent — 创建或追加
Map<Integer, List<Integer>> graph = new HashMap<>();
graph.computeIfAbsent(u, k -> new ArrayList<>()).add(v);

// 遍历
for (Map.Entry<String, Integer> e : map.entrySet()) {
    e.getKey(); e.getValue();
}
for (String k : map.keySet()) { ... }
for (int v : map.values()) { ... }
map.forEach((k, v) -> { ... });
```

### TreeMap — 对应 `std::map`（有序）

```java
TreeMap<Integer, String> tm = new TreeMap<>();
tm.put(5, "a");
tm.firstKey();           // 最小 key
tm.lastKey();
tm.floorKey(7);          // <= 7 的最大 key
tm.ceilingKey(7);        // >= 7
tm.lowerKey(7);          // < 7
tm.higherKey(7);         // > 7
tm.firstEntry();         // 返回 Map.Entry<K,V>
tm.pollFirstEntry();
tm.headMap(10);          // 子映射 < 10
tm.subMap(5, 10);        // [5, 10)
// 全部 O(log n)
```

### LinkedHashMap — 保留插入顺序 / LRU 友好
```java
// 默认按插入顺序遍历
LinkedHashMap<K,V> lhm = new LinkedHashMap<>();

// 按访问顺序（LRU 用法）：第三个参数 accessOrder=true
LinkedHashMap<Integer, Integer> lru = new LinkedHashMap<>(16, 0.75f, true) {
    protected boolean removeEldestEntry(Map.Entry<Integer, Integer> eldest) {
        return size() > capacity;
    }
};
```

---

## 4. Queue / Deque（队列/双端队列）

### ArrayDeque — **同时**当 Queue / Stack / Deque 用（推荐）

```java
import java.util.ArrayDeque;
import java.util.Deque;

Deque<Integer> deque = new ArrayDeque<>();

// 当 Stack（push/pop/peek 操作栈顶 = 头部）
deque.push(1);           // 入栈头
deque.pop();             // 弹栈头
deque.peek();            // 看栈头

// 当 Queue（offer 入尾，poll 出头）
deque.offer(1);          // 入队尾 == offerLast
deque.poll();            // 出队头 == pollFirst
deque.peek();            // 队头 == peekFirst

// Deque 双端
deque.offerFirst(1);     // 入头
deque.offerLast(1);      // 入尾
deque.pollFirst();
deque.pollLast();
deque.peekFirst();
deque.peekLast();

deque.size();
deque.isEmpty();
```

### ⚠️ 不要用 Stack 类！
```java
Stack<Integer> stack = new Stack<>();   // ❌ 别用！synchronized 慢，且继承 Vector 设计差
Deque<Integer> stack = new ArrayDeque<>();  // ✅
```

### LinkedList 也实现 Deque，但比 ArrayDeque 慢约 2x，不推荐

### PriorityQueue — 对应 `std::priority_queue`，但**默认是小顶堆**！

```java
import java.util.PriorityQueue;

PriorityQueue<Integer> minHeap = new PriorityQueue<>();
minHeap.offer(5);
minHeap.offer(1);
minHeap.peek();          // 1 (最小)
minHeap.poll();          // 1
// O(log n) 插入/删除，O(1) peek

// 大顶堆：用 reverseOrder 比较器
PriorityQueue<Integer> maxHeap = new PriorityQueue<>(Comparator.reverseOrder());
// 等价：(a, b) -> b - a   （但 b-a 有溢出风险！）
PriorityQueue<Integer> maxHeapSafe = new PriorityQueue<>((a, b) -> Integer.compare(b, a));

// 自定义比较（如 [num, freq] 按 freq 升序）
PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[1] - b[1]);

// 多键比较
PriorityQueue<int[]> pq2 = new PriorityQueue<>(
    Comparator.<int[]>comparingInt(a -> a[0])
              .thenComparingInt(a -> a[1])
);

// 注意：PQ 没有 O(log n) 的 remove(任意元素)，只有 remove(对象) 是 O(n)
//       要删任意元素或更新优先级 → 用"懒删除"配合标记
```

### ⚠️ PQ 没有 decrease-key
- C++ 也没有，但 Java 同样没有
- 实现 Dijkstra 时用 **lazy deletion**：每次 poll 检查是否过期

---

## 5. 复杂度速查表

| 数据结构 | get/contains | add/put | remove | 有序 | 备注 |
|---------|--------------|---------|--------|------|------|
| `int[]` | O(1) | - | - | - | 大小固定 |
| `ArrayList` | O(1) | O(1)摊 | O(n) | 插入序 | std::vector |
| `LinkedList` | O(n) | O(1)首尾 | O(n) | 插入序 | 别用，用 ArrayDeque |
| `HashSet` / `HashMap` | O(1) | O(1) | O(1) | 无 | std::unordered_* |
| `LinkedHashSet/Map` | O(1) | O(1) | O(1) | 插入/访问 | LRU 用 |
| `TreeSet` / `TreeMap` | O(log n) | O(log n) | O(log n) | 排序 | std::* (红黑树) |
| `ArrayDeque` | - | O(1)首尾 | O(1)首尾 | - | Stack/Queue/Deque |
| `PriorityQueue` | O(1) peek | O(log n) | O(log n) poll | - | 小顶堆默认 |

---

## 6. 选择决策树

```
需要按 key 查找？
├─ 是 → Map
│       ├─ 需要排序 / floor/ceiling → TreeMap
│       ├─ 需要保留插入顺序 → LinkedHashMap
│       └─ 都不要 → HashMap (★ 最常用)
└─ 否 → 
    ├─ 只存元素，去重 → Set (同上选 Hash/Tree/LinkedHash)
    ├─ 顺序访问，按 index → ArrayList (★)
    ├─ 双端 / 栈 / 队列 → ArrayDeque
    ├─ 取最小/最大 → PriorityQueue
    └─ 大小固定 + 性能极致 → 数组 int[]
```

---

## 7. 常用 import（建议直接 `java.util.*`）

```java
import java.util.*;       // 一锅端，LeetCode 推荐
// 如果嫌大：
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
import java.util.Deque;
import java.util.ArrayDeque;
import java.util.PriorityQueue;
import java.util.TreeMap;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
```

LeetCode 的 IDE 自带，本地写直接 `import java.util.*;` 最省事。
