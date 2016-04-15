/*
Given an array of integers and an integer k, find out whether there are two distinct indices i and j in the array such that nums[i] = nums[j] and the difference between i and j is at most k.

Author: Shuheng Li
*/

class Solution {
public:
	bool containsNearbyDuplicate(vector<int>& nums, int k) {
		map<int, int> m;
		int n = nums.size();
		if (k == 0 || n <= 1) return false;
		for (int i = 0; i < n; i++) {
			if (m.find(nums[i]) != m.end()) {
				if (i - m[nums[i]] <= k) return true;
			}
			m[nums[i]] = i;
		}
		return false;
	}
};