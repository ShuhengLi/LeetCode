# 数组与字符串

## 1. 数组（Array）

### 声明与初始化
```java
int[] a = new int[10];                    // 全 0 初始化
int[] b = {1, 2, 3, 4, 5};                // 字面量
int[] c = new int[]{1, 2, 3};             // 显式 new
int[][] grid = new int[m][n];             // 二维全 0
int[][] grid2 = {{1,2,3},{4,5,6}};        // 二维字面量

// 不规则二维（jagged array）
int[][] jagged = new int[3][];
jagged[0] = new int[5];
jagged[1] = new int[2];
```

### 访问与属性
```java
a.length       // 注意：是属性不是方法（vs C++ vector.size()）
a[0]
a[a.length - 1]

// ⚠️ Integer[] vs int[]
int[] prim;        // 真数组，元素是 int
Integer[] boxed;   // 元素是 Integer 引用，可装 null
```

### Arrays 工具类（必会）
```java
import java.util.Arrays;

// 排序
Arrays.sort(a);                         // 升序，O(n log n)
Arrays.sort(a, 2, 5);                   // [2, 5) 区间排序
Arrays.sort(boxed, Collections.reverseOrder());  // 降序，仅对 Integer[] 有效
Arrays.sort(boxed, (x, y) -> y - x);    // 自定义比较器，仅 Object[]
// ⚠️ int[] 不能用 lambda 比较器！必须先转成 Integer[]

// 二分查找（数组必须先排序）
int idx = Arrays.binarySearch(a, target);
// 找到：返回 index；找不到：返回 -(insertion_point + 1)
// 转换：if (idx < 0) idx = -idx - 1;  // 得到插入位置 (= lower_bound)

// 填充
Arrays.fill(a, 0);                      // 全填 0
Arrays.fill(a, 2, 5, 0);                // [2, 5) 填 0
for (int[] row : grid) Arrays.fill(row, -1);  // 二维填 -1

// 复制
int[] copy = Arrays.copyOf(a, 10);              // 复制前 10 个（不足补 0）
int[] sub = Arrays.copyOfRange(a, 2, 5);        // [2, 5)

// 比较
Arrays.equals(a, b);                    // 元素全等
Arrays.deepEquals(grid1, grid2);        // 二维

// 打印
System.out.println(Arrays.toString(a));         // [1, 2, 3]
System.out.println(Arrays.deepToString(grid));  // [[1,2],[3,4]]

// 哈希（用作 HashMap key 时必备！）
Arrays.hashCode(a);     // 数组本身 hashCode 不基于内容！必须用这个
```

### 常用模式
```java
// 反转数组（无 reverse 内置）
void reverse(int[] a) {
    int l = 0, r = a.length - 1;
    while (l < r) { int t = a[l]; a[l++] = a[r]; a[r--] = t; }
}

// 交换（无 swap 内置）
int t = a[i]; a[i] = a[j]; a[j] = t;

// 数组转 List
List<Integer> list = Arrays.stream(a).boxed().collect(Collectors.toList());
// 或
List<Integer> list = new ArrayList<>();
for (int x : a) list.add(x);

// List 转数组
int[] arr = list.stream().mapToInt(Integer::intValue).toArray();
Integer[] boxed = list.toArray(new Integer[0]);
```

---

## 2. 字符串（String）

### 核心：String 是不可变的（immutable）

```java
String s = "hello";
String s2 = s + " world";       // 创建新对象，原 s 不变
s.toUpperCase();                // ❌ 不会改 s！返回新字符串
s = s.toUpperCase();            // ✅ 必须重新赋值
```

**循环里拼接 → 严重性能问题（O(n²)），用 StringBuilder！**

### 常用方法
```java
String s = "hello";
s.length();                     // 长度（vs array.length 没括号）
s.charAt(0);                    // 'h'，访问字符
s.indexOf('l');                 // 2，第一次出现
s.indexOf("ll");                // 子串首位置
s.lastIndexOf('l');             // 最后一次
s.substring(1);                 // "ello"
s.substring(1, 4);              // "ell"，[1,4)
s.contains("ell");              // true
s.startsWith("he");             // true
s.endsWith("lo");
s.replace('l', 'L');            // 替换字符
s.replace("ll", "LL");          // 替换子串
s.toLowerCase();
s.toUpperCase();
s.trim();                       // 去首尾空白
s.strip();                      // 同 trim 但支持 Unicode 空白（Java 11+）
s.split(",");                   // → String[]，按 regex 分割
s.split(",", 3);                // 限制分片数

// 比较 ★★★ 必看陷阱
s.equals(s2);                   // ✅ 内容比较
s == s2;                        // ❌ 引用比较！会出 bug
s.equalsIgnoreCase(s2);
s.compareTo(s2);                // 字典序比较，<0 / 0 / >0
```

