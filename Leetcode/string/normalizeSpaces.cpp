//______aaa______bb____c_________ => a_b_c
//
class Solution {
public:
    string normalizeSpace(string s) {
        int i = 0;
        int j = 0;
        int count = 0;
        while(j < s.size()){
            while(j < s.size() && s[j] == ' '){
                j++;
            }
            if(j >= s.size()) break;
            if(count) s[i++]=' ';
            while(j < s.size() && s[j] != ' '){
                s[i++] = s[j++];
            }
            count++;
        }
        return s.substr(0, i);// s.resize(i);
    }
};
//Time: O(N)
//Space: O(1)
