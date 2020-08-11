/*76. Minimum Window Substring
 *
Given a string S and a string T, find the minimum window in S which will contain all the characters in T in complexity O(n).

Example:

Input: S = "ADOBECODEBANC", T = "ABC"
Output: "BANC"
Note:

If there is no such window in S that covers all characters in T, return the empty string "".
If there is such window, you are guaranteed that there will always be only one unique minimum window in S.
*/
class Solution {
public:
    string minWindow(string s, string t)
    {

        if(s.size() == 0 || t.size() == 0)
            return "";

        int i = 0, j = 0, f = 0, ans = INT_MAX, temp = 0;
        string res = "";
        unordered_map<char, int> map, maps;

		//Frequency map:
        for(int k=0; k<t.size(); k++)
            map[t[k]]++;

        for(int j=0; j<s.size(); j++)
        {
            if(map.count(s[j]))
            {
                maps[s[j]]++;
                if(map[s[j]] == maps[s[j]])
                    f++;                   //Keeping count for all distinct letters in t to be present in s
            }

            while(f == map.size() && i<=j)
            {
                temp = j-i+1;
                if(ans > temp)
                {
					//Potential solution:

                    ans = temp;
                    res = s.substr(i, ans);
                }
                if(map.count(s[i]))
                {
                    maps[s[i]]--;
                    if(maps[s[i]] < map[s[i]])
                        f--;
                }
                i++;
            }
        }

        return res;
    }
};
