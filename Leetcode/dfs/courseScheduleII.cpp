/*
210. Course Schedule II

There are a total of numCourses courses you have to take, labeled from 0 to
numCourses - 1. You are given an array prerequisites where
prerequisites[i] = [ai, bi] indicates that you must take course bi first if
you want to take course ai.

Return the ordering of courses you should take to finish all courses. If there
are many valid answers, return any of them. If it is impossible to finish all
courses, return an empty array.

Example 1:
  Input: numCourses = 2, prerequisites = [[1,0]]
  Output: [0,1]

Example 2:
  Input: numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
  Output: [0,2,1,3] or [0,1,2,3]

Example 3:
  Input: numCourses = 1, prerequisites = []
  Output: [0]

Constraints:
  - 1 <= numCourses <= 2000
  - 0 <= prerequisites.length <= numCourses * (numCourses - 1)
  - prerequisites[i].length == 2
  - 0 <= ai, bi < numCourses
  - ai != bi
  - All the pairs [ai, bi] are distinct.
*/
#include <vector>
#include <queue>
using namespace std;

// DFS topological sort: post-order gives reverse topological order.
// Time: O(V+E), Space: O(V+E)
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses);
        for (auto& p : prerequisites)
            graph[p[1]].push_back(p[0]);

        vector<int> state(numCourses, 0); // 0=unvisited, 1=visiting, 2=done
        vector<int> order;

        for (int i = 0; i < numCourses; i++)
            if (!dfs(graph, state, i, order)) return {};

        reverse(order.begin(), order.end());
        return order;
    }

    bool dfs(vector<vector<int>>& graph, vector<int>& state, int node, vector<int>& order) {
        if (state[node] == 1) return false; // cycle
        if (state[node] == 2) return true;

        state[node] = 1;
        for (int nei : graph[node])
            if (!dfs(graph, state, nei, order)) return false;
        state[node] = 2;
        order.push_back(node);
        return true;
    }
};

// ===================== Alternative: BFS Kahn's Algorithm =====================
// In-degree based topological sort. Produces order directly without reversal.
// Time: O(V+E), Space: O(V+E)
class Solution_v2 {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses);
        vector<int> indegree(numCourses, 0);

        for (auto& p : prerequisites) {
            graph[p[1]].push_back(p[0]);
            indegree[p[0]]++;
        }

        queue<int> q;
        for (int i = 0; i < numCourses; i++)
            if (indegree[i] == 0) q.push(i);

        vector<int> order;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            order.push_back(node);
            for (int nei : graph[node])
                if (--indegree[nei] == 0) q.push(nei);
        }
        return order.size() == numCourses ? order : vector<int>{};
    }
};
