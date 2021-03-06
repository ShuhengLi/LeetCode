/*532. K-diff Pairs in an Array
 * Given an array of integers and an integer k, you need to find the number of unique k-diff pairs in the array. Here a k-diff pair is defined as an integer pair (i, j), where i and j are both numbers in the array and their absolute difference is k.

Example 1:
Input: [3, 1, 4, 1, 5], k = 2
Output: 2
Explanation: There are two 2-diff pairs in the array, (1, 3) and (3, 5).
Although we have two 1s in the input, we should only return the number of unique pairs.
Example 2:
Input:[1, 2, 3, 4, 5], k = 1
Output: 4
Explanation: There are four 1-diff pairs in the array, (1, 2), (2, 3), (3, 4) and (4, 5).
*/
class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        if(k < 0) return 0;
        int res = 0;
        map<int, int> m;
        if(k == 0){
            for(auto n : nums){
                if(m.find(n) != m.end() && m[n] == 1) res += 1;
                m[n]++;
            }
            return res;
        }

        for(auto n : nums){
            if(m.find(n) != m.end()) continue;
            if(m.find(n - k) != m.end()) res += m[n-k];
            if(m.find(n + k) != m.end()) res += m[n+k];
            m[n]++;
        }
        return res;

    }
};

