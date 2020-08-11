/*127. Word Ladder
 * Given two words (beginWord and endWord), and a dictionary's word list, find the length of shortest transformation sequence from beginWord to endWord, such that:

Only one letter can be changed at a time.
Each transformed word must exist in the word list.
Note:

Return 0 if there is no such transformation sequence.
All words have the same length.
All words contain only lowercase alphabetic characters.
You may assume no duplicates in the word list.
You may assume beginWord and endWord are non-empty and are not the same.
Example 1:

Input:
beginWord = "hit",
endWord = "cog",
wordList = ["hot","dot","dog","lot","log","cog"]
*/
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_map<string, unordered_set<string>>m;
        set<string> visited;
        for(auto word:wordList){
            for(int i = 0; i < word.size();i++){
                auto temp = word;
                temp[i] = '*';
                m[temp].insert(word);
            }
        }
        queue<string> q;
        int res = 1;
        q.push(beginWord);
        while(!q.empty()){
            int size = q.size();
            res++;
            for(int i = 0 ;i < size;i++){

            auto cur = q.front(); q.pop();
            if(visited.find(cur) != visited.end()) continue;
            visited.insert(cur);
            for(int i = 0; i < cur.size();i++){
                auto temp = cur;
                temp[i] = '*';
                if(m.find(temp)!=m.end()){
                    if(m[temp].find(endWord) != m[temp].end()) return res;
                    for(auto it = m[temp].begin(); it != m[temp].end(); it++){
                        if(visited.find(*it) == visited.end()){
                            q.push(*it);
                        }
                    }
                }
            }
            }
        }
        return 0;

    }
};
