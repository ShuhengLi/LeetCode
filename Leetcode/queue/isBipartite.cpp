/*785. Is Graph Bipartite?
 * Given an undirected graph, return true if and only if it is bipartite.

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
        int size = graph.size();
        bool even = true;;
        if(!size) return true;
        vector<int> flag(size, -1);
        queue<int> q;

        for(int z = 0; z < size; z++){
            if(flag[z]==-1){
                q.push(z);
                while(!q.empty()){
                    int nextLeve = q.size();
                    for(int i = 0; i < nextLeve; i++){
                            auto node = q.front();q.pop();

                            if((even && flag[node]==1) || (!even&&flag[node]==0)){
                            return false;
                    }
                    flag[node] = even ? 0 : 1;

                    for(int j = 0; j < graph[node].size(); j++){
                        if(flag[graph[node][j]] == -1){
                        q.push(graph[node][j]);
                    }
                }
            }
            even = !even;
        }
            }
        }

        return true;
    }
};

//
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int s = graph.size();
        if(!s) return true;
        vector<bool> visit(s, false);
        vector<int> party(s, 0);
        queue<int> q;
        int index = 0;
        while(index < s){
            if(visit[index]){
                index++;
                continue;
            } 
            q.push(index);
            while(!q.empty()){
                int l = q.size();
                for(int i = 0; i < l; i++){
                    auto front = q.front(); q.pop();
                    visit[front] = true;
                    if(party[front] == 0){
                        party[front] = 1;
                    }
                    for(int j = 0; j < graph[front].size(); j++){
                        int next_index = graph[front][j];
                        if(party[next_index] == party[front]) return false;
                        if(party[next_index] == 0) party[next_index] = party[front] == 1? 2: 1;
                        if(!visit[next_index]){
                            q.push(next_index);
                        }
                    }
                }
            }
            index++;
        }
        return true;
    }
};
