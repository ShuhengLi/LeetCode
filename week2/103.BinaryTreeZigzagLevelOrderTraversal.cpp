/*103. Binary Tree Zigzag Level Order Traversal
Given a binary tree, return the zigzag level order traversal of its nodes' values. (ie, from left to right, then right to left for the next level and alternate between).

For example:
Given binary tree [3,9,20,null,null,15,7],
    3
   / \
  9  20
    /  \
   15   7
return its zigzag level order traversal as:
[
  [3],
  [20,9],
  [15,7]
]
*/
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> res;
        if(!root) return res;
        queue<TreeNode*> q;
        q.push(root);
        int count = 0;
        while(!q.empty()){
            int size = q.size();
            vector<int> cur;
            for(int i = 0; i < size; i++){
                auto node = q.front(); q.pop();
                cur.push_back(node->val);
                if(node->left) q.push(node->left);
                if(node->right) q.push(node->right);
            }
            if(count%2){
                reverse(cur.begin(), cur.end());
            }
            res.push_back(cur);
            count++;
        }
        return res;        
    }
};
