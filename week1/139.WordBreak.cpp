/* 139. Word Break
Given a non-empty string s and a dictionary wordDict containing a list of non-empty words, determine if s can be segmented into a space-separated sequence of one or more dictionary words.

Note:

The same word in the dictionary may be reused multiple times in the segmentation.
You may assume the dictionary does not contain duplicate words.
Example 1:

Input: s = "leetcode", wordDict = ["leet", "code"]
Output: true
Explanation: Return true because "leetcode" can be segmented as "leet code".
*/
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        set<string> dict;
        vector<bool> dp(s.size(), false);
        for(auto w:wordDict){
            dict.insert(w);
        }
        for(int i = 0; i<s.size(); i++){
            auto sub = s.substr(0, i+1 );
            if(dict.find(sub)!=dict.end()){
                dp[i] = true;
                continue;
            }
            for(int j = 0; j < sub.size() - 1; j++){
                if(dp[j]){
                    int start = j + 1;
                    int len = sub.size() - j - 1;
                    auto rest = s.substr(start, len);
                    if(dict.find(rest) != dict.end()){
                        dp[i] = true;
                        break;
                    }
                }
            }
        }
        return dp[s.size() -1];
    }
};
