/*88. Merge Sorted Array
 * Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1 as one sorted array.

Note:

The number of elements initialized in nums1 and nums2 are m and n respectively.
You may assume that nums1 has enough space (size that is equal to m + n) to hold additional elements from nums2.
Example:

Input:
nums1 = [1,2,3,0,0,0], m = 3
nums2 = [2,5,6],       n = 3

Output: [1,2,2,3,5,6]
*/
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int last = nums1.size() - 1;
        m--;
        n--;
        for(int i = last; i >= 0; i--){
            int bigger;
            if(m>=0 && n >= 0){
                if(nums1[m]>nums2[n]){
                    bigger = nums1[m--];
                }
                else{
                    bigger = nums2[n--];
                }

            }else if(m>=0){
                bigger = nums1[m--];
            }else{
                bigger = nums2[n--];
            }

            nums1[i] = bigger;
        }

    }
};
