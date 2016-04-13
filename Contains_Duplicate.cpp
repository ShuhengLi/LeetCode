/*
Given an array of integers, find if the array contains any duplicates. Your function should return true if any value appears at least twice in the array, and it should return false if every element is distinct.

Author: Shuheng Li
*/

class Solution {
public:
	bool containsDuplicate(vector<int>& nums) {
		set<int> a;
		a.insert(nums.begin(), nums.end());
		return a.size() != nums.size();
	}
};