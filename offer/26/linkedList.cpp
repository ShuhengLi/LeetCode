//reverse linked list
LinkNode* reverse(LinkNode* r){
  if(!r || !r->next) return;
  auto temp = reverse(r->next);
  r->next->next = temp;
  r->next = NULL;
  return temp;
}

//reverse linked list in pair
LinkNode* reversePair(LinkNode* head){
  if(!head || !head->next) return;
  auto newhead = reverse(head->next->next);
  auto next = head->next;
  newhead->next = newhead;
  next->next = newhead;
  return next;
}

TreeNode * reverseTree(TreeNode* root){
  if(!root || !root->left) return root;
  auto newroot = reverseTree(root->left);
  newroot->left = root->right;
  newroot->right = root;
  root->left = NULL;
  root->right = NULL;
  return newroot;
}
//Time: O(N)
