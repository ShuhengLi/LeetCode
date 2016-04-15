/*
Given a non-negative number represented as an array of digits, plus one to the number.

The digits are stored such that the most significant digit is at the head of the list.

Author: Shuheng Li
*/

class Solution {
public:
	vector<int> plusOne(vector<int>& digits) {

		int n = digits.size() - 1;
		while (digits[n] == 9 && n >= 0) {
			digits[n] = 0;
			n--;
		}
		if (n == -1) {
			digits.insert(digits.begin(), 1);
		}
		else {
			digits[n] = digits[n] + 1;
		}
		return digits;
	}
};