/*72. Edit Distance
Add to List

Share
Given two words word1 and word2, find the minimum number of operations required to convert word1 to word2.

You have the following 3 operations permitted on a word:

Insert a character
Delete a character
Replace a character
Example 1:

Input: word1 = "horse", word2 = "ros"
Output: 3
Explanation: 
horse -> rorse (replace 'h' with 'r')
rorse -> rose (remove 'r')
rose -> ros (remove 'e')
Example 2:

Input: word1 = "intention", word2 = "execution"
Output: 5
Explanation: 
intention -> inention (remove 't')
inention -> enention (replace 'i' with 'e')
enention -> exention (replace 'n' with 'x')
exention -> exection (replace 'n' with 'c')
exection -> execution (insert 'u')
*/
class Solution {
public:
    int minDistance(string word1, string word2) {
        if(word1.empty()) return word2.size();
        if(word2.empty()) return word1.size();
        int n = word1.size();
        int m = word2.size();
        vector<vector<int>> dp;
        for(int i = 0; i < n+1;i++ ){
            vector<int> cur(m+1, 0);
            dp.push_back(cur);
        }
        for(int i = 0; i < m + 1;i++ ){
            dp[0][i] = i;
        }
        for(int i = 0; i < n + 1;i++ ){
            dp[i][0] = i;
        }
        for(int i = 1; i < n + 1; i++){
            for(int j = 1; j < m + 1; j++){
                if(word1[i-1] == word2[j-1]){
                    dp[i][j] = dp[i-1][j-1];
                }else{
                    dp[i][j] = min(min(dp[i-1][j], dp[i][j-1]), dp[i-1][j-1]) + 1;
                }
            }
        }
        return dp[n][m];
    }
};
//Time:O(m*n)



class Solution {
  public:
    int minDistance(string word1, string word2, int s1, int s2) {
      if(word1.empty()) return word2.size();
      if(word2.empty()) return word1.size();
      int nothing = INT_MAX;
      if(word1[s1] == word2[s2]){
        nothing = minDistance(word1, word2, s1, s2);
      }
      int r = 1 + minDistance(word1, word2, s1, s2);
      int d = 1 + minDistance(word1, word2, s1+1, s2);
      int i = 1 + minDistance(word1, word2, s1, s2+1);
      return min(noting, r, d, i);
    }
};
//Time(4 ^ 2n)
//
//

class Solution {
  public:
    int minDistance(string word1, string word2, int s1, int s2) {
      if(word1.empty()) return word2.size();
      if(word2.empty()) return word1.size();
      int nothing = INT_MAX;
      s1 = word1.size();
      s2 = word2.size();
      int dp[s1+1][s2+1];
      for(int i = 0; i<= s1; i++){
        for(int j = 0; j <= s2; j++){
          if(i == 0){
            dp[i][j] = j;
          }else if(j == 0){
            dp[i][j] = i;
          }else if(word1[i - 1] == word2[j - 1]){
            dp[i][j] = dp[i-1][j-1];
          }else{
            dp[i][j] = min(min(dp[i][j-1], dp[i-1][j]), dp[i-1][j-1]) + 1;
          }
        }
      }
      return dp[s1][s2];
    }
};
