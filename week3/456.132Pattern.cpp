/*456. 132 Pattern
 * Given a sequence of n integers a1, a2, ..., an, a 132 pattern is a subsequence ai, aj, ak such that i < j < k and ai < ak < aj. Design an algorithm that takes a list of n numbers as input and checks whether there is a 132 pattern in the list.

Note: n will be less than 15,000.

Example 1:
Input: [1, 2, 3, 4]

Output: False

Explanation: There is no 132 pattern in the sequence.
Example 2:
Input: [3, 1, 4, 2]

Output: True

Explanation: There is a 132 pattern in the sequence: [1, 4, 2].
*/
class Solution {
public:
    bool find132pattern(vector<int>& nums) {
        int n = nums.size();
        if(n < 3) return false;
        vector<int> dp(n);
        stack<int> s;
        dp[0] = nums[0];
        for(int i = 1; i < n;i++)
            dp[i] = min(dp[i-1], nums[i]);
        
        for(int i = n-1; i>=0; i--){
            if(nums[i] > dp[i]){
                while(!s.empty() && s.top() <= dp[i]){
                    s.pop();
                }
                if(!s.empty() && s.top() < nums[i]) return true;
                s.push(nums[i]);
            }
        }
        return false;
    }
};
