/*130. Surrounded Regions
Given a 2D board containing 'X' and 'O' (the letter O), capture all regions surrounded by 'X'.

A region is captured by flipping all 'O's into 'X's in that surrounded region.

Example:

X X X X
X O O X
X X O X
X O X X
After running your function, the board should be:

X X X X
X X X X
X X X X
X O X X
*/
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        int n = board.size();
        if(!n) return;
        int m = board[0].size();
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(board[i][j] == 'O'){
                    if(i == 0 || i == n - 1 || j == 0 || j == m-1){
                        board[i][j] = '2';
                    }else{
                        board[i][j] = '1';
                    }
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    while(!q.empty()){
                        int cur_x = q.front().first;
                        int cur_y = q.front().second;q.pop();
                        for(auto d:directions){
                            int x = cur_x + d.first;
                            int y = cur_y + d.second;
                            if(x < 0 || x == n || y < 0 || y == m || board[x][y] != 'O') continue;
                            if(x == 0 || x == n - 1 || y == 0 || y == m-1){
                                board[x][y] = '2';
                            }else{
                                board[x][y] = '1';
                            }
                            q.push({x, y});
                        }
                    }
                }
            }
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(board[i][j] == '2'){
                    board[i][j] = 'O';
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    while(!q.empty()){
                        int cur_x = q.front().first;
                        int cur_y = q.front().second;q.pop();
                        for(auto d:directions){
                            int x = cur_x + d.first;
                            int y = cur_y + d.second;
                            if(x < 0 || x == n || y < 0 || y == m || board[x][y] == 'X'|| board[x][y] == 'O') continue;
                            board[x][y] = 'O';
                            q.push({x, y});
                        }
                    }
                }
            }
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(board[i][j] == '1'){
                    board[i][j] = 'X';
                }
            }
        }

    }
};
