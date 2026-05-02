/*
 * 438. Find All Anagrams in a String
 * Medium
 * [Atlassian Phone Screen — ✅ confirmed multiple 2024/2025 candidate reports]
 *
 * Given strings s and p, return all start indices of p's anagrams in s.
 * An anagram of p is a permutation of p's characters.
 *
 * Example 1:
 *   s = "cbaebabacd", p = "abc"  →  [0, 6]   ("cba", "bac")
 *
 * Example 2:
 *   s = "abab", p = "ab"         →  [0, 1, 2]
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach: Fixed-size sliding window + frequency count
 *   Maintain window of size |p|. Compare char frequencies.
 *   Optimization: use a `diff` counter (# chars whose freq doesn't match p)
 *   so we avoid O(26) comparison on every step.
 *
 *   - Init: count p's chars. For first |p| chars of s, update window and diff.
 *   - Slide: add right char → if freq now matches p → diff--
 *            remove left char → if freq was matching p  → diff++
 *   - When diff == 0, current window is an anagram → record index.
 *
 *   Time: O(|s| + |p|), Space: O(1) (fixed size arrays)
 *
 * Note: Atlassian often follows up with LC 76 Minimum Window Substring
 *       (variable-size window), which generalizes this pattern.
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> result;
        int ns = s.size(), np = p.size();
        if (ns < np) return result;

        vector<int> pFreq(26, 0), wFreq(26, 0);
        for (char c : p) pFreq[c - 'a']++;

        int diff = 0;
        for (int i = 0; i < 26; ++i) if (pFreq[i]) diff++;

        for (int r = 0; r < ns; ++r) {
            // Add right character
            int rc = s[r] - 'a';
            wFreq[rc]++;
            if (wFreq[rc] == pFreq[rc])      diff--;
            else if (wFreq[rc] == pFreq[rc] + 1) diff++;

            // Remove left character when window full
            if (r >= np) {
                int lc = s[r - np] - 'a';
                if (wFreq[lc] == pFreq[lc])      diff++;
                else if (wFreq[lc] == pFreq[lc] + 1) diff--;
                wFreq[lc]--;
            }

            if (diff == 0) result.push_back(r - np + 1);
        }
        return result;
    }

    // ── Bonus: LC 76 Minimum Window Substring ───────────────────
    // (Atlassian interviewers commonly ask this as follow-up)
    string minWindow(string s, string t) {
        vector<int> need(128, 0);
        for (char c : t) need[c]++;
        int left = 0, count = t.size(), minLen = INT_MAX, start = 0;
        for (int right = 0; right < (int)s.size(); ++right) {
            if (need[s[right]]-- > 0) count--;
            while (count == 0) {
                if (right - left + 1 < minLen) { minLen = right - left + 1; start = left; }
                if (need[s[left++]]++ == 0) count++;
            }
        }
        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};

/*
 * ── Follow-up (Atlassian onsite) ────────────────────────────────
 * Q: Why not just sort both windows and compare?
 * A: Sorting window on every slide is O(k log k) per step → O(n × k log k) total.
 *    Frequency array diff approach is O(n) total — much better for large s.
 *
 * Q: What if s and p have Unicode characters?
 * A: Use unordered_map<char32_t, int> instead of int[26].
 *
 * Q: How to count the number of distinct anagram substrings?
 * A: Same sliding window but use unordered_set<string> to deduplicate.
 *    Or sort each window of size |p| → O(n × p log p), only for small p.
 *
 * Q: Confluence search context — fuzzy match?
 * A: Anagram matching is too strict for real search. Real fuzzy match uses
 *    edit distance (Levenshtein) or phonetic matching (Soundex).
 *    Atlassian Confluence uses Elasticsearch with BM25 scoring.
 */
