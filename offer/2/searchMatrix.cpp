/*74. Search a 2D Matrix
 * Write an efficient algorithm that searches for a value in an m x n matrix. This matrix has the following properties:

Integers in each row are sorted from left to right.
The first integer of each row is greater than the last integer of the previous row.
Example 1:

Input:
matrix = [
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]
target = 3
Output: true
Example 2:

Input:
matrix = [
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]
target = 13
Output: false
*/
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if(matrix.empty()||matrix[0].empty()) return false;
        int row = matrix.size();
        int col = matrix[0].size();
        int l = 0;
        int r = row * col - 1;
        while(l <= r){
            int m = l + (r-l)/2;
            int row_index = m / col;
            int col_index = m % col;
            int cur = matrix[row_index][col_index];
            if(cur < target){
                l = m + 1;

            }else if(cur > target){
                r = m -1;
            }else{
                return true;
            }
        }
        return false;
    }
};
