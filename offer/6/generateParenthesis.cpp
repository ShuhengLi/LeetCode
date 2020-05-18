/*22. Generate Parentheses
 * Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.

For example, given n = 3, a solution set is:

[
  "((()))",
  "(()())",
  "(())()",
  "()(())",
  "()()()"
]
*/
class Solution {
public:
    vector<string> res;
    vector<string> generateParenthesis(int n) {
        string cur;
        int a = 0, b = 0;
        helper(n, a, b, cur);
        return res;
    }
    void helper(int n, int a, int b, string cur){
        if(a + b == 2*n){
            res.push_back(cur);
            return;
        }
        if(a<n){
            helper(n, a+1, b, cur + '(');
        }
        if(b < a){
            helper(n, a, b + 1, cur + ')');
        }
    }
};
