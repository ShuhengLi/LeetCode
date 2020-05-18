/*101. Symmetric Tree
Easy
Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).

For example, this binary tree [1,2,2,3,4,4,3] is symmetric:

    1
   / \
  2   2
 / \ / \
3  4 4  3
*/
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        return isSymmetric(root , root);
    }
    bool isSymmetric(TreeNode* head1, TreeNode* head2){
        if(!head1 && !head2) return true;
        if(!head1 || !head2) return false;
        return head1->val == head2->val && isSymmetric(head1->left, head2->right) &&isSymmetric(head1->right, head2->left);
    }
};
