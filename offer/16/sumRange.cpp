/*
303. Range Sum Query - Immutable
Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.

Example:
Given nums = [-2, 0, 3, -5, 2, -1]

sumRange(0, 2) -> 1
sumRange(2, 5) -> -1
sumRange(0, 5) -> -3
*/
class NumArray {
public:
    vector<int> dp;
    vector<int> nums;
    NumArray(vector<int>& input) {
        nums = input;
        dp.resize(nums.size());
        assign();
    }
    
    int sumRange(int i, int j) {
        return dp[j] - dp[i] + nums[i];
    }

    void assign(){
        int sum = 0;
        for(int i = 0; i< nums.size(); i++){
            sum += nums[i];
            dp[i] = sum;
        }
    }
};
