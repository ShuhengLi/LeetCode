/*692. Top K Frequent Words
 * Given a non-empty list of words, return the k most frequent elements.

Your answer should be sorted by frequency from highest to lowest. If two words have the same frequency, then the word with the lower alphabetical order comes first.

Example 1:
Input: ["i", "love", "leetcode", "i", "love", "coding"], k = 2
Output: ["i", "love"]
Explanation: "i" and "love" are the two most frequent words.
    Note that "i" comes before "love" due to a lower alphabetical order.
Example 2:
Input: ["the", "day", "is", "sunny", "the", "the", "the", "sunny", "is", "is"], k = 4
Output: ["the", "is", "sunny", "day"]
Explanation: "the", "is", "sunny" and "day" are the four most frequent words,
    with the number of occurrence being 4, 3, 2 and 1 respectively.
*/
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        vector<string>  res;
        if(k > words.size()) return words;
        if(words.empty()) return res;
        map<string, int> m;
        for(auto s:words){
            m[s]++;
        }
        priority_queue<pair<int, string>> q;
        for(auto it = m.begin(); it!= m.end(); it++){
            q.push(make_pair(it->second, it->first));
        }
        for(int i = 0; i< k; i++){
            auto temp = q.top(); q.pop();
            res.push_back(temp.second);
        }
        return res;
    }
};
