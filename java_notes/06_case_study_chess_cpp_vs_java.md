# Case Study：Chess Board OOP — Java vs C++ 全方位对照

> 用同一道 SoFi 真题（[ChessKnightValidator.java](../sofi/code/04_ChessKnightValidator.java)）做 1:1 实现，把 OOP 类设计中的所有 Java/C++ 差异一次讲透。

---

## Part 1: 完整 C++ 实现

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <cmath>
#include <utility>

// ─── 前向声明 ────────────────────────────────────────────────
// C++ 需要前向声明才能让 Piece 引用 ChessBoard
class ChessBoard;

// ─── 棋盘坐标 ────────────────────────────────────────────────
struct Position {
    int row, col;

    Position(int r, int c) : row(r), col(c) {}

    bool isValid() const {                       // const 表示不修改成员
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    // toString 等价：重载 << 运算符
    friend std::ostream& operator<<(std::ostream& os, const Position& p) {
        return os << "(" << p.row << "," << p.col << ")";
    }
};

// ─── 棋子类型枚举 ────────────────────────────────────────────
enum class PieceType { KNIGHT, BISHOP, ROOK, QUEEN, KING, PAWN };
// 用 enum class（C++11+）而不是 enum，避免名字冲突

// ─── 抽象棋子基类 ────────────────────────────────────────────
class Piece {
protected:
    const PieceType type;
    Position position;

    bool basicCheck(const Position& target) const {
        return target.isValid() && !(target == position);
    }

public:
    Piece(PieceType t, Position pos) : type(t), position(pos) {}

    // ★ 虚析构函数：多态基类必须有！否则 delete 子类会 UB
    virtual ~Piece() = default;

    // ★ 纯虚函数 = Java 的 abstract method
    virtual bool isValidMove(const Position& target,
                             const ChessBoard& board) const = 0;

    PieceType getType() const { return type; }
    Position getPosition() const { return position; }
};

// ─── 棋盘 ─────────────────────────────────────────────────────
class ChessBoard {
private:
    // unique_ptr 表示棋盘"拥有"这些棋子；析构时自动 delete
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid{};

public:
    void placePiece(std::unique_ptr<Piece> piece) {
        Position pos = piece->getPosition();
        grid[pos.row][pos.col] = std::move(piece);
        // ★ std::move 表示所有权转移，调用后原 piece 失效
    }

    bool isOccupied(const Position& pos) const {
        return grid[pos.row][pos.col] != nullptr;
    }

    // 返回非拥有的原始指针（observer pattern）
    Piece* getPieceAt(const Position& pos) const {
        return grid[pos.row][pos.col].get();
    }

    bool isPathBlockedStraight(const Position& from, const Position& to) const {
        // C++ 没有 Integer.signum，手动实现
        int rowStep = (to.row > from.row) - (to.row < from.row);
        int colStep = (to.col > from.col) - (to.col < from.col);
        int r = from.row + rowStep;
        int c = from.col + colStep;
        while (r != to.row || c != to.col) {
            if (grid[r][c] != nullptr) return true;
            r += rowStep;
            c += colStep;
        }
        return false;
    }

    bool isPathBlockedDiagonal(const Position& from, const Position& to) const {
        return isPathBlockedStraight(from, to);  // 逻辑相同
    }
};

// ─── Knight ───────────────────────────────────────────────────
class Knight : public Piece {
private:
    static constexpr int MOVES[8][2] = {
        {-2, -1}, {-2, +1},
        {-1, -2}, {-1, +2},
        {+1, -2}, {+1, +2},
        {+2, -1}, {+2, +1}
    };

public:
    Knight(Position pos) : Piece(PieceType::KNIGHT, pos) {}

    bool isValidMove(const Position& target,
                     const ChessBoard& board) const override {
        if (!basicCheck(target)) return false;
        int dr = std::abs(target.row - position.row);
        int dc = std::abs(target.col - position.col);
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }

    std::vector<Position> getAllValidMoves(const ChessBoard& board) const {
        std::vector<Position> result;
        for (const auto& delta : MOVES) {
            Position target(position.row + delta[0], position.col + delta[1]);
            if (isValidMove(target, board)) {
                result.push_back(target);
            }
        }
        return result;
    }
};

// ─── Bishop / Rook / King ─────────────────────────────────────
class Bishop : public Piece {
public:
    Bishop(Position pos) : Piece(PieceType::BISHOP, pos) {}

