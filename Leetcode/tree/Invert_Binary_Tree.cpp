/*
Invert a binary tree.

4
/   \
2     7
/ \   / \
1   3 6   9
to
4
/   \
7     2
/ \   / \
9   6 3   1

Author: Shuheng Li
*/

class Solution {
public:
	TreeNode* invertTree(TreeNode* root) {
		if (root == NULL) return root;
		stack<TreeNode*> s;
		s.push(root);
		while (s.empty() != 1) {
			TreeNode* node = s.top();
			s.pop();
			TreeNode* temp = node->left;
			node->left = node->right;
			node->right = temp;
			if (node->left != NULL) {
				s.push(node->left);
			}
			if (node->right != NULL) {
				s.push(node->right);
			}
		}
		return root;
	}
};