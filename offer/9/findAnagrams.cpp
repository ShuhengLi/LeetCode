/*438. Find All Anagrams in a String
 * Given a string s and a non-empty string p, find all the start indices of p's anagrams in s.

Strings consists of lowercase English letters only and the length of both strings s and p will not be larger than 20,100.

The order of output does not matter.

Example 1:

Input:
s: "cbaebabacd" p: "abc"

Output:
[0, 6]

Explanation:
The substring with start index = 0 is "cba", which is an anagram of "abc".
The substring with start index = 6 is "bac", which is an anagram of "abc".
*/
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int>  res;
        map<char, int> m;
        for(auto c:p){
            m[c]++;
        }
        int count = m.size();
        for(int i = 0; i< p.size(); i++){
            auto c = s[i];
            if(m.find(c) != m.end()){
                m[c]--;
                if(m[c] == 0){
                    count--;
                }
                if(count==0){
                    res.push_back(0);
                }
            }
        }
        int slow = 0;
        for(int i = p.size(); i < s.size(); i++){
            if(m.find(s[slow])!=m.end()){
                if(m[s[slow]]==0){
                    count++;
                }
                m[s[slow]]++;
            }
            slow++;

            auto c = s[i];
            if(m.find(c) != m.end()){
                m[c]--;
                if(m[c] == 0){
                    count--;
                }
                if(count==0){
                    res.push_back(slow);
                }
            }
        }
        return res;
    }
};
//Time:O(N)
