/*257. Binary Tree Paths
 * Given a binary tree, return all root-to-leaf paths.

Note: A leaf is a node with no children.

Example:

Input:

   1
 /   \
2     3
 \
  5

Output: ["1->2->5", "1->3"]

Explanation: All root-to-leaf paths are: 1->2->5, 1->3
*/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<string> res;
    vector<string> binaryTreePaths(TreeNode* root) {
        if(!root) return res;
        string cur;
        preorder(cur, root);
        return res;

    }
    void preorder(string cur, TreeNode* root){
        cur += to_string(root->val);
        if(!root->left && !root->right){
            res.push_back(cur);
            return;
        }
        if(root->left) preorder(cur + "->", root->left);
        if(root->right) preorder(cur + "->", root->right);

    }
};
