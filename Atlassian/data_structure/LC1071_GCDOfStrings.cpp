/*
 * 1071. Greatest Common Divisor of Strings
 * Easy
 * [Atlassian DSA - HIGH Frequency from candidate reports]
 *
 * For two strings s and t, we say "t divides s" iff s = t + t + ... + t
 * (some number of concatenations).
 * Return the largest string x such that x divides both str1 and str2.
 *
 * Examples:
 *   "ABCABC", "ABC"     -> "ABC"
 *   "ABABAB", "ABAB"    -> "AB"
 *   "LEET",   "CODE"    -> ""
 *
 * ─────────────────────────────────────────────────────────────────
 * Key insight:
 *   If a common divisor string x exists, then str1 + str2 == str2 + str1.
 *   (Because both reduce to repeats of x.)
 *   The length of the answer is gcd(|str1|, |str2|).
 *   Verify by checking str1 + str2 == str2 + str1 first.
 *
 * Time:  O(n + m)
 * Space: O(n + m) for concatenation check (can be eliminated)
 * ─────────────────────────────────────────────────────────────────
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string gcdOfStrings(string str1, string str2) {
        // Necessary & sufficient condition
        if (str1 + str2 != str2 + str1) return "";
        int g = gcd((int)str1.size(), (int)str2.size());
        return str1.substr(0, g);
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: Why is str1+str2 == str2+str1 the right test?
 * A: If a common base x exists, both are repeats of x → concatenation order
 *    irrelevant. Conversely, by Levi's lemma in formal language theory,
 *    if uv = vu, then both u and v are powers of the same string.
 *
 * Q: Without string concatenation (memory tight)?
 * A: Verify directly: candidate length = gcd(|s|, |t|); confirm candidate
 *    repeats fill both strings exactly. O(n + m) time, O(g) space.
 *
 * Q: Multi-string GCD?
 * A: Iteratively gcdOfStrings(answer, next), short-circuit on "".
 *
 * Q: Atlassian angle — Confluence?
 * A: Detection of repeated content blocks in pages, template extraction.
 */

int main() {
    Solution sol;
    cout << sol.gcdOfStrings("ABCABC", "ABC") << endl;     // ABC
    cout << sol.gcdOfStrings("ABABAB", "ABAB") << endl;    // AB
    cout << sol.gcdOfStrings("LEET", "CODE") << endl;      // (empty)
    cout << sol.gcdOfStrings("AAAA", "AA") << endl;        // AA
    return 0;
}
