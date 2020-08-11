/*279. Perfect Squares
 * Given a positive integer n, find the least number of perfect square numbers (for example, 1, 4, 9, 16, ...) which sum to n.

Example 1:

Input: n = 12
Output: 3
Explanation: 12 = 4 + 4 + 4.
Example 2:

Input: n = 13
Output: 2
Explanation: 13 = 4 + 9.
*/
class Solution {
public:
    int numSquares(int n) {
        set<int> s;
        vector<int> dp(n+1, INT_MAX);
        for(int i = 1; i<= n; i++){
            int half = sqrt(i);
            s.insert(half * half);
            if(half * half == i) {
                dp[i] = 1;
                continue;
            }
            for(auto it = s.begin(); it != s.end(); it++){
                auto reminder = i % (*it);
                if(reminder == 0){
                    dp[i] = min(dp[i], i / (*it));
                }else{
                    dp[i] = min(dp[i], i / (*it) + dp[reminder]);
                }
            }
        }
        return dp[n];
    }
};
