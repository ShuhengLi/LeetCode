TreeNode* LCA(TreeNode* root, int a, int b){
  if(!root || root->val == a || root->val == b) return root;
  auto left = LCA(root->left, a, b);
  auto right = LCA(root->right, a, b);
  if(left && right) return root;
  return left? left:right;
}
