/*
Given a nested list of integers, return the sum of all integers in the list weighted by their depth.

Each element is either an integer, or a list -- whose elements may also be integers or other lists.
*/

class Solution:
{
public:
	int depthSum(vector<NestedInteger>& nestedList) {
		return Helper(nestedList, 1);
	}
	int Helper(vector<NestedInteger>& nestedList, int depth) {
		int res = 0;
		for (auto n : nestedList) {
			if (n.isInterger()){
				res = res + n.getInterger() * depth;
			}
			else {
				res = res + Helper(n.getList(), depth + 1);
			}
		}
		return res;
	}

};