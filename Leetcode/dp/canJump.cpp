/*55. Jump Game
 * Given an array of non-negative integers, you are initially positioned at the first index of the array.

Each element in the array represents your maximum jump length at that position.

Determine if you are able to reach the last index.



Example 1:

Input: nums = [2,3,1,1,4]
Output: true
Explanation: Jump 1 step from index 0 to 1, then 3 steps to the last index.
*/
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if(nums.empty()) return false;
        vector<bool> ans(nums.size(), false);
        ans[0] = true;
        for(int i = 0; i < nums.size(); i++){
            if(ans[i]){
                for(int j = 1; j <= nums[i]; j++){
                    if(i + j >= nums.size() - 1) return true; //check boundry
                    ans[j + i] = true;
                }
            }
        }
        return ans[nums.size() - 1];
    }
};
//Time: O(N^2)
//
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if(nums.empty()) return false;
        int global = 0;
        for(int i = 0; i <= global ; i++){ //update global all the time;
            global = max(global, i + nums[i]);
            if(global >= nums.size() - 1) return true;
        }
        return false;
    }
};
//Time: O(N)
