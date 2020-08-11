/*695. Max Area of Island
 * Given a non-empty 2D array grid of 0's and 1's, an island is a group of 1's (representing land) connected 4-directionally (horizontal or vertical.) You may assume all four edges of the grid are surrounded by water.

Find the maximum area of an island in the given 2D array. (If there is no island, the maximum area is 0.)

Example 1:

[[0,0,1,0,0,0,0,1,0,0,0,0,0],
 [0,0,0,0,0,0,0,1,1,1,0,0,0],
 [0,1,1,0,1,0,0,0,0,0,0,0,0],
 [0,1,0,0,1,1,0,0,1,0,1,0,0],
 [0,1,0,0,1,1,0,0,1,1,1,0,0],
 [0,0,0,0,0,0,0,0,0,0,1,0,0],
 [0,0,0,0,0,0,0,1,1,1,0,0,0],
 [0,0,0,0,0,0,0,1,1,0,0,0,0]]
Given the above grid, return 6. Note the answer is not 11, because the island must be connected 4-directionally.
Example 2:

[[0,0,0,0,0,0,0,0]]
*/
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int res = 0;
        int n = grid.size();
        if(!n) return 0;
        int m = grid[0].size();
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(grid[i][j] == 1){
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    int local = 0;
                    while(!q.empty()){
                        auto cur = q.front(); q.pop();
                        grid[i][j] = 0;
                        //if(visited[cur.first][cur.second]) continue;
                        //visited[cur.first][cur.second] = true;
                        local++;
                        grid[cur.first][cur.second] = 0;
                        for(auto d:directions){
                            int x = cur.first + d.first;
                            int y = cur.second+ d.second;
                            if(x < 0 || x >= n || y < 0 || y == m || grid[x][y]==0 ) continue;
                            grid[x][y] = 0;
                            q.push({x,y});
                        }
                    }
                    res = max(res, local);
                }
            }
        }
        return res;
    }
};
