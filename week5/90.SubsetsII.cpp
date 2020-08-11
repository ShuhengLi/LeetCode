/*90. Subsets II
 * Given a collection of integers that might contain duplicates, nums, return all possible subsets (the power set).

Note: The solution set must not contain duplicate subsets.

Example:

Input: [1,2,2]
Output:
[
  [2],
  [1],
  [1,2,2],
  [2,2],
  [1,2],
  []
]
*/
class Solution {
public:
    void sol(vector<int>& nums,vector<vector<int>>& answer, vector<int>& current, int index){
        if(index==nums.size()){
            answer.push_back(current);
            return;
        }
        current.push_back(nums[index]);
        sol(nums,answer,current,index+1);
        current.pop_back();
        while(index<nums.size()-1 && nums[index]==nums[index+1]) index++;
        sol(nums,answer,current,index+1);
        return;
    }
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        vector<vector<int>> answer;
        vector<int> current;
        sort(nums.begin(),nums.end());
        sol(nums,answer,current,0);
        return answer;
    }
};
