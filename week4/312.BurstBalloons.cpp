/*312. Burst Balloons
 * Given n balloons, indexed from 0 to n-1. Each balloon is painted with a number on it represented by array nums. You are asked to burst all the balloons. If the you burst balloon i you will get nums[left] * nums[i] * nums[right] coins. Here left and right are adjacent indices of i. After the burst, the left and right then becomes adjacent.

Find the maximum coins you can collect by bursting the balloons wisely.
Example:

Input: [3,1,5,8]
Output: 167
Explanation: nums = [3,1,5,8] --> [3,5,8] -->   [3,8]   -->  [8]  --> []
             coins =  3*1*5      +  3*5*8    +  1*3*8      + 1*8*1   = 167
*/
class Solution {
public:
    int maxCoins(vector<int>& nums) {
        int n = nums.size();
        if(!n) return 0;
        nums.insert(nums.begin(), 1);
        nums.push_back(1);

        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
        for(int l = 1; l <= n; l++){
            for(int i = 1; i <= n - l + 1; i++){
                int j = i + l - 1;
                for(int m = i; m <= j; m++){
                    dp[i][j]= max(dp[i][j], dp[i][m-1] + dp[m+1][j] + nums[m]*nums[i-1]*nums[j+1]);
                }
            }
        }
        return dp[1][n];
    }
};

