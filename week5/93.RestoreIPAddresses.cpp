/*93. Restore IP Addresses
 * Given a string containing only digits, restore it by returning all possible valid IP address combinations.

A valid IP address consists of exactly four integers (each integer is between 0 and 255) separated by single points.

Example:

Input: "25525511135"
Output: ["255.255.11.135", "255.255.111.35"]
*/
class Solution {
public:
    vector<string> res;
    vector<string> restoreIpAddresses(string s) {
        if(s.size() > 12) return res;//bug1: check length
        string cur;
        dfs(0, cur, 0, s);
        return res;
    }
    void dfs(int count, string cur, int index, string s){
        if(index >= s.size()) return; //bug 2 check boundary
        if(count == 4){
            res.push_back(cur);
            return;
        }
        if(count == 3){
            if(s[index] == '0' && index < s.size() - 1) return ;// bug3 check 0 condition for 3rd '.'
            string sub = s.substr(index);
            long val = stol(sub);
            if(val <= 255){
                cur += sub;
                dfs(count +1, cur, index, s);
                for(int j = 0; j < sub.size(); j++) cur.pop_back();
            }
            return;
        }
        for(int i = index; i < s.size(); i++){
            string sub = s.substr(index, i - index + 1);
            int val = stoi(sub);
            if(val <= 255){
                cur += sub;
                cur += '.';
                dfs(count + 1, cur, i + 1, s);
                for(int j = 0; j <= sub.size(); j++) cur.pop_back();
            }else{
                break;
            }
            if(val == 0) break;
        }
    }
};
