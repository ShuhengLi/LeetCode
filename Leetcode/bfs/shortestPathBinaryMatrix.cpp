/*
1091. Shortest Path in Binary Matrix

Given an n x n binary matrix grid, return the length of the shortest clear path
in the matrix. If there is no clear path, return -1.

A clear path in a binary matrix is a path from the top-left cell (0, 0) to the
bottom-right cell (n - 1, n - 1) such that:
  - All the visited cells of the path are 0.
  - All the adjacent cells of the path are 8-directionally connected.

The length of a clear path is the number of visited cells of this path.

Example 1:
  Input: grid = [[0,1],[1,0]]
  Output: 2

Example 2:
  Input: grid = [[0,0,0],[1,1,0],[1,1,0]]
  Output: 4

Example 3:
  Input: grid = [[1,0,0],[1,1,0],[1,1,0]]
  Output: -1

Constraints:
  - n == grid.length == grid[i].length
  - 1 <= n <= 100
  - grid[i][j] is 0 or 1.
*/
#include <vector>
#include <queue>
using namespace std;

// BFS on grid with 8 directions. Store distance in-place to avoid visited set.
// Time: O(n^2), Space: O(n^2)
class Solution {
public:
    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        int n = grid.size();
        if (grid[0][0] || grid[n-1][n-1]) return -1;

        queue<pair<int,int>> q;
        q.push({0, 0});
        grid[0][0] = 1;

        int dirs[][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            if (r == n-1 && c == n-1) return grid[r][c];

            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                if (grid[nr][nc] != 0) continue;
                grid[nr][nc] = grid[r][c] + 1;
                q.push({nr, nc});
            }
        }
        return -1;
    }
};
