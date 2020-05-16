/*303. Range Sum Query - Immutable
 *
Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.

Example:
Given nums = [-2, 0, 3, -5, 2, -1]

sumRange(0, 2) -> 1
sumRange(2, 5) -> -1
sumRange(0, 5) -> -3
*/
class NumArray {
public:
    vector<vector<int>> dp;
    NumArray(vector<int>& nums) {
        int size = nums.size();
        init(dp, size);
        assign(nums);
    }

    int sumRange(int i, int j) {
        return dp[i][j];

    }
    void init(vector<vector<int>>& dp, int size){
        vector<int> cur(size, 0);
        for(int i = 0; i< size; i++){
            dp.push_back(cur);
        }
    }
    void assign(vector<int>& nums){
        int sum = 0;
        for(int i = 0; i< nums.size(); i++){
            sum += nums[i];
            dp[0][i] = sum;

        }
        for(int i = 1; i< nums.size(); i++){
            for(int j = i; j< nums.size(); j++){
                dp[i][j] = dp[i-1][j] -  nums[i - 1];
            }
        }
    }
};
