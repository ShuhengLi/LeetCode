/*3. Longest Substring Without Repeating Characters
 *Given a string, find the length of the longest substring without repeating characters.

Example 1:

Input: "abcabcbb"
Output: 3 
Explanation: The answer is "abc", with the length of 3. 
*/
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if(s.empty() || s.size() == 1) return s.size();
        map<char, int>m;
        int i = 0, j = 0;
        int res = INT_MIN;
        for(int j = 0; j < s.size(); j++){
            if(m.find(s[j]) != m.end()){
                if(m[s[j]] >= i){
                    i = m[s[j]] + 1;
                }
            }
            m[s[j]] = j;
            int temp = j - i + 1;
            cout << temp << endl;
            res = max(temp, res);
            
        }
        return res ;
    }
};
//with k dupilicate
class Solution {
public:
    int lengthOfLongestSubstring(string s, int k) {
        if(s.empty() || s.size() == 1) return s.size();
        map<char, int>m;
        int i = 0, j = 0;
        int res = INT_MIN;
        for(int j = 0; j < s.size(); j++){
            int c = s[j];
            if(m.find(c) != m.end() && m[c] == k){
                while(m[c] == k){
                    m[s[i++]]--;
                }
            }
            m[c] ++;
            int temp = j - i + 1;
            res = max(temp, res);
        }
        return res ;
    }
};
