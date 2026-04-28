import java.util.ArrayDeque;
import java.util.Deque;

/**
 * SoFi On-site - 真题（Glassdoor 2024/2025 确认）
 *
 * 题目：1-D Track Object Collision（一维轨道物体碰撞）
 * ─────────────────────────────────────────────────────────────────
 * 一维轨道上有若干物体，用 int[] 表示：
 *   - 数值的绝对值代表物体的"质量"（也代表"大小"）
 *   - 数值的正负代表运动方向：正 = 向右，负 = 向左
 *   - 所有物体以相同速度运动
 *
 * 碰撞规则：
 *   - 当一个右行物体（正）遇到左行物体（负）时，会发生碰撞
 *   - 质量大的物体保留，质量小的物体爆炸（消失）
 *   - 如果两者质量相等，两个都爆炸
 *   - 同方向物体永远不会碰撞
 *
 * 返回所有碰撞结束后剩余的物体（保持原顺序）。
 *
 * 示例 1：
 *   Input:  [5, 10, -5]
 *   Output: [5, 10]
 *   解释：10 和 -5 碰撞，10 保留；5 在 10 左边不会追上
 *
 * 示例 2：
 *   Input:  [8, -8]
 *   Output: []
 *   解释：质量相等，两个都爆炸
 *
 * 示例 3：
 *   Input:  [10, 2, -5]
 *   Output: [10]
 *   解释：2 和 -5 碰撞，2 爆炸；-5 继续向左，与 10 碰撞，-5 爆炸
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路：栈（Stack）
 *
 *   核心观察：
 *     - 栈中保存"当前还活着的物体"
 *     - 遍历输入数组，每个新物体：
 *       - 如果是正数（向右）：直接入栈（暂时不会与栈中物体碰撞）
 *       - 如果是负数（向左）：可能与栈顶的正数物体碰撞
 *
 *   碰撞处理（while 循环）：
 *     while (栈非空 && 栈顶为正 && |当前| > |栈顶|): 栈顶爆炸 pop
 *     - 如果栈顶为正且 |栈顶| == |当前|: 两者都爆炸，pop 栈顶，不入栈
 *     - 如果栈顶为正且 |栈顶| > |当前|: 当前爆炸，不入栈
 *     - 否则（栈空或栈顶为负）: 当前入栈
 *
 *   时间复杂度：O(n)，每个物体最多入栈/出栈一次
 *   空间复杂度：O(n)
 */
public class AsteroidCollision {

    public static int[] asteroidCollision(int[] asteroids) {
        Deque<Integer> stack = new ArrayDeque<>();

        for (int a : asteroids) {
            boolean alive = true;

            // 只有 a 是负数 + 栈顶是正数才会碰撞
            while (alive && a < 0 && !stack.isEmpty() && stack.peek() > 0) {
                int top = stack.peek();

                if (top < -a) {
                    // 栈顶质量小 → 栈顶爆炸，a 继续向左可能撞下一个
                    stack.pop();
                } else if (top == -a) {
                    // 质量相等 → 两者都爆炸
                    stack.pop();
                    alive = false;
                } else {
                    // 栈顶质量大 → a 爆炸
                    alive = false;
                }
            }

            if (alive) {
                stack.push(a);
            }
        }

        // 栈底是最左边，需要反转
        int[] result = new int[stack.size()];
        for (int i = result.length - 1; i >= 0; i--) {
            result[i] = stack.pop();
        }
        return result;
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        print(asteroidCollision(new int[]{5, 10, -5}));      // [5, 10]
        print(asteroidCollision(new int[]{8, -8}));          // []
        print(asteroidCollision(new int[]{10, 2, -5}));      // [10]
        print(asteroidCollision(new int[]{-2, -1, 1, 2}));   // [-2, -1, 1, 2]（同方向不碰）
        print(asteroidCollision(new int[]{-2, -2, 1, -2}));  // [-2, -2, -2]
        print(asteroidCollision(new int[]{1, -2, -2, -2}));  // [-2, -2, -2]
    }

    private static void print(int[] arr) {
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < arr.length; i++) {
            if (i > 0) sb.append(", ");
            sb.append(arr[i]);
        }
        sb.append("]");
        System.out.println(sb);
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 为什么用栈？
 * A: 栈天然保存"待处理且按时间顺序"的元素。新进来的左行物体只可能与
 *    已经入栈的右行物体碰撞，而且碰撞顺序是栈顶（最近的右行）先撞，
 *    符合 LIFO 语义。
 *
 * Q: 时间复杂度怎么证明是 O(n)？
 * A: 每个物体最多入栈一次、出栈一次，所以总操作数 ≤ 2n = O(n)。
 *    虽然有嵌套 while，但 amortized（均摊）分析仍是线性。
 *
 * Q: 边界情况？
 * A: - 空数组 → 返回空
 *    - 全部正数或全部负数 → 不碰撞，原样返回
 *    - 多个连续相同方向 → 不影响（同方向不碰撞）
 *    - 一个物体连续撞掉多个：如 [1,2,3,-10] → [-10]，循环 pop 处理
 *
 * Q: 如果速度不一致怎么办？
 * A: 题目就复杂了，需要按时间顺序模拟碰撞。
 *    可以用事件驱动：计算每对相邻物体的碰撞时间，用优先队列按时间处理。
 *    这是物理模拟问题，复杂度大幅上升。
 *
 * Q: 如果是 2D 平面上的物体？
 * A: 这就是 N-body simulation，每个物体都可能与所有其他碰撞，
 *    O(n^2) 朴素，可用 spatial hashing / quad-tree 优化到接近 O(n log n)。
 *
 * 对应 LeetCode：LC 735 Asteroid Collision (Medium，面试高频)
 */
