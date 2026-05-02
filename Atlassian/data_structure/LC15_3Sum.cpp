/*
 * 15. 3Sum
 * Medium
 *
 * Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]]
 * such that i != j, i != k, and j != k, and nums[i] + nums[j] + nums[k] == 0.
 * Notice that the solution set must not contain duplicate triplets.
 *
 * Example 1:
 * Input: nums = [-1,0,1,2,-1,-4]
 * Output: [[-1,-1,2],[-1,0,1]]
 *
 * Example 2:
 * Input: nums = [0,1,1]
 * Output: []
 *
 * Constraints:
 * - 3 <= nums.length <= 3000
 * - -10^5 <= nums[i] <= 10^5
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Sort + Two Pointers
    // Time: O(n^2), Space: O(log n) for sorting
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        sort(nums.begin(), nums.end());

        for (int i = 0; i < (int)nums.size() - 2; i++) {
            // Skip duplicates for the first element
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            // Early termination: if smallest possible sum > 0, no more solutions
            if (nums[i] > 0) break;

            int left = i + 1, right = (int)nums.size() - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == 0) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    // Skip duplicates
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                } else if (sum < 0) {
                    left++;
                } else {
                    right--;
                }
            }
        }
        return result;
    }
};
