/*
Given a string, we can "shift" each of its letter to its successive letter, for example: "abc" -> "bcd". We can keep "shifting" which forms the sequence:

"abc" -> "bcd" -> ... -> "xyz"
Given a list of strings which contains only lowercase alphabets, group all strings that belong to the same shifting sequence.

For example, given: ["abc", "bcd", "acef", "xyz", "az", "ba", "a", "z"],
Return:

[
["abc","bcd","xyz"],
["az","ba"],
["acef"],
["a","z"]
]
*/

class Solution {
public:
	vector<vector<string>> groupStrings(vector<string>& strings) {
		unordered_map<string, vector<string>> mp;
		for (string s : strings) {
			mp[preProcess(s)].push_back(s);
		}
		vector<vector<string>> res;
		for (auto m : mp) {
			vector<string> tmp = m.second;
			sort(tmp.begin(), tmp.end());
			res.push_back(tmp);
		}
		return res;
	}
	string preProcess(string s) {
		string res;
		for (int i = 1; i<s.size(); ++i) {
			int tmp = s[i] - s[i - 1] >= 0 ? s[i] - s[i - 1] : s[i] - s[i - 1] + 26;
			res += 'a' + tmp;
		}
		return res;
	}
};