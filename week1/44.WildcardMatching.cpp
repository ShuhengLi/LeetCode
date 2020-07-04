/* 44. Wildcard Matching
Given an input string (s) and a pattern (p), implement wildcard pattern matching with support for '?' and '*'.

'?' Matches any single character.
'*' Matches any sequence of characters (including the empty sequence).
The matching should cover the entire input string (not partial).

Note:

s could be empty and contains only lowercase letters a-z.
p could be empty and contains only lowercase letters a-z, and characters like ? or *.
Example 1:

Input:
s = "aa"
p = "a"
Output: false
Explanation: "a" does not match the entire string "aa".
*/
class Solution {
public:
    bool isMatch(string s, string p) {
        int n = s.size();
        int m = p.size();
        if(!m && !n) return true;
        if(!m) return false;
        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));
        dp[0][0] = true;
        for(int i = 1; i <= m; i++){
            if(p[i-1] != '*') break;
            dp[0][i] = true;
        }
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= m; j++){
                if((p[j-1] == s[i-1] || p[j-1] == '?') && dp[i-1][j-1] ==true ){
                    dp[i][j] = true;
                }else if(p[j-1] == '*' ){
                    dp[i][j] = dp[i-1][j]|| dp[i][j-1];
                }
            }
        }
        return dp[n][m];
    }
};
