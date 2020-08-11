/*Minimum Window Subsequence
 * Given strings S and T, find the minimum (contiguous) substring W of S, so that T is a subsequence of W.

If there is no such window in S that covers all characters in T, return the empty string "". If there are multiple such minimum-length windows, return the one with the smallest starting index.

 Notice
All the strings in the input will only contain lowercase letters.
The length of S will be in the range [1, 20000].
The length of T will be in the range [1, 100].
Have you met this question in a real interview?
Example
Example 1:

Input：S="jmeqksfrsdcmsiwvaovztaqenprpvnbstl"，T="u"
Output：""
Explanation： unable to match
Example 2:

Input：S = "abcdebdde"， T = "bde"
Output："bcde"
Explanation："bcde" is the answer and "deb" is not a smaller window because the elements of T in the window must occur in order.
*/
class Solution {
public:
    /**
     * @param S: a string
     * @param T: a string
     * @return: the minimum substring of S
     */
    string minWindow(string &S, string &T) {
         int n = S.size();
         int m = T.size();
         int i = 0;
         int j = 0;
         int start = -1;
         int len = INT_MAX;
         while(i < n){
             if(S[i] == T[j]){
                 if(++j == m){
                     int end = i+1;
                     while(--j >= 0){
                         while(S[i--] != T[j]);
                     }
                     i++, j++;
                     if(end - i < len){
                         len = end -i;
                         start = i;
                     }
                 }
             }
             i++;
         }
         return (start == -1) ? "" : S.substr(start, len);
/*
        string res;
        if(S.size()<T.size()) return res;
        int n = T.size();
        int m = S.size();
        vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
        for(int i = 0; i < m+1; i++){
            dp[0][i] = 1;
        }
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= m;j++){
                if(S[j-1] != T[i-1]){
                    if(dp[i][j-1]){
                        dp[i][j] = dp[i][j-1] + 1;
                    }
                }else{
                    if(dp[i-1][j]&&dp[i-1][j]>=i){
                        dp[i][j] = dp[i-1][j];
                    }
                }
            }
        }
        int end = 0;
        int len = INT_MAX;
        for(auto row:dp){
            for(auto c:row){
                cout << c << " ";
            }
            cout << endl;
        }
        for(int i = 1; i <= m; i++ ){
            if(dp[n][i] && dp[n][i] < len){
                len = dp[n][i];
                end = i;
            }
        }
        if(end==0) return res;
        int start =  end - len;
        res=S.substr(start, len);
        return res;

*/
    }
};
