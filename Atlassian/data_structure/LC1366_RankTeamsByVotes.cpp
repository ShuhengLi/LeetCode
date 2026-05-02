/*
 * 1366. Rank Teams by Votes
 * Medium
 * [Atlassian Frequently Asked - Voting System]
 *
 * In a special ranking system, each voter gives a rank from highest to lowest to all
 * teams participating in the competition.
 *
 * The ordering of teams is decided by who received the most position-one votes.
 * If two or more teams tie on position-one votes, consider position-two votes, etc.
 * If still tied, rank alphabetically.
 *
 * Example 1:
 * Input: votes = ["ABC","ACB","ABC","ACB","ACB"]
 * Output: "ACB"
 * Explanation: Team A was ranked first by 5 voters. Team B was ranked second by 2
 * voters and third by 3 voters. Team C was ranked second by 3 voters and third by 2.
 *
 * Example 2:
 * Input: votes = ["WXYZ","XYZW"]
 * Output: "XWYZ"
 *
 * Constraints:
 * - 1 <= votes.length <= 1000
 * - 1 <= votes[i].length <= 26
 * - votes[i].length == votes[j].length for all i, j
 * - votes[i][j] is an English uppercase letter
 * - All characters of votes[i] are unique
 * - All the characters in votes[0] appear in all votes[i]
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Count votes per position, then custom sort
    // Time: O(n * k + k^2 * log k) where n = voters, k = teams, Space: O(k^2)
    string rankTeams(vector<string>& votes) {
        int numTeams = votes[0].size();
        unordered_map<char, vector<int>> voteCount;

        for (char c : votes[0]) {
            voteCount[c].assign(numTeams, 0);
        }

        for (const string& vote : votes) {
            for (int i = 0; i < (int)vote.size(); i++) {
                voteCount[vote[i]][i]++;
            }
        }

        string teams = votes[0];
        sort(teams.begin(), teams.end(), [&](char a, char b) {
            const auto& countA = voteCount[a];
            const auto& countB = voteCount[b];
            for (int i = 0; i < numTeams; i++) {
                if (countA[i] != countB[i]) return countA[i] > countB[i]; // descending
            }
            return a < b; // alphabetical tiebreaker
        });

        return teams;
    }
};
