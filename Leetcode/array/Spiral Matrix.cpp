/*
Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.

For example,
Given the following matrix:

[
[ 1, 2, 3 ],
[ 4, 5, 6 ],
[ 7, 8, 9 ]
]
You should return [1,2,3,6,9,8,7,4,5].

Author: Shuheng Li
*/

class Solution {
public:
	vector<int> spiralOrder(vector<vector<int>>& matrix) {
		vector<int> res;

		int m = matrix.size();
		if (m == 1) return matrix[0];
		if (m == 0) return res;

		int n = matrix[0].size();
		res.resize(m*n, 0);
		int i = 0, j = 0, c = 0, k = 0;

		int total = m * n - 1;

		while (c <= total) {
			i = k;
			while (j < n - k && c <= total) {
				res[c++] = matrix[i][j++];
			}
			j--;
			i++;

			while (i < m - k&& c <= total) {
				res[c++] = matrix[i++][j];
			}
			j--;
			i--;

			while (j >= k&& c <= total) {
				res[c++] = matrix[i][j--];
			}
			j++;
			i--;
			while (i > k&& c <= total) {
				res[c++] = matrix[i--][j];
			}
			j++;
			k++;

		}
		return res;
	}
};