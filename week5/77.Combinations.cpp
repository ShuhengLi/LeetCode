/*77. Combinations
 * Given two integers n and k, return all possible combinations of k numbers out of 1 ... n.

Example:

Input: n = 4, k = 2
Output:
[
  [2,4],
  [3,4],
  [2,3],
  [1,2],
  [1,3],
  [1,4],
]
*/
class Solution {
public:
    int K;
    int N;
    vector<vector<int>> res;
    vector<vector<int>> combine(int n, int k) {
        K = k;
        N = n;
        vector<int> cur;
        dfs(1, 0, cur);
        return res;
    }
    void dfs(int level, int count, vector<int> cur){
        if(count == K){
            res.push_back(cur);
            return;
        }
        for(int i = level; i <= N; i++){
            cur.push_back(i);
            dfs(i + 1, count+1, cur);
            cur.pop_back();
        }
    }

};
