/*
286. Walls and Gates (Premium)

You are given an m x n grid rooms initialized with these three possible values:
  - -1  A wall or an obstacle.
  - 0   A gate.
  - INF Infinity means an empty room. We use the value 2^31 - 1 = 2147483647
        to represent INF as you may assume that the distance to a gate is less
        than 2147483647.

Fill each empty room with the distance to its nearest gate. If it is impossible
to reach a gate, it should be filled with INF.

Example 1:
  Input: rooms = [[INF,-1,0,INF],[INF,INF,INF,-1],[INF,-1,INF,-1],[0,-1,INF,INF]]
  Output: [[3,-1,0,1],[2,2,1,-1],[1,-1,2,-1],[0,-1,3,4]]

Example 2:
  Input: rooms = [[-1]]
  Output: [[-1]]

Constraints:
  - m == rooms.length
  - n == rooms[i].length
  - 1 <= m, n <= 250
  - rooms[i][j] is -1, 0, or 2^31 - 1.
*/
#include <vector>
#include <queue>
using namespace std;

// Multi-source BFS from all gates simultaneously.
// Each empty room is reached by the nearest gate first due to BFS level-order property.
// Time: O(m*n), Space: O(m*n)
class Solution {
public:
    void wallsAndGates(vector<vector<int>>& rooms) {
        int m = rooms.size(), n = rooms[0].size();
        queue<pair<int,int>> q;

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (rooms[i][j] == 0) q.push({i, j});

        int dirs[] = {0, 1, 0, -1, 0};
        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            for (int d = 0; d < 4; d++) {
                int nr = r + dirs[d], nc = c + dirs[d+1];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
                if (rooms[nr][nc] != INT_MAX) continue;
                rooms[nr][nc] = rooms[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
};
