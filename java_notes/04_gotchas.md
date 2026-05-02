# Java 刷题高频坑（必看）

> 给 C++ 背景的人专门挑出来的踩坑清单，每个都是面试时实际会出 bug 的。

---

## 1. `==` vs `.equals()` ★★★

```java
String a = "hello";
String b = new String("hello");

a == b              // false（不同对象引用）
a.equals(b)         // true（内容相等）

Integer x = 1000;
Integer y = 1000;
x == y              // false（超出 cache 范围）
x.equals(y)         // true

Integer p = 100;
Integer q = 100;
p == q              // true（[-128, 127] 在 Integer cache）
```

**规则**：
- 基本类型（`int`, `long`, `double`, `char`, `boolean`）用 `==`
- 引用类型（`Integer`, `String`, `int[]`, 自定义类）用 `.equals()`

**最坑场景**：把 `Integer` 当 `int` 用
```java
List<Integer> list = ...;
if (list.get(0) == 1000) ...   // 可能 false！要么用 equals，要么先 intValue
if (list.get(0).intValue() == 1000) ...   // ✅
if (list.get(0).equals(1000)) ...         // ✅
```

---

## 2. `int / Integer` 自动装箱（autoboxing）陷阱

### NullPointerException 隐患
```java
Map<Integer, Integer> map = new HashMap<>();
int x = map.get(5);              // 💥 NPE！map 里没 5，返回 null，自动拆箱炸
int x = map.getOrDefault(5, 0);  // ✅
```

### 删除元素的歧义
```java
List<Integer> list = new ArrayList<>(Arrays.asList(1, 2, 3));
list.remove(1);                  // 删除 index 1（值 2）！不是删值 1
list.remove(Integer.valueOf(1)); // ✅ 删值 1
list.remove((Integer) 1);        // ✅
```

### 性能损耗
```java
// ❌ 频繁装箱
Map<Integer, Integer> cnt = new HashMap<>();
for (int i = 0; i < 1000000; i++) cnt.put(i, i);  // 每次装/拆箱

// ✅ 大数据量直接用数组
int[] cnt = new int[1000000];
```

---

## 3. 整数溢出与除法

```java
int a = Integer.MAX_VALUE;
int b = a + 1;                   // 溢出 → -2^31

int mid = (left + right) / 2;    // 大数会溢出
int mid = left + (right - left) / 2;     // ✅ 二分查找标准写法

// 比较器溢出（PriorityQueue / Arrays.sort 常见）
(a, b) -> a - b                  // ❌ a, b 接近 MIN/MAX 时溢出
(a, b) -> Integer.compare(a, b)  // ✅

// 强转大数
long x = (long) Integer.MAX_VALUE * 2;     // ✅ 先转 long 再乘
long x = Integer.MAX_VALUE * 2;            // ❌ 先溢出，再转 long
long x = (long)(Integer.MAX_VALUE * 2);    // ❌ 同上

// 负数除法
-7 / 2                  // -3（向 0 截断）
Math.floorDiv(-7, 2)    // -4（向负无穷取整）

// 负数模
-7 % 3                  // -1
Math.floorMod(-7, 3)    // 2（永远非负）— 取模成正数推荐
```

---

## 4. String 不可变 + 性能

```java
// ❌ O(n²)
String s = "";
for (int i = 0; i < n; i++) s += i;

// ✅ O(n)
StringBuilder sb = new StringBuilder();
for (int i = 0; i < n; i++) sb.append(i);
return sb.toString();

// String 修改字符必须先转 char[]
String s = "hello";
s.charAt(0) = 'H';                   // ❌ 编译错
char[] arr = s.toCharArray();
arr[0] = 'H';
s = new String(arr);                  // ✅
```

---

## 5. 数组 / 集合互转的坑

### `Arrays.asList(int[])` 不是你想的那样
```java
int[] arr = {1, 2, 3};
List<int[]> wrong = Arrays.asList(arr);       // ❌ 一个元素，是 int[] 本身
List<Integer> correct = Arrays.stream(arr).boxed().collect(Collectors.toList());

// Integer[] 才正常
Integer[] boxed = {1, 2, 3};
List<Integer> ok = Arrays.asList(boxed);     // ✅ 但是固定大小！
```