    bool isValidMove(const Position& target,
                     const ChessBoard& board) const override {
        if (!basicCheck(target)) return false;
        int dr = std::abs(target.row - position.row);
        int dc = std::abs(target.col - position.col);
        if (dr != dc) return false;
        return !board.isPathBlockedDiagonal(position, target);
    }
};

class Rook : public Piece {
public:
    Rook(Position pos) : Piece(PieceType::ROOK, pos) {}

    bool isValidMove(const Position& target,
                     const ChessBoard& board) const override {
        if (!basicCheck(target)) return false;
        if (target.row != position.row && target.col != position.col)
            return false;
        return !board.isPathBlockedStraight(position, target);
    }
};

class King : public Piece {
public:
    King(Position pos) : Piece(PieceType::KING, pos) {}

    bool isValidMove(const Position& target,
                     const ChessBoard& board) const override {
        if (!basicCheck(target)) return false;
        int dr = std::abs(target.row - position.row);
        int dc = std::abs(target.col - position.col);
        return dr <= 1 && dc <= 1;
    }
};

// ─── 测试 ─────────────────────────────────────────────────────
int main() {
    ChessBoard board;

    // 棋盘获得 Knight 的所有权（unique_ptr 转移）
    auto knight = std::make_unique<Knight>(Position(4, 4));
    Knight* knightPtr = knight.get();           // 保留非拥有指针用于测试
    board.placePiece(std::move(knight));        // knight 之后不可用！

    std::cout << std::boolalpha;                // 让 cout 输出 true/false
    std::cout << "=== Knight at (4,4) ===\n";
    std::cout << "Move to (2,3): "
              << knightPtr->isValidMove(Position(2, 3), board) << "\n";
    std::cout << "Move to (3,3): "
              << knightPtr->isValidMove(Position(3, 3), board) << "\n";

    auto moves = knightPtr->getAllValidMoves(board);
    std::cout << "All valid moves: ";
    for (const auto& m : moves) std::cout << m << " ";
    std::cout << "\n";

    // King
    auto king = std::make_unique<King>(Position(0, 4));
    King* kingPtr = king.get();
    board.placePiece(std::move(king));

    std::cout << "\n=== King at (0,4) ===\n";
    std::cout << "Move to (1,4): "
              << kingPtr->isValidMove(Position(1, 4), board) << "\n";
    std::cout << "Move to (2,4): "
              << kingPtr->isValidMove(Position(2, 4), board) << "\n";

    return 0;
    // 棋盘析构时，所有 unique_ptr 自动释放棋子内存
    // ★ Java 等 GC，C++ 在 scope 结束时确定性析构
}
```

---

## Part 2: 差异详解（按重要性排序）

### 🔥 #1 内存管理 — 最大的差异

| Java | C++ |
|------|-----|
| GC 自动回收 | 必须手动管理（或用智能指针） |
| `new Knight(...)` 返回引用，自动 GC | `new Knight(...)` 返回 raw 指针，需手动 `delete` |
| 没有"所有权"概念 | unique_ptr / shared_ptr / 裸指针有不同语义 |
| 没有析构顺序保证 | RAII：scope 结束确定性析构 |

**Java**:
```java
Knight knight = new Knight(new Position(4, 4));   // GC 管
board.placePiece(knight);                          // 共享引用
// 不需要 delete
```

**C++ (现代写法)**:
```cpp
auto knight = std::make_unique<Knight>(Position(4, 4));  // unique 所有权
board.placePiece(std::move(knight));                     // 转移所有权
// knight 失效，board 析构时自动释放
```

**C++ (老式不推荐)**:
```cpp
Knight* knight = new Knight(Position(4, 4));  // 裸 new
board.placePiece(knight);                      // 谁负责 delete？不清楚
// 需要手动 delete，否则内存泄漏
```

### 🔥 #2 多态：virtual / override / 纯虚

| 概念 | Java | C++ |
|------|------|-----|
| 虚函数（默认） | 所有非 static 方法**默认 virtual** | 必须显式 `virtual` 才支持多态 |
| 纯虚 / 抽象 | `abstract` 关键字 | `virtual ... = 0;` |
| 重写检查 | `@Override` 注解（可选但推荐） | `override` 关键字（C++11+，强烈推荐） |
| 终止重写 | `final` 方法 | `final` 关键字（C++11+） |
| 析构函数 | 不需要（GC） | **多态基类必须 `virtual ~Base()`** ★★★ |

```java
abstract class Piece {
    abstract boolean isValidMove(...);    // 抽象
    public void foo() {}                   // 默认 virtual
}
```
```cpp
class Piece {
public:
    virtual ~Piece() = default;            // ★ 必须 virtual！
    virtual bool isValidMove(...) const = 0;  // 纯虚
    void foo() {}                          // 非 virtual！子类不能覆盖多态行为
};
```

**血泪坑**：C++ 基类不写 `virtual ~Base()`，通过基类指针 `delete` 子类对象 → undefined behavior（子类析构不调用，资源泄漏）。

---

### 🔥 #3 const 正确性

C++ 的 `const` 比 Java 的 `final` 强大得多。

| 场景 | Java | C++ |
|------|------|-----|
| 常量值 | `final int x = 5;` | `const int x = 5;` |
| 不可重新赋值的引用 | `final` 应用引用 | `int* const p`（指针不可改） |
| 不能通过此引用修改对象 | **没有！** | `const int* p` / `const T&` |
| const 方法 | 没有对应 | `void foo() const;`（方法不修改成员） |
| const 容器 | `Collections.unmodifiableList()` | `const std::vector<T>&` |

```cpp
bool isValid() const {  // ← 这个 const 表示"不修改 this"
    return row >= 0 && row < 8;
}

void process(const Position& pos);    // 不能修改 pos
const Position& getPos() const;       // 返回不可修改的引用
```

**Java 没有**："这个方法不修改对象状态"的编译时保证。Java 只能靠约定 + immutable 类设计。

---

### 🔥 #4 引用传递 vs 值传递

| | Java | C++ |
|---|------|-----|
| 基本类型 | 值传递 (int, double, ...) | 值传递 (默认) |
| 对象 | **引用拷贝**（看似值传递，实则引用） | **可选**：值/引用/指针 |
| 显式引用 | 没有，全部隐式 | `T&` / `const T&` |
| 显式指针 | 没有 | `T*` |
| 修改参数 | 修改对象的字段会影响调用者 | 取决于声明 |

**Java**:
```java
void modify(Position p) {
    p.row = 999;       // 调用者的 Position 也会改（同一对象）
    p = new Position(0,0);  // 但重新指向不影响调用者
}
```

**C++**:
```cpp
void modify1(Position p);          // 值拷贝，不影响外部
void modify2(Position& p);         // 引用，可改
void modify3(const Position& p);   // 引用，但只读（推荐传大对象）
void modify4(Position* p);         // 指针，可空，可改
```

**经验法则**（C++ 函数参数）：
- 基本类型 / 小结构（int、small struct）→ 值传递
- 大对象只读 → `const T&`（避免拷贝）
- 需要修改 → `T&`
- 可空 → `T*`

---

### 🔥 #5 类声明 / 嵌套 / 静态

| | Java | C++ |
|---|------|-----|
| 嵌套类 | `static class Inner {}` 内部静态类 | `struct/class Inner {}` 在 class 内 |
| 类与文件 | 一个 public class 一个 .java 文件 | 任意，header (.h/.hpp) + source (.cpp) |
| 默认访问 | 包级私有 | `class` 默认 private，`struct` 默认 public |
| 静态成员 | `static int x;` | `static int x;` 类内声明 + 类外定义 |
| 静态常量 | `static final int X = 5;` | `static constexpr int X = 5;` (C++17 起类内即可定义) |

**Java**:
```java
public class ChessKnightValidator {
    static class Position { ... }
    static class Piece { ... }
    static class Knight extends Piece { ... }
    public static void main(...) { ... }
}
// 一个文件搞定（LeetCode 友好）
```

**C++**:
```cpp
// 通常放在 chess.h 头文件 + chess.cpp 实现
struct Position { ... };
class Piece { ... };
class Knight : public Piece { ... };
int main() { ... }
// 顶层平铺，没有"嵌套类"必要
```

---

### 🔥 #6 继承语法

```java
class Knight extends Piece {
    Knight(Position pos) {
        super(PieceType.KNIGHT, pos);   // 调父类构造
    }

