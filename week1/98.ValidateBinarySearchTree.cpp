/*  98. Validate Binary Search Tree
Given a binary tree, determine if it is a valid binary search tree (BST).

Assume a BST is defined as follows:

The left subtree of a node contains only nodes with keys less than the node's key.
The right subtree of a node contains only nodes with keys greater than the node's key.
Both the left and right subtrees must also be binary search trees.


Example 1:

    2
   / \
  1   3

Input: [2,1,3]
Output: true
*/
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return dfs(root);
    }
    bool dfs(TreeNode* root, long high = LONG_MAX, long low = LONG_MIN) {
        if(!root) return true;
        if(root->val >= high || root->val <=low) return false;
        return dfs(root->left, root->val, low) && dfs(root->right, high, root->val);
    }
};
