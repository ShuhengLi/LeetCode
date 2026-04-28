/**
 * SoFi Tech Screen - 真题 (2024/2025 高频)
 *
 * 题目：Make Pairs Anagrams
 * ─────────────────────────────────────────────────────────────────
 * 给一个 String[][] pairs，每个元素是一对单词 [wordA, wordB]。
 * 对每一对，计算需要改变多少个字符才能让两个单词互为 anagram（字母异位词）。
 * 如果两个单词长度不同，返回 -1（无法成为 anagram）。
 * 最终返回 int[] result，result[i] 对应 pairs[i] 需要改变的字符数。
 *
 * 示例：
 *   Input:  [["tea", "eat"], ["abc", "xyz"], ["ab", "abc"]]
 *   Output: [0, 3, -1]
 *
 *   "tea" vs "eat"：都是 {a:1, e:1, t:1}，已经是 anagram → 0 次改变
 *   "abc" vs "xyz"：没有共同字符，需要改变 3 次
 *   "ab" vs "abc"：长度不同 → -1
 *
 * 约束：
 *   - 单词只含小写字母
 *   - 1 <= wordA.length, wordB.length <= 10^5
 *   - 1 <= pairs.length <= 10^4
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路分析：
 *   两个字符串互为 anagram 的条件：长度相等且每个字母出现次数相同。
 *
 *   核心计算方法：
 *     用频率数组 freq[26] 记录 wordA 中每个字母的出现次数。
 *     遍历 wordB，每遇到字母 c，如果 freq[c] > 0 就"匹配"掉（freq[c]--），否则说明这个字符在 wordA 里已经用完了，wordA 需要多一个位置来放它。
 *     最后 freq 中剩余 > 0 的计数之和，就是 wordA 中"多余的"字符数，这些就是需要被替换的。
 *     等价地：changes = (不匹配的字符数) = freq 中正值之和
 *
 *   时间复杂度：O(n) per pair，n = max(len(A), len(B))
 *   空间复杂度：O(1)（固定 26 个字母）
 */
public class MinChangesToMakeAnagrams {

    public static int[] minChanges(String[][] pairs) {
        int[] result = new int[pairs.length];

        for (int i = 0; i < pairs.length; i++) {
            result[i] = minChangesForPair(pairs[i][0], pairs[i][1]);
        }

        return result;
    }

    private static int minChangesForPair(String a, String b) {
        // 长度不同不可能是 anagram
        if (a.length() != b.length()) {
            return -1;
        }

        int[] freq = new int[26];

        // 统计 a 中每个字母的频率
        for (char c : a.toCharArray()) {
            freq[c - 'a']++;
        }

        // 用 b 的字母去"消耗" freq
        for (char c : b.toCharArray()) {
            if (freq[c - 'a'] > 0) {
                freq[c - 'a']--;  // 匹配成功，消耗一个
            }
            // 否则 b 中有 a 没有的字符，需要替换（由 freq 正值统计）
        }

        // freq 中剩余的正值 = a 中多余的字符数 = 需要替换的次数
        int changes = 0;
        for (int count : freq) {
            changes += count;
        }

        return changes;
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        String[][] pairs = {
            {"tea", "eat"},   // 期望: 0
            {"abc", "xyz"},   // 期望: 3
            {"ab", "abc"},    // 期望: -1
            {"fcrxzwscanmligyxyvkpqrxlsabotj", "jxwtrhvujlmrpdoqbisbwhmgpmeoke"}, // 期望: 23
        };

        int[] result = minChanges(pairs);
        for (int i = 0; i < pairs.length; i++) {
            System.out.printf("[\"%s\", \"%s\"] -> %d%n",
                pairs[i][0], pairs[i][1], result[i]);
        }
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 为什么不用 HashMap 而用 int[26]？
 * A: 题目保证只有小写字母，int[26] O(1) 空间更快，且缓存友好。
 *    如果涉及 Unicode/大小写，再换 HashMap<Character, Integer>。
 *
 * Q: 为什么 changes = freq 正值之和，而不是两边差值总和？
 * A: 因为一次"替换"操作：把 A 中多余的字符替换成 B 中缺少的字符，
 *    一步解决一个正值和一个负值，所以只需统计一侧（正值侧）即可。
 *
 * Q: 能否用 Arrays.sort 解决？
 * A: 可以。sort 两个字符串然后逐位比较，但 O(n log n)，不如 O(n) 优。
 *
 * Q: 并发场景下如何处理多对？
 * A: 每对独立，可以用 Java Stream + parallel() 或 CompletableFuture 并行处理。
 *    Senior 考察点：线程安全（result 数组按 index 写入，天然安全）。
 */