    @Override
    boolean isValidMove(...) { ... }
}
```
```cpp
class Knight : public Piece {           // ← public 继承
public:
    Knight(Position pos) : Piece(PieceType::KNIGHT, pos) {}
                            // ↑ 初始化列表调父类构造

    bool isValidMove(...) const override {  // override 关键字
        ...
    }
};
```

| 概念 | Java | C++ |
|------|------|-----|
| 继承关键字 | `extends` | `:` |
| 继承类型 | 单继承 + 多接口 | **多重继承**（强大但易乱） |
| 调父类构造 | `super(...)` 在构造函数体内首行 | **初始化列表** `: Base(...)`（更高效） |
| 接口 | `interface` + `implements` | 纯虚类（多个） |

---

### 🔥 #7 enum 差异

```java
enum PieceType { KNIGHT, BISHOP, ROOK }
PieceType t = PieceType.KNIGHT;
// Java enum 是真正的对象，可有方法、字段
```
```cpp
enum class PieceType { KNIGHT, BISHOP, ROOK };  // ← 必须 class
PieceType t = PieceType::KNIGHT;                // ← 命名空间访问
// 如果用旧式 enum，KNIGHT 会污染外部命名空间
```

| | Java enum | C++ `enum class` |
|---|-----------|---------------|
| 类型安全 | 强（真正对象） | 强（不会隐式转 int） |
| 命名空间 | 自动隔离 | 用 `EnumType::Value` |
| 可加方法 | ✅ | ❌ 只能整数值 |
| switch | ✅ | ✅ |

---

### 🔥 #8 字符串 / 输出

| | Java | C++ |
|---|------|-----|
| 字符串类 | `String`（不可变） | `std::string`（可变） |
| 长度 | `s.length()` | `s.size()` 或 `s.length()` |
| 拼接慢循环 | 用 `StringBuilder` | 用 `std::ostringstream` 或 `+=`（vector 化优化） |
| 输出 | `System.out.println(...)` | `std::cout << ... << "\n"` |
| 格式化 | `String.format("%d", x)` | `std::format("{}", x)`（C++20）或 printf |
| 字符串字面量 | `"abc"` 是 String | `"abc"` 是 `const char*` |
| toString | `@Override toString()` | 重载 `operator<<` |

```java
@Override
public String toString() { return "(" + row + "," + col + ")"; }
```
```cpp
friend std::ostream& operator<<(std::ostream& os, const Position& p) {
    return os << "(" << p.row << "," << p.col << ")";
}
```

---

### 🔥 #9 容器对照

| Java | C++ |
|------|-----|
| `ArrayList<T>` | `std::vector<T>` |
| `HashMap<K,V>` | `std::unordered_map<K,V>` |
| `TreeMap<K,V>` | `std::map<K,V>` |
| `HashSet<T>` | `std::unordered_set<T>` |
| `ArrayDeque<T>` | `std::deque<T>` |
| `PriorityQueue<T>` (小顶堆) | `std::priority_queue<T>` (大顶堆!) |
| `int[]` | `std::array<int, N>` (固定) / `int[N]` (C 风格) / `std::vector<int>` (动态) |
| `int[][]` | `std::array<std::array<int, N>, M>` 或 `std::vector<std::vector<int>>` |

```java
List<Position> result = new ArrayList<>();
result.add(target);
```
```cpp
std::vector<Position> result;
result.push_back(target);     // 或 emplace_back（避免拷贝）
```

---

### 🔥 #10 容器迭代

```java
for (int[] delta : MOVES) { ... }                  // range-for
for (Map.Entry<K,V> e : map.entrySet()) { ... }
list.forEach(x -> ...);
```
```cpp
for (const auto& delta : MOVES) { ... }            // C++11+ range-for
for (const auto& [k, v] : map) { ... }             // C++17 解构
std::for_each(list.begin(), list.end(), [](auto x){...});

// 老式
for (auto it = list.begin(); it != list.end(); ++it) { ... }
```

---

### 🔥 #11 智能指针 ★（C++ 独有）

Java 没有这个概念，全靠 GC。C++ 必须懂：

```cpp
std::unique_ptr<T>  // 唯一所有权，不可复制，可 move
std::shared_ptr<T>  // 共享所有权，引用计数
std::weak_ptr<T>    // 弱引用，避免循环
T*                  // 非拥有指针 (raw)
```

**对应 Java 的"什么"**：

| C++ | Java 类比 |
|-----|----------|
| `T x;` (栈) | 没有真正对应（Java 一切对象在堆） |
| `std::unique_ptr<T>` | Java 引用 + 严格"只能一个变量持有"约定 |
| `std::shared_ptr<T>` | Java 引用（GC + ref count） |
| `T*` | Java 引用（但可能 null + 不拥有） |

---

### 🔥 #12 std::move 与移动语义

C++ 独有概念，Java 没有。

```cpp
std::unique_ptr<Knight> knight = std::make_unique<Knight>(...);
board.placePiece(std::move(knight));   // 所有权转移
// 此后 knight 是 nullptr / moved-from 状态
```

```java
Knight knight = new Knight(...);
board.placePiece(knight);               // 共享引用，knight 仍可用
```

---

### 🔥 #13 lambda / 闭包

```java
list.sort((a, b) -> a.row - b.row);
final int x = 5;
Runnable r = () -> System.out.println(x);  // 自动捕获 effectively final
```
```cpp
std::sort(v.begin(), v.end(), [](const Position& a, const Position& b) {
    return a.row < b.row;
});

int x = 5;
auto f = [x]() { std::cout << x; };       // [x] 值捕获
auto g = [&x]() { x++; };                  // [&x] 引用捕获
auto h = [=]() { ... };                    // [=] 全部值捕获
auto i = [&]() { ... };                    // [&] 全部引用捕获
```

C++ lambda 必须显式声明捕获方式（更明确，但啰嗦）。

---

### 🔥 #14 异常处理

| | Java | C++ |
|---|------|-----|
| 抛出 | `throw new Exception("msg")` | `throw std::runtime_error("msg")` |
| 捕获 | `catch (Exception e)` | `catch (const std::exception& e)` |
| Checked exception | 有（必须 declare 或 catch） | 没有（已废弃 throw 声明） |
| RAII / cleanup | `try-with-resources` | RAII 自动析构 |

---

### 🔥 #15 头文件 / 编译模型

Java 一个 `.java` 文件 = 一个编译单元，导入靠 `import`。

C++ 需要分头文件 + 源文件：

```cpp
// chess.h (声明)
#pragma once          // 防止重复包含
#include <vector>

class Piece {
public:
    virtual bool isValidMove(...) = 0;
};

// chess.cpp (实现)
#include "chess.h"
// ...

// main.cpp
#include "chess.h"
int main() { ... }
```

LeetCode 上 C++ 单文件提交，但生产代码必须分。

---

### 🔥 #16 操作符重载

```cpp
bool operator==(const Position& other) const {
    return row == other.row && col == other.col;
}

friend std::ostream& operator<<(std::ostream& os, const Position& p) {
    return os << "(" << p.row << "," << p.col << ")";
}
```

Java 没有！只能写 `equals()` / `compareTo()` / `toString()` 方法。

---

### 🔥 #17 静态成员定义

```java
private static final int[][] MOVES = {{...}};   // 一行搞定
```

```cpp
class Knight : public Piece {
    static constexpr int MOVES[8][2] = {{...}};  // C++17 起可类内定义
    // 老 C++（< C++17）必须类内声明 + 类外定义：
    // static const int MOVES[8][2];   // 类内
    // const int Knight::MOVES[8][2] = {{...}};  // 类外（在 .cpp）
};
```

---

### 🔥 #18 nullptr / null

```java
if (piece == null) ...
```
```cpp
if (piece == nullptr) ...    // C++11 起，类型安全
if (piece == NULL) ...        // 老式，是宏，类型不安全
```

C++ 还有"指针为空"和"对象为空"的区分（智能指针 `if (!ptr)`）。

---

### 🔥 #19 构造函数初始化列表 ★

C++ 的初始化列表更高效（直接初始化 vs 默认构造再赋值）。

```cpp
class Piece {
    PieceType type;
    Position position;
public:
    // 初始化列表（推荐）
    Piece(PieceType t, Position pos) : type(t), position(pos) {}

