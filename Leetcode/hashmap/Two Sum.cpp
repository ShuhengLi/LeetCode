/*
Given an array of integers, return indices of the two numbers such that they add up to a specific target.

You may assume that each input would have exactly one solution.

Example:
Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
UPDATE (2016/2/13):
The return format had been changed to zero-based indices. Please read the above updated description carefully.

Subscribe to see which companies asked this question

Author: Shuheng Li
Date: 04/27/2016
*/

class Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		vector<int> res;
		map<int, int> m;
		for (int i = 0; i < nums.size(); i++) {
			if (m.find(target - nums[i]) != m.end()) {
				res.push_back(m[target - nums[i]]);
				res.push_back(i);
				return res;
			}
			m[nums[i]] = i;
		}
		return res;
	}
};