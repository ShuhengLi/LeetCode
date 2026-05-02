# Java 常用算法模板（可背可抄）

> 全部带 Java 标准实现，面试时能直接默写。

---

## 1. 二分查找（Binary Search）

```java
// 模板 1：找 target 精确位置（找不到返回 -1）
int binarySearch(int[] nums, int target) {
    int l = 0, r = nums.length - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;     // 防溢出
        if (nums[mid] == target) return mid;
        else if (nums[mid] < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

// 模板 2：找第一个 >= target（lower_bound）
int lowerBound(int[] nums, int target) {
    int l = 0, r = nums.length;        // 注意 r = length
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] >= target) r = mid;
        else l = mid + 1;
    }
    return l;                           // 可能 == nums.length
}

// 模板 3：二分答案（minimize 满足条件的 x）
int binarySearchAnswer(int lo, int hi) {
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (check(mid)) hi = mid;       // mid 可行，往左找更小
        else lo = mid + 1;
    }
    return lo;
}
```

---

## 2. 双指针（Two Pointers）

```java
// 同向（滑动窗口）
int slidingWindow(int[] nums, int k) {
    int l = 0, sum = 0, ans = 0;
    for (int r = 0; r < nums.length; r++) {
        sum += nums[r];
        while (sum > k) sum -= nums[l++];   // 收缩条件
        ans = Math.max(ans, r - l + 1);
    }
    return ans;
}

// 对向（two sum 排序后）
int[] twoSum(int[] nums, int target) {
    int l = 0, r = nums.length - 1;
    while (l < r) {
        int s = nums[l] + nums[r];
        if (s == target) return new int[]{l, r};
        else if (s < target) l++;
        else r--;
    }
    return new int[0];
}

// 快慢指针（链表环检测 / 中点）
ListNode detectCycle(ListNode head) {
    ListNode slow = head, fast = head;
    while (fast != null && fast.next != null) {
        slow = slow.next;
        fast = fast.next.next;
        if (slow == fast) return slow;     // 有环
    }
    return null;
}
```

---

## 3. BFS 模板

```java
// 二维网格 BFS
int bfs(int[][] grid, int sr, int sc) {
    int m = grid.length, n = grid[0].length;
    int[][] dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    Queue<int[]> q = new ArrayDeque<>();
    boolean[][] visited = new boolean[m][n];

    q.offer(new int[]{sr, sc});
    visited[sr][sc] = true;
    int steps = 0;

    while (!q.isEmpty()) {
        int size = q.size();          // 当前层节点数
        for (int i = 0; i < size; i++) {
            int[] cur = q.poll();
            int r = cur[0], c = cur[1];
            // 处理 cur

            for (int[] d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
                if (visited[nr][nc] || grid[nr][nc] == 0) continue;
                visited[nr][nc] = true;
                q.offer(new int[]{nr, nc});
            }
        }
        steps++;
    }
    return steps;
}

// 图的 BFS（最短路径，无权图）
int bfsGraph(int n, List<List<Integer>> graph, int src, int dst) {
    Queue<Integer> q = new ArrayDeque<>();
    int[] dist = new int[n];
    Arrays.fill(dist, -1);
    q.offer(src);
    dist[src] = 0;
    while (!q.isEmpty()) {
        int u = q.poll();
        if (u == dst) return dist[u];
        for (int v : graph.get(u)) {
            if (dist[v] != -1) continue;
            dist[v] = dist[u] + 1;
            q.offer(v);
        }
    }
    return -1;
}
```

---

## 4. DFS 模板

```java
// 递归 DFS（图）
void dfs(int u, List<List<Integer>> graph, boolean[] visited) {
    visited[u] = true;
    for (int v : graph.get(u)) {
        if (!visited[v]) dfs(v, graph, visited);
    }
}

// 递归 DFS（二维网格）
void dfsGrid(int[][] grid, int r, int c, boolean[][] visited) {
    int m = grid.length, n = grid[0].length;
    if (r < 0 || r >= m || c < 0 || c >= n) return;
    if (visited[r][c] || grid[r][c] == 0) return;
    visited[r][c] = true;
    int[][] dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int[] d : dirs) dfsGrid(grid, r + d[0], c + d[1], visited);
}

// 迭代 DFS（用栈）
void dfsIter(int start, List<List<Integer>> graph) {
    Deque<Integer> stack = new ArrayDeque<>();
    boolean[] visited = new boolean[graph.size()];
    stack.push(start);
    while (!stack.isEmpty()) {
        int u = stack.pop();
        if (visited[u]) continue;
        visited[u] = true;
        for (int v : graph.get(u)) {
            if (!visited[v]) stack.push(v);
        }
    }
}
```

---

## 5. Backtracking（回溯）模板

