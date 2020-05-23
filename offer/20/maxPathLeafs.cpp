int res = INT_MIN;
int maxPathLeafs(TreeNode* root){
  if(!root) return 0;
  int l = maxPathLeafs(root->left);
  int r = maxPathLeafs(root->right);
  if(root->left && root->right){
    int cur = l + r + root->val;
    res = max(cur, res);
    return max(l, r) + root->val;
  }
  //leaf value could be negtive and if that's the case, return max(l, r) would be 0 instead of correct negitive leaf value;
  return root->left? root->val + l: root->val + r;
}
