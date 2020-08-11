/*340. Longest Substring with At Most K Distinct Characters
 * Given a string S, find the length of the longest substring T that contains at most k distinct characters.

Example
Example 1:

Input: S = "eceba" and k = 3
Output: 4
Explanation: T = "eceb"
Example 2:

Input: S = "WORLD" and k = 4
Output: 4
Explanation: T = "WORL" or "ORLD"
*/
class Solution {
public:
    int lengthOfLongestSubstringKDistinct(string &s, int k) {
        unordered_map<char, int> m;
        int slow = 0;
        int res = 0;
        for(int i = 0; i < s.size(); i++){
            m[s[i]] = i;
            if(m.size() > k){
                while(m[s[slow]] != slow){
                    slow++;
                }
                m.erase(s[slow]);
                slow++;
                //move slow
            }
            res = max(res, i - slow + 1);
        }
        return res;
    }
};
