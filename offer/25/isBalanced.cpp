//human form left AND right
bool isBalanced(TreeNode* root){
  if(!root) return 0;
  int l = isBalanced(root->left);
  int r = isBalanced(root->right);
  if(l == -1 || r == -1 || abs(l-r) > 1){
    return -1;
  }
  return max(l, r) + 1;
}

//max path from leaf to leaf
int maxLeafPath(TreeNode* root){
  if(!root) return 0;
  int l = maxLeafPath(root->left);
  int r = maxLeafPath(root->right);
  if(root->left && root->righ){
    int c = l + r + root->val;
    res = max(res, cur);
  }
  return root->val + max(l, r);
}

/*
 * 124. Binary Tree Maximum Path Sum
Given a non-empty binary tree, find the maximum path sum.

For this problem, a path is defined as any sequence of nodes from some starting node to any node in the tree along the parent-child connections. 
The path must contain at least one node and does not need to go through the root.
Example 2:

Input: [-10,9,20,null,null,15,7]

   -10
   / \
  9  20
    /  \
   15   7

Output: 42
*/
class Solution {
public:
    int res = INT_MIN;
    int maxPathSum(TreeNode* root) {
        maxPath(root);
        return res;
    }
    int maxPath(TreeNode* root) {
        if(!root) return 0;
        int l = maxPath(root->left);
        int r = maxPath(root->right);
        l = max(0, l);
        r = max(0, r);
        res = max(res, root->val + r + l);
        return max(l, r) + root->val;
    }
};
