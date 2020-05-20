/*226. Invert Binary TreeInvert a binary tree.

Example:

Input:

     4
   /   \
  2     7
 / \   / \
1   3 6   9
Output:

     4
   /   \
  7     2
 / \   / \
9   6 3   1
*/
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if(!root) return root;
        auto newleft = invertTree(root->left);
        auto newright = invertTree(root->right);
        root-> right = newleft;
        root-> left = newright;
        return root;
    }
};
