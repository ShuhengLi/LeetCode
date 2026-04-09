/*
Determine whether an integer is a palindrome. Do this without extra space.

Author: Shuheng Li
*/

class Solution {
public:
	bool isPalindrome(int x) {
		if (x<0)return false;
		int n = 1;
		int t = x;
		while (t / 10) {
			t = t / 10;
			n = n * 10;
		}
		t = x;
		while (n>1) {
			if (x % 10 != x / n) return false;
			x = x% n;
			x = x / 10;
			n = n / 100;
		}
		return true;
	}
};