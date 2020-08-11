/*413. Arithmetic Slices
A sequence of numbers is called arithmetic if it consists of at least three elements and if the difference between any two consecutive elements is the same.

For example, these are arithmetic sequences:

1, 3, 5, 7, 9
7, 7, 7, 7
3, -1, -5, -9
The following sequence is not arithmetic.

1, 1, 2, 5, 7
 
A zero-indexed array A consisting of N numbers is given. A slice of that array is any pair of integers (P, Q) such that 0 <= P < Q < N.

A slice (P, Q) of the array A is called arithmetic if the sequence:
A[P], A[P + 1], ..., A[Q - 1], A[Q] is arithmetic. In particular, this means that P + 1 < Q.

The function should return the number of arithmetic slices in the array A.

 
Example:

A = [1, 2, 3, 4]

return: 3, for 3 arithmetic slices in A: [1, 2, 3], [2, 3, 4] and [1, 2, 3, 4] itself.
*/
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
        int n = A.size();
        if(!n)  return 0;
        vector<int> diff(n, INT_MAX);// bug: make sure default value not 0
        for(int i = 0; i<n - 1;i++){
            diff[i] = A[i+1] - A[i];
        }
        vector<int> sum;
        int count= 1;
        int max = 3;
        for(int i = 0; i< n -1 ;i++){
            if(diff[i] == diff[i+1]){
               count++;
            }else{
                if(count >= 2){
                    sum.push_back(count + 1);
                    if(count + 1 > max) max = count + 1;
                }
                count = 1;
            }
        }

        vector<int> dp(max+1, 1);
        for(int i = 4; i<=max; i++){
            dp[i] = dp[i-1] + (i -2);
        }
        int res=0;
        for(auto i:sum){
            res += dp[i];
        }
        return res;
    }
};
