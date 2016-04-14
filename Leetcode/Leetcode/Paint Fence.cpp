/*
There is a fence with n posts, each post can be painted with one of the k colors.

You have to paint all the posts such that no more than two adjacent fence posts have the same color.

Return the total number of ways you can paint the fence.

Author: Shuheng Li
*/

class Solution {
public:
	int numWays(int n, int k) {
		if (!k || !n) return 0;
		if (n == 1) return k;
		int diff = k*(k - 1); //last two posts are different;
		int same = k; // last two posts are same.
		for (int i = 2; i<n; ++i) {
			int newSame = diff;
			int newDiff = (k - 1) * (same + diff);
			diff = newDiff;
			same = newSame;
		}
		return same + diff;
	}
};