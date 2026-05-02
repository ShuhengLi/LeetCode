/*
 * 49. Group Anagrams
 * Medium
 * [Atlassian Frequently Asked]
 *
 * Given an array of strings strs, group the anagrams together.
 * You can return the answer in any order.
 *
 * Example 1:
 * Input: strs = ["eat","tea","tan","ate","nat","bat"]
 * Output: [["bat"],["nat","tan"],["ate","eat","tea"]]
 *
 * Example 2:
 * Input: strs = [""]
 * Output: [[""]]
 *
 * Constraints:
 * - 1 <= strs.length <= 10^4
 * - 0 <= strs[i].length <= 100
 * - strs[i] consists of lowercase English letters
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Sort each string as key
    // Time: O(n * k log k) where k = max string length, Space: O(nk)
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> map;
        for (const string& s : strs) {
            string key = s;
            sort(key.begin(), key.end());
            map[key].push_back(s);
        }
        vector<vector<string>> result;
        for (auto& [key, group] : map) {
            result.push_back(group);
        }
        return result;
    }

    // Alternative: character count as key - O(nk) time
    vector<vector<string>> groupAnagramsCountKey(vector<string>& strs) {
        unordered_map<string, vector<string>> map;
        for (const string& s : strs) {
            int count[26] = {};
            for (char c : s) count[c - 'a']++;
            string key(count, count + 26);
            map[key].push_back(s);
        }
        vector<vector<string>> result;
        for (auto& [key, group] : map) {
            result.push_back(group);
        }
        return result;
    }
};
