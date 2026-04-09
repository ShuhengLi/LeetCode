/*
39. Combination Sum

Given an array of distinct integers candidates and a target integer target,
return a list of all unique combinations of candidates where the chosen numbers
sum to target. You may return the combinations in any order.

The same number may be chosen from candidates an unlimited number of times. Two
combinations are unique if the frequency of at least one of the chosen numbers
is different.

Example 1:
  Input: candidates = [2,3,6,7], target = 7
  Output: [[2,2,3],[7]]

Example 2:
  Input: candidates = [2,3,5], target = 8
  Output: [[2,2,2,2],[2,3,3],[3,5]]

Example 3:
  Input: candidates = [2], target = 1
  Output: []

Constraints:
  - 1 <= candidates.length <= 30
  - 2 <= candidates[i] <= 40
  - All elements of candidates are distinct.
  - 1 <= target <= 40
*/
#include <vector>
#include <algorithm>
using namespace std;

// DFS backtracking. Use same index i (not i+1) to allow reuse of elements.
// Time: O(N^(T/M)), Space: O(T/M) where T=target, M=min candidate
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(candidates, target, 0, path, res);
        return res;
    }

    void dfs(vector<int>& cands, int remain, int start, vector<int>& path, vector<vector<int>>& res) {
        if (remain == 0) { res.push_back(path); return; }
        if (remain < 0) return;

        for (int i = start; i < cands.size(); i++) {
            path.push_back(cands[i]);
            dfs(cands, remain - cands[i], i, path, res); // i, not i+1 (can reuse)
            path.pop_back();
        }
    }
};

// ===================== Improved Solution =====================
// Sort candidates first, then prune early when remain < cands[i].
// Avoids exploring branches that will definitely exceed target.
// Time: O(N^(T/M)), but faster in practice due to pruning
class Solution_v2 {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vector<vector<int>> res;
        vector<int> path;
        dfs(candidates, target, 0, path, res);
        return res;
    }

    void dfs(vector<int>& cands, int remain, int start, vector<int>& path, vector<vector<int>>& res) {
        if (remain == 0) { res.push_back(path); return; }

        for (int i = start; i < cands.size() && cands[i] <= remain; i++) {
            path.push_back(cands[i]);
            dfs(cands, remain - cands[i], i, path, res);
            path.pop_back();
        }
    }
};
