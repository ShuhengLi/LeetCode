/*16. 3Sum Closest
 * Given an array nums of n integers and an integer target, find three integers in nums such that the sum is closest to target. Return the sum of the three integers. You may assume that each input would have exactly one solution.



Example 1:

Input: nums = [-1,2,1,-4], target = 1
Output: 2
Explanation: The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
*/
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        int diff = INT_MAX;
        int res;
        for(int i = 0; i < nums.size()-2;i++){
            int l = i + 1;
            int r = nums.size()-1;
            while(l < r){
                int val = nums[i] + nums[l] + nums[r];
                if(val == target){
                    return target;
                }else if(val < target){
                    if(diff > target - val){
                        diff = target - val;
                        res = val;
                    }
                    l++;
                }else{
                    if(diff > val - target ){
                        diff = val - target;
                        res = val;
                    }
                    r--;
                }
            }
        }
        return res;

    }
};
