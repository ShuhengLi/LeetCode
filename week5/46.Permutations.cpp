/*46. Permutations
 * Given a collection of distinct integers, return all possible permutations.

Example:

Input: [1,2,3]
Output:
[
  [1,2,3],
  [1,3,2],
  [2,1,3],
  [2,3,1],
  [3,1,2],
  [3,2,1]
]
*/
class Solution {
public:
    vector<vector<int>> res;
    vector<vector<int>> permute(vector<int>& nums) {
        dfs(nums, 0);
        return res;
    }
    void dfs(vector<int>& nums, int level){
        if(level == nums.size()){
            res.push_back(nums);
            return;
        }
        for(int i = level; i < nums.size(); i++){
            swap(nums[i], nums[level]);
            dfs(nums, level + 1);
            swap(nums[i], nums[level]);
        }
    }
};
