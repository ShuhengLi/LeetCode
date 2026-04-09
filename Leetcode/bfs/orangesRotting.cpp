/*
994. Rotting Oranges

You are given an m x n grid where each cell can have one of three values:
  - 0 representing an empty cell,
  - 1 representing a fresh orange,
  - 2 representing a rotten orange.

Every minute, any fresh orange that is 4-directionally adjacent to a rotten
orange becomes rotten.

Return the minimum number of minutes that must elapse until no cell has a
fresh orange. If this is impossible, return -1.

Example 1:
  Input: grid = [[2,1,1],[1,1,0],[0,1,1]]
  Output: 4

Example 2:
  Input: grid = [[2,1,1],[0,1,1],[1,0,1]]
  Output: -1
  Explanation: The orange in the bottom left corner (row 2, column 0) is never
  rotten, because rotting only happens 4-directionally.

Example 3:
  Input: grid = [[0,2]]
  Output: 0
  Explanation: Since there are already no fresh oranges at minute 0, the answer is just 0.

Constraints:
  - m == grid.length
  - n == grid[i].length
  - 1 <= m, n <= 10
  - grid[i][j] is 0, 1, or 2.
*/
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        if(grid.empty() || grid[0].empty()) return -1;
        int rows = grid.size();
        int columns = grid[0].size();
        queue<pair<int, int>> q;
        int res = 0;
        for(int i = 0; i< rows; i++){
            for(int j = 0; j < columns; j++){
                if(grid[i][j] == 2){
                    q.push(make_pair(i,j));
                }
            }
        }
        while(!q.empty()){
            int size = q.size();
            //THIS FLAG!!!!
            bool flag = false;
            for(int s = 0; s < size; s++){
                auto top = q.front(); q.pop();
                int i = top.first;
                int j = top.second;
                if(j - 1 >= 0 &&grid[i][j-1] ==1){
                if(!flag){
                    flag = true;
                    res += 1;
                }
                    grid[i][j-1] = 2;
                    q.push(make_pair(i, j-1));
                }
                if(j + 1 < columns &&grid[i][j+1] ==1){
                if(!flag){
                    flag = true;
                    res += 1;
                }
                    grid[i][j+1] = 2;
                    q.push(make_pair(i, j+1));
                }
                if(i - 1 >= 0 && grid[i-1][j] ==1){
                if(!flag){
                    flag = true;
                    res += 1;
                }
                    grid[i-1][j] = 2;
                    q.push(make_pair(i-1, j));
                }
                if(i + 1 < rows && grid[i+1][j] ==1){
                if(!flag){
                    flag = true;
                    res += 1;
                }
                    grid[i+1][j] = 2;
                    q.push(make_pair(i+1, j));
                }
            }
        }
        for(int i = 0; i< rows; i++){
            for(int j = 0; j < columns; j++){
                if(grid[i][j] == 1){
                    return -1;
                }
            }
        }
        return res;
    }
};

// ===================== Improved Solution =====================
// Cleaner multi-source BFS: count fresh oranges upfront, decrement as they rot.
// Eliminates the flag logic and uses a direction array for cleaner traversal.
// Time: O(m*n), Space: O(m*n)
class Solution_v2 {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        queue<pair<int,int>> q;
        int fresh = 0;

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 2) q.push({i, j});
                else if (grid[i][j] == 1) fresh++;
            }

        if (fresh == 0) return 0;

        int dirs[] = {0, 1, 0, -1, 0};
        int minutes = 0;

        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                auto [r, c] = q.front(); q.pop();
                for (int d = 0; d < 4; d++) {
                    int nr = r + dirs[d], nc = c + dirs[d+1];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
                    if (grid[nr][nc] != 1) continue;
                    grid[nr][nc] = 2;
                    fresh--;
                    q.push({nr, nc});
                }
            }
            minutes++;
        }
        return fresh == 0 ? minutes - 1 : -1;
    }
};
