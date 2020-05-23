/*132. Palindrome Partitioning II
Given a string s, partition s such that every substring of the partition is a palindrome.

Return the minimum cuts needed for a palindrome partitioning of s.

Example:

Input: "aab"
Output: 1
Explanation: The palindrome partitioning ["aa","b"] could be produced using 1 cut.
*/
class Solution {
public:
    int minCut(string s) {
        if(s.empty() || isValid(s, 0, s.size()-1)) return 0;
        vector<int> dp(s.size(), INT_MAX);
        for(int i = 0; i < s.size(); i++){
            if(isValid(s, 0, i)){
                dp[i] = 0;
                continue;
            }
            for(int j = 0; j < i; j++){
                if(isValid(s, j+1, i)){
                    int cur = dp[j] + 1;
                    dp[i] = min(dp[i], cur);
                }
            }
        }
        return dp[s.size()-1];
    }
    bool isValid(string& s, int l, int r){
        while(l < r){
            if(s[l++] != s[r--]) return false;
        }
        return true;
    }
};
