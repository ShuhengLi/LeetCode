/*543. Diameter of Binary Tree
Given a binary tree, you need to compute the length of the diameter of the tree. The diameter of a binary tree is the length of the longest path between any two nodes in a tree. This path may or may not pass through the root.

Example:
Given a binary tree
          1
         / \
        2   3
       / \     
      4   5    
Return 3, which is the length of the path [4,2,1,3] or [5,2,1,3].
*/
class Solution {
public:
    int res;
    int diameterOfBinaryTree(TreeNode* root) {
        res = 0;
        if(!root) return 0;
        dfs(root);
        return res - 1;

    }
    int dfs(TreeNode* root) {
        if(!root) return 0;
        int l = dfs(root->left);
        int r = dfs(root->right);
        res = max(res, l + r + 1);
        return 1 + (l > r ? l : r);
    }
};
