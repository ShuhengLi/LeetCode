TreeNode* LCA(TreeNode* root, TreeNode* a, TreeNode* b){
  if(!root || root == a || root == b) return root;
  auto l = LCA(root->left, a, b);
  auto r = LCA(root->right, a, b);
  if(l && r) return root;
  return l? l : r;
}


//with parent node: use set to save the path of root 2 a, and iterate to root to b


//Find k nodes of ancestors
TreeNode* LCA(TreeNode* root, TreeNode* a, TreeNode* b, TreeNode* c, TreeNode* d){
  auto temp = LCA(root, a, b);
  auto temp2 = LCA(root, c, d);
  auto res = LCA(temp, temp2);
  return res;
}
//Call how many time of LCA? (k/2 + k/4 ...k/8) == (k) Total Time: O(kn)
//k-way all together
TreeNode* LCA(TreeNode* root, set<TreeNode*> s){
  if(!root || s.find(root) != s.end()) return root;
  auto l = LCA(root->left, s);
  auto r = LCA(root->right, s);
  if(l && r) return root;
  return l? l : r;
}
//Time: O(n)
//

//2 node in k branch tree
//vector<TreeNode*> children
TreeNode* LCA(TreeNode* root, TreeNode* a, TreeNode* b){
  if(!root || root == a || root == b) return root;
  int count = 0;
  TreeNode* res = NULL;
  for(auto c:root->children){
    auto child = LCA(c);
    if(child) count++;
    if(count == 2) return root;
    res = c;
  }
  return temp;
}

//k node in k branch tree
//vector<TreeNode*> children
TreeNode* LCA(TreeNode* root, set<TreeNode*> s){
  if(!root || s.find(root) != s.end()) return root;
  int count = 0;
  TreeNode* res = NULL;
  for(auto c:root->children){
    auto child = LCA(c);
    if(child) count++;
    if(count > 1) return root;
    res = c;
  }
  return temp;
}

/*LCA a b in a very large tree contains billions of nodes
//case 1:Run LCA(a, b, level_limit)  if return c for sure. BFS
//case 2: if return a or b, call Find(M1, b), Find(M2, b) ... Find(Mn, b) 
//      :Run LCA(a, Mn, level_limit)  
//case 3: return null: call LCA(M1,a, b), LCA(M2,a, b) ... LCA(Mn,a ,b) 
//   3.1: return 2 valid value: M3, M7; LCA(root, M3, M7);
//   3.2: return 1 valid value: This is final result
*/      


//LCA in binary tree
//if root > a , root < b return root; if root < a && root < b; LCA(root->rihgt)
TreeNode* LCA(TreeNode* root, TreeNode* a, TreeNode* b){
  if(!root) return root;
  if(root->val < a->val && root->val < b->val){
    return LCA(root-> right, a, b);
  }
  if(root->val > a->val && root->val > b->val){
    return LCA(root-> left, a, b);
  }
  return root;
}
//Time:O(logN)
