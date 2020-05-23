/*  48. Rotate Image
You are given an n x n 2D matrix representing an image.

Rotate the image by 90 degrees (clockwise).

Note:

You have to rotate the image in-place, which means you have to modify the input 2D matrix directly. DO NOT allocate another 2D matrix and do the rotation.

Example 1:

Given input matrix =
[
  [1,2,3],
  [4,5,6],
  [7,8,9]
],

rotate the input matrix in-place such that it becomes:
[
  [7,4,1],
  [8,5,2],
  [9,6,3]
]
*/
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        if(matrix.size() <= 1) return;
        helper(matrix, 0, matrix.size());
        return;
    }
    void helper(vector<vector<int>>& matrix, int level, int n) {
        if(n <= 1) return;
        for(int i = 0; i< n - 1; i++){
            int tail = n + level - 1;
            int temp = matrix[level][i + level];
            matrix[level][i + level] = matrix[tail - i][level];
            matrix[tail - i][level]  = matrix[tail][tail - i];
            matrix[tail][tail - i]  = matrix[level + i][tail];
            matrix[level + i][tail] = temp;
        }
        helper(matrix, level + 1, n - 2);
    }
};
