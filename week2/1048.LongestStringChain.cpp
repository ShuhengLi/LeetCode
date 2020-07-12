/*1048. Longest String Chain
Given a list of words, each word consists of English lowercase letters.

Let's say word1 is a predecessor of word2 if and only if we can add exactly one letter anywhere in word1 to make it equal to word2.  For example, "abc" is a predecessor of "abac".

A word chain is a sequence of words [word_1, word_2, ..., word_k] with k >= 1, where word_1 is a predecessor of word_2, word_2 is a predecessor of word_3, and so on.

Return the longest possible length of a word chain with words chosen from the given list of words.

Example 1:

Input: ["a","b","ba","bca","bda","bdca"]
Output: 4
Explanation: one of the longest word chain is "a","ba","bda","bdca".
*/

class Solution {
public:
    static bool mycompare(string a, string b){
        return a.size() < b.size();
    }

    int longestStrChain(vector<string>& words) {
        if(words.empty()) return 0;
        unordered_map<string, int> m;
        int res = 1;
        for(int i = 0; i < words.size(); i++){
            auto cur = words[i];
            m[cur] = 1;
        }
        sort(words.begin(), words.end(), mycompare);
        for(int i = 0; i< words.size(); i++){
            auto cur = words[i];
            for(int j = 0; j < cur.size(); j++){
                auto temp = cur;
                temp.erase(j, 1);
                if(m.find(temp) != m.end()){
                    m[cur] = max(m[cur], m[temp] + 1);
                }
            }
            res = max(res, m[cur]);
        }
        return res;
    }
};
