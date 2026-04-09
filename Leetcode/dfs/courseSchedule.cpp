/*
207. Course Schedule

There are a total of numCourses courses you have to take, labeled from 0 to
numCourses - 1. You are given an array prerequisites where
prerequisites[i] = [ai, bi] indicates that you must take course bi first if
you want to take course ai.

Return true if you can finish all courses. Otherwise, return false.

Example 1:
  Input: numCourses = 2, prerequisites = [[1,0]]
  Output: true
  Explanation: There are 2 courses to take. To take course 1 you should have
  finished course 0. So it is possible.

Example 2:
  Input: numCourses = 2, prerequisites = [[1,0],[0,1]]
  Output: false
  Explanation: There are 2 courses to take. To take course 1 you should have
  finished course 0, and to take course 0 you should also have finished
  course 1. So it is impossible.

Constraints:
  - 1 <= numCourses <= 2000
  - 0 <= prerequisites.length <= 5000
  - prerequisites[i].length == 2
  - 0 <= ai, bi < numCourses
  - All the pairs prerequisites[i] are unique.
*/
#include <vector>
#include <queue>
using namespace std;

// DFS cycle detection using 3-color marking (white/gray/black).
// If we encounter a gray node during DFS, there's a cycle.
// Time: O(V+E), Space: O(V+E)
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses);
        for (auto& p : prerequisites)
            graph[p[1]].push_back(p[0]);

        // 0 = unvisited, 1 = in current path, 2 = done
        vector<int> state(numCourses, 0);

        for (int i = 0; i < numCourses; i++)
            if (hasCycle(graph, state, i)) return false;
        return true;
    }

    bool hasCycle(vector<vector<int>>& graph, vector<int>& state, int node) {
        if (state[node] == 1) return true;   // cycle
        if (state[node] == 2) return false;  // already processed

        state[node] = 1;
        for (int nei : graph[node])
            if (hasCycle(graph, state, nei)) return true;
        state[node] = 2;
        return false;
    }
};

// ===================== Alternative: BFS Kahn's Algorithm =====================
// Topological sort via in-degree. If we can process all nodes, no cycle exists.
// Some interviewers prefer this iterative approach over recursive DFS.
// Time: O(V+E), Space: O(V+E)
class Solution_v2 {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses);
        vector<int> indegree(numCourses, 0);

        for (auto& p : prerequisites) {
            graph[p[1]].push_back(p[0]);
            indegree[p[0]]++;
        }

        queue<int> q;
        for (int i = 0; i < numCourses; i++)
            if (indegree[i] == 0) q.push(i);

        int processed = 0;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            processed++;
            for (int nei : graph[node])
                if (--indegree[nei] == 0) q.push(nei);
        }
        return processed == numCourses;
    }
};