```java
// 通用框架
void backtrack(state, path, result) {
    if (满足终止条件) {
        result.add(new ArrayList<>(path));   // 注意拷贝！
        return;
    }
    for (选择 : 候选集) {
        // 做选择
        path.add(选择);
        modifyState();
        // 递归
        backtrack(state, path, result);
        // 撤销选择
        path.remove(path.size() - 1);
        revertState();
    }
}

// 例：全排列
List<List<Integer>> permute(int[] nums) {
    List<List<Integer>> result = new ArrayList<>();
    boolean[] used = new boolean[nums.length];
    backtrack(nums, new ArrayList<>(), used, result);
    return result;
}

void backtrack(int[] nums, List<Integer> path, boolean[] used, List<List<Integer>> result) {
    if (path.size() == nums.length) {
        result.add(new ArrayList<>(path));
        return;
    }
    for (int i = 0; i < nums.length; i++) {
        if (used[i]) continue;
        used[i] = true;
        path.add(nums[i]);
        backtrack(nums, path, used, result);
        path.remove(path.size() - 1);
        used[i] = false;
    }
}
```

---

## 6. Dynamic Programming

### 1D DP
```java
// 爬楼梯：f[i] = f[i-1] + f[i-2]
int climbStairs(int n) {
    if (n <= 2) return n;
    int a = 1, b = 2;
    for (int i = 3; i <= n; i++) {
        int c = a + b;
        a = b; b = c;
    }
    return b;
}
```

### 2D DP
```java
// LCS
int lcs(String a, String b) {
    int m = a.length(), n = b.length();
    int[][] dp = new int[m + 1][n + 1];
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (a.charAt(i - 1) == b.charAt(j - 1)) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}
```

### 记忆化搜索（top-down）
```java
Map<Integer, Integer> memo = new HashMap<>();
int dp(int n) {
    if (n <= 1) return n;
    if (memo.containsKey(n)) return memo.get(n);
    int result = dp(n - 1) + dp(n - 2);
    memo.put(n, result);
    return result;
}
```

---

## 7. Union-Find（并查集）

```java
class UnionFind {
    int[] parent, rank;
    int count;                       // 连通分量数

    UnionFind(int n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) parent[i] = i;
        count = n;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);   // 路径压缩
        return parent[x];
    }

    boolean union(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank[rx] < rank[ry]) { parent[rx] = ry; }
        else if (rank[rx] > rank[ry]) { parent[ry] = rx; }
        else { parent[ry] = rx; rank[rx]++; }
        count--;
        return true;
    }
}
```

---

## 8. Trie（字典树）

```java
class Trie {
    static class Node {
        Node[] children = new Node[26];
        boolean isEnd = false;
    }
    Node root = new Node();

    void insert(String word) {
        Node cur = root;
        for (char c : word.toCharArray()) {
            int i = c - 'a';
            if (cur.children[i] == null) cur.children[i] = new Node();
            cur = cur.children[i];
        }
        cur.isEnd = true;
    }

    boolean search(String word) {
        Node n = find(word);
        return n != null && n.isEnd;
    }

    boolean startsWith(String prefix) {
        return find(prefix) != null;
    }

    private Node find(String s) {
        Node cur = root;
        for (char c : s.toCharArray()) {
            int i = c - 'a';
            if (cur.children[i] == null) return null;
            cur = cur.children[i];
        }
        return cur;
    }
}
```

---

## 9. Topological Sort（拓扑排序）

```java
// Kahn's BFS
int[] topoSort(int n, int[][] edges) {
    List<List<Integer>> g = new ArrayList<>();
    for (int i = 0; i < n; i++) g.add(new ArrayList<>());
    int[] indeg = new int[n];
    for (int[] e : edges) {
        g.get(e[0]).add(e[1]);
        indeg[e[1]]++;
    }

    Queue<Integer> q = new ArrayDeque<>();
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.offer(i);

    int[] order = new int[n];
    int idx = 0;
    while (!q.isEmpty()) {
        int u = q.poll();
        order[idx++] = u;
        for (int v : g.get(u)) {
            if (--indeg[v] == 0) q.offer(v);
        }
    }
    return idx == n ? order : new int[0];   // 有环返回空
}
```

---

## 10. Dijkstra（最短路径，带权）

```java
int[] dijkstra(int n, List<int[]>[] graph, int src) {
    int[] dist = new int[n];
    Arrays.fill(dist, Integer.MAX_VALUE);
    dist[src] = 0;

    PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[1] - b[1]);
    pq.offer(new int[]{src, 0});

    while (!pq.isEmpty()) {
        int[] cur = pq.poll();
        int u = cur[0], d = cur[1];
        if (d > dist[u]) continue;     // lazy deletion

        for (int[] edge : graph[u]) {
            int v = edge[0], w = edge[1];
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.offer(new int[]{v, dist[v]});
            }
        }
    }
    return dist;
}
```

---

## 11. 单调栈

```java
// Next Greater Element
int[] nextGreater(int[] nums) {
    int n = nums.length;
    int[] result = new int[n];
    Arrays.fill(result, -1);
    Deque<Integer> stack = new ArrayDeque<>();   // 存 index
    for (int i = 0; i < n; i++) {
        while (!stack.isEmpty() && nums[stack.peek()] < nums[i]) {
            result[stack.pop()] = nums[i];
        }
        stack.push(i);
    }
    return result;
}
```

