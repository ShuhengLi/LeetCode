/*
17. Letter Combinations of a Phone Number

Given a string containing digits from 2-9 inclusive, return all possible letter
combinations that the number could represent. Return the answer in any order.

A mapping of digits to letters (just like on the telephone buttons):
  2 -> abc, 3 -> def, 4 -> ghi, 5 -> jkl,
  6 -> mno, 7 -> pqrs, 8 -> tuv, 9 -> wxyz

Example 1:
  Input: digits = "23"
  Output: ["ad","ae","af","bd","be","bf","cd","ce","cf"]

Example 2:
  Input: digits = ""
  Output: []

Example 3:
  Input: digits = "2"
  Output: ["a","b","c"]

Constraints:
  - 0 <= digits.length <= 4
  - digits[i] is a digit in the range ['2', '9'].
*/
#include <vector>
#include <string>
using namespace std;

// DFS backtracking over digit positions.
// Time: O(4^n * n) where n = digits.length (4 for keys like 7,9)
// Space: O(n) recursion stack
class Solution {
    string mapping[10] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        vector<string> res;
        string path;
        dfs(digits, 0, path, res);
        return res;
    }

    void dfs(string& digits, int idx, string& path, vector<string>& res) {
        if (idx == digits.size()) { res.push_back(path); return; }

        for (char c : mapping[digits[idx] - '0']) {
            path.push_back(c);
            dfs(digits, idx + 1, path, res);
            path.pop_back();
        }
    }
};
