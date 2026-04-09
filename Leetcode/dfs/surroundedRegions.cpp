/*
130. Surrounded Regions

Given an m x n matrix board containing 'X' and 'O', capture all regions that
are 4-directionally surrounded by 'X'.

A region is captured by flipping all 'O's into 'X's in that surrounded region.

Example 1:
  Input: board = [["X","X","X","X"],["X","O","O","X"],["X","X","O","X"],["X","O","X","X"]]
  Output: [["X","X","X","X"],["X","X","X","X"],["X","X","X","X"],["X","O","X","X"]]
  Explanation: Notice that an 'O' should not be flipped if:
  - It is on the border, or
  - It is adjacent to an 'O' that should not be flipped.

Example 2:
  Input: board = [["X"]]
  Output: [["X"]]

Constraints:
  - m == board.length
  - n == board[i].length
  - 1 <= m, n <= 200
  - board[i][j] is 'X' or 'O'.
*/
#include <vector>
using namespace std;

// Reverse thinking: instead of finding surrounded regions, find unsurrounded ones.
// DFS from border 'O's to mark them safe ('#'), then flip remaining 'O' -> 'X'.
// Time: O(m*n), Space: O(m*n) recursion stack
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        int m = board.size(), n = board[0].size();

        // Mark border-connected 'O's as safe
        for (int i = 0; i < m; i++) {
            dfs(board, i, 0);
            dfs(board, i, n - 1);
        }
        for (int j = 0; j < n; j++) {
            dfs(board, 0, j);
            dfs(board, m - 1, j);
        }

        // Flip: 'O' -> 'X' (surrounded), '#' -> 'O' (safe)
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'O') board[i][j] = 'X';
                if (board[i][j] == '#') board[i][j] = 'O';
            }
    }

    void dfs(vector<vector<char>>& board, int i, int j) {
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size()) return;
        if (board[i][j] != 'O') return;
        board[i][j] = '#';
        dfs(board, i+1, j);
        dfs(board, i-1, j);
        dfs(board, i, j+1);
        dfs(board, i, j-1);
    }
};
