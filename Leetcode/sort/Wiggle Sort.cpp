/*
Total Accepted : 1291 Total Submissions : 3008 Difficulty : Medium
Given an unsorted array nums, reorder it in - place such that nums[0] <= nums[1] >= nums[2] <= nums[3]....
For example, given nums = [3, 5, 2, 1, 6, 4], one possible answer is[1, 6, 2, 5, 3, 4].

Author: shuheng li
Date: 04/27/2017
*/

public class Solution {
	public void wiggleSort(int[] nums) {
		if (nums == null || nums.length<2) return;

		for (int i = 1; i<nums.length; i++) {
			if ((i % 2 == 1 && (nums[i] < nums[i - 1])) || (i % 2 == 0) && (nums[i] > nums[i - 1])) {
				int temp = nums[i];
				nums[i] = nums[i - 1];
				nums[i - 1] = temp;
			}
		}

	}
}