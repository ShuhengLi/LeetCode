/*55. Jump Game
 *
 * Given an array of non-negative integers, you are initially positioned at the first index of the array.

Each element in the array represents your maximum jump length at that position.

Determine if you are able to reach the last index.



Example 1:

Input: nums = [2,3,1,1,4]
Output: true
Explanation: Jump 1 step from index 0 to 1, then 3 steps to the last index.
Example 2:

Input: nums = [3,2,1,0,4]
Output: false
Explanation: You will always arrive at index 3 no matter what. Its maximum jump length is 0, which makes it impossible to reach the last index.
*/
//this is will exceed time limit
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if(nums.empty())  return false;
        vector<bool> dp(nums.size(), false);
        dp[0]= true;
        for(int i = 0; i< nums.size(); i++){
            if(!dp[i]) return false;
            if(i == nums.size() - 1) break;
            for(int j = 0; j <= nums[i]; j++){
                dp[i+j] = true;
            }
        }
      return true;
    }
};
//time: O(N^2)


//greedy
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if(nums.empty()) return false;
        int global = 0;
        for(int i = 0; i <= global; i++){
            global = max(nums[i] + i, global);
            if(global >= nums.size()-1) return true;
        }
        return false;
    }
};
