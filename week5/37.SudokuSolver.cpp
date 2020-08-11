/*37. Sudoku Solver
 * Write a program to solve a Sudoku puzzle by filling the empty cells.

A sudoku solution must satisfy all of the following rules:

Each of the digits 1-9 must occur exactly once in each row.
Each of the digits 1-9 must occur exactly once in each column.
Each of the the digits 1-9 must occur exactly once in each of the 9 3x3 sub-boxes of the grid.
Empty cells are indicated by the character '.'.

*/
class Solution {
public:
    vector<unordered_set<int>> rows;
    vector<unordered_set<int>> cols;
    vector<unordered_set<int>> blocks;
    void solveSudoku(vector<vector<char>>& board) {
        vector<vector<int>> todo;
        rows.resize(9);
        cols.resize(9);
        blocks.resize(9);
        for(int i = 0; i < 9 ;i++){
            for(int j = 1; j <= 9;j++){
                rows[i].insert(j);
                cols[i].insert(j);
                blocks[i].insert(j);
            }
        }
        for(int i = 0; i < 9 ;i++){
            for(int j = 0; j < 9;j++){
                if(board[i][j]=='.') {
                    todo.push_back({i, j, findBlock(i, j)});
                    continue;
                }
                int val = board[i][j] - '0';
                rows[i].erase(val);
                cols[j].erase(val);
                blocks[findBlock(i, j)].erase(val);
            }
        }
        dfs(todo, 0, board);
    }
    bool dfs(vector<vector<int>>& todo, int level, vector<vector<char>>& b){
        if(level == todo.size()){
            return true;
        }
        int x = todo[level][0];
        int y = todo[level][1];
        int i = todo[level][2];
        set<int> s = getCommon(x, y, i);
        bool res = false;
        if (s.empty()) return false;
        for(auto it = s.begin(); it != s.end();it++){
            int val = *it;
            b[x][y] = val + '0';
            rows[x].erase(val);
            cols[y].erase(val);
            blocks[i].erase(val);
            res = dfs(todo, level + 1, b);
            if(res) return res;
            rows[x].insert(val);
            cols[y].insert(val);
            blocks[i].insert(val);
        }
        return res;

    }

    set<int> getCommon(int x, int y,int i){
        set<int> res;
        for(auto it = rows[x].begin(); it != rows[x].end(); it++){
            if((cols[y].find(*it) != cols[y].end() )&&(blocks[i].find(*it)!= blocks[i].end()) ){
                res.insert(*it);
            }
        }
        return res;
    }

    int findBlock(int i, int j){
        int res = 0;
        if(i <= 2){
            if(j <= 2){
                res = 0;
            }else if(j <= 5){
                res = 1;
            }else{
                res = 2;
            }

        }else if(i <= 5){
            if(j <= 2){
                res = 3;
            }else if(j <= 5){
                res = 4;
            }else{
                res = 5;
            }
        }else{
            if(j <= 2){
                res = 6;
            }else if(j <= 5){
                res = 7;
            }else{
                res = 8;
            }
        }
        return res;
    }
};
