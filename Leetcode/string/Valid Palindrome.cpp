/*
Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.

For example,
"A man, a plan, a canal: Panama" is a palindrome.
"race a car" is not a palindrome.

Note:
Have you consider that the string might be empty? This is a good question to ask during an interview.

For the purpose of this problem, we define empty string as valid palindrome.

Subscribe to see which companies asked this question

Show Tags
Show Similar Problems

Author: Shuheng Li
Date: 04/27/2016
*/

class Solution {
public:
	bool isPalindrome(string s) {
		int k = 0;
		for (int i = 0; i < s.size(); i++) {
			if (isalnum(s[i])) s[k++] = toupper(s[i]);
		}
		int l = 0, r = k - 1;
		for (int i = 0; i < k; i++) {
			if (s[i] != s[k - 1 - i]) return false;
		}
		return true;
	}
};