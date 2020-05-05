/*34. Find First and Last Position of Element in Sorted Array
 * Given an array of integers nums sorted in ascending order, find the starting and ending position of a given target value.

Your algorithm's runtime complexity must be in the order of O(log n).

If the target is not found in the array, return [-1, -1].

Example 1:

Input: nums = [5,7,7,8,8,10], target = 8
Output: [3,4]
Example 2:

Input: nums = [5,7,7,8,8,10], target = 6
Output: [-1,-1]
*/
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        vector<int> res(2, -1);
        if(!nums.size()) return res;
        int l = 0, r = nums.size()-1;
        while(l + 1 < r){
            int m = l + (r - l) / 2;
            if(nums[m] < target){
                l = m + 1;
            }else if (nums[m] > target){
                r = m - 1;
            }else{
                r = m;
            }
        }
        if(nums[l] == target){
            res[0] = l;
        }else if (nums[r] == target){
            res[0] = r;
        }
        l = 0; r = nums.size()-1;
        while(l + 1 < r){
            int m = l + (r - l) / 2;
            if(nums[m] < target){
                l = m + 1;
            }else if (nums[m] > target){
                r = m - 1;
            }else{
                l = m;
            }
        }
        if(nums[r] == target){
            res[1] = r;
        }else if (nums[l] == target){
            res[1] = l;
        }
        return res;
    }
};
//Time: O(logN)
//Space: O(1)
