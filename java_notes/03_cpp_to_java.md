# C++ → Java 速查映射

> 来自 C++ 直接迁移最快路径，按数据结构分类。

---

## 1. 容器对照（核心）

| C++ STL | Java | 备注 |
|---------|------|------|
| `vector<int>` | `int[]` 或 `ArrayList<Integer>` | 大小固定用前者，可变用后者 |
| `vector<string>` | `String[]` 或 `List<String>` | |
| `string` | `String` (immutable) / `StringBuilder` (可变) | 频繁拼接必须用 SB |
| `array<T, N>` | `int[N]` | |
| `unordered_set<T>` | `HashSet<T>` | |
| `unordered_map<K,V>` | `HashMap<K,V>` | |
| `set<T>` | `TreeSet<T>` | 有序 |
| `map<K,V>` | `TreeMap<K,V>` | 有序 |
| `multiset<T>` | `TreeMap<T, Integer>` | Java 没有 multiset，用 map 计数模拟 |
| `multimap<K,V>` | `Map<K, List<V>>` | 同上 |
| `stack<T>` | `Deque<T>` via `ArrayDeque` | 别用 Stack 类 |
| `queue<T>` | `Deque<T>` via `ArrayDeque` | 用 offer/poll |
| `deque<T>` | `ArrayDeque<T>` | |
| `priority_queue<T>` | `PriorityQueue<T>` (默认**小顶堆**！) | C++ 默认大顶堆，相反 |
| `pair<A,B>` | 没有！用 `int[]` / `Object[]` / `Map.Entry` / 自定义类 | |
| `tuple<A,B,C>` | 没有！用 `int[]` 或自定义类 | |

---

## 2. 操作对照（最高频）

### 添加 / 访问 / 删除

```cpp
// C++
vector<int> v;
v.push_back(1);            v.pop_back();
v[0];                      v.front(); v.back();
v.size();                  v.empty();

map<int,int> m;
m[1] = 2;                  m[1];          // 注意：[]访问会创建默认值
m.count(1);                m.erase(1);

set<int> s;
s.insert(1);               s.count(1);    s.erase(1);
*s.begin(); *s.rbegin();
s.lower_bound(5); s.upper_bound(5);

priority_queue<int> pq;    // 大顶堆
pq.push(1);                pq.top();      pq.pop();

stack<int> st;
st.push(1);                st.top();      st.pop();

queue<int> q;
q.push(1);                 q.front();     q.pop();
```

```java
// Java
List<Integer> list = new ArrayList<>();
list.add(1);               list.remove(list.size() - 1);
list.get(0);               list.get(0); list.get(list.size() - 1);
list.size();               list.isEmpty();

Map<Integer,Integer> map = new HashMap<>();
map.put(1, 2);             map.get(1);    // 不存在返回 null（不会创建）
map.containsKey(1);        map.remove(1);

Set<Integer> set = new HashSet<>();
set.add(1);                set.contains(1);  set.remove(1);
// HashSet 无序，TreeSet 才能 first/last/floor/ceiling

TreeSet<Integer> ts = new TreeSet<>();
ts.first(); ts.last();
ts.ceiling(5);             // >= 5 最小 = lower_bound
ts.higher(5);              // > 5 最小  = upper_bound

PriorityQueue<Integer> minPq = new PriorityQueue<>();           // 小顶堆
PriorityQueue<Integer> maxPq = new PriorityQueue<>(Comparator.reverseOrder());
minPq.offer(1);            minPq.peek();  minPq.poll();

Deque<Integer> stack = new ArrayDeque<>();
stack.push(1);             stack.peek();  stack.pop();

Deque<Integer> queue = new ArrayDeque<>();
queue.offer(1);            queue.peek();  queue.poll();
```

---

## 3. 排序对照

```cpp
// C++
sort(v.begin(), v.end());                       // 升序
sort(v.begin(), v.end(), greater<int>());       // 降序
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

// 自定义结构体
sort(arr.begin(), arr.end(), [](auto& a, auto& b) {
    if (a.first != b.first) return a.first < b.first;
    return a.second > b.second;
});
```

