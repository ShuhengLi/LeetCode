/*140. Word Break II
 * Given a non-empty string s and a dictionary wordDict containing a list of non-empty words, add spaces in s to construct a sentence where each word is a valid dictionary word. Return all such possible sentences.

Note:

The same word in the dictionary may be reused multiple times in the segmentation.
You may assume the dictionary does not contain duplicate words.
Example 1:

Input:
s = "catsanddog"
wordDict = ["cat", "cats", "and", "sand", "dog"]
Output:
[
  "cats and dog",
  "cat sand dog"
]
*/
class Solution {
public:
    unordered_set<string>dict;
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        for(auto word : wordDict){
            dict.insert(word);
        }
        unordered_map<int, vector<string>>dp;
        return solve(s, 0, dp, mp);
    }
    
    vector<string> solve(string s, int idx, unordered_map<int, vector<string>>&dp){
        if(dp.find(idx) != dp.end()) return dp[idx];
        string temp = "";
        int n = s.size();
        vector<string>res;
        for(int i=idx; i<n; i++){
            temp+=s[i];
            if(dict.find(temp) != dict.end()){
                if(i==n-1) res.push_back(temp);
                else{
                    vector<string>tempvec = solve(s, i+1, dp);
                    if(tempvec.size())
                    for(auto str : tempvec){
                        res.push_back(temp + " " + str);
                    }
                }
            }
        }
        dp[idx] = res;
        return res;
    }
};
