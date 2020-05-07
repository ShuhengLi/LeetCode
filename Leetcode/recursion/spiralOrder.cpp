/*54. Spiral Matrix
 * Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.

Example 1:

Input:
[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
Output: [1,2,3,6,9,8,7,4,5]
Example 2:

Input:
[
  [1, 2, 3, 4],
  [5, 6, 7, 8],
  [9,10,11,12]
]
Output: [1,2,3,4,8,12,11,10,9,5,6,7]
*/
public:
    vector<int> res;
    int nums;
    int g = 0;
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        if(matrix.empty()) return res;
        if(matrix[0].empty()) return res;
        int rows = matrix.size();
        int size = matrix[0].size();
        nums = rows * size;
        spiral(matrix, 0, size, rows, 0);
        return res;

    }
    void spiral(vector<vector<int>>& matrix, int index, int length, int rows, int count){
        if(length == 1) {
            for (int i = 0; i< rows; i++){
                res.push_back(matrix[index + i][index]);
                count++;
            }
           return;

        }
        if(rows == 1){
            for (int i = 0; i< length; i++){
                res.push_back(matrix[index][i + index]);
                count++;
            }
           return;
        }
        for(int i = 0; i < length - 1;i++){
            res.push_back(matrix[index][i + index]);
            count++;
        }
        if(count == nums) return;

        int tail = length - 1;
        for(int i = 0; i < rows - 1;i++){
            res.push_back(matrix[i + index][index + tail]);
            count++;
        }
        if(count == nums) return;
        int end = rows - 1;
        for(int i = length - 1; i > 0 ;i--){
            res.push_back(matrix[index + end][i + index]);
            count++;
        }
        if(count == nums) return;
        for(int i = rows - 1 ; i > 0; i--){
            res.push_back(matrix[i + index][index]);
            count++;
        }
        if(count == nums) return;
        spiral(matrix, index + 1, length -2, rows - 2, count);
        return;
    }
};
//Time: O(N)
//