---

## 12. Top K（PriorityQueue 经典）

```java
// Top K 大：用最小堆，size = K
int[] topKLargest(int[] nums, int k) {
    PriorityQueue<Integer> minHeap = new PriorityQueue<>();
    for (int x : nums) {
        minHeap.offer(x);
        if (minHeap.size() > k) minHeap.poll();
    }
    int[] result = new int[k];
    for (int i = k - 1; i >= 0; i--) result[i] = minHeap.poll();
    return result;
}

// Top K 频次（按 value 排序）
List<Integer> topKFrequent(int[] nums, int k) {
    Map<Integer, Integer> cnt = new HashMap<>();
    for (int x : nums) cnt.merge(x, 1, Integer::sum);

    PriorityQueue<int[]> minHeap = new PriorityQueue<>((a, b) -> a[1] - b[1]);
    for (var e : cnt.entrySet()) {
        minHeap.offer(new int[]{e.getKey(), e.getValue()});
        if (minHeap.size() > k) minHeap.poll();
    }
    List<Integer> result = new ArrayList<>();
    while (!minHeap.isEmpty()) result.add(0, minHeap.poll()[0]);
    return result;
}
```

---

## 13. 链表（LinkedList）模板

```java
class ListNode {
    int val;
    ListNode next;
    ListNode(int val) { this.val = val; }
}

// 反转链表
ListNode reverse(ListNode head) {
    ListNode prev = null, cur = head;
    while (cur != null) {
        ListNode next = cur.next;
        cur.next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

// dummy head 简化删除/插入
ListNode removeElements(ListNode head, int val) {
    ListNode dummy = new ListNode(0);
    dummy.next = head;
    ListNode cur = dummy;
    while (cur.next != null) {
        if (cur.next.val == val) cur.next = cur.next.next;
        else cur = cur.next;
    }
    return dummy.next;
}

// 合并两个有序链表
ListNode merge(ListNode a, ListNode b) {
    ListNode dummy = new ListNode(0), cur = dummy;
    while (a != null && b != null) {
        if (a.val <= b.val) { cur.next = a; a = a.next; }
        else { cur.next = b; b = b.next; }
        cur = cur.next;
    }
    cur.next = a != null ? a : b;
    return dummy.next;
}
```

---

## 14. 二叉树模板

```java
class TreeNode {
    int val;
    TreeNode left, right;
    TreeNode(int val) { this.val = val; }
}

// 三种遍历（递归）
void inorder(TreeNode root, List<Integer> result) {
    if (root == null) return;
    inorder(root.left, result);
    result.add(root.val);
    inorder(root.right, result);
}

// 中序迭代
List<Integer> inorderIter(TreeNode root) {
    List<Integer> result = new ArrayList<>();
    Deque<TreeNode> stack = new ArrayDeque<>();
    TreeNode cur = root;
    while (cur != null || !stack.isEmpty()) {
        while (cur != null) {
            stack.push(cur);
            cur = cur.left;
        }
        cur = stack.pop();
        result.add(cur.val);
        cur = cur.right;
    }
    return result;
}

// 层序（BFS）
List<List<Integer>> levelOrder(TreeNode root) {
    List<List<Integer>> result = new ArrayList<>();
    if (root == null) return result;
    Queue<TreeNode> q = new ArrayDeque<>();
    q.offer(root);
    while (!q.isEmpty()) {
        int size = q.size();
        List<Integer> level = new ArrayList<>();
        for (int i = 0; i < size; i++) {
            TreeNode n = q.poll();
            level.add(n.val);
            if (n.left != null) q.offer(n.left);
            if (n.right != null) q.offer(n.right);
        }
        result.add(level);
    }
    return result;
}
```

---

## 15. 频次统计（最高频用法）

```java
// 字符频次（小写字母）
int[] freq = new int[26];
for (char c : s.toCharArray()) freq[c - 'a']++;

// 通用对象频次
Map<T, Integer> cnt = new HashMap<>();
for (T x : nums) cnt.merge(x, 1, Integer::sum);

// 检查 anagram
boolean isAnagram(String s, String t) {
    if (s.length() != t.length()) return false;
    int[] f = new int[26];
    for (char c : s.toCharArray()) f[c - 'a']++;
    for (char c : t.toCharArray()) {
        if (--f[c - 'a'] < 0) return false;
    }
    return true;
}
```

---

## 16. LeetCode 提交模板（直接复制即可）

```java
import java.util.*;

class Solution {
    public int yourMethod(int[] nums, int target) {
        // your code
        return 0;
    }
}
```

---

## 17. 本地调试模板

```java
import java.util.*;

public class Main {
    public static void main(String[] args) {
        Solution sol = new Solution();
        int[] nums = {1, 2, 3, 4, 5};
        int result = sol.yourMethod(nums, 9);
        System.out.println(result);
        System.out.println(Arrays.toString(nums));   // 数组打印
    }

    static class Solution {
        public int yourMethod(int[] nums, int target) {
            return 0;
        }
    }
}
```
