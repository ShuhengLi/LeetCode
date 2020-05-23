void printTreeLevel(TreeNode* root){
  if(!root) return;
  queue<TreeNode*> q;
  q.push(root);
  vector<vector<int>> res;
  while(!q.empty()){
    int s = q.size();
    vector<int> cur;
    for(int i = 0; i < s; i++){
      auto f = q.front(); q.pop();
      cur.push_back(f->val);
      if(f->left) q.push_back(f->left);
      if(f->right) q.push_back(f->right);
    }
    res.push_back(cur);
  }
  return res;
}

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
       vector<vector<int>>  res;
       if(!root) return res;
        deque<TreeNode*> q;
        q.push_front(root);
        bool flag = true;
        while(!q.empty()){
            int s = q.size();
            vector<int> cur;
            for(int i = 0; i < s; i++){
                if(flag){
                    auto f = q.front(); q.pop_front();
                    cur.push_back(f->val);
                    if(f->left) q.push_back(f->left);
                    if(f->right) q.push_back(f->right);
                }else{
                    auto f = q.back(); q.pop_back();
                    cur.push_back(f->val);
                    if(f->right) q.push_front(f->right);
                    if(f->left) q.push_front(f->left);
                }
            }
            flag = 1 - flag;
            res.push_back(cur);
           }
        return res;
    }
};
