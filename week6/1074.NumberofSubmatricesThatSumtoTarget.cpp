/*1074. Number of Submatrices That Sum to Target
 * Given a matrix, and a target, return the number of non-empty submatrices that sum to target.

A submatrix x1, y1, x2, y2 is the set of all cells matrix[x][y] with x1 <= x <= x2 and y1 <= y <= y2.

Two submatrices (x1, y1, x2, y2) and (x1', y1', x2', y2') are different if they have some coordinate that is different: for example, if x1 != x1'.



Example 1:

Input: matrix = [[0,1,0],[1,1,1],[0,1,0]], target = 0
Output: 4
Explanation: The four 1x1 submatrices that only contain 0.
Example 2:

Input: matrix = [[1,-1],[-1,1]], target = 0
Output: 5
Explanation: The two 1x2 submatrices, plus the two 2x1 submatrices, plus the 2x2 submatrix.
*/
class Solution {
public:
    int numSubmatrixSumTarget(vector<vector<int>>& matrix, int target) {
        int res = 0;
        if(matrix.empty() || matrix[0].empty()) return res;
        int n = matrix.size();
        int m = matrix[0].size();
        for(int i = 0; i < n; i++){
            vector<int> sum(m, 0);
            for(int j = i; j< n; j++){
                for(int x = 0; x < m; x++){
                    sum[x] += matrix[j][x];
                }
                res += check(sum, target);
            }
        }
        return res;
    }
    int check(vector<int>& dp, int t){
        int res = 0;
        unordered_map<int,int> m;
        m[0] = 1;
        int sum = 0;
        for(auto n:dp){
            sum += n;
            if(m.find(sum - t)!= m.end()) res += m[sum - t];
            m[sum]++;
        }
        return res;

    }

};
/* o(N^4)
class Solution {
public:
    int numSubmatrixSumTarget(vector<vector<int>>& matrix, int target) {
        int res = 0;
        if(matrix.empty() || matrix[0].empty()) return res;
        int n = matrix.size();
        int m = matrix[0].size();
        auto dp = init(matrix);
        for(int i = 0; i < n; i++){
            for(int j = 0; j< m; j++){
                for(int x = i; x < n; x++){
                    for(int y = j; y < m; y++){
                        int sum = get(i, j, x, y, dp);
                        if(sum == target)  res++;
                    }
                }
            }
        }
        return res;

    }
    vector<vector<int>> init(vector<vector<int>>& matrix){
        int n = matrix.size();
        int m = matrix[0].size();
        vector<vector<int>> dp(n, vector<int>(m, 0));
        dp[0][0] = matrix[0][0];
        for(int i = 1; i < m; i++){
            dp[0][i] = dp[0][i-1] + matrix[0][i];
        }
        return dp;
    }
    int get(int i, int j, int x, int y, vector<vector<int>>& dp){
        if(i == 0 && j == 0) return dp[x][y];
        if(i == 0) return dp[x][y] - dp[x][j-1];
        if(j == 0) return dp[x][y] - dp[i-1][y];
        return dp[x][y] - dp[x][j-1] - dp[i-1][y] + dp[i-1][j-1];
    }

};
*/

/*
 * Number of subarrays having sum exactly equal to k
Given an unsorted array of integers, find the number of subarrays having sum exactly equal to a given number k.
Examples:

Input : arr[] = {10, 2, -2, -20, 10},
        k = -10
Output : 3
Subarrays: arr[0...3], arr[1...4], arr[3..4]
have sum exactly equal to -10.

Input : arr[] = {9, 4, 20, 3, 10, 5},
            k = 33
Output : 2
Subarrays : arr[0...2], arr[2...4] have sum
exactly equal to 33.
*/
int findSubarraySum(int arr[], int n, int sum)
{
    unordered_map<int, int> prevSum;
    int res = 0;
    // Sum of elements so far.
    int currsum = 0;

    for (int i = 0; i < n; i++) {
        currsum += arr[i];
        if (currsum == sum)
            res++;
        if (prevSum.find(currsum - sum) != prevSum.end())
            res += (prevSum[currsum - sum]);
        prevSum[currsum]++;
    }

    return res;
}
