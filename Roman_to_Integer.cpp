/*
Given a roman numeral, convert it to an integer.
Input is guaranteed to be within the range from 1 to 3999.

Author: Shuheng Li	
*/
class Solution {
public:
	int romanToInt(string s) {
		int idx[256];
		idx['I'] = 1; idx['V'] = 5; idx['X'] = 10; idx['L'] = 50; idx['C'] = 100; idx['D'] = 500; idx['M'] = 1000;
		int n = s.size();
		if (n < 1) return 0;
		int res = idx[s[0]];
		for (int i = 1; i < n; i++) {
			res = res + idx[s[i]];
			if (idx[s[i - 1]] < idx[s[i]]) {
				res = res - 2 * idx[s[i - 1]];
			}
		}
		return res;
	}
};
