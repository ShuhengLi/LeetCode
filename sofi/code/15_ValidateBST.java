import java.util.ArrayDeque;
import java.util.Deque;

/**
 * SoFi DSA Round - 真题（树类高频，2024/2025 候选人报告）
 *
 * 题目：Validate Binary Search Tree
 * ─────────────────────────────────────────────────────────────────
 * 给一个二叉树根节点，判断它是否是有效的 BST（二叉搜索树）。
 *
 * BST 定义（严格）：
 *   - 节点的左子树只包含值 < node.val 的节点
 *   - 节点的右子树只包含值 > node.val 的节点
 *   - 左右子树都必须是 BST
 *
 * 注意：不允许有相等的值（严格 BST）。
 *
 * 示例 1（合法）：
 *       2
 *      / \
 *     1   3
 *   Output: true
 *
 * 示例 2（非法）：
 *       5
 *      / \
 *     1   4
 *        / \
 *       3   6
 *   Output: false (4 在 5 的右子树但 4 < 5)
 *
 * 关键陷阱：
 *   不能只检查 node.left.val < node.val < node.right.val，
 *   因为 BST 要求"整个左子树 < node < 整个右子树"，不只是直接子节点。
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路对比：
 *
 * 方法一：递归 + 上下界 ← 推荐面试默认
 *   每个节点必须在 (min, max) 区间内。
 *   递归到左子树时，max 更新为 node.val
 *   递归到右子树时，min 更新为 node.val
 *
 * 方法二：中序遍历必严格递增
 *   BST 的中序遍历产生有序序列。
 *   遍历时维护 prev，若当前 <= prev 即非法。
 *   可用迭代版本节省栈空间。
 *
 *   时间复杂度：O(n)
 *   空间复杂度：O(h) h=树高（递归栈）
 */
public class ValidateBST {

    static class TreeNode {
        int val;
        TreeNode left, right;
        TreeNode(int val) { this.val = val; }
    }

    // ─── 方法一：递归 + 上下界 ────────────────────────────────────
    public static boolean isValidBST(TreeNode root) {
        // 用 Long 边界处理 Integer.MIN_VALUE / MAX_VALUE 节点
        return validate(root, Long.MIN_VALUE, Long.MAX_VALUE);
    }

    private static boolean validate(TreeNode node, long min, long max) {
        if (node == null) return true;

        // 当前节点必须严格 > min 且 < max
        if (node.val <= min || node.val >= max) return false;

        return validate(node.left, min, node.val)
            && validate(node.right, node.val, max);
    }

    // ─── 方法二：中序遍历（迭代）─────────────────────────────────
    public static boolean isValidBSTInorder(TreeNode root) {
        Deque<TreeNode> stack = new ArrayDeque<>();
        TreeNode current = root;
        Long prev = null;

        while (current != null || !stack.isEmpty()) {
            // 一路向左
            while (current != null) {
                stack.push(current);
                current = current.left;
            }

            // 处理节点
            current = stack.pop();
            if (prev != null && current.val <= prev) return false;
            prev = (long) current.val;

            // 转向右子树
            current = current.right;
        }

        return true;
    }

    // ─── 方法二变体：中序遍历（递归）─────────────────────────────
    private static long prevValRecursive = Long.MIN_VALUE;
    private static boolean validInorder = true;

    public static boolean isValidBSTInorderRecursive(TreeNode root) {
        prevValRecursive = Long.MIN_VALUE;
        validInorder = true;
        inorder(root);
        return validInorder;
    }

    private static void inorder(TreeNode node) {
        if (node == null || !validInorder) return;
        inorder(node.left);
        if (node.val <= prevValRecursive) {
            validInorder = false;
            return;
        }
        prevValRecursive = node.val;
        inorder(node.right);
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        // 例 1：合法 BST [2,1,3]
        TreeNode root1 = new TreeNode(2);
        root1.left = new TreeNode(1);
        root1.right = new TreeNode(3);
        System.out.println(isValidBST(root1));        // true
        System.out.println(isValidBSTInorder(root1)); // true

        // 例 2：非法 [5,1,4,null,null,3,6]
        TreeNode root2 = new TreeNode(5);
        root2.left = new TreeNode(1);
        root2.right = new TreeNode(4);
        root2.right.left = new TreeNode(3);
        root2.right.right = new TreeNode(6);
        System.out.println(isValidBST(root2));        // false
        System.out.println(isValidBSTInorder(root2)); // false

        // 例 3：单节点
        System.out.println(isValidBST(new TreeNode(1))); // true

        // 例 4：相等值非法
        TreeNode root3 = new TreeNode(1);
        root3.left = new TreeNode(1);
        System.out.println(isValidBST(root3)); // false

        // 例 5：边界 Integer.MIN_VALUE
        TreeNode root4 = new TreeNode(Integer.MIN_VALUE);
        root4.right = new TreeNode(Integer.MAX_VALUE);
        System.out.println(isValidBST(root4)); // true
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 为什么不能只检查左 < 父 < 右？
 * A: 经典陷阱。BST 要求整个子树都满足顺序，不只是直接子节点。
 *    例如 [5,1,4,null,null,3,6]：4 是 5 的右子节点，4>3>1>5？
 *    单看 (5, 4) 看不出问题，但 4<5 已经违反 BST。
 *    所以必须传递"祖先约束"（上下界）。
 *
 * Q: 为什么用 Long 而不是 Integer 上下界？
 * A: 节点值可能等于 Integer.MIN_VALUE 或 MAX_VALUE。
 *    若初始用 Integer.MIN_VALUE，根节点恰好是它就误判失败。
 *    用 Long.MIN_VALUE / MAX_VALUE 避免此问题。
 *    或者改用 Integer min/max（boxed）传 null 表示无下界。
 *
 * Q: 三种方法的对比？
 * A: - 上下界递归：直观，无需额外状态
 *    - 中序递归：依赖外部状态（成员变量），不利于并行/线程安全
 *    - 中序迭代：栈空间可控，提前发现错误可提前返回
 *    Senior 推荐迭代版本（空间可控、易调试）。
 *
 * Q: 如何处理"允许相等"的变体？
 * A: 把 <= 改成 <：
 *    - 上下界版本：node.val < min || node.val > max
 *    - 中序版本：current.val < prev
 *
 * Q: SoFi 业务关联？
 * A: 验证账户层级结构、订单依赖、客户群体分组等场景中，
 *    BST 校验是数据完整性检查的常见操作。
 *
 * Q: 大树（百万节点）有何优化？
 * A: 迭代避免栈溢出；可以提前剪枝（一旦发现违反立即返回 false）。
 *    并行：左右子树验证可并行（CompletableFuture / ForkJoin），
 *    但需要正确传递上下界。
 *
 * 相关 LeetCode：
 *   - LC 98  Validate BST (本题)
 *   - LC 99  Recover BST (恢复被交换的节点)
 *   - LC 230 Kth Smallest in BST (中序遍历)
 *   - LC 700 Search in BST
 */
