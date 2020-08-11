/*283. Move Zeroes
 * Given an array nums, write a function to move all 0's to the end of it while maintaining the relative order of the non-zero elements.

Example:

Input: [0,1,0,3,12]
Output: [1,3,12,0,0]
*/
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0;
        for(int i = 0 ; i < nums.size(); i++){
            if(nums[i]){
                nums[slow++] = nums[i];
            }
        }
        for(int i = slow; i < nums.size(); i++){
            nums[i] = 0;
        }

    }
};
