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
    int longestOnes(vector<int>& A, int K) {
        int s = 0;
        int count = 0;
        int res = INT_MIN;
        for(int i = 0; i < A.size(); i++){
            if(!A[i]){
                if(count < K) {
                    count++;
                }else{
                    while(s < A.size() &&A[s] == 1){
                        s++;
                    }
                    s++;
                    if(s >= A.size()) break;
                }
            }
            int temp = i - s + 1;
            res = max(res, temp);
        }
        return res;
    }
};
