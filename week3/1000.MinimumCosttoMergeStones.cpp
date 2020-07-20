/*1000. Minimum Cost to Merge Stones
 * There are N piles of stones arranged in a row.  The i-th pile has stones[i] stones.

A move consists of merging exactly K consecutive piles into one pile, and the cost of this move is equal to the total number of stones in these K piles.

Find the minimum cost to merge all piles of stones into one pile.  If it is impossible, return -1.



Example 1:

Input: stones = [3,2,4,1], K = 2
Output: 20
Explanation:
We start with [3, 2, 4, 1].
We merge [3, 2] for a cost of 5, and we are left with [5, 4, 1].
We merge [4, 1] for a cost of 5, and we are left with [5, 5].
We merge [5, 5] for a cost of 10, and we are left with [10].
The total cost was 20, and this is the minimum possible.
Example 2:

Input: stones = [3,2,4,1], K = 3
Output: -1
Explanation: After any merge operation, there are 2 piles left, and we can't merge anymore.  So the task is impossible.
*/
class Solution {
public:
    int mergeStones(vector<int>& stones, int K) {
        int n = stones.size();
        if((n-1) % (K-1) != 0) return -1;

        vector<int> sum(n+1, 0);
        for(int i = 0; i < n;i++){
            sum[i+1] = sum[i] + stones[i];
        }
        vector<vector<int>> dp(n, vector<int>(n, INT_MAX));
        for(int i = 0; i < n; i++){
            dp[i][i] = 0;
        }
        for(int l = 2; l <= n; l++){
            for(int i = 0; i <= n - l; i++){
                int j = i + l - 1;
                for(int m = i; m < j; m += K-1){
                    dp[i][j] = min(dp[i][j], dp[i][m] + dp[m+1][j]);
                }
                if((l - 1) % (K-1) == 0){
                    dp[i][j] += sum[j+1]-sum[i];
                }
            }
        }
        return dp[0][n-1];
    }
};
