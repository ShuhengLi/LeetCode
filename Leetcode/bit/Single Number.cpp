/*
Given an array of integers, every element appears twice except for one. Find that single one.

Note:
Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?

Subscribe to see which companies asked this question

Show Tags
Show Similar Problems
Author: shuheng li
Date: 04/27/2016
*/
class Solution {
public:
	int singleNumber(vector<int>& nums) {
		int s = 0;
		for (int i = 0; i < nums.size(); i++) {
			s = s^ nums[i];
		}
		return s;
	}
};