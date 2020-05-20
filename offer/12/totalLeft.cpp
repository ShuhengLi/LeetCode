int totalLeft(TreeNode* root){
  if(!root) return 0;
  int left = totalLeft(root->left);
  int right = totalLeft(root->right);
  root->total_left = left;
  return left + right + 1;
}
