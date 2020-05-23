/*
59. Spiral Matrix II
Given a positive integer n, generate a square matrix filled with elements from 1 to n2 in spiral order.

Example:

Input: 3
Output:
[
 [ 1, 2, 3 ],
 [ 8, 9, 4 ],
 [ 7, 6, 5 ]
]
*/
class Solution {
public:
    vector<vector<int>> res;
    vector<vector<int>> generateMatrix(int n) {
        res.resize(n, vector<int>(n));
        helper(0, 1, n);
        return res;
    }
    void helper(int row, int start, int n){
        if(n==0) return;
        if(n == 1){
            res[row][row] = start;
            return;
        }
        for(int i  = 0; i < n - 1; i++){
            res[row][i + row] = start;
            start++;
        }
        int tail = row + n - 1;
        for(int i  = 0; i < n - 1; i++){
            res[i + row][tail] = start;
            start++;
        }
        for(int i  = n - 1; i > 0; i--){
            res[tail][i + row] = start;
            start++;
        }
        for(int i  = n - 1; i > 0; i--){
            res[i + row][row] = start;
            start++;
        }
        helper(row + 1, start, n - 2);
    }
};
