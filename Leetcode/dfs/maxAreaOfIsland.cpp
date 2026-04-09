/*
695. Max Area of Island

You are given an m x n binary matrix grid. An island is a group of 1's
(representing land) connected 4-directionally (horizontal or vertical). You may
assume all four edges of the grid are surrounded by water.

The area of an island is the number of cells with a value 1 in the island.

Return the maximum area of an island in grid. If there is no island, return 0.

Example 1:
  Input: grid = [[0,0,1,0,0,0,0,1,0,0,0,0,0],
                 [0,0,0,0,0,0,0,1,1,1,0,0,0],
                 [0,1,1,0,1,0,0,0,0,0,0,0,0],
                 [0,1,0,0,1,1,0,0,1,0,1,0,0],
                 [0,1,0,0,1,1,0,0,1,1,1,0,0],
                 [0,0,0,0,0,0,0,0,0,0,1,0,0],
                 [0,0,0,0,0,0,0,1,1,1,0,0,0],
                 [0,0,0,0,0,0,0,1,1,0,0,0,0]]
  Output: 6

Example 2:
  Input: grid = [[0,0,0,0,0,0,0,0]]
  Output: 0

Constraints:
  - m == grid.length
  - n == grid[i].length
  - 1 <= m, n <= 50
  - grid[i][j] is either 0 or 1.
*/
// DFS with return value - variation of Number of Islands
// Time: O(m*n), Space: O(m*n)
#include <vector>
using namespace std;

class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int maxArea = 0;
        for (int i = 0; i < grid.size(); i++)
            for (int j = 0; j < grid[0].size(); j++)
                if (grid[i][j]) maxArea = max(maxArea, dfs(grid, i, j));
        return maxArea;
    }

    int dfs(vector<vector<int>>& grid, int i, int j) {
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size()) return 0;
        if (grid[i][j] == 0) return 0;
        grid[i][j] = 0;
        return 1 + dfs(grid, i+1, j) + dfs(grid, i-1, j)
                 + dfs(grid, i, j+1) + dfs(grid, i, j-1);
    }
};
