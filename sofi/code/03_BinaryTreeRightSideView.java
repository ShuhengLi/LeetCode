import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
 * SoFi Tech Screen - 真题 (2024/2025)
 *
 * 题目：Binary Tree Right Side View
 * ─────────────────────────────────────────────────────────────────
 * 给一个二叉树的根节点 root，想象站在树的右侧，从上到下返回所有可见的节点值。
 * 即：返回每一层最右边节点的值。
 * 要求时间复杂度 O(n)。
 *
 * 示例：
 *         1          ← 看到 1
 *        / \
 *       2   3        ← 看到 3
 *        \   \
 *         5   4      ← 看到 4
 *
 *   Output: [1, 3, 4]
 *
 * SoFi 面试变体：
 *   候选人需要先从文件中解析数据构建二叉树，再返回右视图。
 *   解析格式通常类似 LeetCode 的 level-order 序列化格式：
 *     "1,2,3,null,5,null,4"
 *
 * 约束：
 *   - 节点数 [0, 100]
 *   - -100 <= val <= 100
 * ─────────────────────────────────────────────────────────────────
 *
 * 方法一：BFS（层序遍历）
 *   每层遍历完时，记录该层最后一个节点的值。
 *   时间: O(n)，空间: O(w) w 为最大宽度（最坏 O(n)）
 *
 * 方法二：DFS（先右后左）
 *   先访问右子树，再访问左子树；用 depth 控制只记录每层第一次访问的值。
 *   时间: O(n)，空间: O(h) h 为树高（最坏 O(n)）
 */
public class BinaryTreeRightSideView {

    static class TreeNode {
        int val;
        TreeNode left, right;
        TreeNode(int val) { this.val = val; }
    }

    // ─── 方法一：BFS ─────────────────────────────────────────────
    public static List<Integer> rightSideViewBFS(TreeNode root) {
        List<Integer> result = new ArrayList<>();
        if (root == null) return result;

        Queue<TreeNode> queue = new LinkedList<>();
        queue.offer(root);

        while (!queue.isEmpty()) {
            int levelSize = queue.size();  // 本层节点数

            for (int i = 0; i < levelSize; i++) {
                TreeNode node = queue.poll();

                // 只记录每层最后一个节点
                if (i == levelSize - 1) {
                    result.add(node.val);
                }

                if (node.left != null)  queue.offer(node.left);
                if (node.right != null) queue.offer(node.right);
            }
        }

        return result;
    }

    // ─── 方法二：DFS（先右后左）─────────────────────────────────
    public static List<Integer> rightSideViewDFS(TreeNode root) {
        List<Integer> result = new ArrayList<>();
        dfs(root, 0, result);
        return result;
    }

    private static void dfs(TreeNode node, int depth, List<Integer> result) {
        if (node == null) return;

        // 第一次访问到该深度（从右侧进入），记录该值
        if (depth == result.size()) {
            result.add(node.val);
        }

        // 先右后左，确保右侧节点优先被记录
        dfs(node.right, depth + 1, result);
        dfs(node.left,  depth + 1, result);
    }

    // ─── 从字符串解析二叉树（SoFi 面试实际考察场景）──────────────
    // 格式："1,2,3,null,5,null,4"（Level-order，null 表示缺失节点）
    public static TreeNode buildTreeFromString(String data) {
        if (data == null || data.isEmpty() || data.equals("null")) return null;

        String[] vals = data.split(",");
        TreeNode root = new TreeNode(Integer.parseInt(vals[0].trim()));
        Queue<TreeNode> queue = new LinkedList<>();
        queue.offer(root);

        int i = 1;
        while (!queue.isEmpty() && i < vals.length) {
            TreeNode node = queue.poll();

            // 处理左子节点
            if (i < vals.length && !vals[i].trim().equals("null")) {
                node.left = new TreeNode(Integer.parseInt(vals[i].trim()));
                queue.offer(node.left);
            }
            i++;

            // 处理右子节点
            if (i < vals.length && !vals[i].trim().equals("null")) {
                node.right = new TreeNode(Integer.parseInt(vals[i].trim()));
                queue.offer(node.right);
            }
            i++;
        }

        return root;
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        // 手动构建树：1 -> 2(left), 3(right) -> 5(2's right), 4(3's right)
        TreeNode root = buildTreeFromString("1,2,3,null,5,null,4");

        System.out.println("BFS: " + rightSideViewBFS(root)); // 期望: [1, 3, 4]
        System.out.println("DFS: " + rightSideViewDFS(root)); // 期望: [1, 3, 4]

        // 只有左子树
        TreeNode root2 = buildTreeFromString("1,2,null,3");
        System.out.println("BFS: " + rightSideViewBFS(root2)); // 期望: [1, 2, 3]
        System.out.println("DFS: " + rightSideViewDFS(root2)); // 期望: [1, 2, 3]

        // 空树
        System.out.println("BFS: " + rightSideViewBFS(null)); // 期望: []
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: BFS vs DFS 哪种更好？
 * A: 取决于场景：
 *    - BFS 更直观，天然按层处理，适合按层相关的问题
 *    - DFS 空间更优（栈深 O(h) vs 队列宽 O(w)）对于平衡树 O(log n)
 *    - Senior 考察：能说出两种并分析 trade-off
 *
 * Q: 如果是"左视图"呢？
 * A: BFS：记录每层第一个节点（i == 0）
 *    DFS：先左后右，先访问左子树
 *
 * Q: 如何"从文件读取"构建树？
 * A: 用 BufferedReader 读取文件，解析每行或逗号分隔的序列，
 *    复用上面的 buildTreeFromString 逻辑。
 *    面试中 SoFi 实际要求候选人现场实现 buildFromFile，考察文件 IO + 树构建。
 *
 * Q: 如何处理大型树（内存受限）？
 * A: 可以用迭代 DFS（显式栈），避免深递归导致栈溢出。
 *    对超大树，可以分片处理或用外部存储（B树思路）。
 *
 * 对应 LeetCode：
 *   - LC 199 Binary Tree Right Side View (Medium)
 *   - LC 102 Binary Tree Level Order Traversal (Medium)
 */
