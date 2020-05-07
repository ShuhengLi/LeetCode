/*343. Integer Break
 * Given a positive integer n, break it into the sum of at least two positive integers and maximize the product of those integers. Return the maximum product you can get.

Example 1:

Input: 2
Output: 1
Explanation: 2 = 1 + 1, 1 × 1 = 1.
Example 2:

Input: 10
Output: 36
Explanation: 10 = 3 + 3 + 4, 3 × 3 × 4 = 36.
*/
class Solution {
public:
    int integerBreak(int n) {
        vector<int> a(n+1, 0);
        a[1] = 1;
        for(int i = 2; i <= n; i ++){
            int global = 0;
            int half = i /2;
            for(int j = 1; j<= half; j++){
                global = max(max(j, a[j]) * max(i - j , a[i - j]), global);
            }
            a[i] = global;
        }
        return a[n];
    }
};//Time: O(N^2)
