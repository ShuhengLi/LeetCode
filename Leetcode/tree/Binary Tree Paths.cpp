/*
Given a binary tree, return all root-to-leaf paths.

For example, given the following binary tree:

1
/   \
2     3
\
5
All root-to-leaf paths are:

["1->2->5", "1->3"]

Author: Shuheng Li
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
	vector<string> res;
public:
	vector<string> binaryTreePaths(TreeNode* root) {
		string s;
		help(root, s);
		return res;
	}
	void help(TreeNode* root, string s) {
		if (root == NULL) return;
		s = s + to_string(root->val);
		if (!root->left && !root->right) res.push_back(s);
		if (root->left) help(root->left, s + "->");
		if (root->right) help(root->right, s + "->");
	}
};