/*139. Word Break
 * Given a non-empty string s and a dictionary wordDict containing a list of non-empty words, determine if s can be segmented into a space-separated sequence of one or more dictionary words.

Note:

The same word in the dictionary may be reused multiple times in the segmentation.
You may assume the dictionary does not contain duplicate words.
Example 1:

Input: s = "leetcode", wordDict = ["leet", "code"]
Output: true
Explanation: Return true because "leetcode" can be segmented as "leet code".
Example 2:

Input: s = "applepenapple", wordDict = ["apple", "pen"]
Output: true
Explanation: Return true because "applepenapple" can be segmented as "apple pen apple".
*/
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        set<string> dict(wordDict.begin(), wordDict.end());
        if(s.empty()) return false;
        vector<bool> ans(s.size(),false); //for this one size is n - 1

        for(int i = 0; i < s.size(); i++){
            auto sub = s.substr(0, i + 1);
            if(dict.find(sub) != dict.end()){ //if all segment is found in dict
                ans[i] = true;
                continue;
            }
            for(int j = 1; j <= i; j ++){
                if(ans[j-1]){
                    auto temp = s.substr(j , i - j+1 );
                    if(dict.find(temp) != dict.end()) {
                        ans[i] = true;
                        break;
                    }
                }
            }
        }
        return ans[s.size()-1];
    }
};
