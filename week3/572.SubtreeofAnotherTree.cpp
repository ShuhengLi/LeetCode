/*572. Subtree of Another Tree
 * Given two non-empty binary trees s and t, check whether tree t has exactly the same structure and node values with a subtree of s. A subtree of s is a tree consists of a node in s and all of this node's descendants. The tree s could also be considered as a subtree of itself.

Example 1:
Given tree s:

     3
    / \
   4   5
  / \
 1   2
Given tree t:
   4
  / \
 1   2
Return true, because t has the same structure and node values with a subtree of s.
*/
class Solution {
public:
    bool isSubtree(TreeNode* s, TreeNode* t) {
        return findHead(s, t);
    }
    bool findHead(TreeNode* s, TreeNode* t) {
        if(s==NULL) return false;
        bool res = false;
        if(s->val == t->val) {
            res = compare(s, t);
        }
        auto l = findHead(s->left, t);
        auto r = findHead(s->right, t);
        return res || l|| r;
    }
    bool compare(TreeNode* s, TreeNode* t) {
        if(!s && !t) return true;
        if(!s || !t) return false;
        return s->val == t->val && compare(s->left, t->left) && compare(s->right, t->right);
    }
};
