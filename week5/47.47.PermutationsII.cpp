/*47. Permutations II
 * Given a collection of numbers that might contain duplicates, return all possible unique permutations.

Example:

Input: [1,1,2]
Output:
[
  [1,1,2],
  [1,2,1],
  [2,1,1]
]
*/
class Solution {
public:
    vector<vector<int>> res;
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        dfs(nums, 0);
        return res;
    }
    void dfs(vector<int>& nums, int level){
        if(level == nums.size()){
            res.push_back(nums);
            return;
        }
        set<int> s;
        for(int i = level; i < nums.size(); i++){
            if(s.find(nums[i]) == s.end()){
                swap(nums[i], nums[level]);
                dfs(nums, level + 1);
                swap(nums[i], nums[level]);
            }
            s.insert(nums[i]);
        }
    }
};
