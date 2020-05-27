/* 700. Search in a Binary Search Tree
Given the root node of a binary search tree (BST) and a value. 
You need to find the node in the BST that the node's value equals the given value. Return the subtree rooted with that node. 
If such node doesn't exist, you should return NULL.
*/
//Find a node
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        if(!root || root->val == val) return root;
        TreeNode* l = NULL;
        TreeNode* r = NULL;
        if(val > root->val) r = searchBST(root->right, val);
        if(val < root->val) l = searchBST(root->left, val);
        return l?l:r;
    }
};

//Find a node just closest to val;
public:
TreeNode* res;
void searchBSTCloseK(TreeNode* root, int val) {
  if(!root) return; 
  if(root->val == val){
    res = root;
    return ;
  }
  if((res->val - val) > (root->val - val)){
    res = root;
  }

  if(val > root->val) r = searchBST(root->right, val);
  if(val < root->val) l = searchBST(root->left, val);
  return l?l:r;
}

//Find largest node just smaller than val;
public:
TreeNode* res;
void searchBSTCloseK(TreeNode* root, int val) {
  if(!root) return; 
  if(root->val  >= val){
    searchBSTCloseK(root->left, val);
  }else{
    res = root;
    searchBSTCloseK(root->right, val);
  }
  return;
}

//Remove a node from BST
//Use recusion
//case 1: node is leaf, delete;
//case 2: node has only one child, return the only child
//case 3: node has only two children
//  step1: find the node with the smallest value in the current node right sub tree
//  step2: copy the value to current node
//  step3: recurse agian to delete the node from the origianl target node's right sub tree.
//
//450. Delete Node in a BST
class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if(!root) return root;
        auto res = helper(root, key);
        return res;
    }
    TreeNode* helper(TreeNode* root, int key) {
        if(!root) return root;
        if(root->val == key){
            if(!root->left && !root->right){
                return NULL;
            }else if(!root->left || !root->right){
                return root->left?root->left:root->right;
            }else{
                auto small = findSmallest(root->right);
                int temp = small ->val;
                helper(root, temp);
                root->val = temp;
            }
        }else if(root->val < key){
            root->right = helper(root->right, key);
        }else{
            root->left = helper(root->left, key);
        }
        return root;
    }
    TreeNode* findSmallest(TreeNode* root) {
        auto res = root;
        while(res->left){
            res = res->left;
        }
        return res;
    }
};
//Insert a node  to BST
//Have to strore parent pointer
//701. Insert into a Binary Search Tree
class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if(!root){
            TreeNode* node = new TreeNode(val);
            root = node;
            return root;
        }
        helper(root, NULL,val);
        return root;
    }
    void helper(TreeNode* root, TreeNode* p, int val){
        if(!root){
            TreeNode* node = new TreeNode(val);
            if(val < p->val){
                p->left = node;
            }else{
                p->right = node;
            }
            return;
        }
        if(root->val < val) helper(root->right, root, val);
        if(root->val > val) helper(root->left, root, val);
        return;
    }
};
