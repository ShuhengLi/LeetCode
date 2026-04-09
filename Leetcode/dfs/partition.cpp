/*
131. Palindrome Partitioning

Given a string s, partition s such that every substring of the partition is a
palindrome. Return all possible palindrome partitioning of s.

Example 1:
  Input: s = "aab"
  Output: [["a","a","b"],["aa","b"]]

Example 2:
  Input: s = "a"
  Output: [["a"]]

Constraints:
  - 1 <= s.length <= 16
  - s contains only lowercase English letters.
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

// ===================== Improved Solution =====================
// Precompute palindrome table with DP to avoid repeated palindrome checks.
// Pass string by reference. Use local result instead of class member.
// Time: O(n * 2^n), Space: O(n^2) for DP table
class Solution_v2 {
public:
    vector<vector<string>> partition(string s) {
        int n = s.size();
        // dp[i][j] = true if s[i..j] is a palindrome
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        for (int len = 1; len <= n; len++)
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                dp[i][j] = (s[i] == s[j]) && (len <= 3 || dp[i+1][j-1]);
            }

        vector<vector<string>> res;
        vector<string> path;
        dfs(s, dp, 0, path, res);
        return res;
    }

    void dfs(const string& s, vector<vector<bool>>& dp, int start,
             vector<string>& path, vector<vector<string>>& res) {
        if (start == s.size()) { res.push_back(path); return; }

        for (int end = start; end < s.size(); end++) {
            if (!dp[start][end]) continue;  // O(1) palindrome check
            path.push_back(s.substr(start, end - start + 1));
            dfs(s, dp, end + 1, path, res);
            path.pop_back();
        }
    }
};
