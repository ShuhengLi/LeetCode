/* Binary Tree Longest Consecutive Sequence II
 * Given a binary tree, find the length(number of nodes) of the longest consecutive sequence(Monotonic and adjacent node values differ by 1) path.
The path could be start and end at any node in the tree

Have you met this question in a real interview?
Example
Example 1:

Input:
{1,2,0,3}
Output:
4
Explanation:
    1
   / \
  2   0
 /
3
0-1-2-3
*/
class Solution {
public:
    /**
     * @param root: the root of binary tree
     * @return: the length of the longest consecutive sequence path
     */
    int res;
    int longestConsecutive2(TreeNode * root) {
        if(!root) return 0;
        res = 0;
        helper(root, root);//bug: parent is itself
        // write your code here
        return res;
    }
    pair<int, int> helper(TreeNode* root, TreeNode* p){
        if(!root) return {0, 0};
        auto l = helper(root->left, root);
        auto r = helper(root->right, root);
        res = max(res, l.first + r.second + 1);
        res = max(res, r.first + l.second + 1);
        int a = 0, b = 0;

        if(root->val == p->val + 1) a = max(l.first, r.first) + 1;
        else if(root->val == p->val - 1) b = max(l.second, r.second) + 1;
        return {a, b};
    }
};
