/*
Given a binary tree, return the bottom-up level order traversal of its nodes' values. (ie, from left to right, level by level from leaf to root).

For example:
Given binary tree {3,9,20,#,#,15,7},
3
/ \
9  20
/  \
15   7
return its bottom-up level order traversal as:
[
[15,7],
[9,20],
[3]
]

Author: Shuheng Li
*/
class Solution {
public:
	vector<vector<int>> levelOrderBottom(TreeNode* root) {
		vector<vector<int>> res;
		vector<int> temp;
		if (root == NULL) return res;
		queue<TreeNode*> q;
		q.push(root);
		while (!q.empty()) {
			int n = q.size();
			for (int i = 0; i < n; i++) {
				TreeNode* t = q.front();
				q.pop();
				temp.push_back(t->val);
				if (t->left != NULL) q.push(t->left);
				if (t->right != NULL) q.push(t->right);
			}
			res.insert(res.begin(), temp);
			temp.clear();
		}
		return res;
	}
};
