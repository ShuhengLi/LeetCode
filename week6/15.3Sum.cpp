/*15. 3Sum
 * Given an array nums of n integers, are there elements a, b, c in nums such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.

Note:

The solution set must not contain duplicate triplets.

Example:

Given array nums = [-1, 0, 1, 2, -1, -4],

A solution set is:
[
  [-1, 0, 1],
  [-1, -1, 2]
]
*/
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        set<vector<int>> s;
        vector<vector<int>> res;
        if(nums.size() < 3) return res;
        sort(nums.begin(), nums.end());
        for(int i = 1; i< nums.size() - 1; i++){
            int l = 0;
            int r = nums.size()  - 1;
            while(l < i && r > i){
                int val = nums[i] + nums[l] + nums[r];
                if(val == 0){
                    vector<int> cur = {nums[l], nums[i], nums[r]};
                    s.insert(cur);
                    l++;
                    r--;
                }else if (val > 0){
                    r--;
                }else{
                    l++;
                }
            }
        }
        vector<vector<int>> res2(s.begin(), s.end());
        return res2;
    }
};
