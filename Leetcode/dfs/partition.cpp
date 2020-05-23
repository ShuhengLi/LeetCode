/* 131. Palindrome Partitioning
Given a string s, partition s such that every substring of the partition is a palindrome.

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
    vector<vector<string>>  res;
    vector<vector<string>> partition(string s) {
        if(s.empty()) return res;
        vector<string>  cur; 
        helper(s, 0, cur);
        return res;
    }
    void helper(string s, int start, vector<string>& cur){
        if(start >= s.size()){
            res.push_back(cur);
            return;
        }
        for(int i = start + 1; i <= s.size(); i++){
            string sub = s.substr(start, i - start);
            if(valid(sub)){
                cur.push_back(sub);
                helper(s,i ,cur);
                cur.pop_back();
            }
        }
    }
    
    bool valid(string s){
        int i = 0;
        int j = s.size()-1;
        while(i<j){
            if(s[i]!=s[j]){
                return false;
            }
            i++;
            j--;
        }
        return true;
    }
};
