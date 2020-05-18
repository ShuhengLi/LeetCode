/*98. Validate Binary Search Tree
 * Given a binary tree, determine if it is a valid binary search tree (BST).

Assume a BST is defined as follows:

The left subtree of a node contains only nodes with keys less than the node's key.
The right subtree of a node contains only nodes with keys greater than the node's key.
Both the left and right subtrees must also be binary search trees.


Example 1:

    2
   / \
  1   3

Input: [2,1,3]
Output: true
*/
//test case: [1,1] [INT_MAX]
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return helper(root);
    }
    bool helper(TreeNode* root, long min = LONG_MIN, long max = LONG_MAX){
        if (!root) return true;
        bool left = false, right = false;
        if (root->val > min){
           left = helper(root->left, min, root->val);  
        }
        
        if (root->val < max){
          right = helper(root->right, root->val, max);
        }
        return left&&right;
    }
};
