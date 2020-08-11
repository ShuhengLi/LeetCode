/*1091. Shortest Path in Binary Matrix
 * In an N by N square grid, each cell is either empty (0) or blocked (1).

A clear path from top-left to bottom-right has length k if and only if it is composed of cells C_1, C_2, ..., C_k such that:

Adjacent cells C_i and C_{i+1} are connected 8-directionally (ie., they are different and share an edge or corner)
C_1 is at location (0, 0) (ie. has value grid[0][0])
C_k is at location (N-1, N-1) (ie. has value grid[N-1][N-1])
If C_i is located at (r, c), then grid[r][c] is empty (ie. grid[r][c] == 0).
Return the length of the shortest such clear path from top-left to bottom-right.  If such a path does not exist, return -1.
*/
class Solution {
public:
    typedef pair<int, pair<int, int>> p;
    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        int n = grid.size();
        if(!n) return 0;
        vector<vector<int>> dist(n, vector<int>(n, -1));
        vector<vector<bool>> visited(n, vector<bool>(n, false));
        if(grid[0][0] == 1) return dist[n-1][n-1];
        priority_queue<p, vector<p>, greater<p>> q;
        q.push({1, {0, 0}});
        while(!q.empty()){
            auto cur_dist = q.top().first;
            auto x = q.top().second.first;
            auto y = q.top().second.second;q.pop();
            if(visited[x][y]) continue;
            visited[x][y] = true;
            dist[x][y] = cur_dist;
            for(int i = x-1; i <= x + 1; i++){
                for(int j = y-1; j <= y+1; j++){
                    if(i < 0 || i == n || j < 0 || j == n || visited[i][j] || grid[i][j] == 1) continue;
                    q.push({cur_dist + 1, {i, j}});
                }
            }

        }
        return dist[n-1][n-1];
    }
};
