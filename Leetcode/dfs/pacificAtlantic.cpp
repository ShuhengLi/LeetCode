/*
417. Pacific Atlantic Water Flow

There is an m x n rectangular island that borders both the Pacific Ocean and
Atlantic Ocean. The Pacific Ocean touches the island's left and top edges, and
the Atlantic Ocean touches the island's right and bottom edges.

The island is partitioned into a grid of square cells. You are given an m x n
integer matrix heights where heights[r][c] represents the height above sea
level of the cell at coordinate (r, c).

The island receives a lot of rain. The rain water can flow to neighboring cells
directly north, south, east, and west if the neighboring cell's height is less
than or equal to the current cell's height. Water can flow from any cell
adjacent to an ocean into the ocean.

Return a 2D list of grid coordinates result where result[i] = [ri, ci] denotes
that rain water can flow from cell (ri, ci) to both the Pacific and Atlantic
oceans.

Example 1:
  Input: heights = [[1,2,2,3,5],[3,2,3,4,4],[2,4,5,3,1],[6,7,1,4,5],[5,1,1,2,4]]
  Output: [[0,4],[1,3],[1,4],[2,2],[3,0],[3,1],[4,0]]

Example 2:
  Input: heights = [[1]]
  Output: [[0,0]]

Constraints:
  - m == heights.length
  - n == heights[r].length
  - 1 <= m, n <= 200
  - 0 <= heights[r][c] <= 10^5
*/
// DFS from both oceans - reverse flow pattern (Google/Amazon)
// Time: O(m*n), Space: O(m*n)
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        int m = heights.size(), n = heights[0].size();
        vector<vector<bool>> pacific(m, vector<bool>(n, false));
        vector<vector<bool>> atlantic(m, vector<bool>(n, false));

        for (int i = 0; i < m; i++) {
            dfs(heights, pacific, i, 0);
            dfs(heights, atlantic, i, n - 1);
        }
        for (int j = 0; j < n; j++) {
            dfs(heights, pacific, 0, j);
            dfs(heights, atlantic, m - 1, j);
        }

        vector<vector<int>> res;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (pacific[i][j] && atlantic[i][j])
                    res.push_back({i, j});
        return res;
    }

    void dfs(vector<vector<int>>& heights, vector<vector<bool>>& visited, int i, int j) {
        visited[i][j] = true;
        int dirs[] = {0, 1, 0, -1, 0};
        for (int d = 0; d < 4; d++) {
            int ni = i + dirs[d], nj = j + dirs[d+1];
            if (ni < 0 || ni >= heights.size() || nj < 0 || nj >= heights[0].size()) continue;
            if (visited[ni][nj] || heights[ni][nj] < heights[i][j]) continue;
            dfs(heights, visited, ni, nj);
        }
    }
};
