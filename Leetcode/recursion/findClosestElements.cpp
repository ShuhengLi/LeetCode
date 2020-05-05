/*658. Find K Closest Elements
 *Given a sorted array, two integers k and x, find the k closest elements to x in the array. The result should also be sorted in ascending order. If there is a tie, the smaller elements are always preferred.

Example 1:
Input: [1,2,3,4,5], k=4, x=3
Output: [1,2,3,4]
Example 2:
Input: [1,2,3,4,5], k=4, x=-1
Output: [1,2,3,4]
*/
class Solution {
public:
    vector<int> findClosestElements(vector<int>& nums, int k, int x) {
        if(nums.empty())return nums;
        if(k > nums.size()) return nums;
        vector<int> res;
        int l = 0;
        int r = nums.size() - 1;
        while(l + 1 < r){
            int m = l + (r - l) / 2;
            if(nums[m] == x){
                l = m;
                r = m + 1;
                break;
            }else if (nums[m] < x){
                l = m;
            }else{
                r = m;
            }
        }
        while(k > 0){
            if(l < 0 && r < nums.size()){
                res.push_back(nums[r++]);
            }else if(l >= 0 && r >= nums.size()){
                res.push_back(nums[l--]);
            }else if(l >= 0 && r < nums.size() && abs(nums[l] - x) <= abs(nums[r] - x)){
                res.push_back(nums[l--]);
            }else if(l >= 0 && r < nums.size() && abs(nums[l] - x) > abs(nums[r] - x)){
                res.push_back(nums[r++]);
            }
            k--;
        }
        sort(res.begin(), res.end());
        return res;
    }
};
//Time: O(logN + K +KlogK)
//space: O(k)
