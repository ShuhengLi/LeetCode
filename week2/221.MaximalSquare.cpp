/*221. Maximal Square
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
        int n = matrix.size();
        if(!n) return 0;
        int m = matrix[0].size();
        vector<vector<int>> dp(n, vector<int>(m, 0));
        for(int i = 0; i < n; i++){
            dp[i][0] = matrix[i][0] - '0';
            res = max(dp[i][0], res);
        }
        for(int i = 0; i < m; i++){
            dp[0][i] = matrix[0][i] - '0';
            res = max(dp[0][i], res);
        }
        for(int i = 1; i< n;i++){
            for(int j = 1; j < m; j++){
                if(matrix[i][j] == '1'){
                    dp[i][j] = min(min(dp[i-1][j], dp[i][j-1]), dp[i-1][j-1]) + 1;
                    res = max(dp[i][j], res);
                }
            }
        }
        return res*res;
    }
};
