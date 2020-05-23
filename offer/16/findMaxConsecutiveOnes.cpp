/*
485. Max Consecutive Ones
Share
Given a binary array, find the maximum number of consecutive 1s in this array.

Example 1:
Input: [1,1,0,1,1,1]
Output: 3
Explanation: The first two digits or the last three digits are consecutive 1s.
    The maximum number of consecutive 1s is 3.
*/
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        if(nums.empty()) return 0;
        vector<int> dp(nums.size(), 0);
        dp[0] = nums[0];
        int res = dp[0];
        for(int i = 1; i< nums.size(); i++){
            if(nums[i]){
                dp[i] = dp[i-1] + 1;
            }
            res = max(res, dp[i]);
        }
        return res;
    }
};
