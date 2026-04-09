/*
297. Serialize and Deserialize Binary Tree

Serialization is the process of converting a data structure or object into a
sequence of bits so that it can be stored in a file or memory buffer, or
transmitted across a network connection link to be reconstructed later in the
same or another computer environment.

Design an algorithm to serialize and deserialize a binary tree. There is no
restriction on how your serialization/deserialization algorithm should work.
You just need to ensure that a binary tree can be serialized to a string and
this string can be deserialized to the original tree structure.

Example 1:
  Input: root = [1,2,3,null,null,4,5]
  Output: [1,2,3,null,null,4,5]

Example 2:
  Input: root = []
  Output: []

Constraints:
  - The number of nodes in the tree is in the range [0, 10^4].
  - -1000 <= Node.val <= 1000
*/
#include <string>
#include <queue>
#include <sstream>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// BFS level-order serialization. Null children are encoded as "null".
// Time: O(n), Space: O(n)
class Codec {
public:
    string serialize(TreeNode* root) {
        if (!root) return "";
        string res;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            if (node) {
                res += to_string(node->val) + ",";
                q.push(node->left);
                q.push(node->right);
            } else {
                res += "null,";
            }
        }
        return res;
    }

    TreeNode* deserialize(string data) {
        if (data.empty()) return nullptr;
        stringstream ss(data);
        string token;
        getline(ss, token, ',');
        TreeNode* root = new TreeNode(stoi(token));
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            if (getline(ss, token, ',') && token != "null") {
                node->left = new TreeNode(stoi(token));
                q.push(node->left);
            }
            if (getline(ss, token, ',') && token != "null") {
                node->right = new TreeNode(stoi(token));
                q.push(node->right);
            }
        }
        return root;
    }
};
