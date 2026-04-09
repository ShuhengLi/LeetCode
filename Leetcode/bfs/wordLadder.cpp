/*
127. Word Ladder

A transformation sequence from word beginWord to word endWord using a dictionary
wordList is a sequence of words beginWord -> s1 -> s2 -> ... -> sk such that:
  - Every adjacent pair of words differs by a single letter.
  - Every si for 1 <= i <= k is in wordList. Note that beginWord does not need
    to be in wordList.
  - sk == endWord

Given two words, beginWord and endWord, and a dictionary wordList, return the
number of words in the shortest transformation sequence from beginWord to
endWord, or 0 if no such sequence exists.

Example 1:
  Input: beginWord = "hit", endWord = "cog",
         wordList = ["hot","dot","dog","lot","log","cog"]
  Output: 5
  Explanation: "hit" -> "hot" -> "dot" -> "dog" -> "cog"

Example 2:
  Input: beginWord = "hit", endWord = "cog",
         wordList = ["hot","dot","dog","lot","log"]
  Output: 0

Constraints:
  - 1 <= beginWord.length <= 10
  - endWord.length == beginWord.length
  - 1 <= wordList.length <= 5000
  - All words have the same length.
  - All words consist of lowercase English letters.
*/
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

// Standard BFS - treat each word as a node, connect words differing by 1 char.
// Time: O(M^2 * N) where M = word length, N = wordList size
// Space: O(M * N)
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (!dict.count(endWord)) return 0;

        queue<string> q;
        q.push(beginWord);
        int steps = 1;

        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                string word = q.front(); q.pop();
                if (word == endWord) return steps;

                for (int i = 0; i < word.size(); i++) {
                    char orig = word[i];
                    for (char c = 'a'; c <= 'z'; c++) {
                        word[i] = c;
                        if (dict.count(word)) {
                            q.push(word);
                            dict.erase(word);
                        }
                    }
                    word[i] = orig;
                }
            }
            steps++;
        }
        return 0;
    }
};

// ===================== Improved Solution =====================
// Bidirectional BFS - search from both ends, meet in the middle.
// Reduces search space from O(b^d) to O(b^(d/2)) where b=branching, d=depth.
// Time: O(M^2 * N), but much faster in practice
// Space: O(M * N)
class Solution_v2 {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (!dict.count(endWord)) return 0;

        unordered_set<string> front = {beginWord};
        unordered_set<string> back = {endWord};
        dict.erase(beginWord);
        dict.erase(endWord);
        int steps = 1;

        while (!front.empty() && !back.empty()) {
            // Always expand the smaller frontier
            if (front.size() > back.size()) swap(front, back);

            unordered_set<string> nextFront;
            for (string word : front) {
                for (int i = 0; i < word.size(); i++) {
                    char orig = word[i];
                    for (char c = 'a'; c <= 'z'; c++) {
                        word[i] = c;
                        if (back.count(word)) return steps + 1;
                        if (dict.count(word)) {
                            nextFront.insert(word);
                            dict.erase(word);
                        }
                    }
                    word[i] = orig;
                }
            }
            front = nextFront;
            steps++;
        }
        return 0;
    }
};
