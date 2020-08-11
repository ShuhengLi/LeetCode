/*1004. Max Consecutive Ones III
 * Given an array A of 0s and 1s, we may change up to K values from 0 to 1.

Return the length of the longest (contiguous) subarray that contains only 1s.



Example 1:

Input: A = [1,1,1,0,0,0,1,1,1,1,0], K = 2
Output: 6
Explanation:
[1,1,1,0,0,1,1,1,1,1,1]
Bolded numbers were flipped from 0 to 1.  The longest subarray is underlined.
*/
class Solution {
public:
    int longestOnes(vector<int>& a, int k) {
        int slow = 0;
        int res = 0;
        int count = 0;
        for(int i = 0; i < a.size(); i++){
            if(!a[i]){
                if(count < k){
                    count++;
                }else{
                    while(a[slow]!=0) slow++;
                    slow++;
                }
            }
            if(i-slow+1 > res) res = i-slow+1;
            //res = max(res, i - slow + 1);
        }
        return res;
    }
};
