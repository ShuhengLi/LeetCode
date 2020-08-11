/*78. Subsets
Given a set of distinct integers, nums, return all possible subsets (the power set).

Note: The solution set must not contain duplicate subsets.

Example:

Input: nums = [1,2,3]
Output:
[
  [3],
  [1],
  [2],
  [1,2,3],
  [1,3],
  [2,3],
  [1,2],
  []
]
*/
class Solution {
public:
    struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};
    unordered_set<vector<int>, VectorHash> s;
    //set<vector<int>> s;
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<int> cur;
        dfs(0, cur, nums);
        vector<vector<int>> res(s.begin(), s.end());
        return res;
    }
    void dfs(int level, vector<int>& cur, vector<int> & nums){
        s.insert(cur);
        if(level == nums.size()) return;

        cur.push_back(nums[level]);
        dfs(level+1, cur, nums);
        cur.pop_back();
        dfs(level+1, cur, nums);
    }
};
