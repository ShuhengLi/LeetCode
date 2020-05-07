/*674. Longest Continuous Increasing Subsequence
 * Given an unsorted array of integers, find the length of longest continuous increasing subsequence (subarray).

Example 1:
Input: [1,3,5,4,7]
Output: 3
Explanation: The longest continuous increasing subsequence is [1,3,5], its length is 3.
Even though [1,3,5,7] is also an increasing subsequence, it's not a continuous one where 5 and 7 are separated by 4.
Example 2:
*/
class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        if(nums.empty()) return 0;
        vector<int> a(nums.size(), 0);
        a[0] = 1;
        int global= 1;
        for(int i = 1; i < nums.size(); i++ ){
            if(nums[i] > nums[i-1]){
                a[i] = a[i-1] + 1;
            }else{
                a[i] = 1;
            }
            global = max(a[i], global);
        }
        return global;
    }
};
//O(N)
//
//A[] track the last result 
class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        if(nums.empty()) return 0;
        vector<int> a(nums.size(), 0);
        a[0] = 1;
        int index = 0;
        for(int i = 1; i < nums.size(); i++ ){
            if(nums[i] > nums[i-1]){
                a[i] = max(a[index], i - index + 1); //+1 is important
            }else{
                a[i] = a[i-1];
                index = i;
            }
        }
        return a[nums.size()-1];
    }
};
