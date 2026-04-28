import java.util.HashMap;
import java.util.Map;

/**
 * SoFi Tech Screen - 真题 (2024/2025 高频)
 *
 * 题目：Smallest "Clean" Subarray
 * ─────────────────────────────────────────────────────────────────
 * 给一个整数数组 nums 和整数 k。
 * 定义：一个子数组是 "clean"（干净的），如果它包含至少 k 个不同的数字。
 * 返回最小的 "clean" 子数组的长度。
 * 如果不存在这样的子数组，返回 -1。
 *
 * 示例 1：
 *   Input:  nums = [1, 2, 1, 3, 2], k = 3
 *   Output: 3
 *   解释：子数组 [1, 2, 1, 3] → 去重后 {1,2,3} 有 3 个不同值，长度 4
 *          子数组 [2, 1, 3]    → 去重后 {2,1,3} 有 3 个不同值，长度 3 ← 最小
 *
 * 示例 2：
 *   Input:  nums = [1, 1, 1], k = 2
 *   Output: -1
 *   解释：整个数组只有 1 个不同值，无法满足 k = 2
 *
 * 约束：
 *   - 1 <= nums.length <= 10^5
 *   - 1 <= k <= 10^5
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路分析：Sliding Window（滑动窗口）
 *
 *   暴力：枚举所有子数组 O(n^2) → 不满足要求
 *
 *   滑动窗口策略（适用于"最小满足条件的子数组"）：
 *     双指针 left, right。right 不断右移扩大窗口，将 nums[right] 加入窗口；
 *     一旦窗口内不同值数量 >= k，尝试缩小左边（left 右移），
 *       - 每次缩小前更新答案（当前窗口长度）
 *       - 缩小直到不满足条件为止，然后继续扩大 right
 *
 *   关键数据结构：HashMap<Integer, Integer> count 记录窗口内每个数字的出现次数
 *     - 加入：count[v]++；如果 count[v] 变为 1，说明新增了一个不同值，distinct++
 *     - 移除：count[v]--；如果 count[v] 变为 0，说明一个不同值消失了，distinct--
 *
 *   时间复杂度：O(n)，每个元素最多入窗/出窗各一次
 *   空间复杂度：O(min(n, unique values))
 */
public class SmallestCleanSubarray {

    public static int smallestCleanSubarray(int[] nums, int k) {
        int n = nums.length;
        Map<Integer, Integer> count = new HashMap<>();
        int distinct = 0;   // 当前窗口内不同值的数量
        int minLen = Integer.MAX_VALUE;
        int left = 0;

        for (int right = 0; right < n; right++) {
            // 将 nums[right] 加入窗口
            int val = nums[right];
            count.put(val, count.getOrDefault(val, 0) + 1);
            if (count.get(val) == 1) {
                distinct++;  // 新的不同值进入窗口
            }

            // 窗口满足条件时，尽量缩小左边
            while (distinct >= k) {
                // 当前窗口 [left, right] 是一个合法 clean 子数组
                minLen = Math.min(minLen, right - left + 1);

                // 尝试移除左边元素
                int leftVal = nums[left];
                count.put(leftVal, count.get(leftVal) - 1);
                if (count.get(leftVal) == 0) {
                    distinct--;  // 不同值消失
                    count.remove(leftVal);  // 清理 map，节省空间
                }
                left++;
            }
        }

        return minLen == Integer.MAX_VALUE ? -1 : minLen;
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        System.out.println(smallestCleanSubarray(new int[]{1, 2, 1, 3, 2}, 3)); // 期望: 3
        System.out.println(smallestCleanSubarray(new int[]{1, 1, 1}, 2));        // 期望: -1
        System.out.println(smallestCleanSubarray(new int[]{1, 2, 3, 4, 5}, 3)); // 期望: 3
        System.out.println(smallestCleanSubarray(new int[]{5, 5, 5, 1, 2, 5}, 3)); // 期望: 4 ([5,1,2,5] → no, [1,2,5]=3)
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 为什么滑动窗口在这里适用？
 * A: 满足"单调性"：right 向右移动时窗口不同值只增不减（加入）；
 *    left 向右移动时只减不增（移除）。这种单调性使得双指针不会"回头"。
 *    "最小满足条件子数组"是滑动窗口经典场景。
 *
 * Q: 与 "最多 k 个不同值的最长子数组" 有什么区别？
 * A: 本题是"至少 k 个不同值的最短"。收缩条件是满足时尽量缩左（找最小），
 *    而那道题是不满足时停止收缩（找最大）。方向相反但框架相同。
 *
 * Q: 如果 k=1，结果是什么？
 * A: 任意单个元素都是 clean，答案是 1。
 *
 * Q: 有没有 O(1) 空间的解法？
 * A: 没有通用的，因为需要追踪窗口内元素种类，至少需要 O(distinct) 空间。
 *
 * 相关 LeetCode：
 *   - LC 3 (Longest Substring Without Repeating Characters) - 滑窗基础
 *   - LC 76 (Minimum Window Substring) - 同类"最小满足"
 *   - LC 340 (Longest Substring with At Most K Distinct) - 反向问题
 */
