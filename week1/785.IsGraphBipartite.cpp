/* 785. Is Graph Bipartite?
Given an undirected graph, return true if and only if it is bipartite.

Recall that a graph is bipartite if we can split it's set of nodes into two independent subsets A and B such that every edge in the graph has one node in A and another node in B.

The graph is given in the following form: graph[i] is a list of indexes j for which the edge between nodes i and j exists.  Each node is an integer between 0 and graph.length - 1.  There are no self edges or parallel edges: graph[i] does not contain i, and it doesn't contain any element twice.

Example 1:
Input: [[1,3], [0,2], [1,3], [0,2]]
Output: true
Explanation: 
The graph looks like this:
0----1
|    |
|    |
3----2
We can divide the vertices into two groups: {0, 2} and {1, 3}.
*/
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        if(n <= 1) return true;
        vector<int> visited(n, 0);
        queue<int> q;
        for(int i = 0;i < n; i++){
            if(visited[i] == 0){
                q.push(i);
                visited[i] = 1;
                while(!q.empty()){
                        auto cur = q.front(); q.pop();
                        for(auto child:graph[cur]){
                            if(visited[child] == visited[cur]){
                                return false;
                            }
                            if(visited[child] == 0){
                                visited[child] =  -visited[cur];
                                q.push(child);
                            }
                        }
                }
            }
        }
        return true;
    }
};
