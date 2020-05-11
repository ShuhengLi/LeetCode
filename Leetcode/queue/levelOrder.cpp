/*102. Binary Tree Level Order Traversal
 * Given a binary tree, return the level order traversal of its nodes' values. (ie, from left to right, level by level).

For example:
Given binary tree [3,9,20,null,null,15,7],
    3
   / \
  9  20
    /  \
   15   7
return its level order traversal as:
[
  [3],
  [9,20],
  [15,7]
]
*/
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        vector<int> current;
        queue<TreeNode*> q;
        if(!root) return res;
        q.push(root);
        while(!q.empty()){
            int size = q.size();
            for(int i = 0; i< size;i++){
                auto front = q.front(); q.pop();
                current.push_back(front->val);
                if(front->left) q.push(front->left);
                if(front->right) q.push(front->right);
            }
            res.push_back(current);
            current.clear();
        }
        return res;
    }
};
