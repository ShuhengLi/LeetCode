/*125. Valid Palindrome
 * Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.

Note: For the purpose of this problem, we define empty string as valid palindrome.

Example 1:

Input: "A man, a plan, a canal: Panama"
Output: true
Example 2:

Input: "race a car"
Output: false
*/
class Solution {
public:
    bool isPalindrome(string s) {
        string newS;
        for(auto c:s){
            if(isalpha(c)){
                newS += toupper(c);
            }else if(isdigit(c)){
                newS += c;
            }
        }
        int i = 0, j= newS.size()-1;
        while(i < j){
            if(newS[i++] != newS[j--]) return false;
        }
        return true;
    }
};
