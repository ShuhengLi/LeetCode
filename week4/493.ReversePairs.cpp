/*493. Reverse Pairs
 * Given an array nums, we call (i, j) an important reverse pair if i < j and nums[i] > 2*nums[j].

You need to return the number of important reverse pairs in the given array.

Example1:

Input: [1,3,2,3,1]
Output: 2
Example2:

Input: [2,4,3,5,1]
Output: 3
*/
class Solution {
public:
    int reversePairs(vector<int>& nums) {
        return mergeSort(nums, 0, nums.size() - 1);
    }
    int mergeSort(vector<int>& nums, int l, int r){
        if(l >= r) return 0;
        int res = 0;
        int m = (l + r) / 2;
        res += mergeSort(nums, l, m);
        res += mergeSort(nums, m + 1, r);
        res += combine(nums, l, m, r);
        return res;
    }
    int combine(vector<int>& nums, int l, int m, int r){
        int i = l, j = m + 1;
        int res = 0;
        while(i <= m && j <= r){
            if((long long)nums[i] > 2 *(long long) nums[j]){
                res += m - i + 1;
                j++;
            }else{
                i++;
            }
        }
        vector<int> a(r-l+1);

        i = l, j = m + 1;
        int k = 0;
        while(i <= m && j <= r){
            if(nums[i] < nums[j]){
                a[k++] = nums[i++];
            }else{
                a[k++] = nums[j++];
            }
        }
        while(i <= m){
            a[k++] = nums[i++];
        }
        while(j <= r){
            a[k++] = nums[j++];
        }
        for(int i = 0; i < a.size(); i++){
            nums[i + l] = a[i];
        }
        return res;
    }
};
