/*976. Largest Perimeter Triangle
 * Given an array A of positive lengths, return the largest perimeter of a triangle with non-zero area, formed from 3 of these lengths.

If it is impossible to form any triangle of non-zero area, return 0.



Example 1:

Input: [2,1,2]
Output: 5
Example 2:

Input: [1,2,1]
Output: 0
*/
class Solution {
public:
    int largestPerimeter(vector<int>& A) {
        int res = 0;
        if(A.empty() || A.size() < 3) return 0;
        sort(A.begin(), A.end());
        res = A[A.size()-1] + A[A.size()-2];
        for (int i = A.size() - 3; i >= 0; i--){
            res += A[i];
            if(A[i] + A[i+1] > A[i+2]){
                return res;
            }else{
                res -= A[i+2];
            }
        }
        return 0;
    }
};
