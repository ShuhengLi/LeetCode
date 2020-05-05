/*Given an input string, reverse the string word by word.

 

Example 1:

Input: "the sky is blue"
Output: "blue is sky the"
Example 2:

Input: "  hello world!  "
Output: "world! hello"
Explanation: Your reversed string should not contain leading or trailing spaces.
Example 3:

Input: "a good   example"
Output: "example good a"
Explanation: You need to reduce multiple spaces between two words to a single space in the reversed string.
*/
// I love Yahoo

class Solution {
public:
    string reverseWords(string s) {
        normalizeSpace(s);
        reverse(s, 0, s.size()-1);
        int i = 0;
        int j = 0;
        while(j < s.size()){
            while(j < s.size() && s[j]!= ' ') {
                j++;
            }
            reverse(s, i, j - 1);
            j++;
            i = j;
        }
        return s;
    }
    void normalizeSpace(string& s) {
        int i = 0;
        int j = 0;
        int count = 0;
        while(j < s.size()){
            while(j < s.size() && s[j] == ' ') j++;
            if(j == s.size()) break;
            if(count) s[i++] = ' ';
            while(j < s.size() && s[j] != ' ') s[i++] = s[j++];
            count++;
        }
        s.resize(i);
        return;
    }
    void reverse(string& s, int i, int j){
        while(i < j){
            swap(s[i++], s[j--]);
        }
    }
};
//Time: O(N)
//Space: O(1)