### `Arrays.asList()` 返回的是固定大小的 List
```java
List<Integer> list = Arrays.asList(1, 2, 3);
list.add(4);                    // 💥 UnsupportedOperationException
list.set(0, 5);                 // ✅ 修改 OK
list.remove(0);                 // 💥

// 真要可变：
List<Integer> list = new ArrayList<>(Arrays.asList(1, 2, 3));
```

### `List.of(...)` 是不可变（Java 9+）
```java
List<Integer> list = List.of(1, 2, 3);    // 完全不可变，连 set 都不行
```

---

## 6. 二维数组 / 集合的初始化

```java
int[][] grid = new int[m][n];           // ✅ 全 0
boolean[][] vis = new boolean[m][n];    // ✅ 全 false
Integer[][] g = new Integer[m][n];      // ⚠️ 全 null，不是 0

// List of List —— 必须每行单独 new！
List<List<Integer>> result = new ArrayList<>();
for (int i = 0; i < n; i++) {
    result.add(new ArrayList<>());      // 否则 NPE
}

// 数组的"clone"是浅拷贝（一维 OK，二维不够）
int[][] copy = grid.clone();             // ❌ 行还是共享引用
int[][] copy = new int[m][];
for (int i = 0; i < m; i++) copy[i] = grid[i].clone();   // ✅
```

---

## 7. PriorityQueue 默认是小顶堆（与 C++ 相反）

```java
PriorityQueue<Integer> pq = new PriorityQueue<>();          // 小顶堆！
pq.offer(3); pq.offer(1); pq.offer(2);
pq.poll();      // 1（最小）

// 想要大顶堆：
PriorityQueue<Integer> max = new PriorityQueue<>(Comparator.reverseOrder());
PriorityQueue<Integer> max2 = new PriorityQueue<>((a, b) -> b - a);   // 仅小数据安全
```

### `pq.remove(任意元素)` 是 O(n)，没有 decrease-key
- Dijkstra 等算法用"懒删除"模式

---

## 8. HashMap / HashSet 的对象作 key

```java
class Point {
    int x, y;
    Point(int x, int y) { this.x = x; this.y = y; }
    // ⚠️ 没重写 equals/hashCode，HashMap 找不到！
}

Set<Point> set = new HashSet<>();
set.add(new Point(1, 2));
set.contains(new Point(1, 2));   // false！默认按地址比

// ✅ 重写 equals + hashCode（IDE 可生成）
@Override public boolean equals(Object o) { ... }
@Override public int hashCode() { return Objects.hash(x, y); }

// 或干脆用 String / int[] 编码
Set<String> set = new HashSet<>();
set.add(x + "," + y);

// 或用 record（Java 14+，自动生成 equals/hashCode）
record Point(int x, int y) {}
```

### `int[]` 作 key 也不行（默认 hashCode 基于地址）
```java
Map<int[], Integer> map = new HashMap<>();   // ❌ 不能正常工作
// ✅ 改用 String key 或转成 List<Integer>
String key = Arrays.toString(arr);
```

---

## 9. 方法/变量隐式 final 之谜

```java
int x = 5;
list.forEach(item -> System.out.println(x));  // ✅ x 是 effectively final

x = 10;                                       // 一旦改了
list.forEach(item -> System.out.println(x));  // ❌ 编译错

// lambda / 内部类捕获的局部变量必须是 final 或 effectively final
// 对应 C++ 的 [&] 引用捕获，但 Java 必须 const 语义
```

**绕过方法**：用单元素数组或 AtomicInteger
```java
int[] count = {0};
list.forEach(item -> count[0]++);   // ✅ 修改的是数组内容，不是 count 变量
```

---

## 10. 递归栈深度（JVM 默认 ~10000）

```java
// 深递归易栈溢出
void dfs(TreeNode node) {
    dfs(node.left);
    dfs(node.right);
}

// 解决：
// 1) 改迭代（用栈模拟）
// 2) 增大栈：开新线程
new Thread(null, () -> { dfs(root); }, "stack", 1 << 26).start();
// 3) 尾递归 Java 不优化，必须改迭代
```

---

## 11. `final` ≠ `const`

