/*
Given a binary tree, return the level order traversal of its nodes' values. (ie, from left to right, level by level).

For example:
Given binary tree {3,9,20,#,#,15,7},
3
/ \
9  20
/  \
15   7
return its level order traversal as:
[
[3],
[9,20],
[15,7]
]

Author:Shuheng Li
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
public:
	vector<vector<int>> levelOrder(TreeNode* root) {
		vector<vector<int>> res;
		vector<int> temp;
		queue<TreeNode*> q;
		if (!root) return res;
		q.push(root);
		while (!q.empty()) {
			int n = q.size();
			for (int i = 0; i < n; i++) {
				TreeNode* n = q.front();
				q.pop();
				temp.push_back(n->val);
				if (n->left) q.push(n->left);
				if (n->right) q.push(n->right);
			}
			res.push_back(temp);
			temp.clear();
		}
		return res;
	}
};