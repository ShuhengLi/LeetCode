/*
Given a positive integer, return its corresponding column title as appear in an Excel sheet.

For example:

1 -> A
2 -> B
3 -> C
...
26 -> Z
27 -> AA
28 -> AB
Credits:
Special thanks to @ifanchu for adding this problem and creating all test cases.

Subscribe to see which companies asked this question

Author: SHuheng Li
Date: 04/27/2016
*/

class Solution {
public:
	string convertToTitle(int n) {
		string res;
		while (n != 0) {
			n = n - 1;
			char t = n % 26 + 'A';
			n = n / 26;
			res = t + res;
		}
		return res;
	}
};