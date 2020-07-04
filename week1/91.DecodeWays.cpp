/* 91. Decode Ways
A message containing letters from A-Z is being encoded to numbers using the following mapping:

'A' -> 1
'B' -> 2
...
'Z' -> 26
Given a non-empty string containing only digits, determine the total number of ways to decode it.

Example 1:

Input: "12"
Output: 2
Explanation: It could be decoded as "AB" (1 2) or "L" (12).
Example 2:

Input: "226"
Output: 3
Explanation: It could be decoded as "BZ" (2 26), "VF" (22 6), or "BBF" (2 2 6).
*/
class Solution {
public:
    int numDecodings(string s) {
        if(s.empty()) return 0;
        vector<int> dp(s.size() + 1, 1);
        if(s[s.size()-1] - '0' == 0) dp[s.size()-1] = 0;
        for(int i = dp.size() - 3; i>=0; i--){
            if(s[i]-'0'==1|| (s[i]-'0'==2 && s[i+1] -'0' <=6)) {
              dp[i] = dp[i+1] + dp[i+2];
            } else if(s[i]-'0'==0){
                dp[i] = 0;
            }else{
                dp[i] = dp[i+1];
            }
        }
        return dp[0];
    }
};
