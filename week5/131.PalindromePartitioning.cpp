/*131. Palindrome Partitioning
 * Given a string s, partition s such that every substring of the partition is a palindrome.

Return all possible palindrome partitioning of s.

Example:

Input: "aab"
Output:
[
  ["aa","b"],
  ["a","a","b"]
]
*/
class Solution {
public:
    vector<vector<string>> res;
    vector<vector<string>> partition(string s) {
        vector<string> cur;
        dfs(s, 0, cur);
        return res;
    }

    void dfs(string& s, int level, vector<string>& cur){
        if(level == s.size()){
            res.push_back(cur);
            return;
        }
        for(int i = level; i < s.size(); i++){
            string sub = s.substr(level, i - level + 1);//bug: should start from level not i
            if(valid(sub)){
                cur.push_back(sub);
                dfs(s, i+1 , cur);
                cur.pop_back();
            }
        }
    }

    bool valid(string& s){
        int i = 0, j = s.size()-1;
        while(i <= j){
            if(s[i++]!=s[j--]) return false;
        }
        return true;
    }
};

