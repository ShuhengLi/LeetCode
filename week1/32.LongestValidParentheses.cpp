/* 32. Longest Valid Parentheses
Given a string containing just the characters '(' and ')', find the length of the longest valid (well-formed) parentheses substring.

Example 1:

Input: "(()"
Output: 2
Explanation: The longest valid parentheses substring is "()"
Example 2:

Input: ")()())"
Output: 4
Explanation: The longest valid parentheses substring is "()()"
*/
class Solution {
public:
    int longestValidParentheses(string s) {
        int res = 0;
        stack<int> st;
        vector<int> dp(s.size());
        int last = 0;
        int cur = 0;
        for(int i = 0; i<s.size(); i++){
            if(s[i]=='('){
                dp[i] = 0;
                st.push(i);
            }else{
                if(st.empty()){
                    last = 0;
                }else{
                    int top = st.top();
                    if(top == 0){
                        last = dp[i-1]+1;
                    }else if(s[top-1] == '('){
                        last= max(dp[st.top()-1], dp[i-1])+1;
                    }else{
                        last= dp[st.top()-1] + dp[i-1]+1;
                    }
                    st.pop();
                }
                dp[i]=last;
            }
            res=max(res,dp[i]);
        }
        return res*2;

    }
};
