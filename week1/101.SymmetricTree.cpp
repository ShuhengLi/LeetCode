/* 101. Symmetric Tree
Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).

For example, this binary tree [1,2,2,3,4,4,3] is symmetric:

    1
   / \
  2   2
 / \ / \
3  4 4  3
 

But the following [1,2,2,null,3,null,3] is not:

    1
   / \
  2   2
   \   \
   3    3
*/
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        return helper(root, root);

    }
    bool helper(TreeNode* r1, TreeNode* r2) {
        if(!r1 && !r2) return true;
        if(!r1 || !r2) return false;
        return r1->val == r2->val && helper(r1->left, r2->right) && helper(r1->right, r2->left);

    }
};