    // 函数体内赋值（不推荐，多一次默认构造）
    Piece(PieceType t, Position pos) {
        type = t;        // ❌ 编译错（type 是 const）
        position = pos;
    }
};
```

Java 没有这个区分。

---

### 🔥 #20 final / friend

| | Java | C++ |
|---|------|-----|
| 不可继承的类 | `final class C` | `class C final` |
| 不可重写方法 | `final` 方法 | `virtual void f() final;` |
| 不可改变量 | `final int x = 5` | `const int x = 5` |
| 友元 | 没有！ | `friend class X;` 允许 X 访问私有 |

---

## Part 3: 完整速查对照表

| 概念 | Java | C++ |
|------|------|-----|
| 类继承 | `extends Base` | `: public Base` |
| 接口 | `interface` + `implements` | 纯虚基类 |
| 抽象方法 | `abstract` | `virtual ... = 0;` |
| 重写 | `@Override` | `override` |
| 不可变量 | `final int x` | `const int x` |
| const 方法 | (无) | `void f() const` |
| 嵌套类 | `static class` | `class` 内嵌或顶层 |
| 调父类构造 | `super(...)` 函数体 | `: Base(...)` 初始化列表 |
| 静态常量 | `static final` | `static constexpr` |
| 析构 | (GC) | `virtual ~T() = default;` |
| null | `null` | `nullptr` |
| 输出 | `System.out.println` | `std::cout << "\n"` |
| toString | `toString()` 方法 | `operator<<` 重载 |
| equals | `equals()` 方法 | `operator==` 重载 |
| 引用 | (隐式所有对象) | `T&` |
| 指针 | (无) | `T*` |
| 智能指针 | (无，靠 GC) | `unique_ptr` / `shared_ptr` |
| 移动 | (无) | `std::move(x)` |
| 值/引用传递 | 隐式（对象引用） | 显式（`T`/`T&`/`T*`） |
| List | `ArrayList<T>` | `std::vector<T>` |
| HashMap | `HashMap<K,V>` | `std::unordered_map<K,V>` |
| TreeMap | `TreeMap<K,V>` | `std::map<K,V>` |
| Lambda | `(x) -> ...` | `[capture](args) { ... }` |
| 异常 | `throw new E()` | `throw E()` |
| 头文件 | `import` | `#include` |
| 编译 | 一文件一编译单元 | 头/源分离 |
| 多重继承 | 仅多接口 | 真正多重继承 |
| 泛型 | 类型擦除 | 模板（编译期展开） |
| operator overload | (无) | 支持 |
| friend | (无) | 支持 |

---

## Part 4: 一图总结心智模型

```
Java:  对象 = 引用 + 自动 GC + 默认 virtual
       简单但抽象层多，性能不可预测

C++:   对象 = 值 / 引用 / 指针 + RAII + 显式 virtual
       灵活强大，但要懂内存模型，否则随时 UB
```

### 三个最影响你写法的差异

1. **内存所有权**
   - Java：扔出去就行，GC 收尾
   - C++：必须想清楚"谁拥有它"，用 unique_ptr 表达

2. **virtual 是 opt-in**
   - Java：每个方法都能多态
   - C++：默认不能，必须 `virtual`，且基类析构必须 virtual

3. **const 正确性**
   - Java：基本没这概念
   - C++：参数、返回值、方法都要标 const，IDE / 编译器靠这个保证安全

---

## Part 5: 编译运行（C++ 版本）

```bash
# 保存为 chess.cpp
g++ -std=c++17 -Wall -O2 chess.cpp -o chess
./chess
```

输出：
```
=== Knight at (4,4) ===
Move to (2,3): true
Move to (3,3): false
All valid moves: (2,3) (2,5) (3,2) (3,6) (5,2) (5,6) (6,3) (6,5)

=== King at (0,4) ===
Move to (1,4): true
Move to (2,4): false
```
