/*216. Combination Sum III
 * Find all possible combinations of k numbers that add up to a number n, given that only numbers from 1 to 9 can be used and each combination should be a unique set of numbers.

Note:

All numbers will be positive integers.
The solution set must not contain duplicate combinations.
Example 1:

Input: k = 3, n = 7
Output: [[1,2,4]]
Example 2:

Input: k = 3, n = 9
Output: [[1,2,6], [1,3,5], [2,3,4]]
*/
class Solution {
public:
    vector<vector<int>> res;
    int K;
    int T;
    vector<vector<int>> combinationSum3(int k, int n) {
        K = k;
        T = n;
        vector<int> cur;
        dfs(0, cur, 1);//bug level start from 1, not 0;
        return res;
    }
    void dfs(int val, vector<int>& cur, int level){
        if(cur.size() == K){
            if(val == T) res.push_back(cur);
            return;
        }
        for(int i = level; i <= 9; i++){//bug2: include 9
            cur.push_back(i);
            dfs(val + i, cur, i + 1);
            cur.pop_back();
        }
    }
};
