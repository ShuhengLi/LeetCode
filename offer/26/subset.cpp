void subset(string s, int level, string& cur){
  if(level == s.size()){
    res.push_back(cur);
    return;
  }
  cur.push_back(s[level]);
  subset(s, level + 1, cur);
  cur.pop_back();
  subset(s, level + 1, cur);
}
/*
416. Partition Equal Subset Sum
Given a non-empty array containing only positive integers, find if the array can be partitioned into two subsets such that the sum of elements in both subsets is equal.

Example 1:

Input: [1, 5, 11, 5]

Output: true

Explanation: The array can be partitioned as [1, 5, 5] and [11].


Example 2:

Input: [1, 2, 3, 5]

Output: false

Explanation: The array cannot be partitioned into equal sum subsets.

*/
//Doesn'tt pass system because time
class Solution {
public:

    long half;
    bool canPartition(vector<int>& nums) {
        long sum = accumulate(nums.begin(), nums.end(), 0);
        if(sum % 2) return false;
        half = sum / 2;
        vector<int> cur;
        return help(nums, cur, 0);

    }

    bool help(vector<int>& nums, vector<int>& cur, int i){
        if(i >= nums.size()){
            return check(cur);
        }
        cur.push_back(nums[i]);
        bool a = help(nums, cur, i + 1);
        if(a) return a;
        cur.pop_back();
        bool b = help(nums, cur, i + 1);
        return a | b;
    }

    bool check(vector<int>& nums){
        long sum = accumulate(nums.begin(), nums.end(), 0);
        return sum == half;
    }
};

//print out all subset only with k elements
//2.1a solution 1
//time: O(2^n)
class Solution {
public:
    vector<vector<int>> res;

    vector<vector<int>> subsets(vector<int>& nums) {
        vector<int> current;
        helper(nums,current,  0, 2);
        return res;
    }
    //2 basecases
    void helper(vector<int>& nums,vector<int>& current,  int index, int k){
        if(current.size() == k){
            res.push_back(current);
            return;
        }
        if(index >= nums.size()) return;
        current.push_back(nums[index]);
        helper(nums,current,  index + 1, k);
        current.pop_back();
        helper(nums,current, index + 1, k);
    }
};

/*90. Subsets II
Given a collection of integers that might contain duplicates, nums, return all possible subsets (the power set).

Note: The solution set must not contain duplicate subsets.
*/
class Solution {
public:
    set<vector<int>> s;
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        vector<vector<int>> res;
        if(nums.empty()) return res;
        vector<int> cur;
        helper(nums, cur, 0);
        res.resize(s.size());
        copy(s.begin(), s.end(), res.begin());
        return res;
    }
    void helper(vector<int>& nums, vector<int> cur, int i){
        if(i>=nums.size()){
            sort(cur.begin(), cur.end());
            s.insert(cur);
            return;
        }
        cur.push_back(nums[i]);
        helper(nums, cur, i+1);
        cur.pop_back();
        helper(nums, cur, i+1);

    }
};
//method 2 for sorted array
class Solution {
public:
    vector<vector<int>> res;
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        if(nums.empty()) return res;
        vector<int> cur;
        helper(nums, cur, 0);
        return res;
    }
    void helper(vector<int>& nums, vector<int>& cur, int i){
        if(i>=nums.size()){
            res.push_back(cur);
            return;
        }
        cur.push_back(nums[i]);
        helper(nums, cur, i+1);
        cur.pop_back();
        while(i < nums.size() - 1 && nums[i+1] == nums[i] ){
            i++;
        }
        helper(nums, cur, i+1);
    }
};
//2.1e k elements with dupulicate
class Solution {
  public:
    int k
    vector<vector<int>> res;
    vector<vector<int>> subsetsWithDup(vector<int>& nums, int n) {
      k = n;
      sort(nums.begin(), nums.end());
      if(nums.empty()) return res;
      vector<int> cur;
      helper(nums, cur, 0);
      return res;
    }
    void helper(vector<int>& nums, vector<int>& cur, int i){
      if(current.size() == k){
        res.push_back(current);
        return;
      }
      if(index >= nums.size()) return;
      cur.push_back(nums[i]);
      helper(nums, cur, i+1);
      cur.pop_back();
      while(i < nums.size() - 1 && nums[i+1] == nums[i] ){
        i++;
      }
      helper(nums, cur, i+1);
    }
};
