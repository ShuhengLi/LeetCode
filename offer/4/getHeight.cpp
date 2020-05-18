int getHeight(TreeNode* root){
  if(!root) return 0;
  int left = getHeight(root->left);
  int right = getHeight(root->right);
  return max(left, right) + 1;
}
