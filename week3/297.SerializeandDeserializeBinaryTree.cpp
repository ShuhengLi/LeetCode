/*297. Serialize and Deserialize Binary Tree
 * Serialization is the process of converting a data structure or object into a sequence of bits so that it can be stored in a file or memory buffer, or transmitted across a network connection link to be reconstructed later in the same or another computer environment.

Design an algorithm to serialize and deserialize a binary tree. There is no restriction on how your serialization/deserialization algorithm should work. You just need to ensure that a binary tree can be serialized to a string and this string can be deserialized to the original tree structure.

Example:

You may serialize the following tree:

    1
   / \
  2   3
     / \
    4   5

as "[1,2,3,null,null,4,5]"
*/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:
    queue<string> q;
    string serialize(TreeNode* root) {
        if(root == NULL)
            return "X#";
        string temp = to_string(root->val) + "#";
        temp+=serialize(root->left) + serialize(root->right);
        return temp;
    }
    TreeNode* deserializeHelper(queue<string> & q){
        if(q.empty())
            return NULL;
        if(q.front() == "X"){
            q.pop();
            return NULL;
        }
        TreeNode* root = new TreeNode(stoi(q.front()));
        q.pop();
        root->left = deserializeHelper(q);
        root->right = deserializeHelper(q);
        return root;

    }

    TreeNode* deserialize(string data) {
        stringstream ss(data);
        string token;
        while(std::getline(ss, token, '#'))
            q.push(token);
        return deserializeHelper(q);
    }
};
