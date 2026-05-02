/*
 * 200. Number of Islands
 * Medium
 * [Atlassian DSA Round — ✅ confirmed multiple Glassdoor/Blind 2024/2025 reports]
 *
 * Given an m×n grid of '1' (land) and '0' (water), count the number of islands.
 * An island is surrounded by water and formed by connecting adjacent lands
 * horizontally or vertically.
 *
 * Example 1:
 *   grid = [["1","1","1","1","0"],
 *           ["1","1","0","1","0"],
 *           ["1","1","0","0","0"],
 *           ["0","0","0","0","0"]]
 *   Output: 1
 *
 * Example 2:
 *   grid = [["1","1","0","0","0"],
 *           ["1","1","0","0","0"],
 *           ["0","0","1","0","0"],
 *           ["0","0","0","1","1"]]
 *   Output: 3
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach 1: BFS (recommended — no stack overflow risk)
 *   For each unvisited '1': BFS to mark the whole island as '0', count++.
 *   Time: O(m×n), Space: O(min(m,n)) — max BFS queue width
 *
 * Approach 2: DFS (concise, but risky on huge grids)
 *   Recursively flood-fill '1' → '0'.
 *   Time: O(m×n), Space: O(m×n) worst case recursion depth
 *
 * Approach 3: Union-Find (best for dynamic/distributed scenarios)
 *   Each '1' is a node. Union adjacent '1's. Final component count = answer.
 *   Time: O(m×n × α) ≈ O(m×n), Space: O(m×n)
 *
 * Atlassian follow-up (reported):
 *   Q: "Grid is distributed across multiple machines — how do you count?"
 *   A: Each machine computes local components. At boundaries, send edge info
 *      to coordinator for cross-machine Union-Find merging.
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // ── Approach 1: BFS ──────────────────────────────────────────
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size(), count = 0;
        vector<pair<int,int>> dirs = {{0,1},{0,-1},{1,0},{-1,0}};

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == '1') {
                    ++count;
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    grid[i][j] = '0';
                    while (!q.empty()) {
                        auto [r, c] = q.front(); q.pop();
                        for (auto [dr, dc] : dirs) {
                            int nr = r + dr, nc = c + dc;
                            if (nr >= 0 && nr < m && nc >= 0 && nc < n
                                && grid[nr][nc] == '1') {
                                grid[nr][nc] = '0';
                                q.push({nr, nc});
                            }
                        }
                    }
                }
            }
        }
        return count;
    }

    // ── Approach 2: DFS ──────────────────────────────────────────
    int numIslandsDFS(vector<vector<char>>& grid) {
        int count = 0;
        for (int i = 0; i < (int)grid.size(); ++i)
            for (int j = 0; j < (int)grid[0].size(); ++j)
                if (grid[i][j] == '1') { dfs(grid, i, j); ++count; }
        return count;
    }

private:
    void dfs(vector<vector<char>>& g, int i, int j) {
        if (i < 0 || i >= (int)g.size() || j < 0 || j >= (int)g[0].size()
            || g[i][j] != '1') return;
        g[i][j] = '0';
        dfs(g, i+1, j); dfs(g, i-1, j);
        dfs(g, i, j+1); dfs(g, i, j-1);
    }
};

/*
 * ── Follow-up (Atlassian onsite) ────────────────────────────────
 * Q: BFS vs DFS — which to prefer?
 * A: BFS avoids stack overflow on huge grids (10^6 × 10^6).
 *    DFS is more concise. For interviews: state preference and justify.
 *
 * Q: How to handle dynamic addLand(r,c)?  → LC 305 Number of Islands II
 * A: Union-Find. On addLand: mark '1', count++,
 *    union each '1'-neighbor (count-- per successful union).
 *
 * Q: Max area variant?  → LC 695 Max Area of Island
 * A: Track current island size in BFS/DFS, maintain global max.
 */
