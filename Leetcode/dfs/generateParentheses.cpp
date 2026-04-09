/*
22. Generate Parentheses

Given n pairs of parentheses, write a function to generate all combinations of
well-formed parentheses.

Example 1:
  Input: n = 3
  Output: ["((()))","(()())","(())()","()(())","()()()"]

Example 2:
  Input: n = 1
  Output: ["()"]

Constraints:
  - 1 <= n <= 8
*/
#include <vector>
#include <string>
using namespace std;

// DFS with pruning: only add '(' if open < n, only add ')' if close < open.
// This naturally generates only valid combinations.
// Time: O(4^n / sqrt(n)) — nth Catalan number
// Space: O(n) recursion stack
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        string path;
        dfs(n, 0, 0, path, res);
        return res;
    }

    void dfs(int n, int open, int close, string& path, vector<string>& res) {
        if (path.size() == 2 * n) { res.push_back(path); return; }

        if (open < n) {
            path.push_back('(');
            dfs(n, open + 1, close, path, res);
            path.pop_back();
        }
        if (close < open) {
            path.push_back(')');
            dfs(n, open, close + 1, path, res);
            path.pop_back();
        }
    }
};