```java
// Java
Arrays.sort(arr);                              // int[] 升序
Arrays.sort(arr, Collections.reverseOrder());   // ❌ int[] 不支持！
                                                //    只能 Integer[]
Arrays.sort(boxed, Collections.reverseOrder());

// int[][] 按第一列排序
Arrays.sort(intervals, (a, b) -> a[0] - b[0]);
// 多键
Arrays.sort(intervals,
    Comparator.<int[]>comparingInt(a -> a[0])
              .thenComparingInt(a -> -a[1]));    // 第二列降序

// List
List<int[]> list = ...;
list.sort((a, b) -> a[0] - b[0]);
Collections.sort(list, ...);

// ⚠️ a - b 比较器有溢出风险，安全用 Integer.compare
Arrays.sort(arr, (a, b) -> Integer.compare(a, b));
```

---

## 4. 二分查找对照

```cpp
// C++
auto it = lower_bound(v.begin(), v.end(), x);   // >= x 第一个
auto it = upper_bound(v.begin(), v.end(), x);   // > x 第一个
int idx = it - v.begin();
binary_search(v.begin(), v.end(), x);           // 是否存在
```

```java
// Java
int idx = Arrays.binarySearch(arr, x);
// 找到：返回 index（任意一个匹配的）
// 找不到：返回 -(insertion_point + 1)
int insertionPoint = idx < 0 ? -idx - 1 : idx;

// TreeMap/TreeSet 提供 floor/ceiling（更直观）
TreeMap<Integer, Integer> tm = ...;
tm.floorKey(x);    // <= x 最大
tm.lowerKey(x);    // <  x 最大
tm.ceilingKey(x);  // >= x 最小   = lower_bound
tm.higherKey(x);   // >  x 最小   = upper_bound

// Collections.binarySearch 同 Arrays.binarySearch 行为
Collections.binarySearch(list, x);
```

---

## 5. 字符串对照

```cpp
// C++
string s = "hello";
s.size();                    s.length();
s[0];
s.substr(1, 3);              // 从1开始3个字符
s.find("ll");                s.find("ll", 3);
s.replace(...);
s + "abc";                   // 拼接

stringstream ss;
ss << "abc" << 123;
string result = ss.str();
```

```java
// Java
String s = "hello";
s.length();
s.charAt(0);
s.substring(1, 4);           // [1, 4) — 第二个参数是 end，不是 length！
s.indexOf("ll");             s.indexOf("ll", 3);
s.replace("ll", "LL");       // 不修改原 String
s + "abc";                   // ⚠️ 循环里 O(n²)，用 StringBuilder

StringBuilder sb = new StringBuilder();
sb.append("abc").append(123);
String result = sb.toString();
```

---

## 6. pair / 多值返回对照

```cpp
// C++
pair<int, int> p = {1, 2};
p.first; p.second;
auto [a, b] = func();        // C++17 解构

// 返回多值
pair<int, int> func() { return {1, 2}; }
```

```java
// Java —— 没有内置 pair！选项：

// 1) int[]（最常用）
int[] p = {1, 2};
p[0]; p[1];

// 2) Map.Entry（不常用，仅在已有 entry 场景）
Map.Entry<Integer, Integer> e = Map.entry(1, 2);  // Java 9+
e.getKey(); e.getValue();

// 3) 自定义类（推荐复杂结构）
class Pair {
    int first, second;
    Pair(int f, int s) { first = f; second = s; }
}

// 4) record（Java 14+，最干净）
record Pair(int first, int second) {}
Pair p = new Pair(1, 2);
p.first(); p.second();
```

---

## 7. 高频代码片段对比

### 频次统计
```cpp
// C++
unordered_map<char, int> cnt;
for (char c : s) cnt[c]++;
```
```java
// Java
Map<Character, Integer> cnt = new HashMap<>();
for (char c : s.toCharArray()) cnt.merge(c, 1, Integer::sum);
// 或：cnt.put(c, cnt.getOrDefault(c, 0) + 1);
```

### 频次桶（小写字母）
```cpp
int cnt[26] = {0};
for (char c : s) cnt[c - 'a']++;
```
```java
int[] cnt = new int[26];
for (char c : s.toCharArray()) cnt[c - 'a']++;
```

