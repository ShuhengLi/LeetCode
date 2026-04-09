/*
Determine if a Sudoku is valid, according to: Sudoku Puzzles - The Rules.

The Sudoku board could be partially filled, where empty cells are filled with the character '.'.


A partially filled sudoku which is valid.
*/

class Solution {
public:
	bool isValidSudoku(vector<vector<char>>& b) {

		for (int i = 0; i < 9; i++) {
			vector<int> row(9, 0);
			for (int j = 0; j < 9; j++) {
				if (b[i][j] == '.') continue;
				int index = b[i][j] - '1';
				if (row[index] != 0) return false;
				else row[index] = 1;
			}
		}

		for (int j = 0; j < 9; j++) {
			vector<int> col(9, 0);
			for (int i = 0; i < 9; i++) {
				if (b[i][j] == '.') continue;
				int index = b[i][j] - '1';
				if (col[index] != 0) return false;
				else col[index] = 1;
			}
		}

		for (int n = 0; n < 9; n++) {
			vector<int> box(9, 0);
			for (int i = n / 3 * 3; i<n / 3 * 3 + 3; i++) {
				for (int j = n % 3 * 3; j < n % 3 * 3 + 3; j++) {
					if (b[i][j] == '.') continue;
					int index = b[i][j] - '1';
					if (box[index] != 0) return false;
					else box[index] = 1;
				}
			}
		}
		return true;
	}
};