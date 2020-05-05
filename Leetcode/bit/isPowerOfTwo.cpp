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
        if(n <= 0) return false;
        long long a = n;
        return !(a & a-1);
    }
};

//check reminder
class Solution {
public:
    bool isPowerOfTwo(int n) {
        if(n<=0) return false;
        while(n%2==0) n = n/2;
        return n==1;
    }
};
//count number
class Solution {
public:
    bool isPowerOfTwo(int n) {
        if(n <= 0) return false;
        int res = 0;
        unsigned bit = 1; //this has to be unsigned
        for(int i = 0; i < 32; i++){
            if (n&bit){
                res++;
            }
            bit = bit << 1;
        }
        return res == 1;
    }
};
