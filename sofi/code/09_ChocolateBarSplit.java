/**
 * SoFi On-site - 真题（2024/2025 候选人报告）
 *
 * 题目：Chocolate Bar Splitting (Least Gold)
 * ─────────────────────────────────────────────────────────────────
 * 你和朋友分一根长度为 N 的巧克力棒，规则如下：
 *   - 每一轮你必须把当前的巧克力棒"对半切"
 *   - 如果长度为偶数，切成两个 N/2
 *   - 如果长度为奇数，切成 floor(N/2) 和 ceil(N/2)（即 N/2 和 N/2+1）
 *   - 切完之后，你保留较大的一半，把较小的一半给朋友
 *   - 你给出去的每一份"小半"价值等于其长度（朋友收到的金额）
 *   - 你重复这个过程，直到只剩长度 1
 *
 * 问：你最少要付给朋友多少"金币"（= 给出去的所有小份长度之和）？
 *
 * 注意变体：
 *   有的版本要求 "least gold given"，本质是计算所有"小半"的长度之和。
 *
 * 示例 1：
 *   N = 8
 *   8 → 4+4，给4，留4 [给4]
 *   4 → 2+2，给2，留2 [给2]
 *   2 → 1+1，给1，留1 [给1]
 *   总共 4+2+1 = 7
 *
 * 示例 2：
 *   N = 5
 *   5 → 2+3，给2，留3   [给2]
 *   3 → 1+2，给1，留2   [给1]
 *   2 → 1+1，给1，留1   [给1]
 *   总共 2+1+1 = 4
 *
 * 示例 3：
 *   N = 1 → 0（无法再切）
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路一：直接模拟（迭代）
 *   每次循环：
 *     - 偶数：smaller = N/2，N = N/2
 *     - 奇数：smaller = N/2（向下取整），N = N/2 + 1（向上取整）
 *     - gold += smaller
 *   循环直到 N == 1
 *
 *   时间复杂度：O(log N)，每次 N 减半
 *   空间复杂度：O(1)
 *
 * 思路二：递归（更直观）
 *   f(N) = 0 if N <= 1
 *   f(N) = (N/2) + f(N - N/2) if even
 *   f(N) = (N/2) + f(N/2 + 1)  if odd
 *
 * 思路三：数学闭式（深入分析）
 *   对于偶数 N = 2^k，结果是 2^k - 1（等比数列求和）
 *   对于奇数，需要追踪每次 N 的变化（差不多 N - 1 的下界）
 *   实际最简单还是模拟。
 */
public class ChocolateBarSplit {

    // ─── 思路一：迭代模拟 ────────────────────────────────────────
    public static long minGold(long n) {
        if (n <= 1) return 0;

        long gold = 0;
        while (n > 1) {
            long smaller = n / 2;          // 较小一半（向下取整）
            long larger  = n - smaller;    // 较大一半（向上取整）
            gold += smaller;
            n = larger;                    // 你保留较大的，继续切
        }
        return gold;
    }

    // ─── 思路二：递归 ────────────────────────────────────────────
    public static long minGoldRecursive(long n) {
        if (n <= 1) return 0;
        long smaller = n / 2;
        long larger  = n - smaller;
        return smaller + minGoldRecursive(larger);
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        // 偶数情况
        System.out.println(minGold(8));    // 期望: 7  (4+2+1)
        System.out.println(minGold(16));   // 期望: 15 (8+4+2+1)
        System.out.println(minGold(2));    // 期望: 1

        // 奇数情况
        System.out.println(minGold(5));    // 期望: 4  (2+1+1)
        System.out.println(minGold(7));    // 期望: 6  (3+2+1)
        System.out.println(minGold(1));    // 期望: 0
        System.out.println(minGold(0));    // 期望: 0

        // 大数验证
        System.out.println(minGold(1000));
        System.out.println(minGoldRecursive(1000));  // 两种解法应一致

        // 验证一致性
        for (long i = 0; i < 100; i++) {
            assert minGold(i) == minGoldRecursive(i) : "Mismatch at " + i;
        }
        System.out.println("All consistency checks passed.");
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 为什么这是"最少"金币？
 * A: 因为题目规则限定你必须保留较大的一半，所以"最少"实际上是"唯一可能"的总额。
 *    如果题目允许你选择给出哪一半，那才有"最优"决策（greedy: 永远给小的）。
 *    SoFi 的版本是后者，并要求证明这就是最优策略。
 *
 * Q: 为什么贪心给小的最优？
 * A: 反证：若某一步给出大的，则后续要切的总长度 = 小的一半 < 大的一半，
 *    后续累计给出的金币会更多还是更少？答案是更少。但本步给出去的 gold
 *    更大（差值 = 大-小）。需要数学归纳证明：本步多付的差值 ≥ 后续节省的差值。
 *    这是 Senior 考察点：能否证明贪心的正确性。
 *
 * Q: N 很大（10^18）会怎样？
 * A: O(log N) ≈ 60 次循环即可，long 型不会溢出。无需特殊处理。
 *
 * Q: 如果改成"切成 3 份"呢？
 * A: 每次切 N → N/3 + N/3 + N/3（或类似），保留最大的，给出两份其他。
 *    思路相同，只是常数变了：迭代 ~ log_3(N) 次。
 *
 * Q: 这道题考察什么？
 * A: 1) 理解题意能力（奇偶处理）
 *    2) 边界处理（N=0, N=1）
 *    3) 选择合适解法（迭代 vs 递归）
 *    4) 数学/贪心证明（Senior 必问）
 *
 * 这类"过程模拟 + 贪心 + 复杂度分析"题在 SoFi、Robinhood 等金融科技公司
 * 较为常见，反映了"现实业务问题抽象成算法"的能力。
 */
