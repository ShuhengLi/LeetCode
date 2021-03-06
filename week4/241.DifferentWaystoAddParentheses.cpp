/*241. Different Ways to Add Parentheses
 * Given a string of numbers and operators, return all possible results from computing all the different possible ways to group numbers and operators. The valid operators are +, - and *.

Example 1:

Input: "2-1-1"
Output: [0, 2]
Explanation:
((2-1)-1) = 0
(2-(1-1)) = 2
Example 2:

Input: "2*3-4*5"
Output: [-34, -14, -10, -10, 10]
Explanation:
(2*(3-(4*5))) = -34
((2*3)-(4*5)) = -14
((2*(3-4))*5) = -10
(2*((3-4)*5)) = -10
(((2*3)-4)*5) = 10
*/
class Solution {
public:
    vector<int> diffWaysToCompute(string input) {
        vector<int> res;
        int n = input.size();
        for(int i = 0; i < n; i++){
            char c = input[i];
            if(c == '+' || c == '-' || c == '*'){
                vector<int> res1 = diffWaysToCompute(input.substr(0, i));
                vector<int> res2 = diffWaysToCompute(input.substr(i + 1));
                for(int n1 : res1){
                    for(int n2 : res2){
                        if(c == '+')
                            res.push_back(n1 + n2);
                        else if(c == '-')
                            res.push_back(n1 - n2);
                        else
                            res.push_back(n1 * n2);
                    }
                }
            }
        }
        if(res.size() == 0)
            res.push_back(stoi(input));
        return res;
    }
};
