/*767. Reorganize String
 * Given a string S, check if the letters can be rearranged so that two characters that are adjacent to each other are not the same.

If possible, output any possible result.  If not possible, return the empty string.

Example 1:

Input: S = "aab"
Output: "aba"
Example 2:

Input: S = "aaab"
Output: ""
*/
class Solution {
public:
    typedef pair<int, char> p;
    string reorganizeString(string S) {
        int n = S.size();
        string res(n, ' ');
        map<char, int> m;
        for(auto c:S){
            m[c]++;
        }
        int half = ceil((double)n / 2);
        int large = 0;
        priority_queue<p, vector<p>, greater<p>> q;
        for(auto it = m.begin(); it != m.end(); it++){
            q.push({it->second, it->first});
            if(it->second > large){
                large = it->second;
            }
        }

        if(large > half) return "";
        int t = 1;
        while(!q.empty()){
            auto cur = q.top(); q.pop();
            for(int i = 0; i < cur.first; i++){
                if(t >= n) t = 0;
                res[t] = cur.second;
                t += 2;
            }
        }
        return res;
    }
};
