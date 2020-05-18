class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        vector<int> current;
        if(!root) return res;
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty()){
            int size = q.size();
            for(int i=0; i<size; i++ ){
                auto top = q.front();q.pop();
                current.push_back(top->val);
                if(top->left) q.push(top->left);
                if(top->right) q.push(top->right);
            }
            res.push_back(current);
            current.clear();
        }
        return res;
    }
};
