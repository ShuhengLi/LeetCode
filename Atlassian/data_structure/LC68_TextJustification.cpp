/*
 * 68. Text Justification
 * Hard
 * [Atlassian Frequently Asked]
 *
 * Given an array of strings words and a width maxWidth, format the text such that each line
 * has exactly maxWidth characters and is fully (left and right) justified.
 *
 * You should pack as many words as you can in each line. Pad extra spaces ' ' when necessary
 * so that each line has exactly maxWidth characters.
 *
 * Extra spaces between words should be distributed as evenly as possible. If the number of
 * spaces on a line does not divide evenly between words, the empty slots on the left will be
 * assigned more spaces than the slots on the right.
 *
 * For the last line of text, it should be left-justified, and no extra space is inserted
 * between words.
 *
 * Example 1:
 * Input: words = ["This", "is", "an", "example", "of", "text", "justification."], maxWidth = 16
 * Output:
 * [
 *    "This    is    an",
 *    "example  of text",
 *    "justification.  "
 * ]
 *
 * Constraints:
 * - 1 <= words.length <= 300
 * - 1 <= words[i].length <= 20
 * - words[i] consists of only English letters and symbols
 * - 1 <= maxWidth <= 100
 * - words[i].length <= maxWidth
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Greedy line packing + space distribution
    // Time: O(n * maxWidth), Space: O(n * maxWidth)
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        vector<string> result;
        int i = 0, n = words.size();

        while (i < n) {
            // 1. Determine how many words fit on this line
            int j = i;
            int lineLen = words[j].size();
            j++;
            while (j < n && lineLen + 1 + (int)words[j].size() <= maxWidth) {
                lineLen += 1 + words[j].size();
                j++;
            }

            int numWords = j - i;
            int wordsLen = 0;
            for (int k = i; k < j; k++) wordsLen += words[k].size();
            int totalSpaces = maxWidth - wordsLen;

            string line;

            // 2. Last line or single word -> left justify
            if (j == n || numWords == 1) {
                for (int k = i; k < j; k++) {
                    if (k > i) line += ' ';
                    line += words[k];
                }
                // Pad remaining spaces
                line += string(maxWidth - line.size(), ' ');
            } else {
                // 3. Middle lines -> distribute spaces evenly
                int gaps = numWords - 1;
                int spacePer = totalSpaces / gaps;
                int extraSpaces = totalSpaces % gaps;

                for (int k = i; k < j; k++) {
                    line += words[k];
                    if (k < j - 1) {
                        int spaces = spacePer + (k - i < extraSpaces ? 1 : 0);
                        line += string(spaces, ' ');
                    }
                }
            }

            result.push_back(line);
            i = j;
        }

        return result;
    }
};
