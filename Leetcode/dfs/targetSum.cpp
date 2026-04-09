/*
494. Target Sum

You are given an integer array nums and an integer target.

You want to build an expression out of nums by adding one of the symbols '+'
and '-' before each integer in nums and then concatenate all the integers.

For example, if nums = [2, 1], you can add a '+' before 2 and a '-' before 1
and concatenate them to build the expression "+2-1".

Return the number of different expressions that you can build, which evaluates
to target.

Example 1:
  Input: nums = [1,1,1,1,1], target = 3
  Output: 5
  Explanation: There are 5 ways to assign symbols to make the sum of nums be 3.
  -1 + 1 + 1 + 1 + 1 = 3
  +1 - 1 + 1 + 1 + 1 = 3
  +1 + 1 - 1 + 1 + 1 = 3
  +1 + 1 + 1 - 1 + 1 = 3
  +1 + 1 + 1 + 1 - 1 = 3

Example 2:
  Input: nums = [1], target = 1
  Output: 1

Constraints:
  - 1 <= nums.length <= 20
  - 0 <= nums[i] <= 1000
  - 0 <= sum(nums[i]) <= 1000
  - -1000 <= target <= 1000
*/
// DFS brute force + memoization optimization
// Time: O(n*sum), Space: O(n*sum)
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    // Approach 1: Pure DFS (intuitive but O(2^n))
    int findTargetSumWays_bruteforce(vector<int>& nums, int target) {
        return dfs(nums, 0, 0, target);
    }

    int dfs(vector<int>& nums, int i, int sum, int target) {
        if (i == nums.size()) return sum == target ? 1 : 0;
        return dfs(nums, i+1, sum + nums[i], target)
             + dfs(nums, i+1, sum - nums[i], target);
    }

    // Approach 2: DFS + Memoization (O(n*sum))
    unordered_map<long long, int> memo;

    int findTargetSumWays(vector<int>& nums, int target) {
        return dfsMemo(nums, 0, 0, target);
    }

    int dfsMemo(vector<int>& nums, int i, int sum, int target) {
        if (i == nums.size()) return sum == target ? 1 : 0;
        long long key = (long long)i * 2001 + sum + 1000;
        if (memo.count(key)) return memo[key];
        return memo[key] = dfsMemo(nums, i+1, sum + nums[i], target)
                         + dfsMemo(nums, i+1, sum - nums[i], target);
    }
};

// ===================== Improved Solution: 0/1 Knapsack DP =====================
// Math transform: let P = sum of positives, N = sum of negatives.
// P - N = target, P + N = totalSum => P = (target + totalSum) / 2.
// Reduces to: count subsets with sum = P (standard 0/1 knapsack).
// Time: O(n * P), Space: O(P) — typically much smaller state space than memo approach
class Solution_v2 {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int totalSum = 0;
        for (int x : nums) totalSum += x;
        if ((target + totalSum) % 2 != 0 || target + totalSum < 0) return 0;
        int P = (target + totalSum) / 2;

        vector<int> dp(P + 1, 0);
        dp[0] = 1;
        for (int num : nums)
            for (int j = P; j >= num; j--)
                dp[j] += dp[j - num];
        return dp[P];
    }
};
