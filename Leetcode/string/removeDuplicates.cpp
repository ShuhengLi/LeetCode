/*26. Remove Duplicates from Sorted Array
 * Given a sorted array nums, remove the duplicates in-place such that each element appear only once and return the new length.

Do not allocate extra space for another array, you must do this by modifying the input array in-place with O(1) extra memory.

Example 1:

Given nums = [1,1,2],

Your function should return length = 2, with the first two elements of nums being 1 and 2 respectively.

It doesn't matter what you leave beyond the returned length.
*/
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if(nums.empty()) return 0;
        int i = 1;
        for(int j = 1; j < nums.size(); j++){
            if(nums[i-1]!=nums[j]) {
                nums[i++] = nums[j];
            }
        }
        return i;
    }
};
//Time: O(N)
//Space: O(1)
