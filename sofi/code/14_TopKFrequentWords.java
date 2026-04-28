import java.util.*;

/**
 * SoFi DSA Round - 真题（与第5题 Transaction Frequency 相关，高频）
 *
 * 题目：Top K Frequent Words
 * ─────────────────────────────────────────────────────────────────
 * 给一个字符串数组 words 和整数 k。
 * 返回前 k 个出现频率最高的单词。
 *
 * 排序规则：
 *   1. 频率从高到低
 *   2. 频率相同时，按字典序升序（lexicographically smaller first）
 *
 * 示例 1：
 *   words = ["i","love","leetcode","i","love","coding"], k = 2
 *   Output: ["i", "love"]
 *   解释: i 出现 2 次, love 出现 2 次, 字典序 i < love
 *
 * 示例 2：
 *   words = ["the","day","is","sunny","the","the","the","sunny","is","is"], k = 4
 *   Output: ["the","is","sunny","day"]
 *
 * SoFi 业务场景：
 *   - 高频交易类型 Top K
 *   - 热门搜索关键词
 *   - 投诉/客服 ticket 分类 Top K
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路对比：
 *
 * 方法一：HashMap + 排序所有
 *   时间 O(n log n)，空间 O(n)，n = unique 单词数
 *   简单但当 n >> k 时浪费
 *
 * 方法二：HashMap + 最小堆（Min-Heap of size k）← 推荐
 *   维护大小为 k 的堆，遍历时若新元素 > 堆顶则替换
 *   关键：堆比较器要"反向"——堆顶应该是 top-k 中最差的
 *   时间 O(n log k)，空间 O(n + k)
 *
 * 方法三：Bucket Sort O(n)
 *   按频率分桶（频率最大不超过 n），从最高频率桶往下取
 *   仅适用于不需要次序排序的场景；
 *   若同频按字典序，仍需排序桶内元素，整体 O(n + total log per bucket)
 */
public class TopKFrequentWords {

    // ─── 方法二：Min-Heap ─────────────────────────────────────────
    public static List<String> topKFrequent(String[] words, int k) {
        // 统计频率
        Map<String, Integer> count = new HashMap<>();
        for (String w : words) {
            count.merge(w, 1, Integer::sum);
        }

        // 最小堆比较器（反向规则）：
        //   "差的"在堆顶，便于 poll 出去
        //   "差" = 频率低；频率相同时，字典序大（即应排在后面）
        PriorityQueue<Map.Entry<String, Integer>> heap = new PriorityQueue<>(
            (a, b) -> {
                if (!a.getValue().equals(b.getValue())) {
                    return a.getValue() - b.getValue();  // 频率小的在堆顶
                }
                // 频率相同：字典序"大"的在堆顶（先被淘汰）
                return b.getKey().compareTo(a.getKey());
            }
        );

        for (Map.Entry<String, Integer> entry : count.entrySet()) {
            heap.offer(entry);
            if (heap.size() > k) heap.poll();
        }

        // 堆里是 top-k，但顺序是反的，需要逆序输出
        LinkedList<String> result = new LinkedList<>();
        while (!heap.isEmpty()) {
            result.addFirst(heap.poll().getKey());
        }
        return result;
    }

    // ─── 方法一：全部排序 ────────────────────────────────────────
    public static List<String> topKFrequentSort(String[] words, int k) {
        Map<String, Integer> count = new HashMap<>();
        for (String w : words) count.merge(w, 1, Integer::sum);

        List<String> candidates = new ArrayList<>(count.keySet());
        candidates.sort((a, b) -> {
            int fa = count.get(a), fb = count.get(b);
            if (fa != fb) return fb - fa;       // 频率降序
            return a.compareTo(b);              // 字典序升序
        });

        return candidates.subList(0, k);
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        System.out.println(topKFrequent(
            new String[]{"i","love","leetcode","i","love","coding"}, 2));
        // 期望: [i, love]

        System.out.println(topKFrequent(
            new String[]{"the","day","is","sunny","the","the","the","sunny","is","is"}, 4));
        // 期望: [the, is, sunny, day]

        // 排序方法一致性
        System.out.println(topKFrequentSort(
            new String[]{"i","love","leetcode","i","love","coding"}, 2));

        // 边界：k = unique 数
        System.out.println(topKFrequent(new String[]{"a","b","c"}, 3));
        // 期望: [a, b, c]（频率都为 1，按字典序）
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 为什么用最小堆找 Top K（频率最高）？
 * A: 反直觉但正确：维护 size=k 的最小堆，堆顶是当前 top-k 中最小的；
 *    新元素若 > 堆顶则替换。最终堆中即 top-k。
 *    用最大堆需要 size=n，再 poll k 次，空间反而 O(n)。
 *
 * Q: 比较器为什么频率"小"在堆顶但字典序"大"在堆顶？
 * A: 因为我们要淘汰"不优"的元素：
 *    - 频率小的不优 → 堆顶是频率最小的 → poll 出去
 *    - 频率相同时，字典序大的不优（最终结果按字典序升序） → 堆顶是字典序大的
 *    这两个方向看起来矛盾，但从"哪种应该被淘汰"角度看是一致的。
 *
 * Q: O(n log k) vs O(n log n)，何时差距明显？
 * A: 当 k << n 时差距显著。例如 n=10^6, k=10：
 *    - log k ≈ 3.3, log n ≈ 20，约快 6 倍
 *    流式数据（无法全部排序）必须用堆方法。
 *
 * Q: 如何处理流式数据？
 * A: 用同样的堆方法在线维护 top-k：每个新事件更新计数，重新检查堆。
 *    实际生产用 Count-Min Sketch + 堆做近似 top-k（节省内存）。
 *
 * Q: 大数据量分布式怎么做？
 * A: Map-Reduce：
 *    - Map 阶段：每分片输出 (word, 1)
 *    - Reduce 阶段：按 word 聚合频率
 *    - 最后用堆找全局 top-k（小心：每分片 top-k 合并后不一定等于全局 top-k！
 *      需要带频率合并，或每分片输出 top-(k+α) 容错）
 *
 * Q: 为什么不直接用 Java 的 Stream sorted().limit(k)？
 * A: 可以！但 sorted() 是 O(n log n)，不如手写堆。
 *    生产代码追求可读性时可以用 Stream，性能敏感场景手写堆。
 *
 * 相关 LeetCode：
 *   - LC 692 Top K Frequent Words (本题)
 *   - LC 347 Top K Frequent Elements
 *   - LC 215 Kth Largest Element (类似思路，QuickSelect 可 O(n) 平均)
 */
