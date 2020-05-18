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
        helper(nums, 0);
        return res;
    }
    void helper(vector<int>& nums, int index){
        if(index == nums.size()-1){
            res.push_back(nums);
            return;
        }
        for(int i = index; i < nums.size(); i++){
            swap(nums[i], nums[index]);
            helper(nums, index + 1);
            swap(nums[i], nums[index]);
        }
    }
};
//dupilicate
class Solution {
public:
    vector<vector<int>> res;
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        helper(nums, 0);
        return res;
    }
    void helper(vector<int>& nums, int index){
        if(index == nums.size()-1){
            res.push_back(nums);
            return;
        }
        set<int> s;
        for(int i = index; i < nums.size(); i++){
            if(s.find(nums[i]) == s.end()){
                swap(nums[i], nums[index]);
                helper(nums, index + 1);
                swap(nums[i], nums[index]);
                s.insert(nums[i]);
            }
        }
    }
};
