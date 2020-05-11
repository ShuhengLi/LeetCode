/*916. Word Subsets
 * We are given two arrays A and B of words.  Each word is a string of lowercase letters.

Now, say that word b is a subset of word a if every letter in b occurs in a, including multiplicity.  For example, "wrr" is a subset of "warrior", but is not a subset of "world".

Now say a word a from A is universal if for every b in B, b is a subset of a.

Return a list of all universal words in A.  You can return the words in any order.



Example 1:

Input: A = ["amazon","apple","facebook","google","leetcode"], B = ["e","o"]
Output: ["facebook","google","leetcode"]
*/
class Solution {
public:
    vector<string> wordSubsets(vector<string>& A, vector<string>& B) {
        map<char, int> global;
        map<char, int> current;
        vector<string> res;
        for(auto s:B){
            current.clear();
            for(auto c : s){
                current[c]++;
                global[c] = max(current[c], global[c]);
            }
        }
        for(auto s:A){
            current = global;
            for(auto c : s){
                if(current.find(c) != current.end()){// FIND FIRST!!!!!!
                    current[c]--;
                }
            }
            if(checkSum(current)) res.push_back(s);
        }
        return res;
    }
    bool checkSum(map<char, int> m){
        int sum = 0;
        for(auto it = m.begin(); it != m.end(); it ++){
            if(it->second > 0) return false; // check be positive
        }
        return sum <= 0;
    }
};
