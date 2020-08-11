/*17. Letter Combinations of a Phone Number
 * Given a string containing digits from 2-9 inclusive, return all possible letter combinations that the number could represent.

A mapping of digit to letters (just like on the telephone buttons) is given below. Note that 1 does not map to any letters.



Example:

Input: "23"
Output: ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].
*/
class Solution {
public:
    vector<string> res;
    map<char, vector<char>> m;
    vector<string> letterCombinations(string digits) {
        if(digits.empty()) return res;
        m['2'] = {'a', 'b', 'c'};
        m['3'] = {'d', 'e', 'f'};
        m['4'] = {'g', 'h', 'i'};
        m['5'] = {'j', 'k', 'l'};
        m['6'] = {'m', 'n', 'o'};
        m['7'] = {'p', 'q', 'r', 's'};
        m['8'] = {'t', 'u', 'v'};
        m['9'] = {'w', 'x', 'y', 'z'};
        string cur;
        dfs(cur, digits, 0);
        return res;
    }
    void dfs(string& cur, string& digits, int index){
        if(index >= digits.size()){
            res.push_back(cur);
            return;
        }
        char c = digits[index];
        for(int i = 0; i < m[c].size(); i++){
            cur.push_back(m[c][i]);
            dfs(cur, digits, index + 1);
            cur.pop_back();
        }
    }
};
