/*994. Rotting Oranges
Share
In a given grid, each cell can have one of three values:

the value 0 representing an empty cell;
the value 1 representing a fresh orange;
the value 2 representing a rotten orange.
Every minute, any fresh orange that is adjacent (4-directionally) to a rotten orange becomes rotten.

Return the minimum number of minutes that must elapse until no cell has a fresh orange.  If this is impossible, return -1 instead.
Input: [[2,1,1],[1,1,0],[0,1,1]]
Output: 4
Example 2:

Input: [[2,1,1],[0,1,1],[1,0,1]]
Output: -1
Explanation:  The orange in the bottom left corner (row 2, column 0) is never rotten, because rotting only happens 4-directionally.
Example 3:

Input: [[0,2]]
Output: 0
Explanation:  Since there are already no fresh oranges at minute 0, the answer is just 0.
*/
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        if(grid.empty() || grid[0].empty()) return -1;
        int rows = grid.size();
        int columns = grid[0].size();
        queue<pair<int, int>> q;
        int res = 0;
        for(int i = 0; i< rows; i++){
            for(int j = 0; j < columns; j++){
                if(grid[i][j] == 2){
                    q.push(make_pair(i,j));
                }
            }
        }
        while(!q.empty()){
            int size = q.size();
            //THIS FLAG!!!!
            bool flag = false;
            for(int s = 0; s < size; s++){
                auto top = q.front(); q.pop();
                int i = top.first;
                int j = top.second;
                if(j - 1 >= 0 &&grid[i][j-1] ==1){
                if(!flag){
                    flag = true;
                    res += 1;
                }
                    grid[i][j-1] = 2;
                    q.push(make_pair(i, j-1));
                }
                if(j + 1 < columns &&grid[i][j+1] ==1){
                if(!flag){
                    flag = true;
                    res += 1;
                }
                    grid[i][j+1] = 2;
                    q.push(make_pair(i, j+1));
                }
                if(i - 1 >= 0 && grid[i-1][j] ==1){
                if(!flag){
                    flag = true;
                    res += 1;
                }
                    grid[i-1][j] = 2;
                    q.push(make_pair(i-1, j));
                }
                if(i + 1 < rows && grid[i+1][j] ==1){
                if(!flag){
                    flag = true;
                    res += 1;
                }
                    grid[i+1][j] = 2;
                    q.push(make_pair(i+1, j));
                }
            }
        }
        for(int i = 0; i< rows; i++){
            for(int j = 0; j < columns; j++){
                if(grid[i][j] == 1){
                    return -1;
                }
            }
        }
        return res;
    }
};
