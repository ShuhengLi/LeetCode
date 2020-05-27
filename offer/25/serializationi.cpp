//Given a binary tree convert it to a double linked list
//
node* pre = NULL;
void binaryTree2DoubleLinkedList(node* root, node ** head){
  if(!root) return;
  binaryTree2DoubleLinkedList(root->left, head);
  if(!pre){
    pre = root;
  }else{
    pre -> right = root;
    root->left = pre;
  }
  pre = root;
  binaryTree2DoubleLinkedList(root->right, head);
}
//de-seriallization reconstruct a tree by using IN-ORDER
//only xxx-order and in order can form unique string
//key  find root and global recursively slove left subtree and right subtree
//
//Recursion: 1 function call itself
//           2 boll down a big problem into smaler ones, and then restore the global solution by using these smaller ones
//           3 Implementation:
//               a. Base Case
//               b. Recusive rule
//           4 In more details( Recursive function signature must keep same logic) 
TreeNode* construct(vector<int> pre, int pre_l, int pre_r,
                    vector<int> in, int in_l, int in_r,
                    map<int, int> m){
  if(in_l > in_r){
    return NULL;
  }
  int root_index = pre[pre_l];

  TreeNode* node = new TreeNode(root_index);
  int size = m[root_index] - in_l;


  node->left = construct(pre, pre_l + 1, pre_l + size, in, in_l, in_l + size - 1, m);
  node->right= construct(pre, pre_l + size + 1, pre_r, in, in_l + size + 1, in_r, m);
  return node;
}
