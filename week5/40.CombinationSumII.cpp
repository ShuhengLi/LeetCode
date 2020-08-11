/*40. Combination Sum II
 * Given a collection of candidate numbers (candidates) and a target number (target), find all unique combinations in candidates where the candidate numbers sums to target.

Each number in candidates may only be used once in the combination.

Note:

All numbers (including target) will be positive integers.
The solution set must not contain duplicate combinations.
Example 1:

Input: candidates = [10,1,2,7,6,1,5], target = 8,
A solution set is:
[
  [1, 7],
  [1, 2, 5],
  [2, 6],
  [1, 1, 6]
]
*/
class Solution {
public:
    vector<vector<int>> res;
    int T;
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        T = target;
        vector<int> cur;
        sort(candidates.begin(), candidates.end());
        dfs(0, candidates, cur, 0);
        return res;

    }
    void dfs(int sum, vector<int> & nums, vector<int>& cur, int level){//cannot pop_back if pass by reference;
        if(sum >= T){
            if(sum == T) {
                if(find(res.begin(), res.end(), cur)== res.end()) res.push_back(cur);
            }
            return;
        }
        if(level >= nums.size()) return;
        for(int i = level; i < nums.size(); i++){
            cur.push_back(nums[i]);
            dfs(sum + nums[i], nums, cur, i + 1);
            cur.pop_back();
        }
    }
};
