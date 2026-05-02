/*
 * 56. Merge Intervals
 * Medium
 * [Atlassian DSA - Medium Frequency]
 *
 * Given an array of intervals where intervals[i] = [start_i, end_i],
 * merge all overlapping intervals, and return an array of the non-overlapping
 * intervals that cover all the intervals in the input.
 *
 * Examples:
 *   [[1,3],[2,6],[8,10],[15,18]] -> [[1,6],[8,10],[15,18]]
 *   [[1,4],[4,5]]                -> [[1,5]]    (touching counts as overlap)
 *
 * Approach: Sort by start, then sweep
 *   1. Sort intervals by their start time.
 *   2. Iterate; maintain a "current" interval. If next.start <= current.end,
 *      extend current.end = max(current.end, next.end). Else push current
 *      to result and start new current.
 *
 * Time:  O(n log n) — dominated by sorting
 * Space: O(log n) sort stack (output excluded)
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.empty()) return {};

        sort(intervals.begin(), intervals.end(),
             [](const auto& a, const auto& b){ return a[0] < b[0]; });

        vector<vector<int>> result;
        result.push_back(intervals[0]);

        for (size_t i = 1; i < intervals.size(); ++i) {
            auto& last = result.back();
            const auto& cur = intervals[i];

            if (cur[0] <= last[1]) {
                // Overlap → extend
                last[1] = max(last[1], cur[1]);
            } else {
                result.push_back(cur);
            }
        }
        return result;
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: Streaming intervals (online)?
 * A: Use a TreeMap-like structure (std::map<int,int> in C++): on each insert,
 *    find neighboring intervals via lower_bound and merge in place. O(log n) amortized.
 *
 * Q: Insert one new interval into already-sorted list (LC 57)?
 * A: One pass O(n): collect ends < new.start, merge overlapping ones, collect rest.
 *
 * Q: How to detect if any meeting overlaps (boolean)? (LC 252)
 * A: Sort by start, check adjacent: if intervals[i].start < intervals[i-1].end → overlap.
 *
 * Q: Atlassian business angle — Jira Sprint planning?
 * A: Each task has [start, end]. Merge to find total occupied ranges, or
 *    LC 253 "Meeting Rooms II" to find max parallel resources needed.
 */

int main() {
    Solution sol;
    vector<vector<int>> in1 = {{1,3},{2,6},{8,10},{15,18}};
    auto r1 = sol.merge(in1);
    for (auto& iv : r1) cout << "[" << iv[0] << "," << iv[1] << "] ";
    cout << endl;
    // [1,6] [8,10] [15,18]

    vector<vector<int>> in2 = {{1,4},{4,5}};
    auto r2 = sol.merge(in2);
    for (auto& iv : r2) cout << "[" << iv[0] << "," << iv[1] << "] ";
    cout << endl;
    // [1,5]
    return 0;
}
