/*282. Expression Add Operators
 * Given a string that contains only digits 0-9 and a target value, return all possibilities to add binary operators (not unary) +, -, or * between the digits so they evaluate to the target value.

Example 1:

Input: num = "123", target = 6
Output: ["1+2+3", "1*2*3"]
Example 2:

Input: num = "232", target = 8
Output: ["2*3+2", "2+3*2"]
Example 3:

Input: num = "105", target = 5
Output: ["1*0+5","10-5"]
Example 4:

Input: num = "00", target = 0
Output: ["0+0", "0-0", "0*0"]
Example 5:

Input: num = "3456237490", target = 9191
Output: []
*/

#define ll long long
class Solution {
private:
    vector<string> res;

    void dfs(const string& num, int index, ll val, ll prev, int target, string& cur) {
        if (index == num.size()) {
            if (val == target) {
                res.push_back(cur);
            }
            return;
        }

        for (int i = index+1; i <= num.size(); ++i) {
            string lhs = num.substr(index, i-index);
            if (lhs[0] == '0' && lhs.size() > 1) {
                continue;
            }

            ll lhsNum = atoll(lhs.c_str());

            if (index == 0) {
                aux = lhs;
                dfs(num, i, lhsNum, lhsNum, target, aux);
                cur.clear();
            } else {
                aux.push_back('+');
                aux.append(lhs);
                dfs(num, i, val + lhsNum, lhsNum, target, aux);

                aux[aux.length() - lhs.length()-1] = '-';
                dfs(num, i, val - lhsNum, -lhsNum, target, aux);

                aux[aux.length() - lhs.length()-1] = '*';
                dfs(num, i, (val - prev) + (lhsNum * prev), (lhsNum * prev), target, aux);

                aux.erase(aux.length() - lhs.length()-1);
            }
        }
    }
public:
    vector<string> addOperators(const string& num, int target) {
        string cur = "";
        dfs(num, 0, 0, 0, target, cur);

        return res;
    }
};
