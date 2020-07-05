/* 1446. Consecutive Characters
Given a string s, the power of the string is the maximum length of a non-empty substring that contains only one unique character.

Return the power of the string.

 

Example 1:

Input: s = "leetcode"
Output: 2
Explanation: The substring "ee" is of length 2 with the character 'e' only.
Example 2:

Input: s = "abbcccddddeeeeedcba"
Output: 5
Explanation: The substring "eeeee" is of length 5 with the character 'e' only.
*/
class Solution {
public:
    int maxPower(string s) {
        int res = 0;
        if(s.empty()) return res;
        int j = 0;
        for(int i = 0; i < s.size(); i++){
            while(s[i] == s[j]){
                i++;
            }
            int cur = i - j;
            res = max(res, cur);
            j = i;
        }
        return res;
    }
};
