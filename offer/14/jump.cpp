/*45. Jump Game II
 *Given an array of non-negative integers, you are initially positioned at the first index of the array.

Each element in the array represents your maximum jump length at that position.

Your goal is to reach the last index in the minimum number of jumps.

Example:

Input: [2,3,1,1,4]
Output: 2
Explanation: The minimum number of jumps to reach the last index is 2.
    Jump 1 step from index 0 to 1, then 3 steps to the last index.
    */
class Solution {
public:
    int jump(vector<int>& nums) {
        if(nums.empty()) return 1;
        vector<int> dp(nums.size(), INT_MAX);
        dp[0] = 0;
        for(int i = 0; i <= nums.size(); i++){
            for(int j = 1; j <= nums[i]; j++){
                if(i + j > nums.size()-1) break;
                dp[i+j] = min(dp[i+j], dp[i]+1);
                if(i + j == nums.size()-1) return dp[nums.size()-1];
            }
        }
        return dp[nums.size()-1];
    }
};
