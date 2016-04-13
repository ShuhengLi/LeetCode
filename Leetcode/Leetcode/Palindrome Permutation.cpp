/*
Given a string, determine if a permutation of the string could form a palindrome.
For example,
"code"->False, "aab"->True, "carerac"->True.
Hint:
Consider the palindromes of odd vs even length.What difference do you notice ?
Count the frequency of each character.
If each character occurs even number of times, then it must be a palindrome.How about character which occurs odd number of times ?

Author: Shuheng Li
Date: 04/11/2016
*/
class Solution
{
public:
	bool canPermutePalindrome(string s) {
		set<char> a;
		for (auto n : s) {
			auto it = a.find(n);
			if (it != a.end()) {
				a.erase(it);
			}
			else {
				a.insert(n);
			}
		}
		return a.size() <= 1;
	}




};