/*545. Boundary of Binary Tree 
 * Given a binary tree, return the values of its boundary in anti-clockwise direction starting from root. Boundary includes left boundary, leaves, and right boundary in order without duplicate nodes.

Left boundary is defined as the path from root to the left-most node. Right boundary is defined as the path from root to the right-most node. If the root doesn't have left subtree or right subtree, then the root itself is left boundary or right boundary. Note this definition only applies to the input binary tree, and not applies to any subtrees.

The left-most node is defined as a leaf node you could reach when you always firstly travel to the left subtree if exists. If not, travel to the right subtree. Repeat until you reach a leaf node.

The right-most node is also defined by the same way with left and right exchanged.

Example 2

Input:
    ____1_____
   /          \
  2            3
 / \          /
4   5        6
   / \      / \
  7   8    9  10

Ouput:
[1,2,4,7,8,9,10,6,3]
*/
class Solution {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        if (root->left || root->right) res.push_back(root->val);
        leftBoundary(root->left, res);
        leaves(root, res);
        rightBoundary(root->right, res);
        return res;
    }
    void leftBoundary(TreeNode* node, vector<int>& res) {
        if (!node || (!node->left && !node->right)) return;
        res.push_back(node->val);
        if (node->left) leftBoundary(node->left, res);
        else leftBoundary(node->right, res);
    }
    void rightBoundary(TreeNode* node, vector<int>& res) {
        if (!node || (!node->left && !node->right)) return;
        if (node->right) rightBoundary(node->right, res);
        else rightBoundary(node->left, res);
        res.push_back(node->val);
    }
    void leaves(TreeNode* node, vector<int>& res) {
        if (!node) return;
        if (!node->left && !node->right) {
            res.push_back(node->val);
        }
        leaves(node->left, res);
        leaves(node->right, res);
    }
};

//bfs
class Solution {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        vector<int> res, right;
        TreeNode *l = root->left, *r = root->right, *p = root;
        if (root->left || root->right) res.push_back(root->val);
        while (l && (l->left || l->right)) {
            res.push_back(l->val);
            if (l->left) l = l->left;
            else l = l->right;
        }
        stack<TreeNode*> st;
        while (p || !st.empty()) {
            if (p) {
                st.push(p);
                if (!p->left && !p->right) res.push_back(p->val);
                p = p->left;
            } else {
                p = st.top(); st.pop();
                p = p->right;
            }
        }
        while (r && (r->left || r->right)) {
            right.push_back(r->val);
            if (r->right) r = r->right;
            else r = r->left;
        }
        res.insert(res.end(), right.rbegin(), right.rend());
        return res;
    }
};
