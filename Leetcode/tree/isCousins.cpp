/*993. Cousins in Binary Tree
 * In a binary tree, the root node is at depth 0, and children of each depth k node are at depth k+1.

Two nodes of a binary tree are cousins if they have the same depth, but have different parents.

We are given the root of a binary tree with unique values, and the values x and y of two different nodes in the tree.

Return true if and only if the nodes corresponding to the values x and y are cousins.

 Input: root = [1,2,3,null,4], x = 2, y = 3
Output: false
*/
class Solution {
public:
    map<TreeNode*, TreeNode*> m;
    bool isCousins(TreeNode* root, int x, int y) {
        parent(root, NULL);
        auto a = find(root, x);
        auto b = find(root, y);
        if(m[a] == m[b]) return false;
        int height = 0;
        while(a != root){ //compare a NOT m[a]!!!
            height++;
            a = m[a];
        }
        while(b != root){
            height--;
            b = m[b];
        }
        return !height;

    }
    void parent(TreeNode* root, TreeNode* p){
        if(!root) return;
        m[root] = p;
        parent(root->left, root);
        parent(root->right, root);
    }
    TreeNode* find(TreeNode* root, int v){
        if(!root||root->val == v) return root;
        auto l = find(root->left, v);
        auto r = find(root->right, v);
        return l != NULL? l:r;
    }
};
//No parent
class Solution {
public:
    bool isCousins(TreeNode* root, int x, int y) {
        vector<int> a;
        vector<int> b;
        find(root, x, a);
        find(root, y, b);
        if(a.size()!=b.size() || a[1] == b[1]) return false;
        return true;
        
    }
    void find(TreeNode* root, int v, vector<int>& vec){
        if(!root) return;
        if(root->val == v) {
            vec.push_back(root->val);
            return;
        }
        find(root->left, v, vec);
        if(!vec.empty()){
            vec.push_back(root->val);  
            return;
        } 
        find(root->right, v, vec);
        if(!vec.empty()) vec.push_back(root->val);
        return;
    }
};
