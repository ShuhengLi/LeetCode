/*74. Search a 2D Matrix
 *
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
*/
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size();
        if(!m) return false;
        int n = matrix[0].size();
        if(!n) return false;

        int l = 0;
        int r = m * n - 1;
        while( l  <= r){
            int mid = l + ( r - l ) / 2;
            int row = mid / n;
            int col = mid % n;
            if(matrix[row][col] ==  target){
                return true;
            }else if(matrix[row][col] > target){
                r = mid - 1;
            }else{
                l = mid + 1;
            }
        }
        return false;
    }
};
//Time: O(logMN)
//Space: O(1)
