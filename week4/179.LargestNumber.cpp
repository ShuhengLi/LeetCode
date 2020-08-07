/*179. Largest Number
 * Given a list of non negative integers, arrange them such that they form the largest number.

Example 1:

Input: [10,2]
Output: "210"
Example 2:

Input: [3,30,34,5,9]
Output: "9534330"
*/
class Solution {
public:
    struct mycompare{
        bool operator()(const string& s1, const string& s2){
            string a = s1 + s2;
            string b = s2 + s1;
            return a > b;
        }

    };
    string largestNumber(vector<int>& nums) {
        if(nums.empty()) return "";
        vector<string> temp;
        for(auto n:nums){
            temp.push_back(to_string(n));
        }
        sort(temp.begin(), temp.end(), mycompare());
        string res;
        for(auto s:temp){
            res += s;
        }
        if(res[0] == '0') res = "0";
        return res;
    }
};
