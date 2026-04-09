/*
133. Clone Graph (DFS approach)

Given a reference of a node in a connected undirected graph, return a deep copy
(clone) of the graph.

Each node in the graph contains a value (int) and a list of its neighbors.

  class Node {
      public int val;
      public List<Node> neighbors;
  }

The given node will always be the first node with val = 1. You must return the
copy of the given node as a reference to the cloned graph.

Example 1:
  Input: adjList = [[2,4],[1,3],[2,4],[1,3]]
  Output: [[2,4],[1,3],[2,4],[1,3]]

Example 2:
  Input: adjList = [[]]
  Output: [[]]

Example 3:
  Input: adjList = []
  Output: []

Constraints:
  - The number of nodes in the graph is in the range [0, 100].
  - 1 <= Node.val <= 100
  - Node.val is unique for each node.
  - There are no repeated edges and no self-loops in the graph.
  - The Graph is connected and all nodes can be visited starting from the given node.
*/
#include <vector>
#include <unordered_map>
using namespace std;

class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node(int _val) : val(_val) {}
};

// DFS with hashmap: clone node, recursively clone neighbors.
// The hashmap serves as both visited set and original->clone mapping.
// Time: O(V+E), Space: O(V)
class Solution {
    unordered_map<Node*, Node*> cloned;
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        if (cloned.count(node)) return cloned[node];

        cloned[node] = new Node(node->val);
        for (Node* nei : node->neighbors)
            cloned[node]->neighbors.push_back(cloneGraph(nei));
        return cloned[node];
    }
};
