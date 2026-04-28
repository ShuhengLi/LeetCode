import java.util.*;

/**
 * SoFi DSA Round - 真题（高频面试题，多家金融科技公司常考）
 *
 * 题目：Merge Intervals
 * ─────────────────────────────────────────────────────────────────
 * 给一个区间数组 intervals，每个 interval = [start, end]。
 * 合并所有重叠（包括相邻接触）的区间，返回不重叠的区间数组。
 *
 * 示例 1：
 *   Input:  [[1,3],[2,6],[8,10],[15,18]]
 *   Output: [[1,6],[8,10],[15,18]]
 *   解释：[1,3] 和 [2,6] 重叠 → [1,6]
 *
 * 示例 2：
 *   Input:  [[1,4],[4,5]]
 *   Output: [[1,5]]
 *   解释：边界相接也算重叠
 *
 * 示例 3 (SoFi 业务变体)：
 *   交易时间窗口 [start, end] 表示用户活跃时段，合并后求总活跃时长。
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路：排序 + 一次遍历
 *   1. 按 start 升序排序
 *   2. 遍历：维护一个"当前合并区间" current
 *      - 若下一个 interval[i].start <= current.end → 合并 (扩展 current.end)
 *      - 否则将 current 加入结果，开始新的 current
 *
 *   时间复杂度：O(n log n)，瓶颈在排序
 *   空间复杂度：O(log n) 排序栈空间，结果数组不计
 */
public class MergeIntervals {

    public static int[][] merge(int[][] intervals) {
        if (intervals == null || intervals.length == 0) return new int[0][];

        // 按 start 升序排序
        Arrays.sort(intervals, (a, b) -> Integer.compare(a[0], b[0]));

        List<int[]> merged = new ArrayList<>();
        int[] current = intervals[0];

        for (int i = 1; i < intervals.length; i++) {
            int[] next = intervals[i];

            if (next[0] <= current[1]) {
                // 重叠：扩展 current 的 end（取较大的）
                current[1] = Math.max(current[1], next[1]);
            } else {
                // 不重叠：保存 current，开始新区间
                merged.add(current);
                current = next;
            }
        }
        merged.add(current);  // 别忘了最后一个

        return merged.toArray(new int[merged.size()][]);
    }

    // ─── SoFi 业务变体：求合并后总长度 ──────────────────────────
    /**
     * 给一组活跃时段，求用户实际总活跃时长（重叠部分只算一次）
     */
    public static long totalActiveTime(int[][] intervals) {
        int[][] merged = merge(intervals);
        long total = 0;
        for (int[] iv : merged) {
            total += (long)(iv[1] - iv[0]);
        }
        return total;
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        print(merge(new int[][]{{1, 3}, {2, 6}, {8, 10}, {15, 18}}));
        // 期望: [[1,6], [8,10], [15,18]]

        print(merge(new int[][]{{1, 4}, {4, 5}}));
        // 期望: [[1,5]]

        print(merge(new int[][]{{1, 4}, {0, 4}}));
        // 期望: [[0,4]]

        print(merge(new int[][]{{1, 4}, {0, 0}}));
        // 期望: [[0,0], [1,4]]（不重叠）

        print(merge(new int[][]{{1, 4}, {2, 3}}));
        // 期望: [[1,4]]（[2,3] 完全包含）

        // 业务变体
        System.out.println("Total active time: " +
            totalActiveTime(new int[][]{{1, 3}, {2, 6}, {8, 10}}));
        // 期望: (6-1) + (10-8) = 5 + 2 = 7
    }

    private static void print(int[][] arr) {
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < arr.length; i++) {
            if (i > 0) sb.append(", ");
            sb.append(Arrays.toString(arr[i]));
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
 * Q: 为什么按 start 排序，不按 end？
 * A: 按 start 排序后，遍历时只需要关心当前 interval 是否与前一个的 end 重叠。
 *    若按 end 排序，需要在每个 interval 之前查找所有可能重叠的，反而复杂。
 *
 * Q: 如果区间数据不能排序（流式输入）？
 * A: 用 TreeMap<start, end>，每次新区间 [s, e] 进来：
 *    - 找到 floorKey(e)（可能与新区间重叠的最右）
 *    - 不断向左合并，直到 entry.value < s
 *    - 插入合并后的结果
 *    时间：每次插入 O(log n) 摊还
 *
 * Q: 如果是"插入新区间到已排序区间集"？
 * A: LC 57 Insert Interval。一次遍历 O(n)：
 *    1) 收集 end < new.start 的（保留）
 *    2) 合并 start <= new.end 的（扩展 new）
 *    3) 收集 start > new.end 的（保留）
 *
 * Q: 如何并行化？
 * A: 1) 数据分片，每片局部 merge
 *    2) 跨分片可能有需要再合并的（边界）→ 收集所有结果再做一次最终 merge
 *    类似 MergeSort 的思路。
 *
 * Q: SoFi 业务场景应用？
 * A: - 用户活跃时段统计（合并多设备登录时段）
 *    - 服务降级时间窗（合并多个 incident 时段算总停机时间）
 *    - 限制重复扣款（合并相邻交易请求）
 *
 * 相关 LeetCode：
 *   - LC 56  Merge Intervals (本题)
 *   - LC 57  Insert Interval
 *   - LC 252 Meeting Rooms (检测是否有重叠)
 *   - LC 253 Meeting Rooms II (最少需要几个会议室 → 优先队列)
 *   - LC 435 Non-overlapping Intervals (移除最少使其不重叠 → 贪心)
 */
