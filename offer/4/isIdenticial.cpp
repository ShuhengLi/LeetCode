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
    bool isIdentical(TreeNode* root) {
        return isIdentical(root , root);
    }
    bool isIdentical(TreeNode* head1, TreeNode* head2){
        if(!head1 && !head2) return true;
        if(!head1 || !head2) return false;
        if(head1->val != head2->val) return false;
        return  (isIdentical(head1->left, head2->left) &&isIdentical(head1->right, head2->right)) || \
          (isIdentical(head1->left, head2->right) &&isIdentical(head1->right, head2->left)); 
    }
};
//Time: Tree height is still logN, time complexity depends on the last level leaf's node.
//Leaf's node = 4 ^ tree hegiht = 4 ^ ( logN) = n^2
