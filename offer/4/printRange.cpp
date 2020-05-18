//print increasing order
void printRange(TreeNode* root, int k1, int k2){
  if(!root) return;
  if(root > k1) printRange(root->left, k1, k2);
  cout << root->val << endl;
  if(root < k2) printRange(root->right, k1, k2);
  return;
}
