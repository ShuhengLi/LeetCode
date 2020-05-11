/*890. Find and Replace Pattern
 * You have a list of words and a pattern, and you want to know which words in words matches the pattern.

A word matches the pattern if there exists a permutation of letters p so that after replacing every letter x in the pattern with p(x), we get the desired word.

(Recall that a permutation of letters is a bijection from letters to letters: every letter maps to another letter, and no two letters map to the same letter.)

Return a list of the words in words that match the given pattern.

You may return the answer in any order.



Example 1:

Input: words = ["abc","deq","mee","aqq","dkd","ccc"], pattern = "abb"
Output: ["mee","aqq"]
Explanation: "mee" matches the pattern because there is a permutation {a -> m, b -> e, ...}.
"ccc" does not match the pattern because {a -> c, b -> c, ...} is not a permutation,
since a and b map to the same letter.
*/

class Solution {
public:
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string> res;
        vector<int> next(pattern.size(), 0);// DO NOT -1!!!
        map<char, int> temp;
        map<char, int> check;//char's last postion
        //step 1 gernate next index array
        for(int i = 0; i < pattern.size(); i++){
            auto c = pattern[i];
            next[i] = i;
            if(temp.find(c) != temp.end()){
                int lastIndex = temp[c];
                next[lastIndex] = i;
            }
            temp[c] = i;
        }

        //step 2 check each word.
        for(auto w :words){
            check.clear();
            for(int i = 0; i< w.size();i++){
                if(w[i] != w[next[i]] || (check.find(w[i]) != check.end() && i > check[w[i]])){//check find in the map first
                    break;
                }
                if(i == next[i]) check[w[i]] = i;
                if(i == w.size() - 1){
                    res.push_back(w);
                }
            }
        }
        return res;
    }
};
