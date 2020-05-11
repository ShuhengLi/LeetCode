/*958. Check Completeness of a Binary Tree
 * Given a binary tree, determine if it is a complete binary tree.

Definition of a complete binary tree from Wikipedia:
In a complete binary tree every level, except possibly the last, is completely filled, and all nodes in the last level are as far left as possible. It can have between 1 and 2h nodes inclusive at the last level h.
Input: [1,2,3,4,5,6]
Output: true
Explanation: Every level before the last is full (ie. levels with node-values {1} and {2, 3}), and all nodes in the last level ({4, 5, 6}) are as far left as possible.
*/
class Solution {
public:
    bool isCompleteTree(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty() && q.front()){
            int size = q.size();
            for(int i = 0; i< size; i++){
                auto cur = q.front();
                if(!cur)  break;
                q.pop();
                q.push(cur->left);
                q.push(cur->right);
            }
        }
        while(!q.empty() && !q.front()){
            q.pop();
        }
        return q.empty();
    }
};
