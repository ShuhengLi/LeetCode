/*
Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).

For example, this binary tree is symmetric:

1
/ \
2   2
/ \ / \
3  4 4  3
But the following is not:
1
/ \
2   2
\   \
3    3
*/

class Solution {
public:

	bool isSymmetric(TreeNode* root) {
		if (root == NULL) return true;
		return helper(root->left, root->right);
	}

	bool helper(TreeNode* l, TreeNode* r) {
		if (l == NULL || r == NULL) return l == r;
		if (l->val != r->val) return false;
		return helper(l->left, r->right) && helper(l->right, r->left);

	}
};
