/* 99. Recover Binary Search Tree
Two elements of a binary search tree (BST) are swapped by mistake.

Recover the tree without changing its structure.

Example 1:

Input: [1,3,null,null,2]

   1
  /
 3
  \
   2

Output: [3,1,null,null,2]

   3
  /
 1
  \
   2
   */
class Solution {
public:
    vector<TreeNode*> res;
    void recoverTree(TreeNode* root) {
        dfs(root);
        int first;
        int second;
        int flag = true;
        for(int i = 0; i< res.size(); i++){
            if(flag && i < res.size() - 1 && res[i]->val > res[i+1]->val){
                first = i;
                flag = false;
                continue;
            }
            if(i > 0 && res[i]->val < res[i-1]->val){
                second = i;
            }
        }
        int temp = res[first]->val;
        res[first]->val = res[second]->val;
        res[second]->val = temp;
    }
    void dfs(TreeNode* root) {
        if(!root) return;
        dfs(root->left);
        res.push_back(root);
        dfs(root->right);
    }
};
