/*78. Subsets
 * Given a set of distinct integers, nums, return all possible subsets (the power set).

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
    vector<vector<int>> res;

    vector<vector<int>> subsets(vector<int>& nums) {
        vector<int> current;
        helper(nums,current,  0);
        return res;
    }
    void helper(vector<int>& nums,vector<int>& current,  int index){
        if(index >= nums.size()){
            res.push_back(current);
            return;
        }
        current.push_back(nums[index]);
        helper(nums,current, index+1);
        current.pop_back();
        helper(nums,current, index+1);
    }
};
//              {}
//            a      {}
//         ab   a  b   {}
//      abc ab ac a .....
