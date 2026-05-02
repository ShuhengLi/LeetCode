/*
 * 17. Letter Combinations of a Phone Number
 * Medium
 * [Atlassian DSA - Medium Frequency, classic backtracking warm-up]
 *
 * Given a string containing digits 2-9, return all possible letter
 * combinations that the number could represent (phone keypad mapping).
 *
 * 2 -> abc, 3 -> def, 4 -> ghi, 5 -> jkl, 6 -> mno,
 * 7 -> pqrs, 8 -> tuv, 9 -> wxyz
 *
 * Examples:
 *   "23"  -> ["ad","ae","af","bd","be","bf","cd","ce","cf"]
 *   ""    -> []
 *   "2"   -> ["a","b","c"]
 *
 * Approach: Backtracking
 *   - Build current candidate char-by-char.
 *   - At each step pick one letter from the current digit's mapping; recurse.
 *   - On reaching length == digits.size(), commit candidate.
 *
 * Time:  O(4^n * n)  (n = len(digits); 7 and 9 have 4 letters)
 * Space: O(n) recursion depth (output excluded)
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<string> result;
    vector<string> map_ = {
        "",     "",     "abc",  "def",
        "ghi",  "jkl",  "mno",  "pqrs",
        "tuv",  "wxyz"
    };
    string digits_;

    void backtrack(int idx, string& cur) {
        if (idx == (int)digits_.size()) {
            result.push_back(cur);
            return;
        }
        const string& letters = map_[digits_[idx] - '0'];
        for (char c : letters) {
            cur.push_back(c);
            backtrack(idx + 1, cur);
            cur.pop_back();         // undo
        }
    }

public:
    vector<string> letterCombinations(string digits) {
        result.clear();
        if (digits.empty()) return result;
        digits_ = digits;
        string cur;
        cur.reserve(digits.size());
        backtrack(0, cur);
        return result;
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: Iterative (BFS) version?
 * A: Maintain a queue of partial strings; for each digit, take each prefix and
 *    append every letter, replacing the queue.
 *
 * Q: How to handle 1 and 0 if they appear?
 * A: Spec excludes them; if encountered, either skip or treat as literal '1'/'0'.
 *
 * Q: T9 dictionary search variant — given digits, return only words
 *    that exist in dictionary?
 * A: Use a Trie of dictionary words; backtrack only into branches that exist
 *    in Trie. Massive pruning.
 *
 * Q: Atlassian product angle?
 * A: Auto-complete / search suggestion across Jira issue keys, Confluence
 *    pages — same backtracking + Trie pattern.
 */

int main() {
    Solution sol;
    auto r1 = sol.letterCombinations("23");
    for (auto& s : r1) cout << s << " ";
    cout << endl;
    // ad ae af bd be bf cd ce cf

    auto r2 = sol.letterCombinations("");
    cout << "size=" << r2.size() << endl;   // 0

    auto r3 = sol.letterCombinations("9");
    for (auto& s : r3) cout << s << " ";
    cout << endl;
    // w x y z
    return 0;
}
