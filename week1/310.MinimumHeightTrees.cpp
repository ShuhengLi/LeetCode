/* 310. Minimum Height Trees
For an undirected graph with tree characteristics, we can choose any node as the root. The result graph is then a rooted tree. Among all possible rooted trees, those with minimum height are called minimum height trees (MHTs). Given such a graph, write a function to find all the MHTs and return a list of their root labels.

Format
The graph contains n nodes which are labeled from 0 to n - 1. You will be given the number n and a list of undirected edges (each edge is a pair of labels).

You can assume that no duplicate edges will appear in edges. Since all edges are undirected, [0, 1] is the same as [1, 0] and thus will not appear together in edges.

Example 1 :

Input: n = 4, edges = [[1, 0], [1, 2], [1, 3]]

        0
        |
        1
       / \
      2   3 

Output: [1]
Example 2 :

Input: n = 6, edges = [[0, 3], [1, 3], [2, 3], [4, 3], [5, 4]]

     0  1  2
      \ | /
        3
        |
        4
        |
        5 

Output: [3, 4]
*/
class Solution {
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        vector<int> res;
        if(!n) return res;
        if (n == 1) {
            res.push_back(0);
            return res;
        }
        vector<set<int>> adj(n);
        for (auto p : edges) {
            adj[p[0]].insert(p[1]);
            adj[p[1]].insert(p[0]);
        }
        queue<int> q;
        for (int i = 0; i < adj.size(); ++i) {
            if (adj[i].size() == 1) {
                q.push(i);
            }
        }
        //bfs
        while(n>2){
            int leafs = q.size();
            n = n - leafs;
            for(int i = 0; i< leafs;i++){
                auto cur = q.front(); q.pop();
                for(auto s :adj[cur]){
                    adj[s].erase(cur);
                    if(adj[s].size() == 1) q.push(s);
                }
            }
        }
        while (!q.empty()) {
            res.push_back(q.front());
            q.pop();
        }
        return res;
    }
};
