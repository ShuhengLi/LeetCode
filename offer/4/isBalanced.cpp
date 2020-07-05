/*110. Balanced Binary Tree
Given a binary tree, determine if it is height-balanced.

For this problem, a height-balanced binary tree is defined as:

a binary tree in which the left and right subtrees of every node differ in height by no more than 1.

 

Example 1:

Given the following tree [3,9,20,null,null,15,7]:

    3
   / \
  9  20
    /  \
   15   7
*/
class Solution {
public:
    bool isBalanced(TreeNode* root) {
          if(!root) return true;
        int left = getHeight(root->left);
        int right = getHeight(root->right);
        return abs(left-right) <= 1 && isBalanced(root->left) && isBalanced(root->right);
    }
    int getHeight(TreeNode* root){
        if(!root) return 0;
        int left = getHeight(root->left);
        int right = getHeight(root->right);
        return max(left, right) + 1;
    }
};
//Time: O(nlogN)

class Solution {
public:
    bool isBalanced(TreeNode* root) {
          if(!root) return true;
        int res =  getHeight(root);
        return res != -1;
    }
    int getHeight(TreeNode* root){
        if(!root) return 0;
        int left = getHeight(root->left);
        int right = getHeight(root->right);
        if(left == -1 || right == -1){
            return -1;
        }
        if(abs(left - right) > 1) return -1;
        return max(left, right) + 1;
    }
};
//Time:O(N)
