/*
 * 133. Clone Graph
 * Medium
 * [Atlassian DSA Round — ✅ confirmed candidate reports 2024/2025]
 *
 * Given a reference to a node in a connected undirected graph,
 * return a deep copy (clone) of the graph.
 *
 * Each node: int val, vector<Node*> neighbors
 * Node values: 1 to n, all unique.
 *
 * Example:
 *   1 -- 2
 *   |    |
 *   4 -- 3
 *   Input: node 1
 *   Output: deep clone of node 1 (all objects are new, structure identical)
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach: unordered_map<Node*, Node*> + BFS or DFS
 *   Map stores: original node → cloned node (visited check + O(1) lookup)
 *
 * BFS: Queue of original nodes. For each, clone if not yet cloned,
 *      then connect cloned-node's neighbors.
 *
 * DFS: If node already in map → return clone directly (handles cycles).
 *      Else: create clone, put in map, recurse for each neighbor.
 *
 *   Time: O(V + E)   Space: O(V)
 *
 * Atlassian context: Cloning a JIRA workflow graph, copying a Confluence
 * space (pages + links), duplicating a dependency graph.
 */
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node(int v) : val(v) {}
};

class Solution {
public:
    // ── DFS (concise, preferred) ─────────────────────────────────
    unordered_map<Node*, Node*> visited;

    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        if (visited.count(node)) return visited[node];

        Node* clone = new Node(node->val);
        visited[node] = clone;   // register BEFORE recursing (handles cycles)

        for (Node* nb : node->neighbors)
            clone->neighbors.push_back(cloneGraph(nb));

        return clone;
    }

    // ── BFS alternative ─────────────────────────────────────────
    Node* cloneGraphBFS(Node* node) {
        if (!node) return nullptr;
        unordered_map<Node*, Node*> map;
        queue<Node*> q;
        map[node] = new Node(node->val);
        q.push(node);
        while (!q.empty()) {
            Node* cur = q.front(); q.pop();
            for (Node* nb : cur->neighbors) {
                if (!map.count(nb)) {
                    map[nb] = new Node(nb->val);
                    q.push(nb);
                }
                map[cur]->neighbors.push_back(map[nb]);
            }
        }
        return map[node];
    }
};

/*
 * ── Follow-up (Atlassian onsite) ────────────────────────────────
 * Q: Why must you insert into the map BEFORE recursing on neighbors?
 * A: To handle cycles. Without it, A→B→A would infinite-loop.
 *    The map entry acts as a "visited" marker and returns the in-progress clone.
 *
 * Q: Nodes have complex metadata (map<string, any> properties)?
 * A: Deep-copy each property type:
 *    - primitives: direct copy
 *    - strings: copy by value (immutable in most languages)
 *    - nested objects: recursive deep copy
 *    - vectors/lists: copy elements
 *
 * Q: How to verify the clone is truly deep (no shared pointers)?
 * A: BFS both graphs simultaneously. At each step check:
 *    orig_node != clone_node (different addresses)
 *    orig_node->val == clone_node->val
 *    orig_node->neighbors.size() == clone_node->neighbors.size()
 *
 * Q: Confluence Space copy — pages + inter-page links?
 * A: Two-phase approach (same as clone graph but at scale):
 *    Phase 1: Create new page for each original page, build old→new ID map.
 *    Phase 2: Re-write all [[page links]] using the ID map.
 *    → Prevents broken links; scales to millions of pages.
 *
 * Related: LC 138 Copy List with Random Pointer, LC 1490 Clone N-ary Tree
 */
