/*152. Maximum Product Subarray
 * Given an integer array nums, find the contiguous subarray within an array (containing at least one number) which has the largest product.

Example 1:

Input: [2,3,-2,4]
Output: 6
Explanation: [2,3] has the largest product 6.
Example 2:

Input: [-2,0,-1]
Output: 0
Explanation: The result cannot be 2, because [-2,-1] is not a subarray.
*/
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        if(nums.empty()) return 0;
        int maxp = 1;
        int minp= 1;
        int res = INT_MIN;
        for(auto n:nums){
            if(n<0) swap(maxp, minp);
            maxp = max(n, maxp*n);
            minp = min(n, minp*n);
            res = max(maxp, res);
        }
        return res;
    }
};