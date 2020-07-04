/*
133. Clone Graph
Given a reference of a node in a connected undirected graph.

Return a deep copy (clone) of the graph.

Each node in the graph contains a val (int) and a list (List[Node]) of its neighbors.
*/
class Solution {
public:
    Node* cloneGraph(Node* node) {
        if(!node) return NULL;
        map<Node*, Node*> m;
        Node* cur = new Node(node->val);
        m[node] = cur;
        queue<Node*> q;
        q.push(node);
        while(!q.empty()){
            auto temp = q.front(); q.pop();
            auto copy = m[temp];
            for(auto n:temp->neighbors){
                if(m.find(n) != m.end()){
                    copy->neighbors.push_back(m[n]);
                }else{
                    Node* child = new Node(n->val);
                    m[n] = child;
                    copy->neighbors.push_back(child);
                    q.push(n);
                }
            }
        }
        return cur;
    }
};
