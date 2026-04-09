/*
78. Subsets

Given an integer array nums of unique elements, return all possible subsets
(the power set).

The solution set must not contain duplicate subsets. Return the solution in
any order.

Example 1:
  Input: nums = [1,2,3]
  Output: [[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]

Example 2:
  Input: nums = [0]
  Output: [[],[0]]

Constraints:
  - 1 <= nums.length <= 10
  - -10 <= nums[i] <= 10
  - All the numbers of nums are unique.
*/
#include <vector>
using namespace std;

// DFS backtracking: at each level, choose to include or skip elements from start.
// Time: O(2^n * n), Space: O(n)
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(nums, 0, path, res);
        return res;
    }

    void dfs(vector<int>& nums, int start, vector<int>& path, vector<vector<int>>& res) {
        res.push_back(path);
        for (int i = start; i < nums.size(); i++) {
            path.push_back(nums[i]);
            dfs(nums, i + 1, path, res);
            path.pop_back();
        }
    }
};

// ===================== Alternative: Bit Manipulation =====================
// Each subset maps to a bitmask of length n. Iterate all 2^n masks.
// Time: O(2^n * n), Space: O(1) extra (excluding output)
class Solution_v2 {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> res;
        for (int mask = 0; mask < (1 << n); mask++) {
            vector<int> subset;
            for (int i = 0; i < n; i++)
                if (mask & (1 << i)) subset.push_back(nums[i]);
            res.push_back(subset);
        }
        return res;
    }
};
