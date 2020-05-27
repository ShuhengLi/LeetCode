/*
133. Clone Graph
Given a reference of a node in a connected undirected graph.

Return a deep copy (clone) of the graph.

Each node in the graph contains a val (int) and a list (List[Node]) of its neighbors.

class Node {
    public int val;
    public List<Node> neighbors;
}
Input: adjList = [[2,4],[1,3],[2,4],[1,3]]
Output: [[2,4],[1,3],[2,4],[1,3]]
Explanation: There are 4 nodes in the graph.
1st node (val = 1)'s neighbors are 2nd node (val = 2) and 4th node (val = 4).
2nd node (val = 2)'s neighbors are 1st node (val = 1) and 3rd node (val = 3).
3rd node (val = 3)'s neighbors are 2nd node (val = 2) and 4th node (val = 4).
4th node (val = 4)'s neighbors are 1st node (val = 1) and 3rd node (val = 3).
*/
class Solution {
public:
    Node* cloneGraph(Node* node) {
        if(!node) return node;
        map<Node*, Node*> m;
        queue<pair<Node*, Node*>> q;
        set<Node*> s;
        Node* newHead = new Node(node->val);
        m[node] = newHead;
        Node* dummy= newHead;
        q.push(make_pair(node, newHead));
        while(!q.empty()){
            auto front_old = q.front().first; 
            if(s.find(front_old) != s.end()){
                q.pop();
                continue;
            }
            auto front_new = q.front().second;
            q.pop();
            s.insert(front_old);
            for(auto c:front_old->neighbors){
                if(m.find(c) == m.end()){
                    Node* newChild = new Node(c->val);
                    m[c] = newChild;
                }
                front_new -> neighbors.push_back(m[c]);
                q.push(make_pair(c, m[c]));
            }
        }
        return dummy;
    }
};