```java
final int[] arr = {1, 2, 3};
arr[0] = 99;                     // ✅ OK！final 只是引用不能改
arr = new int[]{};               // ❌ 不能重新指向

final List<Integer> list = new ArrayList<>();
list.add(1);                     // ✅ 内容可变
list = new ArrayList<>();        // ❌
```
**记住**：`final` 是 C++ 的 `T* const`，不是 `const T`。

---

## 12. 比较器（Comparator）的细节

```java
// 升序
Arrays.sort(arr, (a, b) -> Integer.compare(a, b));
// 降序
Arrays.sort(arr, (a, b) -> Integer.compare(b, a));

// ⚠️ 比较器必须有 strict ordering，否则抛异常
//    "Comparison method violates its general contract"
//    常见错误：用 boolean 算术
(a, b) -> a > b          // ❌ 不是合法的 int compare（lambda 返回 boolean）
(a, b) -> a > b ? 1 : 0  // ❌ 不传递性！缺少 -1 case
```

---

## 13. `Stream` API（少用但要看懂）

```java
// 求和
int sum = Arrays.stream(arr).sum();
int sum2 = list.stream().mapToInt(Integer::intValue).sum();

// 过滤
List<Integer> even = list.stream()
    .filter(x -> x % 2 == 0)
    .collect(Collectors.toList());

// 频次
Map<Character, Long> freq = s.chars().boxed()
    .collect(Collectors.groupingBy(c -> (char)(int)c, Collectors.counting()));

// 转 int[]
int[] arr = list.stream().mapToInt(Integer::intValue).toArray();

// ⚠️ Stream 不可重用 —— 用过一次就废了
// ⚠️ 大数据量时性能不如循环（lambda 装箱）
// 面试推荐：能用循环就用循环，stream 仅作为锦上添花
```

---

## 14. switch（Java 17+ 表达式）

```java
// 老写法
String day;
switch (n) {
    case 1: day = "Mon"; break;
    case 2: day = "Tue"; break;
    default: day = "?";
}

// 新表达式
String day = switch (n) {
    case 1 -> "Mon";
    case 2 -> "Tue";
    default -> "?";
};

// 模式匹配（Java 21+，少用于刷题）
```

---

## 15. 一行 / 多行的字面量

```java
int[] a = {1, 2, 3};                       // ✅ 字面量
int[][] g = {{1,2},{3,4}};                 // ✅
List<Integer> l = List.of(1, 2, 3);        // ✅ 不可变
List<Integer> l2 = new ArrayList<>(List.of(1,2,3));  // ✅ 可变
Map<Integer,Integer> m = Map.of(1,2,3,4);  // ✅ 不可变，最多 10 对
```

---

## 16. 高频面试 1 行救命

```java
Math.max(a, b); Math.min(a, b);
Math.abs(x);
Math.floorMod(x, m);                       // 永远非负的模
Integer.bitCount(x);                       // x 中 1 的个数
Integer.numberOfTrailingZeros(x);          // lowbit 位置
Long.toBinaryString(x);                    // 二进制字符串
Character.isDigit(c); Character.isLetter(c);
Integer.parseInt(s); Integer.toString(x);
Arrays.toString(arr);                      // 调试打印
Arrays.deepToString(grid);                 // 二维调试
Arrays.copyOfRange(arr, l, r);             // [l, r) 切片
Collections.swap(list, i, j);              // List 的 swap（数组没有！）
Collections.frequency(list, x);            // 某值出现次数（O(n)）
```

---

## 17. 面试时容易写错的细节

| 错 | 对 |
|---|---|
| `arr.length()` | `arr.length`（属性） |
| `s.length` | `s.length()`（方法） |
| `list.length` | `list.size()` |
| `s == "abc"` | `s.equals("abc")` |
| `map.contains(k)` | `map.containsKey(k)`（containsValue 是另一个） |
| `pq.top()` | `pq.peek()` |
| `pq.push(x)` | `pq.offer(x)` 或 `add(x)` |
| `Stack.empty()` | `deque.isEmpty()`（不要用 Stack 类） |
| `Arrays.sort(int[], cmp)` | int[] 不支持比较器！转 Integer[] |
| `s.substr(2, 3)` | `s.substring(2, 5)`（end 不是 length） |
