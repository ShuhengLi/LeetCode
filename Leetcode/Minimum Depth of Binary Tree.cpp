/*
Given a binary tree, find its minimum depth.

The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.
*/

/**
* Definition for a binary tree node.
* struct TreeNode {
*     int val;
*     TreeNode *left;
*     TreeNode *right;
*     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
* };
*/
class Solution {
private:
	queue<TreeNode* > q;
public:
	int minDepth(TreeNode* root) {
		int res = 0;
		if (!root) return res;

		q.push(root);
		while (!q.empty()) {
			res++;
			int n = q.size();
			for (int i = 0; i< n; i++) {
				TreeNode* temp = q.front();
				q.pop();
				if ((!temp->left) && (!temp->right)) return res;
				if (temp->left) q.push(temp->left);
				if (temp->right) q.push(temp->right);
			}
		}
	}
};