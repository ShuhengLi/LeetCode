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
        if(!x) return x;
        if(!n) return 1;
        bool flag = false;
        if(n < 0){
            flag = true;
        }

        double res = helper(x, n);
        if(flag && res){
            res = 1/res;
        }
        return res;

    }
    double helper(double x, int n){
        if(n == 1 || n== -1){
            return x;
        }

        double half = helper(x, n/2);
        double res = half * half;
        if(n%2 != 0){
            res =res * x;
        }
        return res;
    }
};
//Time: O(logN)
//space: O(1)
//   (x, n)
//   (x, n/2)
//   .....
//   (2)
//   (1)
