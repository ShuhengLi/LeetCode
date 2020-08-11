/*567. Permutation in String
 * Given two strings s1 and s2, write a function to return true if s2 contains the permutation of s1. In other words, one of the first string's permutations is the substring of the second string.



Example 1:

Input: s1 = "ab" s2 = "eidbaooo"
Output: True
Explanation: s2 contains one permutation of s1 ("ba").
Example 2:

Input:s1= "ab" s2 = "eidboaoo"
Output: False
*/
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        if(s1.size()>s2.size()) return false;
        vector<int> v(128, 0);
        int last = 0;
        for(auto c:s1){
            v[c]++;
            if(v[c]==1) last++;
        }
        int count = 0;
        for(int i = 0; i  < s1.size();i++){
            char c = s2[i];
            v[c]--;
            if(v[c]==0) count++;
        }
        if(count == last) return true;
        for(int i = s1.size(), slow = 0; i< s2.size(); i++){
            char  c = s2[slow++];
            if(v[c] == 0) count--;
            v[c]++;

            c = s2[i];
            v[c]--;
            if(v[c] == 0) count++;
            if(count == last) return true;
        }
        return false;

    }
};
