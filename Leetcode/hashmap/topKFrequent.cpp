/*
Given a non-empty array of integers, return the k most frequent elements.

Example 1:

Input: nums = [1,1,1,2,2,3], k = 2
Output: [1,2]
Example 2:

Input: nums = [1], k = 1
Output: [1]
*/
class Solution {
public:
      vector<int> topKFrequent(vector<int>& nums, int k) {
          unordered_map<int, int> countMap;
          vector<int> res;
          priority_queue<pair<int, int>> q;

          for(auto element:nums){
              countMap[element]++;
          }
          for(auto element:countMap){
              q.push(make_pair(element.second, element.first));
          }
          for(int i = 0; i < k; i++){
              auto top = q.top();
              res.push_back(top.second);
              q.pop();
          }
          return res;
  }
};
//Time:O(N)
//Space: O(3N)
