/*268. Missing Number
 * Given an array containing n distinct numbers taken from 0, 1, 2, ..., n, find the one that is missing from the array.

Example 1:

Input: [3,0,1]
Output: 2
*/
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = nums.size();
        int total = (1 + n) * n / 2;
        for(auto num : nums){
            total -= num;
        }
        return total;
    }
};
//Time:O(n)
//Space:O(n)
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        set<int> s;
        for(int i = 0; i <= nums.size();i++){
            s.insert(i);
        }
        for(auto n:nums){
            s.erase(n);
        }
        auto it=s.begin();
        return *it;
    }
};
//xor
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int res = 0;
        for(int i = 1; i <= nums.size();i++){
            res = res ^ i;
        }
        for(auto n:nums){
            res = res ^n;
        }
        return res;
    }
};
