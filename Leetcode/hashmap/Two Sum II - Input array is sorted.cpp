/*
Given an array of integers that is already sorted in ascending order, find two numbers such that they add up to a specific target number.

The function twoSum should return indices of the two numbers such that they add up to the target, where index1 must be less than index2. Please note that your returned answers (both index1 and index2) are not zero-based.

You may assume that each input would have exactly one solution.

Input: numbers={2, 7, 11, 15}, target=9
Output: index1=1, index2=2

Author: Shuheng li
Date: 04/27/2016
*/

class Solution {
public:
	vector<int> twoSum(vector<int> &numbers, int target) {
		vector<int> ret(2, -1);
		int left = 0;
		int right = numbers.size() - 1;
		while (left < right)
		{
			int tmp = numbers[left] + numbers[right];
			if (tmp == target)
			{
				ret[0] = left + 1;
				ret[1] = right + 1;
				return ret;
			}
			else if (tmp < target)
				//make tmp larger
				left++;
			else
				//make tmp smaller
				right--;
		}
	}
};