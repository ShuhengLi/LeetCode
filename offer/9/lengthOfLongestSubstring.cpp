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
            auto c = s[j];
            if(m.find(c) != m.end()){
                if(m[c] >= i){
                    i = m[c] + 1;
                }
            }
            m[c] = j;
            int temp = j - i + 1;
            res = max(temp, res);
        }
        return res ;
    }
};
//with k dupilicate
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if(s.size()<=1) return s.size();
        map<char, int> m;
        int k = 1;
        int res = 1;
        int slow = 0;
        for(int i = 0; i<s.size(); i++ ){
            m[s[i]] += 1;
            while(m[s[i]] > k){
                m[s[slow++]] -= 1;
            }
            int length = i - slow + 1;
            res = max(res, length);
        }
        return res;
    }
};
