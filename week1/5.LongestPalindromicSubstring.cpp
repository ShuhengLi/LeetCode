/* 5. Longest Palindromic Substring
Given a string s, find the longest palindromic substring in s. You may assume that the maximum length of s is 1000.

Example 1:

Input: "babad"
Output: "bab"
Note: "aba" is also a valid answer.
Example 2:

Input: "cbbd"
Output: "bb"
*/
//dp
class Solution {
public:
    string longestPalindrome(string s) {
        if(s.size() <= 1) return s;
        vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), false));
        int g_l = 0;
        int g_s = 0;
        for(int i = s.size() - 1; i >= 0 ; i--){
            for(int j = i; j < s.size(); j++){
                //base case;
                if(j == i || (j - i == 1 && s[j] == s[i])){
                    dp[i][j] = true;
                }else if(s[j] == s[i]&&dp[i + 1][j - 1]){ //gernal case
                    dp[i][j] = true;
                }
                //how to update max length
                if(dp[i][j] && j - i + 1 > g_l){
                    g_l = j - i + 1;
                    g_s = i;
                }
            }
        }
        return s.substr(g_s, g_l);
    }
};
//iterate
class Solution {
public:
    string longestPalindrome(string s) {
        if(s.size() <= 1) return s;
        string res;
        int start = 0;
        int len = 0;
        int i = 0;
        while(i < s.size()){
            int l = i, r = i;
            while(r < s.size() && s[l] == s[r]){
                r++;
            }
            i = r;
            r--;
            while(l >= 0 && r < s.size() && s[l] == s[r]){
                l--;
                r++;
            }
            int cur_len = r-l-1;
            if(cur_len > len){
                len = cur_len;
                start = l + 1;
            }
        }
        return s.substr(start, len);
    }
};
