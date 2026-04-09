/*
547. Number of Provinces

There are n cities. Some of them are connected, while some are not. If city a
is connected directly with city b, and city b is connected directly with city c,
then city a is connected indirectly with city c.

A province is a group of directly or indirectly connected cities and no other
cities outside of the group.

You are given an n x n matrix isConnected where isConnected[i][j] = 1 if the
ith city and the jth city are directly connected, and isConnected[i][j] = 0
otherwise.

Return the total number of provinces.

Example 1:
  Input: isConnected = [[1,1,0],[1,1,0],[0,0,1]]
  Output: 2

Example 2:
  Input: isConnected = [[1,0,0],[0,1,0],[0,0,1]]
  Output: 3

Constraints:
  - 1 <= n <= 200
  - n == isConnected.length
  - n == isConnected[i].length
  - isConnected[i][j] is 1 or 0.
  - isConnected[i][i] == 1
  - isConnected[i][j] == isConnected[j][i]
*/
// DFS on adjacency matrix - LinkedIn/Amazon favorite
// Time: O(n^2), Space: O(n)
#include <vector>
using namespace std;

class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size(), count = 0;
        vector<bool> visited(n, false);

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                count++;
                dfs(isConnected, visited, i);
            }
        }
        return count;
    }

    void dfs(vector<vector<int>>& graph, vector<bool>& visited, int node) {
        visited[node] = true;
        for (int j = 0; j < graph.size(); j++)
            if (graph[node][j] && !visited[j])
                dfs(graph, visited, j);
    }
};

// ===================== Alternative: Union-Find =====================
// Count connected components via Union-Find. Start with n components,
// each union reduces count by 1.
// Time: O(n^2 * α(n)) ≈ O(n^2), Space: O(n)
class Solution_v2 {
public:
    vector<int> parent, rank_;

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        return true;
    }

    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size(), count = n;
        parent.resize(n);
        rank_.assign(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;

        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (isConnected[i][j] && unite(i, j))
                    count--;
        return count;
    }
};
