/* 472. Concatenated Words
Given a list of words (without duplicates), please write a program that returns all concatenated words in the given list of words.
A concatenated word is defined as a string that is comprised entirely of at least two shorter words in the given array.

Example:
Input: ["cat","cats","catsdogcats","dog","dogcatsdog","hippopotamuses","rat","ratcatdogcat"]

Output: ["catsdogcats","dogcatsdog","ratcatdogcat"]

Explanation: "catsdogcats" can be concatenated by "cats", "dog" and "cats"; 
 "dogcatsdog" can be concatenated by "dog", "cats" and "dog"; 
"ratcatdogcat" can be concatenated by "rat", "cat", "dog" and "cat".
*/
class Solution {
public:
    static bool myCompare(const string& s1, const string& s2 ){
        return s1.size() < s2.size();
    }
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        vector<string> res;
        if(words.empty()) return res;
        unordered_set<string> s;
        sort(words.begin(), words.end(), myCompare);
        for(auto cur:words){
            if(cur.empty()) continue;//corner case
            vector<bool> dp(cur.size(), false);
            for(int i = 0; i < cur.size(); i++){
                auto sub = cur.substr(0, i + 1);
                if(s.find(sub)!=s.end()){
                    dp[i] = true;
                    continue;
                }
                for(int j = 0; j < sub.size() - 1; j++){
                    if(dp[j]){
                        auto rest = cur.substr(j + 1, sub.size() - j - 1);
                        if(s.find(rest) != s.end()){
                            dp[i] = true;
                            break;
                        }
                    }
                }
            }
            if(dp[cur.size()-1]) res.push_back(cur);
            s.insert(cur);
        }
        return res;
    }
};
