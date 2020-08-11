/*39. Combination Sum
 * Given a set of candidate numbers (candidates) (without duplicates) and a target number (target), find all unique combinations in candidates where the candidate numbers sums to target.

The same repeated number may be chosen from candidates unlimited number of times.

Note:

All numbers (including target) will be positive integers.
The solution set must not contain duplicate combinations.
Example 1:

Input: candidates = [2,3,6,7], target = 7,
A solution set is:
[
  [7],
  [2,2,3]
]
Example 2:

Input: candidates = [2,3,5], target = 8,
A solution set is:
[
  [2,2,2,2],
  [2,3,3],
  [3,5]
]
*/
class Solution {
public:
    vector<vector<int>> res;
    int T;
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        T = target;
        vector<int> cur;
        if(candidates.empty()) return res;
        dfs(0, candidates, cur, 0);
        return res;

    }
    void dfs(int sum, vector<int> & nums, vector<int>& cur, int level){
        if(sum >= T){
            if(sum == T) res.push_back(cur);
            return;
        }
        if(level >= nums.size()) return;
        for(int i = level; i < nums.size(); i++){//bug2: never look back, start from level not 0
            cur.push_back(nums[i]);
            dfs(sum + nums[i], nums, cur, i);
            cur.pop_back();//bug 1:need to pop_back

        }
    }
};
