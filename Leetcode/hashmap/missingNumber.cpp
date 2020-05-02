/*
Given an array containing n distinct numbers taken from 0, 1, 2, ..., n, find the one that is missing from the array.

Example 1:

Input: [3,0,1]
Output: 2
Example 2:

Input: [9,6,4,2,3,5,7,0,1]
Output: 8
*/

class Solution {
public:
    int missingNumber(vector<int>& nums) {
        set<int> s;
        int size = nums.size();
        for(int i = 0; i < size + 1; i++ ){
            s.insert(i);
        }
        for(auto num:nums){
            s.erase(num);
        }
        auto it = s.begin();
        return *it;
    }
};
//Time:O(n)
//Space:O(n)

//math watchout for overflow
    int missingNumber(vector<int>& nums) {
        int n = nums.size();
        int total = (1 + n) * n / 2;
        for(auto num : nums){
            total -= num;
        }
        return total;
    }

