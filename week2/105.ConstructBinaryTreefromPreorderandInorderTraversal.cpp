/*105. Construct Binary Tree from Preorder and Inorder Traversal
 * Given preorder and inorder traversal of a tree, construct the binary tree.

Note:
You may assume that duplicates do not exist in the tree.

For example, given

preorder = [3,9,20,15,7]
inorder = [9,3,15,20,7]
Return the following binary tree:

    3
   / \
  9  20
    /  \
   15   7
*/
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        return helper(preorder, inorder, 0, preorder.size()-1, 0, inorder.size()-1);
    }
    TreeNode* helper(vector<int>& preorder, vector<int>& inorder, int pres, int pree, int ins, int ine) {
        if(pres > pree) return NULL;
        int val = preorder[pres];
        TreeNode* head = new TreeNode(val);
        int indexin;
        for(int i = ins; i<=ine; i++ ){
            if(inorder[i] == val){
                indexin = i;
            }
        }
        int elementsOfleft = indexin - ins;
        head -> left = helper(preorder, inorder, pres + 1, pres + elementsOfleft, ins, indexin-1);
        head -> right = helper(preorder, inorder, pres + elementsOfleft + 1, pree, indexin+1, ine);
        return head;
    }
};
