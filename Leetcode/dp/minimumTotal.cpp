/*120. Triangle
 *
Given a triangle, find the minimum path sum from top to bottom. Each step you may move to adjacent numbers on the row below.

For example, given the following triangle

[
     [2],
    [3,4],
   [6,5,7],
  [4,1,8,3]
]
The minimum path sum from top to bottom is 11 (i.e., 2 + 3 + 5 + 1 = 11).
*/
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        if(triangle.empty()) return 0;
        if(triangle[0].empty()) return 0;
        int rows = triangle.size();
        vector<vector<int>> dp;
        vector<int> base(1, triangle[0][0]);
        dp.push_back(base);
        for(int i = 1; i< rows; i++){
            int column = triangle[i].size();
            vector<int> cur(column, INT_MAX);// INT_MAX!!!!!
            for(int j = 0; j < column; j++){
                if(j < dp[i-1].size()){
                    cur[j]= dp[i-1][j];
                }
                if(j - 1 >= 0){
                    cur[j] = min(cur[j], dp[i-1][j-1] );
                }
                cur[j] += triangle[i][j];
            }
            dp.push_back(cur);
        }
        int g_min = dp[rows - 1][0];
        for(int i = 0; i< dp[rows - 1].size(); i++){
            g_min = min(g_min, dp[rows-1][i]);
        }
        return g_min;
    }
};


