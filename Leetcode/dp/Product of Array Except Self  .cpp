/*
Given an array of n integers where n > 1, nums, return an array output such that output[i] is equal to the product of all the elements of nums except nums[i].

Solve it without division and in O(n).

For example, given [1,2,3,4], return [24,12,8,6].
Author: Shuheng Li
Date: 04/27/2016
*/

class Solution {
public:
	vector<int> productExceptSelf(vector<int>& nums) {
		int total = 1;
		int zeros = 0;
		for (int i = 0; i < nums.size(); i++) {
			if (nums[i] == 0) {
				zeros++;
				if (zeros>1) {
					vector<int> res(nums.size(), 0);
					return res;
				}
				else continue;
			}
			total *= nums[i];
		}
		for (int i = 0; i < nums.size(); i++) {
			if (zeros != 0) {
				if (nums[i] == 0) nums[i] = total;
				else nums[i] = 0;
			}
			else {
				nums[i] = total / nums[i];
			}

		}
		return nums;
	}
};