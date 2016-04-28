/*
The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: "PAHNAPLSIIGYIR"
Write the code that will take a string and make this conversion given a number of rows:

string convert(string text, int nRows);
convert("PAYPALISHIRING", 3) should return "PAHNAPLSIIGYIR".
Subscribe to see which companies asked this question

Author: Shuheng Li
Date: 04/27/2016
*/

class Solution {
public:
	string convert(string s, int num) {
		string res;
		int n = s.size();
		if (num == 1) return s;
		for (int i = 0; i < n; i += 2 * (num - 1)) {
			res = res + s[i];
		}

		for (int i = 1; i < num - 1; i++) {
			for (int j = i; j < n; j += 2 * (num - 1)) {
				res = res + s[j];
				int k = j + 2 * (num - i - 1);
				if (k < n) res = res + s[k];
			}
		}
		for (int i = num - 1; i < n; i += 2 * (num - 1)) {
			res = res + s[i];
		}
		return res;
	}
};