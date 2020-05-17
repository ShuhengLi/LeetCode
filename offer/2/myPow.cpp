/*50. Pow(x, n)
 * Implement pow(x, n), which calculates x raised to the power n (xn).

Example 1:

Input: 2.00000, 10
Output: 1024.00000
Example 2:

Input: 2.10000, 3
Output: 9.26100
Example 3:

Input: 2.00000, -2
Output: 0.25000
Explanation: 2-2 = 1/22 = 1/4 = 0.25
*/
class Solution {
public:
    double myPow(double x, int n) {
        if(n == 0) return 1;
        if(x==0) return 0;
        bool flag = n > 0;
        double res = helper(x, n);
        if(!flag) res = 1/ res;
        return res;
    }

    double helper(double x, int n) {
        double res = 0;
        if(n==1||n==-1){
            return x;
        }
        double half = helper(x, n/2);
        res = half * half;
        if(n%2)  res *= x;
        return res;
    }
};
