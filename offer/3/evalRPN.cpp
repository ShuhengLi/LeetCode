/*150. Evaluate Reverse Polish Notation
 * Evaluate the value of an arithmetic expression in Reverse Polish Notation.

Valid operators are +, -, *, /. Each operand may be an integer or another expression.

Note:

Division between two integers should truncate toward zero.
The given RPN expression is always valid. That means the expression would always evaluate to a result and there won't be any divide by zero operation.
Example 1:

Input: ["2", "1", "+", "3", "*"]
Output: 9
Explanation: ((2 + 1) * 3) = 9
Example 2:

Input: ["4", "13", "5", "/", "+"]
Output: 6
Explanation: (4 + (13 / 5)) = 6
*/
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> nums;
        for(int i = 0; i < tokens.size(); i++){
            if(isdigit(tokens[i][0])||(tokens[i][0] =='-' && tokens[i].size() > 1)){
                int n = stoi(tokens[i]);
                nums.push(n);
            }else{
                auto n1 = nums.top();nums.pop();
                auto n2 = nums.top();nums.pop();
                if(tokens[i][0]== '+'){
                    int temp = n1+n2;
                    nums.push(temp);
                }else if(tokens[i][0] == '*'){
                    int temp = n1*n2;
                    nums.push(temp);
                }else if(tokens[i][0] == '/'){
                    int temp = n2/n1;
                    nums.push(temp);
                }else if(tokens[i][0] == '-'){
                    int temp = n2-n1;
                    nums.push(temp);
                }
            }
        }
        return nums.top();
    }
};
