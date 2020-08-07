/*399. Evaluate Division
 * Equations are given in the format A / B = k, where A and B are variables represented as strings, and k is a real number (floating point number). Given some queries, return the answers. If the answer does not exist, return -1.0.

Example:
Given a / b = 2.0, b / c = 3.0.
queries are: a / c = ?, b / a = ?, a / e = ?, a / a = ?, x / x = ? .
return [6.0, 0.5, -1.0, 1.0, -1.0 ].

The input is: vector<pair<string, string>> equations, vector<double>& values, vector<pair<string, string>> queries , where equations.size() == values.size(), and the values are positive. This represents the equations. Return vector<double>.

According to the example above:

equations = [ ["a", "b"], ["b", "c"] ],
values = [2.0, 3.0],
queries = [ ["a", "c"], ["b", "a"], ["a", "e"], ["a", "a"], ["x", "x"] ].
*/
class Solution {
public:
    vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
        map<string, map<string,double>> m;
        vector<double> res;
        for(int i = 0; i < values.size(); i++){
            auto a = equations[i][0];
            auto b = equations[i][1];
            if(a != b){
                m[a][b] =  values[i];
                m[b][a] = 1 / values[i];
            }
        }
        for(auto r:queries){
            auto a = r[0];
            auto b = r[1];
            if(m.find(a) == m.end() || m.find(b) == m.end()){
                res.push_back(-1.0);
            }else if( a == b){
                res.push_back(1.0);
            }else{
                queue<pair<string, double>> q;
                set<string> visited;
                q.push({a, 1.0});
                visited.insert(a);
                bool find = false;
                while(!q.empty()){
                    auto cur = q.front();q.pop();
                    auto src=cur.first;
                    double v = cur.second;
                    if(src == b){
                        res.push_back(v);
                        find = true;
                        break;
                    }
                    for(auto it = m[src].begin(); it!=m[src].end(); it++){
                        if(visited.find(it->first) != visited.end()){
                            continue;
                        }else{
                            q.push({it->first, v * it->second});
                            visited.insert(it->first);
                        }
                    }
                }
                if(!find){
                    res.push_back(-1.0);
                }
            }
        }
        return res;
    }
};
