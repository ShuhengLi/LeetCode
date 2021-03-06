/*
Given a list of words and two words word1 and word2, return the shortest distance between these two words in the list.
For example,
Assume that words = ["practice", "makes", "perfect", "coding", "makes"].
Given word1 = ��coding��, word2 = ��practice��, return 3.
Given word1 = "makes", word2 = "coding", return 1.

Author: Shuheng Li
*/
class Solution {
	public:
		int shortestDistance(String[] words, String word1, String word2) {
			int idx1 = -1, idx2 = -1, distance = Integer.MAX_VALUE;
			for (int i = 0; i < words.length; i++) {
				if (words[i] == word1) {
					idx1 = i;
					if (idx2 != -1) distance = Math.min(distance, idx1 - idx2);
				}
				if (words[i] == word2) {
					idx2 = i;
					if (idx1 != -1) distance = Math.min(distance, idx2 - idx1);
				}
			}
			return distance;
		}
};
