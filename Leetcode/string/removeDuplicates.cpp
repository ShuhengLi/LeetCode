/*Given a sorted array nums, remove the duplicates in-place such that each element appear only once and return the new length.

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
        int j = 1;
        while(j < nums.size()){
            while(j < nums.size() && nums[j] == nums[i - 1]){
                j++;
            }
            if(j == nums.size()) break;
            nums[i++] = nums[j++];
        }
        return i;
        
    }
};
//Time: O(N)
//Space: O(1)