### 邻接表（图）
```cpp
vector<vector<int>> g(n);
g[u].push_back(v);
```
```java
List<List<Integer>> g = new ArrayList<>();
for (int i = 0; i < n; i++) g.add(new ArrayList<>());
g.get(u).add(v);
// 或更紧凑：
Map<Integer, List<Integer>> g = new HashMap<>();
g.computeIfAbsent(u, k -> new ArrayList<>()).add(v);
```

### 优先队列：自定义小顶堆
```cpp
priority_queue<int, vector<int>, greater<int>> pq;
```
```java
PriorityQueue<Integer> pq = new PriorityQueue<>();   // 默认就是小顶堆
```

### 优先队列：按 pair 第二位排序
```cpp
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;  // 按 first 升序
```
```java
PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[1] - b[1]);  // 按第二位升序
```

### 反转
```cpp
reverse(v.begin(), v.end());
reverse(s.begin(), s.end());
```
```java
Collections.reverse(list);
new StringBuilder(s).reverse().toString();   // String 没 reverse
// int[] 反转要手写
```

### 去重
```cpp
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());
```
```java
Set<Integer> set = new LinkedHashSet<>(list);
List<Integer> unique = new ArrayList<>(set);
```

### lambda 捕获 / 闭包
```cpp
int x = 5;
auto f = [&](int a) { return a + x; };
```
```java
final int x = 5;        // 必须 final 或 effectively final
java.util.function.IntUnaryOperator f = a -> a + x;
```

---

## 8. 循环模式对照

### 范围 for
```cpp
for (auto& x : v) ...                 // 引用，可改
for (auto x : v) ...                  // 拷贝
for (auto& [k, v] : m) ...            // 解构
```
```java
for (int x : arr) ...                 // 注意是值拷贝（int 是基本类型）
for (Integer x : list) ...
for (Map.Entry<K, V> e : map.entrySet()) {
    K k = e.getKey(); V v = e.getValue();
}
for (K k : map.keySet()) ...
for (V v : map.values()) ...
```

### 同时拿 index + 元素
```cpp
for (int i = 0; i < v.size(); i++) v[i] ...
```
```java
for (int i = 0; i < arr.length; i++) arr[i] ...
// 或用 IntStream（罕用）
IntStream.range(0, arr.length).forEach(i -> ...);
```

---

## 9. 输入输出（LeetCode 不用，但本地调试有用）

```cpp
cin >> n;
cout << ans << endl;
```
```java
import java.util.Scanner;
Scanner sc = new Scanner(System.in);
int n = sc.nextInt();
System.out.println(ans);

// 高速 IO（大数据量）
BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
int n = Integer.parseInt(br.readLine().trim());
StringBuilder out = new StringBuilder();
out.append(ans).append('\n');
System.out.print(out);
```

---

## 10. 没法 1:1 对照的差异

| C++ 有，Java 无 / 不同 | Java 替代 |
|----|----|
| 模板（template） | 泛型（generics）— 类型擦除，无法 `new T[]` |
| 引用 `int&` | 没有，全是值传递（基本类型）/ 引用拷贝（对象） |
| 指针 `int*` | 没有指针，只有引用 |
| `const` | `final`（语义不完全相同） |
| 运算符重载 | 没有，必须 `equals` / `compareTo` |
| `pair<A,B>` 内置 | 用 `int[]` / `record` |
| `decltype` / `auto` | `var`（Java 10+，仅局部变量推断） |
| 多继承 | 单继承 + 多接口实现 |
| 析构函数 / RAII | GC + try-with-resources |
| 函数重载默认参数 | 多个 overload 方法 |
| 头文件 / `#include` | `import` + 包结构 |
| 宏 `#define` | 没有，用 `final static` |

---

## 11. 三句心智模型转换

1. **"C++ vector 用 ArrayList，但要装 Integer 而非 int"** — wrapper 是必经之路
2. **"C++ map 用 HashMap，要排序用 TreeMap"** — 默认无序，比 C++ 反过来
3. **"C++ priority_queue 默认大顶堆，Java 默认小顶堆"** — 反的，永远要确认
