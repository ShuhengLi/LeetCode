/*4. Median of Two Sorted Arrays
 * There are two sorted arrays nums1 and nums2 of size m and n respectively.

Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).

You may assume nums1 and nums2 cannot be both empty.

Example 1:

nums1 = [1, 3]
nums2 = [2]

The median is 2.0
Example 2:

nums1 = [1, 2]
nums2 = [3, 4]

The median is (2 + 3)/2 = 2.5
*/

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {

        int m = nums1.size();
        int n = nums2.size();
        if(m > n) return findMedianSortedArrays(nums2, nums1);
        int imin = 0, imax = m, half = (m + n + 1)/2;
        while(imin <= imax){
            int i = (imin + imax )/ 2;
            int j = half - i;
            if( i < imax && nums2[j - 1] > nums1[i]){
                imin = i + 1;
            }else if( i > imin && nums2[j] < nums1[i-1]){
                imax = i -1;
            }else{
                int leftMax = 0;
                if(i==0){
                    leftMax = nums2[j-1];
                }else if(j == 0){
                    leftMax = nums1[i-1];
                }else{
                    leftMax = max(nums1[i-1], nums2[j-1]);
                }
                if((m + n) % 2) return leftMax;
                int rightMin = 0;
                if(i==m){
                    rightMin = nums2[j];
                }else if(j == n){
                    rightMin = nums1[i];
                }else{
                    rightMin = min(nums1[i], nums2[j]);
                }
                double res = double(leftMax + rightMin)/2;
                return res;
            }
        }
        return 0;
    }
};
