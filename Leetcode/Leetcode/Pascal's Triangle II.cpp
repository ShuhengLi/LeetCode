/*
Given an index k, return the kth row of the Pascal's triangle.

For example, given k = 3,
Return [1,3,3,1].

Author: Shuheng Li
*/

class Solution {
public:
	vector<int> getRow(int rowIndex) {
		vector<vector<int>> res;
		vector<int> aaa(1, 1);
		if (rowIndex<1) return aaa;
		for (int i = 0; i <= rowIndex; i++) {
			vector<int> temp(i + 1, 1);
			for (int j = 0; j < i + 1; j++) {
				if (j != 0 && j != i) temp[j] = res[i - 1][j] + res[i - 1][j - 1];
			}
			res.push_back(temp);
		}
		return res[rowIndex];
	}
};