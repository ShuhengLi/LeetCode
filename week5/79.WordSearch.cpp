/*79. Word Search
 * Given a 2D board and a word, find if the word exists in the grid.

The word can be constructed from letters of sequentially adjacent cell, where "adjacent" cells are those horizontally or vertically neighboring. The same letter cell may not be used more than once.

Example:

board =
[
  ['A','B','C','E'],
  ['S','F','C','S'],
  ['A','D','E','E']
]

Given word = "ABCCED", return true.
Given word = "SEE", return true.
Given word = "ABCB", return false.
*/
class Solution {
public:
    int n;
    int m ;
    bool exist(vector<vector<char>>& board, string word) {
        n = board.size();
        if(!n) return false;
        m = board[0].size();
        if(!m) return false;
        char c = word[0];
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(board[i][j] == c){
                    bool res = dfs(word, 0, board, i, j);
                    if(res) return res;
                }
            }
        }

        return false;
    }
    bool dfs(string s, int index, vector<vector<char>>& b, int x, int y){
        if(index == s.size() ) return true;//bug :before the false check
        if(x < 0 || y < 0 || x > n-1 || y > m -1 ||b[x][y] != s[index]) return false;
        char c=b[x][y];
        b[x][y] = '*';
        if(dfs(s, index + 1, b, x+1, y)|| //bug: use if not variable to save temp value;
          dfs(s, index + 1, b, x-1, y)||
          dfs(s, index + 1, b, x, y+1)||
          dfs(s, index + 1, b, x, y-1))
            return true;
        b[x][y] = c;
        return false;

    }
};
