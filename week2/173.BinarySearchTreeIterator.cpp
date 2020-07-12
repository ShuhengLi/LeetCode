/*173. Binary Search Tree Iterator
 Implement an iterator over a binary search tree (BST). Your iterator will be initialized with the root node of a BST.

Calling next() will return the next smallest number in the BST.

Example:
BSTIterator iterator = new BSTIterator(root);
iterator.next();    // return 3
iterator.next();    // return 7
iterator.hasNext(); // return true
iterator.next();    // return 9
iterator.hasNext(); // return true
iterator.next();    // return 15
iterator.hasNext(); // return true
iterator.next();    // return 20
iterator.hasNext(); // return false
*/
class BSTIterator {
public:
    vector<TreeNode*> v;
    int size;
    int it;
    BSTIterator(TreeNode* root) {
        inOrder(root);
        size = v.size();
        it = 0;
    }
    void inOrder(TreeNode* root) {
        if(!root) return;
        inOrder(root->left);
        v.push_back(root);
        inOrder(root->right);
    }


    /** @return the next smallest number */
    int next() {
        return v[it++]->val;
    }

    /** @return whether we have a next smallest number */
    bool hasNext() {
        return it < size;
    }
};

