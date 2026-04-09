/*
Given two sparse matrices A and B, return the result of AB.

You may assume that A's column number is equal to B's row number.

Example:

A = [
[ 1, 0, 0],
[-1, 0, 3]
]

B = [
[ 7, 0, 0 ],
[ 0, 0, 0 ],
[ 0, 0, 1 ]
]


|  1 0 0 |   | 7 0 0 |   |  7 0 0 |
AB = | -1 0 3 | x | 0 0 0 | = | -7 0 3 |
| 0 0 1 |

Author: Shuheng Li
Date: 04/27/2016
*/
class solution{
public:
	vector<vector<int>> multiply(vector<vector<int>>& A, vector<vector<int>>& B) {
		int m = A.size(), n = B.size(), p = B[0].size();
		vector<vector<int>> C(m, vector<int>(p, 0));
		for (int i = 0; i < m; i++)//m是A的行数  
			for (int j = 0; j < n; j++)//n是A的列数和B的行数  
				if (A[i][j])
					for (int k = 0; k < p; k++)//p是B的列数  
						C[i][k] += A[i][j] * B[j][k];
		return C;
	}
}
};