/*785. Is Graph Bipartite?
 * Given an undirected graph, return true if and only if it is bipartite.

Recall that a graph is bipartite if we can split it's set of nodes into two independent subsets A and B such that every edge in the graph has one node in A and another node in B.

The graph is given in the following form: graph[i] is a list of indexes j for which the edge between nodes i and j exists.  Each node is an integer between 0 and graph.length - 1.  There are no self edges or parallel edges: graph[i] does not contain i, and it doesn't contain any element twice.
*/
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        vector<int> flag(graph.size(), -1);
        queue<int> q;
        for(int i = 0; i < graph.size(); i++){
            if(flag[i]!=-1){
                continue;
            }
            flag[i] = 1;
            q.push(i);
            while(!q.empty()){
                int root = q.front(); q.pop();
                for(int j = 0; j < graph[root].size(); j++){
                    int child = graph[root][j];
                    if(flag[child] == -1){
                        flag[child] = 1 - flag[root];
                        q.push(child);
                    }else if(flag[child] == flag[root]){
                        return false;
                    }
                }
            }
        }
        return true;
    }
};
