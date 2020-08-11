/*727. Minimum Window Subsequence
 * Given strings S and T, find the minimum (contiguous) substring W of S, so that T is a subsequence of W.

If there is no such window in S that covers all characters in T, return the empty string "". If there are multiple such minimum-length windows, return the one with the smallest starting index.

 Notice
All the strings in the input will only contain lowercase letters.
The length of S will be in the range [1, 20000].
The length of T will be in the range [1, 100].
Have you met this question in a real interview?
Example
Example 1:

Input：S="jmeqksfrsdcmsiwvaovztaqenprpvnbstl"，T="u"
Output：""
Explanation： unable to match
Example 2:

Input：S = "abcdebdde"， T = "bde"
Output："bcde"
Explanation："bcde" is the answer and "deb" is not a smaller window because the elements of T in the window must occur in order.
*/
class Solution {
public:
    /**
     * @param S: a string
     * @param T: a string
     * @return: the minimum substring of S
     */
    string minWindow(string &S, string &T) {
         int n = S.size();
         int m = T.size();
         int i = 0;
         int j = 0;
         int start = -1;
         int len = INT_MAX;
         while(i < n){
             if(S[i] == T[j]){
                 if(++j == m){
                     int end = i+1;
                     while(--j >= 0){
                         while(S[i--] != T[j]);
                     }
                     i++, j++;
                     if(end - i < len){
                         len = end -i;
                         start = i;
                     }
                 }
             }
             i++;
         }
         return (start == -1) ? "" : S.substr(start, len);
    }
};
