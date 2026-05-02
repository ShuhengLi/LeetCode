/*
 * 366. Find Leaves of Binary Tree
 * Medium
 * [Atlassian #1 Most Frequently Asked]
 *
 * Given the root of a binary tree, collect a tree's nodes as if you were doing this:
 * - Collect all the leaf nodes.
 * - Remove all the leaf nodes.
 * - Repeat until the tree is empty.
 *
 * Example 1:
 * Input: root = [1,2,3,4,5]
 *      1
 *     / \
 *    2   3
 *   / \
 *  4   5
 * Output: [[4,5,3],[2],[1]]
 *
 * Example 2:
 * Input: root = [1]
 * Output: [[1]]
 */
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    // Key insight: group nodes by their "height" from the bottom
    // Leaf = height 0, parent of leaf = height 1, etc.
    // Time: O(n), Space: O(n)
    vector<vector<int>> findLeaves(TreeNode* root) {
        vector<vector<int>> result;
        getHeight(root, result);
        return result;
    }

private:
    int getHeight(TreeNode* node, vector<vector<int>>& result) {
        if (!node) return -1;

        int height = 1 + max(
            getHeight(node->left, result),
            getHeight(node->right, result)
        );

        if (height >= (int)result.size()) {
            result.push_back({});
        }

        result[height].push_back(node->val);
        return height;
    }
};
