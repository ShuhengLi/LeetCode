/*20. Valid Parentheses
 * Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.

An input string is valid if:

Open brackets must be closed by the same type of brackets.
Open brackets must be closed in the correct order.
Note that an empty string is also considered valid.

Example 1:

Input: "()"
Output: true
Example 2:

Input: "()[]{}"
Output: true
*/
class Solution {
public:
    bool isValid(string s) {
        map<char, char> m;
        m['('] = ')';
        m['['] = ']';
        m['{'] = '}';
        stack<char> st;
        for(auto c:s){
            if(m.find(c)!=m.end()){
                st.push(c);
            }else{
                if(st.empty() ){
                    return false;
                }
                auto front = st.top();st.pop();
                if(m[front]!=c){
                    return false;
                }
            }
        }
        return st.empty();

    }
};
