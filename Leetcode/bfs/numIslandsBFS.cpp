/*
200. Number of Islands (BFS approach)

Given an m x n 2D binary grid grid which represents a map of '1's (land) and
'0's (water), return the number of islands.

An island is surrounded by water and is formed by connecting adjacent lands
horizontally or vertically. You may assume all four edges of the grid are
surrounded by water.

Example 1:
  Input: grid = [
    ["1","1","1","1","0"],
    ["1","1","0","1","0"],
    ["1","1","0","0","0"],
    ["0","0","0","0","0"]
  ]
  Output: 1

Example 2:
  Input: grid = [
    ["1","1","0","0","0"],
    ["1","1","0","0","0"],
    ["0","0","1","0","0"],
    ["0","0","0","1","1"]
  ]
  Output: 3

Constraints:
  - m == grid.length
  - n == grid[i].length
  - 1 <= m, n <= 300
  - grid[i][j] is '0' or '1'.
*/
#include <vector>
#include <queue>
using namespace std;

// BFS flood fill. For each unvisited '1', BFS to mark all connected land.
// Time: O(m*n), Space: O(min(m,n)) for the queue
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size(), count = 0;
        int dirs[] = {0, 1, 0, -1, 0};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '0') continue;
                count++;
                grid[i][j] = '0';
                queue<pair<int,int>> q;
                q.push({i, j});
                while (!q.empty()) {
                    auto [r, c] = q.front(); q.pop();
                    for (int d = 0; d < 4; d++) {
                        int nr = r + dirs[d], nc = c + dirs[d+1];
                        if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
                        if (grid[nr][nc] == '0') continue;
                        grid[nr][nc] = '0';
                        q.push({nr, nc});
                    }
                }
            }
        }
        return count;
    }
};
