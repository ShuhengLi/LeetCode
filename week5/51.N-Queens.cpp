/*51. N-Queens
 * he n-queens puzzle is the problem of placing n queens on an n×n chessboard such that no two queens attack each other.



Given an integer n, return all distinct solutions to the n-queens puzzle.

Each solution contains a distinct board configuration of the n-queens' placement, where 'Q' and '.' both indicate a queen and an empty space respectively.

Example:

Input: 4
Output: [
 [".Q..",  // Solution 1
  "...Q",
  "Q...",
  "..Q."],

 ["..Q.",  // Solution 2
  "Q...",
  "...Q",
  ".Q.."]
]
Explanation: There exist two distinct solutions to the 4-queens puzzle as shown above.
*/
class Solution {
public:
    vector<vector<string>> res;
    int N;
    vector<vector<string>> solveNQueens(int n) {
        N = n;
        vector<string> cur(n, string(n, '.'));
        dfs(0, cur);
        return res;
    }
    void dfs(int level, vector<string> &cur){
        if(level == N){
            res.push_back(cur);
            return;
        }
        for(int i = 0; i < N; i++){
            cur[level][i] = 'Q';
            if(valid(cur, level, i)){
                dfs(level+1, cur);//bug 2: should be level not i
            }
            cur[level][i] = '.';
        }
    }
    bool valid(vector<string>& cur, int level, int index){
        for(int i = 0; i < level; i++){
            if(cur[i][index] == 'Q') return false;
            int diff = abs(level - i);//bug 1: should be level not index
            if(index - diff >= 0){
                if(cur[i][index-diff]=='Q') return false;
            }
            if(index + diff < N){
                if(cur[i][index+diff]=='Q') return false;
            }
        }
        return true;
    }
};
