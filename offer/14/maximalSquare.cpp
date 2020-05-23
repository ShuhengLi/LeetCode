/*221. Maximal Square
Medium

2777

71

Add to List

Share
Given a 2D binary matrix filled with 0's and 1's, find the largest square containing only 1's and return its area.

Example:

Input: 

1 0 1 0 0
1 0 1 1 1
1 1 1 1 1
1 0 0 1 0

Output: 4
*/
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        int res = 0;
        if(matrix.empty() || matrix[0].empty()) return 0;
        int rows = matrix.size();
        int cols = matrix[0].size();
        vector<vector<int>> dp(rows, vector<int>(cols, 0);;
        
        for(int i = 0; i < rows; i++){
            dp[i][0] = matrix[i][0] - '0';
            res = max(res, dp[i][0]);
        }
        for(int i = 0; i < cols; i++){
            dp[0][i] = matrix[0][i] - '0';
            res = max(res, dp[0][i]);
        }
        
        for(int i = 1; i < rows; i++){
            for(int j = 1; j < cols; j++){
                if(matrix[i][j] == '0'){
                    dp[i][j] = 0;
                }else{
                    dp[i][j] = min(min(dp[i-1][j-1], dp[i-1][j]), dp[i][j-1]) + 1;
                }
                res = max(res, dp[i][j]);
            }
        }
        return res*res;
    }
};