### char[] 与 String 互转
```java
char[] arr = s.toCharArray();   // 转 char 数组（拷贝）
String s2 = new String(arr);
String s3 = String.valueOf(arr);
String s4 = new String(arr, 0, 3);  // 用前 3 个字符

// 字符判断
Character.isDigit('5');
Character.isLetter('a');
Character.isLetterOrDigit('a');
Character.isUpperCase('A');
Character.isLowerCase('a');
Character.toLowerCase('A');     // 'a'
Character.toUpperCase('a');     // 'A'

// 字符 ↔ 数字（频次桶）
int idx = c - 'a';              // 'a' → 0, 'b' → 1
char c = (char)('a' + idx);
int digit = c - '0';            // '5' → 5

// 反转字符串
new StringBuilder(s).reverse().toString();
```

### StringBuilder（频繁修改用这个）

```java
import java.lang.StringBuilder;  // 自动 import

StringBuilder sb = new StringBuilder();
sb.append("hello");
sb.append(' ');
sb.append(42);                  // 数字也支持
sb.append(true);

sb.length();
sb.charAt(0);
sb.setCharAt(0, 'H');           // 修改字符
sb.insert(0, "Hi");
sb.delete(0, 2);                // 删 [0,2)
sb.deleteCharAt(0);
sb.reverse();                   // 原地反转！

sb.toString();                  // 转回 String

// 高频用法
StringBuilder result = new StringBuilder();
for (int x : nums) {
    result.append(x).append(',');
}
if (result.length() > 0) result.setLength(result.length() - 1);  // 删尾逗号
return result.toString();
```

### String 性能对比
```java
// O(n²) ❌
String s = "";
for (int i = 0; i < n; i++) s += i;   // 每次创建新 String

// O(n) ✅
StringBuilder sb = new StringBuilder();
for (int i = 0; i < n; i++) sb.append(i);
return sb.toString();
```

### 字符串格式化
```java
String.format("%d-%s-%.2f", 1, "abc", 3.14);  // 类似 sprintf
String.join(",", "a", "b", "c");              // "a,b,c"
String.join("-", list);                        // 拼 List<String>

// 重复字符串（Java 11+）
"ab".repeat(3);     // "ababab"

// 多行字符串（Java 15+）
String text = """
    line1
    line2
    """;
```

---

## 3. 二维网格（grid / matrix）模板

```java
int[][] grid = ...;
int m = grid.length;            // 行数
int n = grid[0].length;         // 列数

// 遍历
for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
        grid[i][j] ...
    }
}

// 4 方向
int[][] dirs = {{-1,0},{1,0},{0,-1},{0,1}};
for (int[] d : dirs) {
    int ni = i + d[0], nj = j + d[1];
    if (ni < 0 || ni >= m || nj < 0 || nj >= n) continue;
    // process
}

// 8 方向
int[][] dirs8 = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

// visited
boolean[][] visited = new boolean[m][n];

// 拷贝二维数组
int[][] copy = new int[m][n];
for (int i = 0; i < m; i++) {
    copy[i] = grid[i].clone();   // 一维 clone
}
// 或
int[][] copy = Arrays.stream(grid).map(int[]::clone).toArray(int[][]::new);
```

---

## 4. 整数 / 数字

```java
Integer.MAX_VALUE              // 2^31 - 1
Integer.MIN_VALUE              // -2^31
Long.MAX_VALUE
Math.max(a, b);
Math.min(a, b);
Math.abs(x);
Math.pow(2, 10);               // 返回 double！要 (int) 强转
(int) Math.pow(2, 10);
Math.sqrt(x);
Math.floor(x), Math.ceil(x);   // 返回 double
Math.floorDiv(7, 2);           // 3 （处理负数除法）
Math.floorMod(-7, 3);          // 2 （正模数，处理负数）

// 字符串转数字
Integer.parseInt("123");
Integer.parseInt("-12", 10);
Long.parseLong("123456789");
Integer.parseInt("ff", 16);    // 16进制
Integer.toString(123);
Integer.toBinaryString(10);    // "1010"
Integer.toHexString(255);      // "ff"

// 位运算
Integer.bitCount(7);           // 1 的个数 → 3
Integer.numberOfLeadingZeros(8);
Integer.numberOfTrailingZeros(8);
Integer.highestOneBit(10);     // 最高 1 位的值（8）
```

---

## 5. ⚠️ 数组高频陷阱总结

1. **`int[]` 不能用 lambda 排序** — 转 `Integer[]` 或写循环
2. **`a.length` 没括号**（vs `s.length()` 字符串有括号）
3. **`Arrays.asList(arr)`** 对 `int[]` 返回的是 `List<int[]>`（一个元素），要先 `Arrays.stream(arr).boxed()`
4. **二维数组 `new int[m][n]` 全部初始化为 0**，但 `new Integer[m][n]` 全部是 `null`
5. **数组 hashCode 默认基于地址**，作为 HashMap key 用 `Arrays.hashCode(arr)` 或转成 String
