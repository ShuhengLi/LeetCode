/*200. Number of Islands
 * Given a 2d grid map of '1's (land) and '0's (water), count the number of islands. An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically. You may assume all four edges of the grid are all surrounded by water.



Example 1:

Input: grid = [
  ["1","1","1","1","0"],
  ["1","1","0","1","0"],
  ["1","1","0","0","0"],
  ["0","0","0","0","0"]
]
Output: 1
*/
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int n = grid.size();
        if(!n) return 0;
        int m = grid[0].size();
        vector<pair<int, int>> directoions= {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int res = 0;
        for(int i = 0; i<n; i++){
            for(int j = 0; j < m;j++){
                if(grid[i][j] == '1'){
                    res++;
                    queue<pair<int, int>> q;
                    grid[i][j] = '0';
                    q.push({i,j});
                    while(!q.empty()){
                        int cur_x = q.front().first;
                        int cur_y = q.front().second;q.pop();
                        for(auto d:directoions){
                            auto x = d.first + cur_x;
                            auto y = d.second + cur_y;
                            if(x < 0 || x >= n || y < 0 || y >= m || grid[x][y] == '0') continue;
                            grid[x][y] = '0';
                            q.push({x, y});
                        }
                    }
                }
            }
        }
        return res;
    }
};
