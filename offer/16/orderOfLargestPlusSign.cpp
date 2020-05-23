/*764. Largest Plus Sign
In a 2D grid from (0, 0) to (N-1, N-1), every cell contains a 1, except those cells in the given list mines which are 0. What is the largest axis-aligned plus sign of 1s contained in the grid? Return the order of the plus sign. If there is none, return 0.
An "axis-aligned plus sign of 1s of order k" has some center grid[x][y] = 1 along with 4 arms of length k-1 going up, down, left, and right, and made of 1s. This is demonstrated in the diagrams below. Note that there could be 0s or 1s beyond the arms of the plus sign, only the relevant area of the plus sign is checked for 1s.
Examples of Axis-Aligned Plus Signs of Order k:

Order 1:
000
010
000

Order 2:
00000
00100
01110
00100
00000

Order 3:
0000000
0001000
0001000
0111110
0001000
0001000
0000000
Example 1:

Input: N = 5, mines = [[4, 2]]
Output: 2
Explanation:
11111
11111
11111
11111
11011
In the above grid, the largest plus sign can only be order 2.  One of them is marked in bold.
*/
class Solution {
public:
    int size;
    int orderOfLargestPlusSign(int N, vector<vector<int>>& mines) {
        int res = INT_MIN;
        size = N;
        vector<vector<int>> v(N, vector<int>(N, 1)); //this is how to init a 2d array with size
        vector<vector<int>> l2r(N, vector<int>(N, 0)); 
        vector<vector<int>> r2l(N, vector<int>(N, 0));
        vector<vector<int>> t2b(N, vector<int>(N, 0));
        vector<vector<int>> b2t(N, vector<int>(N, 0));
        for(auto mine:mines){
            int i = mine[0];
            int j = mine[1];
            v[i][j] = 0;
        }
        left2right(v, l2r);
        right2left(v, r2l);
        top2bottom(v, t2b);
        bottom2top(v, b2t);
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                auto cur = min(min(l2r[i][j], r2l[i][j]), min(t2b[i][j], b2t[i][j]));
                res = max(res, cur);
            }
        }
        return res;
    }
    void left2right(vector<vector<int>> v, vector<vector<int>>& m){
        for(int i = 0; i<size; i++){
            m[i][0] = v[i][0];
        }
        for(int i = 0; i < size; i++){
            for(int j = 1; j < size; j++){
                if(v[i][j]){
                    m[i][j] = m[i][j-1] + 1;
                }
            }
        }
    }
    void right2left(vector<vector<int>> v, vector<vector<int>>& m){
        for(int i = 0; i<size; i++){
            m[i][size-1] = v[i][size-1];
        }
        for(int i = 0; i < size; i++){
            for(int j = size - 2; j >= 0; j--){
                if(v[i][j]){
                    m[i][j] = m[i][j+1] + 1;
                }
            }
        }
    }

    void top2bottom(vector<vector<int>> v, vector<vector<int>>& m){
        for(int i = 0; i<size; i++){
            m[0][i] = v[0][i];
        }
        for(int i = 0; i < size; i++){
            for(int j = 1; j < size; j++){
                if(v[j][i]){
                    m[j][i] = m[j-1][i] + 1;
                }
            }
        }
    }
    void bottom2top(vector<vector<int>> v, vector<vector<int>>& m){
        for(int i = 0; i<size; i++){
            m[size-1][i] = v[size-1][i];
        }
        for(int i = 0; i < size; i++){
            for(int j = size-2; j >=0; j--){
                if(v[j][i]){
                    m[j][i] = m[j+1][i] + 1;
                }
            }
        }
    }
    void printV(vector<vector<int>> v){
        for(auto cur:v){
            for(auto c:cur){
                cout << " "<< c << " ";
            }
            cout << endl;
        }
    }
};
