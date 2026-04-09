/*
Rotate an array of n elements to the right by k steps.

For example, with n = 7 and k = 3, the array [1,2,3,4,5,6,7] is rotated to [5,6,7,1,2,3,4].

Note:
Try to come up as many solutions as you can, there are at least 3 different ways to solve this problem.

Author: SHuheng li
Date: 04/27/2016
*/

class Solution {
public:
	void rotate(vector<int>& nums, int k) {
		int n = nums.size();
		k = k % n;
		vector<int> temp(k);

		for (int i = 0; i < k; i++) {
			temp[i] = nums[n - k + i];
		}
		for (int i = n - 1; i >= k; i--) {
			nums[i] = nums[i - k];
		}
		for (int i = 0; i < k; i++) {
			nums[i] = temp[i];
		}
	}
};


class Solution {
public:
	void rotate(vector<int>& nums, int k) {
		int n = nums.size() - 1;
		int i = 0;
		while (i < k) {
			nums.insert(nums.begin(), nums[n]);
			i++;
		}
		nums.erase(nums.begin() + n + 1, nums.end());
	}
};