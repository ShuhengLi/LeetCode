/**
 * SoFi On-site - 真题（confirmed 2024/2025 multiple sources）
 *
 * 题目：Longest Palindromic Substring
 * ─────────────────────────────────────────────────────────────────
 * 给一个字符串 s，找出其中最长的回文子串。
 *
 * 示例 1：
 *   Input:  "babad"
 *   Output: "bab" 或 "aba"（任一即可）
 *
 * 示例 2：
 *   Input:  "cbbd"
 *   Output: "bb"
 *
 * 约束：
 *   - 1 <= s.length <= 1000（LeetCode 5）
 *   - SoFi 面试可能问到长度 10^5，需要 O(n) 解法
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路对比：
 *
 * 方法一：暴力 O(n^3)
 *   枚举所有子串 O(n^2)，每个判断回文 O(n) → 不通过
 *
 * 方法二：DP O(n^2) / O(n^2)
 *   dp[i][j] = s[i..j] 是否回文
 *   dp[i][j] = (s[i] == s[j]) && (j-i < 2 || dp[i+1][j-1])
 *
 * 方法三：中心扩展 O(n^2) / O(1) ← 推荐面试默认
 *   遍历每个位置作为回文中心，向两边扩展。
 *   注意：中心可以是单字符（奇长度）或两字符之间（偶长度）。
 *   总共 2n - 1 个中心，每个扩展最多 O(n) → O(n^2)
 *
 * 方法四：Manacher 算法 O(n) / O(n) ← Senior/Staff 加分项
 *   利用回文的对称性，避免重复扩展。复杂但优雅。
 */
public class LongestPalindromicSubstring {

    // ─── 方法三：中心扩展 ────────────────────────────────────────
    public static String longestPalindrome(String s) {
        if (s == null || s.length() < 2) return s;

        int start = 0, maxLen = 1;

        for (int i = 0; i < s.length(); i++) {
            // 奇长度回文，中心是 s[i]
            int len1 = expandAroundCenter(s, i, i);
            // 偶长度回文，中心在 s[i] 和 s[i+1] 之间
            int len2 = expandAroundCenter(s, i, i + 1);
            int len = Math.max(len1, len2);

            if (len > maxLen) {
                maxLen = len;
                // 计算起点：i 是中心，左边长度 = (len-1)/2
                start = i - (len - 1) / 2;
            }
        }

        return s.substring(start, start + maxLen);
    }

    private static int expandAroundCenter(String s, int left, int right) {
        while (left >= 0 && right < s.length() && s.charAt(left) == s.charAt(right)) {
            left--;
            right++;
        }
        return right - left - 1;  // 扩展失败时，实际长度 = (right-1) - (left+1) + 1
    }

    // ─── 方法四：Manacher 算法 O(n) ──────────────────────────────
    public static String longestPalindromeManacher(String s) {
        if (s == null || s.isEmpty()) return "";

        // 在每个字符之间插入 '#'，统一处理奇偶
        // 例如 "abc" → "^#a#b#c#$"，^ 和 $ 是哨兵防止越界
        StringBuilder sb = new StringBuilder("^");
        for (char c : s.toCharArray()) {
            sb.append('#').append(c);
        }
        sb.append("#$");
        char[] t = sb.toString().toCharArray();
        int n = t.length;

        int[] p = new int[n];  // p[i] = 以 i 为中心的回文半径
        int center = 0, right = 0;

        for (int i = 1; i < n - 1; i++) {
            int mirror = 2 * center - i;  // i 关于 center 的镜像

            if (i < right) {
                p[i] = Math.min(right - i, p[mirror]);
            }

            // 尝试扩展
            while (t[i + p[i] + 1] == t[i - p[i] - 1]) {
                p[i]++;
            }

            // 更新 center 和 right
            if (i + p[i] > right) {
                center = i;
                right = i + p[i];
            }
        }

        // 找最大半径对应的位置
        int maxLen = 0, centerIndex = 0;
        for (int i = 1; i < n - 1; i++) {
            if (p[i] > maxLen) {
                maxLen = p[i];
                centerIndex = i;
            }
        }

        // 还原原字符串中的位置
        int start = (centerIndex - maxLen) / 2;
        return s.substring(start, start + maxLen);
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        // 中心扩展
        System.out.println(longestPalindrome("babad"));   // "bab" 或 "aba"
        System.out.println(longestPalindrome("cbbd"));    // "bb"
        System.out.println(longestPalindrome("a"));        // "a"
        System.out.println(longestPalindrome("ac"));       // "a" 或 "c"
        System.out.println(longestPalindrome("racecar")); // "racecar"

        // Manacher
        System.out.println(longestPalindromeManacher("babad"));   // "aba" or "bab"
        System.out.println(longestPalindromeManacher("cbbd"));    // "bb"
        System.out.println(longestPalindromeManacher("racecar")); // "racecar"
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 中心扩展为什么是 O(n^2)？
 * A: n 个中心 × 每个最多扩展 O(n) → O(n^2)。
 *    "最多"是因为最坏情况整个字符串就是回文（如 "aaaa..."）。
 *
 * Q: DP 解法的复杂度？
 * A: 时间 O(n^2)，空间 O(n^2)。可优化空间到 O(n)（滚动数组），
 *    但仍不如中心扩展（O(1) 空间）实用。
 *
 * Q: Manacher 怎么做到 O(n)？
 * A: 关键洞察：当前位置 i 在已知回文 (center, right) 内时，
 *    可以用对称位置 mirror 的回文半径作为 i 的初始值，
 *    避免从 0 开始扩展。每个位置的 p[i] 总扩展次数 ≤ 1 摊还，
 *    总扩展次数 O(n)。
 *
 * Q: 如何避免奇偶讨论？
 * A: 中心扩展用两次循环（i,i 和 i,i+1）；Manacher 通过插入 '#' 统一处理。
 *
 * Q: 如果要找所有回文子串呢？
 * A: 中心扩展过程中记录每个扩展，O(n^2) 个回文。
 *    Manacher 的 p[i] 数组本身就编码了所有回文。
 *
 * Q: 字符串特别长（GB 级）怎么办？
 * A: 单机 O(n) 仍可处理 GB 级字符串。
 *    分布式：将字符串分块，需要处理跨块的回文（边界问题），较复杂。
 *
 * 对应 LeetCode：LC 5 Longest Palindromic Substring (Medium)
 *
 * SoFi 面试中通常要求中心扩展即可，但如果你能讲出 Manacher 思路，
 * 会大大加分（即使不写完整代码）。
 */
