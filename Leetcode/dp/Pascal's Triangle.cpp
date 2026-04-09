/*Given numRows, generate the first numRows of Pascal's triangle.

For example, given numRows = 5,
Return

[
     [1],
    [1,1],
   [1,2,1],
  [1,3,3,1],
 [1,4,6,4,1]
]

Author: SHuheng Li
*/
class Solution {
public:
	vector<vector<int>> generate(int numRows) {
		vector<vector<int>> res;
		if (numRows<1) return res;
		for (int i = 0; i < numRows; i++) {
			vector<int> temp(i + 1, 1);
			for (int j = 0; j < i + 1; j++) {
				if (j != 0 && j != i) temp[j] = res[i - 1][j] + res[i - 1][j - 1];
			}
			res.push_back(temp);
		}
		return res;
	}
};