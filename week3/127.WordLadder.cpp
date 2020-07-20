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

Output: 5
Explanation: As one shortest transformation is "hit" -> "hot" -> "dot" -> "dog" -> "cog",
return its length 5.
*/
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_map<string, set<string>>m;
        unordered_set<string> visited;
        wordList.push_back(beginWord);
        for(int i = 0; i < wordList.size(); i++){
            auto word = wordList[i];
            for(int j = 0; j < word.size(); j++ ){
                auto temp = word;
                temp[j] = '*';
                m[temp].insert(word);
            }

        }
        queue<string> q;
        int count = 1;
        q.push(beginWord);
        while(!q.empty()){
            int size = q.size();
            count++;
            for(int i = 0; i < size; i++){
                auto front = q.front(); q.pop();
                if(visited.find(front)!=visited.end()) continue;
                visited.insert(front);
                for(int i = 0; i < front.size(); i++){
                    auto temp = front;
                    temp[i] = '*';
                    for(auto it = m[temp].begin(); it!= m[temp].end(); it++){
                        if(*it == endWord){
                            return count;
                        }
                        if(visited.find(*it)!=visited.end()) continue;
                        q.push(*it);
                    }
                }

            }
        }
        return 0;
    }
};
