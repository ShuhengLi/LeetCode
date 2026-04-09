/*
Write a function that takes an unsigned integer and returns the number of ¡¯1' bits it has (also known as the Hamming weight).

For example, the 32-bit integer ¡¯11' has binary representation 00000000000000000000000000001011, so the function should return 3.

Author: Shuheng Li
*/

class Solution {
public:
	int hammingWeight(uint32_t n) {
		int res = 0;
		uint32_t a = 00000000000000000000000000000001;
		for (int i = 0; i < 32; i++) {
			if (n&a == 1) res++;
			n >>= 1;
		}
		return res;
	}
};

//while(n) n&(n-1) count++