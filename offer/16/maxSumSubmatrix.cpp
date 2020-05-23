/*363. Max Sum of Rectangle No Larger Than K
Given a non-empty 2D matrix matrix and an integer k, find the max sum of a rectangle in the matrix such that its sum is no larger than k.

Example:

Input: matrix = [[1,0,1],[0,-2,3]], k = 2
Output: 2 
Explanation: Because the sum of rectangle [[0, 1], [-2, 3]] is 2,
             and 2 is the max number no larger than k (k = 2).
Note:
*/
class Solution {
public:
    int rows;
    int cols;
    vector<vector<int>> sR; 
    vector<vector<int>> sC; 
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        if(matrix.empty() && matrix[0].empty()) return 0;
        int res=INT_MIN;
        int global=INT_MIN;
        rows = matrix.size();
        cols = matrix[0].size();
        sR.resize(rows, vector<int>(cols));
        sC.resize(rows, vector<int>(cols));
        sumRows(matrix, sR);
        sumCols(matrix, sC);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                for(int x = 0; x < cols - j; x++){
                    int sum = 0;
                    for(int y = 0; y < rows - i; y++){
                        sum = sum + sumRowRange(matrix, i + y, j , x);
                        if(sum == k) return k;
                        if(sum <= k){
                            res = max(sum, res);
                        }
                    }
                }
            }
        }
        return res;
    }
    void sumRows(vector<vector<int>>& nums, vector<vector<int>>& m) {
        for(int i = 0; i < rows;  i++){
            m[i][0] = nums[i][0];
        }
        for(int i = 0; i< rows; i++){
            for(int j = 1; j < cols; j++){
                m[i][j] = m[i][j-1] + nums[i][j];
            }
        }
    }
    void sumCols(vector<vector<int>>& nums, vector<vector<int>>& m) {
        for(int i = 0; i < cols;  i++){
            m[0][i] = nums[0][i];
        }
        for(int i = 0; i< cols; i++){
            for(int j = 1; j < rows; j++){
                m[j][i] = m[j-1][i] + nums[j][i];
            }
        }
    }
    int sumRowRange(vector<vector<int>>& nums, int i, int j, int length){
        return sR[i][j+length] - sR[i][j]  + nums[i][j];
    }
};

//Time:O(N^5) 1d prefix


class Solution {
public:
    int rows;
    int cols;
    vector<vector<int>> dp;
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        if(matrix.empty() && matrix[0].empty()) return 0;
        int res=INT_MIN;
        int global=INT_MIN;
        rows = matrix.size();
        cols = matrix[0].size();
        dp.resize(rows, vector<int>(cols));
        sumDp(matrix, dp);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                for(int x = i; x < rows; x++){
                    for(int y = j; y < cols; y++){
                        int sum = sumRowRange(matrix, i, j, x, y);
                        if(sum == k) return k;
                        if(sum <= k){
                            res = max(sum, res);
                        }
                    }
                }
            }
        }
        return res;
    }
    void sumDp(vector<vector<int>>& nums, vector<vector<int>>& m) {
        int sum = 0;
        for(int i = 0; i < rows;  i++){
            sum += nums[i][0];
            m[i][0] = sum;
        }
        sum = 0;
        for(int i = 0; i < cols;  i++){
            sum += nums[0][i];
            m[0][i] = sum;
        }
        for(int i = 1; i< rows; i++){
            for(int j = 1; j < cols; j++){
                m[i][j] = m[i][j-1] + m[i-1][j] + nums[i][j] - m[i-1][j-1];
            }
        }
    }
    int sumRowRange(vector<vector<int>>& nums, int i, int j, int x, int y){
        if(!i && !j) return dp[x][y];
        if(!i) return dp[x][y] - dp[x][j-1];
        if(!j) return dp[x][y] - dp[i-1][y];
        return dp[x][y] - dp[i-1][y] - dp[x][j-1] + dp[i-1][j-1];
    }
};
//Time: O(N^4)??? 2d array




class Solution {
public:
    int rows;
    int cols;
    vector<vector<int>> dp;
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        if(matrix.empty() && matrix[0].empty()) return 0;
        int res=INT_MIN;
        int global=INT_MIN;
        rows = matrix.size();
        cols = matrix[0].size();
        dp.resize(rows, vector<int>(cols));
        sumDp(matrix, dp);
        p(dp);
        for(int i = 0; i < rows; i++){
            for(int j = i; j < rows; j++){
                auto c = combine(i, j);
                int sum = findSumSubArray(c);
                if(sum == k) return k;
                if(sum <= k){
                    res = max(sum, res);
                }
            }
        }
        return res;
    }

    void sumDp(vector<vector<int>>& nums, vector<vector<int>>& m) {
        for(int i = 0; i < cols;  i++){
            m[0][i] = nums[0][i];
        }
        for(int i = 0; i< cols; i++){
            for(int j = 1; j < rows; j++){
                m[j][i] = m[j-1][i] + nums[j][i];
            }
        }
    }
    vector<int> combine(int up, int down){
        if(!up) return dp[down];
        vector<int> res;
        for(int j = 0; j < cols; j++){
            int cur = dp[down][j] - dp[up-1][j];
            res.push_back(cur);
        }
        return res;
    }

    int findSumSubArray(vector<int>& nums){
        if(nums.empty()) return 0;
        int last = nums[0];
        int res = nums[0];
        for(int i = 1; i<nums.size();i++){
            if(last < 0){
                last = nums[i];
            }else{
                last = last + nums[i];
            }
            res = max(res, last);
        }
        return res;
    }

    void p(vector<vector<int>>& nums){
        for(auto n : nums){
            for(auto c:n){
                cout << " " << c << " ";
            }
                cout << endl;
        }
    }
};
//Time:O(N^3)
