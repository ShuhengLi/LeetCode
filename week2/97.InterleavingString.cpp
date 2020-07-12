/*97. Interleaving String
Given s1, s2, s3, find whether s3 is formed by the interleaving of s1 and s2.

Example 1:

Input: s1 = "aabcc", s2 = "dbbca", s3 = "aadbbcbcac"
Output: true
Example 2:

Input: s1 = "aabcc", s2 = "dbbca", s3 = "aadbbbaccc"
Output: false
*/
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if(s1.empty() && s2.empty()&& s3.empty()) return true;
        int n = s1.size();
        int m = s2.size();
        if(n+m != s3.size()) return false;
        vector<vector<bool>> dp(n+1, vector<bool>(m+1, false));
        for(int i = 1; i<= m; i++){
            if(s2[i-1]!=s3[i-1]){
                break;
            }
            dp[0][i] = true;
        }
        for(int i = 1; i<= n; i++){
            if(s1[i-1]!=s3[i-1]){
                break;
            }
            dp[i][0] = true;
        }
        for(int i = 1; i <= n;i++){
            for(int j = 1; j <= m;j++){
                int k = i + j;
                if(dp[i][j-1] && (s2[j-1] == s3[k-1])){
                    dp[i][j] = true;
                }
                if(dp[i-1][j] && (s1[i-1] == s3[k-1])){
                    dp[i][j] = true;
                }
            }
        }
        return dp[n][m];
    }
};
