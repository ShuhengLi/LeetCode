/*1139. Largest 1-Bordered Square
Given a 2D grid of 0s and 1s, return the number of elements in the largest square subgrid that has all 1s on its border, or 0 if such a subgrid doesn't exist in the grid.
Example 1:

Input: grid = [[1,1,1],[1,0,1],[1,1,1]]
Output: 9
Example 2:

Input: grid = [[1,1,0,0]]
Output: 1
*/
class Solution {
public:
    int rows;
    int cols;
    int largest1BorderedSquare(vector<vector<int>>& grid) {
        if(grid.empty() || grid[0].empty()) return 0;
        rows = grid.size();
        cols = grid[0].size();
        vector<vector<int>> b2t(rows, vector<int>(cols));
        vector<vector<int>> r2l(rows, vector<int>(cols));
        bottom2top(grid, b2t);
        right2left(grid, r2l);
        int res = 0;
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(grid[i][j]){
                    int k = min(b2t[i][j],r2l[i][j]) - 1;
                    for(;k >= 0;k--){
                        if(i + k > rows || j + k > cols) continue;
                        int l = k + 1;
                        if(b2t[i][j]>=l && r2l[i][j]>=l && b2t[i][j+k] >= l && r2l[i+k][j] >=l){
                            res = max(res, l);
                            break;
                        }
                    }
                }
            }
        }
        return res * res;
    }
    void bottom2top(vector<vector<int>> v, vector<vector<int>>& m){
        for(int i = 0; i< cols; i++){
            m[rows-1][i] = v[rows-1][i];
        }
        for(int i = 0; i < cols; i++){
            for(int j = rows -2; j >=0; j--){
                if(v[j][i]){
                    m[j][i] = m[j+1][i] + 1;
                }
            }
        }
    }
    void right2left(vector<vector<int>> v, vector<vector<int>>& m){
        for(int i = 0; i< rows; i++){
            m[i][cols-1] = v[i][cols-1];
        }
        for(int i = 0; i < rows; i++){
            for(int j = cols - 2; j >= 0; j--){
                if(v[i][j]){
                    m[i][j] = m[i][j+1] + 1;
                }
            }
        }
    }
};
