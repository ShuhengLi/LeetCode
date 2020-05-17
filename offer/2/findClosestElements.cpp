/*658. Find K Closest Elements
 * Given a sorted array arr, two integers k and x, find the k closest elements to x in the array. The result should also be sorted in ascending order. If there is a tie, the smaller elements are always preferred.



Example 1:

Input: arr = [1,2,3,4,5], k = 4, x = 3
Output: [1,2,3,4]
Example 2:

Input: arr = [1,2,3,4,5], k = 4, x = -1
Output: [1,2,3,4]
*/
class Solution {
public:
    vector<int> findClosestElements(vector<int>& nums, int k, int x) {
        if(nums.empty() || k > nums.size()) return nums;
        int l = 0;
        int r = nums.size() - 1;
        vector<int> res;
        while(l + 1 < r){
            int m = l + (r-l) / 2;
            if(nums[m] == x){
                res.push_back(nums[m]);
                k--;
                l = m - 1;
                r = m + 1;
                break;
            }else if (nums[m] < x){
                l = m;
            }else{
                r = m;
            }
        }

        while(l>=0 && r < nums.size() && k){
            if(abs(nums[l]- x) <= abs(nums[r] -x)){ // watch out for the EQUAL
                res.push_back(nums[l--]);
            }else{
                res.push_back(nums[r++]);
            }
            k--;
        }
        while(l>=0 && k){
            res.push_back(nums[l--]);
            k--;
        }
        while(r< nums.size() && k){
            res.push_back(nums[r++]);
            k--;
        }
        sort(res.begin(), res.end());
        return res;
    }
};
