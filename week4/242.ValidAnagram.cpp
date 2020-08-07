/*242. Valid Anagram
 * Given two strings s and t , write a function to determine if t is an anagram of s.

Example 1:

Input: s = "anagram", t = "nagaram"
Output: true
Example 2:

Input: s = "rat", t = "car"
Output: false
*/
class Solution {
public:
    bool isAnagram(string s, string t) {
        map<char, int> m;
        for(auto c:s){
            m[c]++;
        }
        for(auto c:t){
            m[c]--;
        }
        for(auto it = m.begin(); it!=m.end(); it++){
            if(it->second != 0) return false;
        }
        return true;
    }
};
