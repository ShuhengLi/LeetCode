//Case 2: Straight from root 
//Key: carry a path prefix while traversing the tree
//Q1:find max path from leaf to root
int res = INT_MIN;
int maxPathLeafRoot(TreeNode* root){
  if(!root) return 0;;
  maxPathLeafRoot(root, 0);
  return res;
}

void maxPathLeafRoot(TreeNode* root, int cur){
  if(!root) return; 
  cur += root->val;

  if(root->left && !root->right){
    res = max(res, cur);
    return;
  }

  maxPathLeafRoot(root->left, cur);
  maxPathLeafRoot(root->right, cur);
  return;
}

//max path to k  
/*
437. Path Sum III
You are given a binary tree in which each node contains an integer value.
Find the number of paths that sum to a given value.
The path does not need to start or end at the root or a leaf, but it must go downwards (traveling only from parent nodes to child nodes).
The tree has no more than 1,000 nodes and the values are in the range -1,000,000 to 1,000,000.
Example:

root = [10,5,-3,3,2,null,11,3,-2,null,1], sum = 8

      10
     /  \
    5   -3
   / \    \
  3   2   11
 / \   \
3  -2   1

Return 3. The paths that sum to 8 are:

1.  5 -> 3
2.  5 -> 2 -> 1
3. -3 -> 11
*/
class Solution {
public:
    int res = 0;
    int pathSum(TreeNode* root, int sum) {
        vector<int> v;
        helper(root, sum, v);
        return res;
    }
    void helper(TreeNode* root, int sum, vector<int>& v){
        if(!root) return;
        v.push_back(root->val);
        check(v, sum);
        helper(root->left, sum, v);
        helper(root->right, sum, v);
        revert(v, sum);
        v.pop_back();
    }
    void check( vector<int>& v, int sum){
        int cur = v.back();
        for(int i = 0; i < v.size();i++){
            if(i != v.size() - 1){
                v[i] += cur;
            }
            if(v[i] == sum) res ++;
        }
    }
    void revert( vector<int>& v, int sum){
        int cur = v.back();
        for(int i = 0; i < v.size()-1;i++){
            v[i] -= cur;
        }
    }
};
//Time: O(N^2)
//
//

//Q3: find max path from top to down
int res;
int maxPath(TreeNode* root){
  maxPath(root, 0);
  return res;
}
void maxPath(TreeNode* root, int cur){
  if(!root) return;
  cur =  root->val + (cur > 0)? cur:0;
  res = max(res, cur);
  maxPath(root->left, cur);
  maxPath(root->right, cur);
}
