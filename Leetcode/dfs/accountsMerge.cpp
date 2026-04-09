/*
721. Accounts Merge

Given a list of accounts where each element accounts[i] is a list of strings,
where the first element accounts[i][0] is a name, and the rest of the elements
are emails representing emails of the account.

Now, we would like to merge these accounts. Two accounts definitely belong to
the same person if there is some common email to both accounts. Note that even
if two accounts have the same name, they may belong to different people as
people could have the same name. A person can have any number of accounts
initially, but all of their accounts definitely have the same name.

After merging the accounts, return the accounts in the following format: the
first element of each account is the name, and the rest of the elements are
emails in sorted order. The accounts themselves can be returned in any order.

Example 1:
  Input: accounts = [["John","johnsmith@mail.com","john_newyork@mail.com"],
                     ["John","johnsmith@mail.com","john00@mail.com"],
                     ["Mary","mary@mail.com"],
                     ["John","johnnybravo@mail.com"]]
  Output: [["John","john00@mail.com","john_newyork@mail.com","johnsmith@mail.com"],
           ["Mary","mary@mail.com"],
           ["John","johnnybravo@mail.com"]]

Example 2:
  Input: accounts = [["Gabe","Gabe0@m.co","Gabe3@m.co","Gabe1@m.co"],
                     ["Kevin","Kevin3@m.co","Kevin5@m.co","Kevin0@m.co"],
                     ["Ethan","Ethan5@m.co","Ethan4@m.co","Ethan0@m.co"],
                     ["Hanzo","Hanzo3@m.co","Hanzo1@m.co","Hanzo0@m.co"],
                     ["Fern","Fern5@m.co","Fern1@m.co","Fern0@m.co"],
                     ["Gabe","Gabe0@m.co","Gabe3@m.co","Gabe1@m.co"]]
  Output: [["Ethan","Ethan0@m.co","Ethan4@m.co","Ethan5@m.co"],
           ["Gabe","Gabe0@m.co","Gabe1@m.co","Gabe3@m.co"],
           ["Hanzo","Hanzo0@m.co","Hanzo1@m.co","Hanzo3@m.co"],
           ["Kevin","Kevin0@m.co","Kevin3@m.co","Kevin5@m.co"],
           ["Fern","Fern0@m.co","Fern1@m.co","Fern5@m.co"]]

Constraints:
  - 1 <= accounts.length <= 1000
  - 2 <= accounts[i].length <= 10
  - 1 <= accounts[i][j].length <= 30
  - accounts[i][0] consists of English letters.
  - accounts[i][j] (for j > 0) is a valid email.
*/
// DFS on implicit graph (emails as nodes) - top Meta question
// Time: O(N*K*logNK), Space: O(N*K) where N=accounts, K=max emails
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        // Build adjacency list: email -> all connected emails
        unordered_map<string, vector<string>> graph;
        unordered_map<string, string> emailToName;

        for (auto& acc : accounts) {
            string name = acc[0];
            for (int i = 1; i < acc.size(); i++) {
                emailToName[acc[i]] = name;
                if (i > 1) {
                    graph[acc[1]].push_back(acc[i]);
                    graph[acc[i]].push_back(acc[1]);
                }
            }
        }

        unordered_set<string> visited;
        vector<vector<string>> res;

        for (auto& [email, name] : emailToName) {
            if (visited.count(email)) continue;
            vector<string> component;
            dfs(graph, visited, email, component);
            sort(component.begin(), component.end());
            component.insert(component.begin(), name);
            res.push_back(component);
        }
        return res;
    }

    void dfs(unordered_map<string, vector<string>>& graph, unordered_set<string>& visited,
             const string& email, vector<string>& component) {
        visited.insert(email);
        component.push_back(email);
        for (auto& nei : graph[email])
            if (!visited.count(nei))
                dfs(graph, visited, nei, component);
    }
};

// ===================== Alternative: Union-Find =====================
// Union-Find avoids building explicit graph. Map emails to indices,
// union emails within same account, then group by root.
// Time: O(N*K*α(N*K)) ≈ O(N*K), Space: O(N*K)
class UnionFind {
public:
    vector<int> parent, rank_;
    UnionFind(int n) : parent(n), rank_(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }
};

class Solution_v2 {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        unordered_map<string, int> emailToId;
        unordered_map<string, string> emailToName;
        int id = 0;

        for (auto& acc : accounts)
            for (int i = 1; i < acc.size(); i++) {
                if (!emailToId.count(acc[i])) emailToId[acc[i]] = id++;
                emailToName[acc[i]] = acc[0];
            }

        UnionFind uf(id);
        for (auto& acc : accounts)
            for (int i = 2; i < acc.size(); i++)
                uf.unite(emailToId[acc[1]], emailToId[acc[i]]);

        // Group emails by root
        unordered_map<int, vector<string>> groups;
        for (auto& [email, eid] : emailToId)
            groups[uf.find(eid)].push_back(email);

        vector<vector<string>> res;
        for (auto& [root, emails] : groups) {
            sort(emails.begin(), emails.end());
            emails.insert(emails.begin(), emailToName[emails[0]]);
            res.push_back(emails);
        }
        return res;
    }
};
