/*678. Valid Parenthesis String
 *
 * Given a string containing only three types of characters: '(', ')' and '*', write a function to check whether this string is valid. We define the validity of a string by these rules:

Any left parenthesis '(' must have a corresponding right parenthesis ')'.
Any right parenthesis ')' must have a corresponding left parenthesis '('.
Left parenthesis '(' must go before the corresponding right parenthesis ')'.
'*' could be treated as a single right parenthesis ')' or a single left parenthesis '(' or an empty string.
An empty string is also valid.
Example 1:
Input: "()"
Output: True
Example 2:
Input: "(*)"
Output: True
*/
class Solution {
public:
    bool checkValidString(string s) {
        if(s.empty()) return true;
        int left = 0 ;
        int star = 0;
        for(auto c : s){
            if(c == '('){
                left++;
            }else if( c == '*'){
                star++;
            }else{
                if(left){
                    left--;
                }else if(star){
                    star--;
                }else{
                    return false;
                }
            }
        }
        if(!left) return true;
        left = 0 ;
        star = 0;
        for(int i = s.size()-1; i >= 0; i--){
            auto c = s[i];
            if(c == ')'){
                left++;
            }else if( c == '*'){
                star++;
            }else{
                if(left){
                    left--;
                }else if(star){
                    star--;
                }else{
                    return false;
                }
            }
        }
        if(left && star >= left) left =0;
        return left == 0;
    }
};
//Scan from left to right first, then scan it again reversely
