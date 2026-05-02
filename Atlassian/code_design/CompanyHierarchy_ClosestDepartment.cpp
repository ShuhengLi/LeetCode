/*
 * Company Hierarchy — Closest Common Department
 * [Atlassian Custom — confirmed in 2025 P50 interview reports]
 *
 * Given:
 *   - A company hierarchy as a list of (employeeId, managerId) edges.
 *   - A list of N employees.
 * Return the closest common department (i.e., LCA — lowest common ancestor)
 * of all the given employees.
 *
 * Variant in interview: build the hierarchy from raw edges first, then
 * answer queries.
 *
 * Example:
 *      CEO (1)
 *       /  \
 *     VP1   VP2
 *     /  \    \
 *   M1   M2   M3
 *   /     \
 *  E1     E2
 *
 *   Employees: [E1, E2]      -> VP1 (lowest common manager)
 *   Employees: [E1, E2, M3]  -> CEO
 *   Employees: [M1, E1]      -> M1 (E1's parent is M1; M1 is its own ancestor)
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach 1 (no preprocessing): Walk-up + intersection
 *   1. For each employee, walk up to root collecting the chain.
 *   2. Intersect all chains; the deepest common node is the LCA.
 *   Time:  O(N * H) per query, H = tree height
 *   Space: O(N * H)
 *
 * Approach 2 (multi-query): pairwise LCA reduction
 *   - LCA(a, b, c) = LCA(LCA(a,b), c)  ← associative
 *   - With binary lifting / Euler tour + RMQ, single LCA is O(log n) or O(1).
 *
 * For interview, Approach 1 is acceptable; mention Approach 2 for follow-up.
 * ─────────────────────────────────────────────────────────────────
 */
#include <bits/stdc++.h>
using namespace std;

class CompanyHierarchy {
    unordered_map<string, string> parent;   // employeeId -> managerId
    string root;

public:
    // edges = list of (employee, manager); root has manager == ""
    CompanyHierarchy(vector<pair<string,string>> edges) {
        unordered_set<string> hasParent;
        unordered_set<string> all;
        for (auto& [e, m] : edges) {
            parent[e] = m;
            hasParent.insert(e);
            all.insert(e);
            if (!m.empty()) all.insert(m);
        }
        // root = node with no parent
        for (auto& s : all) {
            if (!hasParent.count(s)) { root = s; break; }
        }
    }

    string closestCommonDept(const vector<string>& employees) {
        if (employees.empty()) return "";
        // Build chain (root -> employee) for each
        vector<vector<string>> chains;
        for (auto& e : employees) {
            chains.push_back(chainToRoot(e));
        }

        // LCA = deepest common prefix
        string lca = root;
        int idx = 0;
        while (true) {
            string candidate;
            bool consistent = true;
            for (auto& chain : chains) {
                if (idx >= (int)chain.size()) { consistent = false; break; }
                if (candidate.empty()) candidate = chain[idx];
                else if (chain[idx] != candidate) { consistent = false; break; }
            }
            if (!consistent) break;
            lca = candidate;
            ++idx;
        }
        return lca;
    }

private:
    // Returns chain from root → node (inclusive)
    vector<string> chainToRoot(const string& node) {
        vector<string> path;
        string cur = node;
        while (!cur.empty()) {
            path.push_back(cur);
            auto it = parent.find(cur);
            cur = (it == parent.end()) ? "" : it->second;
        }
        reverse(path.begin(), path.end());   // root → node
        return path;
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: What if the hierarchy has cycles (data error)?
 * A: Detect with visited set during walk-up; throw/raise on cycle.
 *
 * Q: Streaming queries (millions)?
 * A: Preprocess Euler tour + sparse table → O(1) per LCA query.
 *    Or binary lifting → O(log n) with O(n log n) prep.
 *
 * Q: What if the org changes (employee moves teams)?
 * A: Rebuild affected subtree's data structures, or use link-cut tree
 *    (overkill but technically optimal for dynamic trees).
 *
 * Q: Atlassian Jira angle?
 * A: Find the smallest project/team that "owns" a set of issues — same LCA
 *    on the project hierarchy. Permission inheritance = path from leaf to root.
 */

int main() {
    /*       CEO
     *      /   \
     *    VP1   VP2
     *    / \     \
     *   M1 M2   M3
     *   /    \
     *  E1    E2
     */
    vector<pair<string,string>> edges = {
        {"VP1","CEO"}, {"VP2","CEO"},
        {"M1","VP1"}, {"M2","VP1"}, {"M3","VP2"},
        {"E1","M1"},  {"E2","M2"}
    };

    CompanyHierarchy h(edges);
    cout << h.closestCommonDept({"E1","E2"})       << endl;  // VP1
    cout << h.closestCommonDept({"E1","E2","M3"})  << endl;  // CEO
    cout << h.closestCommonDept({"M1","E1"})       << endl;  // M1
    cout << h.closestCommonDept({"E1"})            << endl;  // E1 (single)
    return 0;
}
