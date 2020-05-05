/*191. Number of 1 Bits
 * Write a function that takes an unsigned integer and return the number of '1' bits it has (also known as the Hamming weight).



Example 1:

Input: 00000000000000000000000000001011
Output: 3
Explanation: The input binary string 00000000000000000000000000001011 has a total of three '1' bits.o
*/
class Solution {
public:
    int hammingWeight(uint32_t n) {
        uint32_t bit = 1; //this has to be usigned
        int res=0;
        for(int i = 0; i < 32; i++){
            if (n & bit){
                res++;
            }
            bit = bit << 1; //shift 1 bit every time
        }
        return res;
    }
};
//Time: O(32)
