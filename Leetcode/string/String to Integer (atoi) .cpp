/*
Implement atoi to convert a string to an integer.

Hint: Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.

Notes: It is intended for this problem to be specified vaguely (ie, no given input specs). You are responsible to gather all the input requirements up front.

Update (2015-02-10):
The signature of the C++ function had been updated. If you still see your function signature accepts a const char * argument, please click the reload button  to reset your code definition.

Author: SHuheng Li
Date: 4/27/2016
*/

class Solution {
public:
	int myAtoi(string str) {
		int sign = 1, base = 0, i = 0;
		while (str[i] == ' ') { i++; }
		if (str[i] == '-' || str[i] == '+') {
			sign = 1 - 2 * (str[i++] == '-');
		}
		while (str[i] >= '0' && str[i] <= '9') {
			if (base >  INT_MAX / 10 || (base == INT_MAX / 10 && str[i] - '0' > 7)) {
				if (sign == 1) return INT_MAX;
				else return INT_MIN;
			}
			base = 10 * base + (str[i++] - '0');
		}
		return base * sign;
	}
};