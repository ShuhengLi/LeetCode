/*51. N-Queens
 * Given an integer n, return all distinct solutions to the n-queens puzzle.

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
        vector<int> cur(n, -1);
        N=n;
        solveNQueens(cur, 0);
        return res;
    }
    void solveNQueens(vector<int>& A, int level) {
        if(level == N){
            addRes(A);
            return;
        }
        for(int i = 0; i < N; i++){
            A[level] = i;
            if(valid(A, level)){
                solveNQueens(A, level+1);
            }
        }
    }
    bool valid(vector<int>& A, int level) {
        int cur = A[level];
        for(int i = level - 1; i >= 0; i--){
            int diff = level - i;
            if(A[i] == cur){
                return false;
            }
            if((A[i] + diff < N)&& A[i] + diff == cur){
                return false;
            }
            if((A[i] - diff >= 0)&& A[i] - diff == cur){
                return false;
            }

        }
        return true;
    }
    void addRes(vector<int> A){
        vector<string> cur;
        for(int i = 0; i< N; i++){
            string s(N, '.');
            int index = A[i];
            s[index] = 'Q';
            cur.push_back(s);
        }
        res.push_back(cur);
    }
};
