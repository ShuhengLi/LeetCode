/*
46. Permutations

Given an array nums of distinct integers, return all the possible permutations.
You can return the answer in any order.

Example 1:
  Input: nums = [1,2,3]
  Output: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]

Example 2:
  Input: nums = [0,1]
  Output: [[0,1],[1,0]]

Example 3:
  Input: nums = [1]
  Output: [[1]]

Constraints:
  - 1 <= nums.length <= 6
  - -10 <= nums[i] <= 10
  - All the integers of nums are unique.
*/
#include <vector>
using namespace std;

// DFS backtracking with used array to track which elements are placed.
// Time: O(n! * n), Space: O(n) for used array + recursion stack
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        dfs(nums, used, path, res);
        return res;
    }

    void dfs(vector<int>& nums, vector<bool>& used, vector<int>& path, vector<vector<int>>& res) {
        if (path.size() == nums.size()) {
            res.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;
            used[i] = true;
            path.push_back(nums[i]);
            dfs(nums, used, path, res);
            path.pop_back();
            used[i] = false;
        }
    }
};

// ===================== Improved Solution =====================
// Swap-based approach: no extra used array or path needed.
// Swap element into position, recurse, swap back.
// Time: O(n! * n), Space: O(n) recursion stack only
class Solution_v2 {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        dfs(nums, 0, res);
        return res;
    }

    void dfs(vector<int>& nums, int start, vector<vector<int>>& res) {
        if (start == nums.size()) { res.push_back(nums); return; }

        for (int i = start; i < nums.size(); i++) {
            swap(nums[start], nums[i]);
            dfs(nums, start + 1, res);
            swap(nums[start], nums[i]);
        }
    }
};
