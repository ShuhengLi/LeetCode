/*763. Partition Labels
 * A string S of lowercase English letters is given. We want to partition this string into as many parts as possible so that each letter appears in at most one part, and return a list of integers representing the size of these parts.



Example 1:

Input: S = "ababcbacadefegdehijhklij"
Output: [9,7,8]
Explanation:
The partition is "ababcbaca", "defegde", "hijhklij".
This is a partition so that each letter appears in at most one part.
A partition like "ababcbacadefegde", "hijhklij" is incorrect, because it splits S into less parts.
*/
class Solution {
public:
    vector<int> partitionLabels(string S) {
        vector<int> finals(26);
        vector<int> res;

        for(int i = 0; i < S.size();i++){
            finals[S[i] - 'a'] = i;
        }
        int start = 0;
        int end = 0;
        for(int i = 0; i < S.size();i++){
            int index = S[i] -'a';
            end = max(end, finals[index]);
            if(i == end){
                res.push_back(end - start+ 1);
                start = i +1;
            }
        }
        return res;
    }
};
