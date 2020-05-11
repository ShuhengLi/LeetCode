/*53. Maximum Subarray
 * Given an integer array nums, find the contiguous subarray (containing at least one number) which has the largest sum and return its sum.

Example:

Input: [-2,1,-3,4,-1,2,1,-5,4],
Output: 6
Explanation: [4,-1,2,1] has the largest sum = 6.
*/
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if(nums.empty())return 0;
        if(nums.size() == 1) return nums[0];
        vector<int> ans(nums.size(), 0);
        ans[0] = nums[0];
        int global = ans[0];
        for(int i = 1; i < nums.size(); i++){
            if(ans[i-1] < 0) {
                ans[i] = max(ans[i-1], nums[i]);
            }else{
                ans[i] = ans[i-1] + nums[i];
            }
            global = max(global, ans[i]);
        }
        return global;
    }
};

//Space O(1) method
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if(nums.empty())return 0;
        if(nums.size() == 1) return nums[0];
        int last = nums[0];
        int global = last;
        for(int i = 1; i < nums.size(); i++){
            if(last < 0) {
                last = max(last, nums[i]);
            }else{
                last = last + nums[i];
            }
            global = max(global, last);
        }
        return global;
    }
};


//return index of max subarray
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if(nums.empty())return 0;
        if(nums.size() == 1) return nums[0];
        int last = nums[0];
        int global = last;
        int start = 0;
        int solu_start = 0;
        int solu_end = 0;
        for(int i = 1; i < nums.size(); i++){
            if(last < 0) {
                last = nums[i];
                start = i;
            }else{
                last = last + nums[i];
            }
            if(last > global){
                global = last;
                solu_start = start;
                solu_end = i;
            }
        }
        return solu_start;
    }
};
