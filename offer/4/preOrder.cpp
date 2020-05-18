void preOrder(TreeNode* root){
  if(!root) return root;
  cout << root -> val << endl;
  preOrder(root->left);
  preOrder(root->right);
}
