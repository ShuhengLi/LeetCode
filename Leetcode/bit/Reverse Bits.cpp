/*
Reverse bits of a given 32 bits unsigned integer.

For example, given input 43261596 (represented in binary as 00000010100101000001111010011100), return 964176192 (represented in binary as 00111001011110000010100101000000).

Follow up:
If this function is called many times, how would you optimize it?

Related problem: Reverse Integer

Credits:
Special thanks to @ts for adding this problem and creating all test cases.

Subscribe to see which companies asked this question

Author: Shuheng Li
*/


class Solution {
public:
	uint32_t reverseBits(uint32_t n) {
		vector<int> s(32, 0);
		uint32_t t = 1;
		for (int i = 0; i <32; i++) {
			if (n&t == 1) { s[i]++; }
			n >>= 1;
		}
		n = 0;
		for (int i = 0; i <= 31; i++) {
			n <<= 1;
			if (s[i] == 1) n = n | 1;
		}
		return n;
	}
};