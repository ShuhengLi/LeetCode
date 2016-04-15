/*
Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.

The brackets must close in the correct order, "()" and "()[]{}" are all valid but "(]" and "([)]" are not.

Author: Shuheng Li
*/

class Solution {
private:
	map<char, char> m;
	stack<char> s;
public:
	bool isValid(string t) {
		m['('] = ')';
		m['['] = ']';
		m['{'] = '}';
		int n = t.size();
		if (n == 0) return true;
		if (n % 2 != 0) return false;

		for (int i = 0; i < n; i++) {
			if (m.find(t[i]) != m.end()) {
				s.push(t[i]);
			}
			else {
				if (s.empty() || m[s.top()] != t[i]) return false;

				s.pop();
			}
		}
		return s.empty();
	}
};