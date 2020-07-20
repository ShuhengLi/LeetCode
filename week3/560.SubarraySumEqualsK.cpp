/*560. Subarray Sum Equals K
 * Given an array of integers and an integer k, you need to find the total number of continuous subarrays whose sum equals to k.

Example 1:

Input:nums = [1,1,1], k = 2
Output: 2
*/
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        if(!n) return 0;
        map<int, int> s;
        s[0] = 1;
        int sum = 0;
        int res = 0;
        for(int i = 0; i < n; i++){
           sum += nums[i];
           if(s.find(sum - k) != s.end()){
               res += s[sum-k];
           }
           s[sum]++;
        }
        return res;
    }
};
