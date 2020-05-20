/*231. Power of Two
 * Given an integer, write a function to determine if it is a power of two.

Example 1:

Input: 1
Output: true
Explanation: 20 = 1
*/
class Solution {
public:
    bool isPowerOfTwo(int n) {
        if(n<=0) return false;
        int res = n&(n-1);
        return res == 0;
    }
};

class Solution {
public:
    bool isPowerOfTwo(int n) {
        if(n <= 0) return false;
        int res = 0;
        unsigned bit = 1;
        for (int i = 0; i < 32; i++){
            int temp = n >> i;
            if(temp & bit){
                res++;
            }
        }
        return res == 1;
    }
};
