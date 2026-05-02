/*
 * 300. Longest Increasing Subsequence
 * Medium
 * [Atlassian DSA - Medium Frequency, DP classic]
 *
 * Given an integer array nums, return the length of the longest strictly
 * increasing subsequence (not necessarily contiguous).
 *
 * Examples:
 *   [10,9,2,5,3,7,101,18] -> 4   (e.g. 2,3,7,101)
 *   [0,1,0,3,2,3]         -> 4
 *   [7,7,7,7,7,7,7]       -> 1
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach 1: DP O(n^2)
 *   dp[i] = LIS ending at index i
 *   dp[i] = 1 + max(dp[j] for j < i if nums[j] < nums[i])
 *   ans = max(dp)
 *
 * Approach 2: Patience sorting / binary search O(n log n)  ← interview gold
 *   - Maintain tails[]: tails[k] = smallest possible tail of any increasing
 *     subsequence of length k+1 seen so far.
 *   - For each x: binary search the first tail >= x, replace with x.
 *     If none found, append (LIS extended).
 *   - Length of tails[] is the answer.
 *   - tails is NOT itself a valid LIS (it's a "best tails so far" register).
 * ─────────────────────────────────────────────────────────────────
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Approach 2: O(n log n)
    int lengthOfLIS(vector<int>& nums) {
        vector<int> tails;
        for (int x : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) tails.push_back(x);
            else *it = x;
        }
        return (int)tails.size();
    }

    // Approach 1: O(n^2) DP
    int lengthOfLIS_DP(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        vector<int> dp(n, 1);
        int best = 1;
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) dp[i] = max(dp[i], dp[j] + 1);
            }
            best = max(best, dp[i]);
        }
        return best;
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: How to also reconstruct one such LIS sequence?
 * A: With O(n^2) DP, store parent[i] = j of the predecessor. With O(n log n),
 *    record the position in tails when each element is placed; rebuild via
 *    a separate parent array. Trickier but doable.
 *
 * Q: Strictly increasing vs non-decreasing?
 * A: Use lower_bound for strictly increasing, upper_bound for non-decreasing.
 *
 * Q: Number of LIS of max length? (LC 673)
 * A: Maintain dp[i] = length AND cnt[i] = number of LIS ending at i.
 *
 * Q: Why does the greedy O(n log n) work?
 * A: Replacing the first tail >= x with x can only make future extensions
 *    easier (smaller tail). Length of tails monotonically reflects max LIS.
 */

int main() {
    Solution sol;
    vector<int> v1 = {10,9,2,5,3,7,101,18};
    cout << sol.lengthOfLIS(v1) << endl;      // 4
    cout << sol.lengthOfLIS_DP(v1) << endl;   // 4

    vector<int> v2 = {0,1,0,3,2,3};
    cout << sol.lengthOfLIS(v2) << endl;      // 4

    vector<int> v3 = {7,7,7,7};
    cout << sol.lengthOfLIS(v3) << endl;      // 1
    return 0;
}
