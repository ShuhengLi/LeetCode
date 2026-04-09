/*
Reverse digits of an integer.

Example1: x = 123, return 321
Example2: x = -123, return -321

click to show spoilers.

Subscribe to see which companies asked this question

Show Tags
Show Similar Problems

Author: Shuheng Li
Date: 04/27/2016
*/

class Solution {
public:
	int reverse(int x) {
		long long res = 0;
		while (x != 0) {
			res = res * 10 + x % 10;
			x = x / 10;
		}
		if (res> INT_MAX || res < INT_MIN) return 0;

		return res;
	}
};