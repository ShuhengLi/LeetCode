/*300. Longest Increasing Subsequence
Given an unsorted array of integers, find the length of longest increasing subsequence.

Example:

Input: [10,9,2,5,3,7,101,18]
Output: 4 
Explanation: The longest increasing subsequence is [2,3,7,101], therefore the length is 4. 
*/.
//O(n^2)
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size();
        if(!n) return 0;
        vector<int> dp(n, 1);
        int res = 1;
        for(int i = 1; i < n;i++){
            for(int j = 0; j < i ;j++){
               if(nums[i] > nums[j]) {
                   dp[i] = max(dp[i], dp[j] + 1);
               }
            }
            res = max(res, dp[i]);
        }
        return res;

    }
};

//O(NlogN)
class Solution {
  public:
    int lengthOfLIS(vector<int>& nums) {
    vector<int> res;
    for(int i=0; i<nums.size(); i++) {
        auto it = std::lower_bound(res.begin(), res.end(), nums[i]);
        if(it==res.end()) res.push_back(nums[i]);
        else *it = nums[i];
    }
    return res.size();
}
