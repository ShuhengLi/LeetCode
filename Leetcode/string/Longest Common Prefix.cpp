/*
Write a function to find the longest common prefix string amongst an array of strings.

Author: Shuheng Li
*/

class Solution {
public:
	string longestCommonPrefix(vector<string>& strs) {
		string res;
		int n = strs.size();
		if (n<1) return res;

		sort(strs.begin(), strs.end(), my);
		for (int i = 0; i < strs[0].size(); i++) {
			res = res + strs[0][i];
			for (int j = 1; j < n; j++) {
				if (res != strs[j].substr(0, i + 1)) {
					res.pop_back();
					return res;
				}
			}
			if (i == strs[0].size() - 1) return res;
		}

	}fdas

	static bool my(string s1, string s2) {
		return s1.size() < s2.size();
	}
};