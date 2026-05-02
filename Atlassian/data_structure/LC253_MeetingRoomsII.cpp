/*
 * 253. Meeting Rooms II
 * Medium
 * [Atlassian DSA - HIGH Frequency — directly relates to Jira/Calendar]
 *
 * Given an array of meeting time intervals, return the minimum number of
 * conference rooms required to host all meetings without conflict.
 *
 * Example:
 *   [[0,30],[5,10],[15,20]] -> 2
 *   [[7,10],[2,4]]          -> 1
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach 1: Min-heap of end times (most intuitive)
 *   - Sort by start time.
 *   - For each meeting:
 *       * If heap.top() <= cur.start  → reuse a room (pop the freed one)
 *       * Push cur.end onto heap (room now busy until cur.end)
 *   - Answer = heap.size()
 *
 *   Time: O(n log n)   Space: O(n)
 *
 * Approach 2: Chronological ordering / two pointers
 *   - Sort starts and ends separately.
 *   - Walk both with pointers; if start[i] < end[j] need new room, else free one.
 *
 *   Time: O(n log n)   Space: O(n)
 *
 * Approach 3: Sweep line / event count
 *   - +1 at start, -1 at end; sort events; track running max.
 *   - Tie-breaker: end events before start events when t equal (a meeting
 *     ending at 10 and one starting at 10 do NOT overlap).
 * ─────────────────────────────────────────────────────────────────
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Approach 1: min-heap (preferred answer in interview)
    int minMeetingRooms(vector<vector<int>>& intervals) {
        if (intervals.empty()) return 0;

        sort(intervals.begin(), intervals.end(),
             [](const auto& a, const auto& b){ return a[0] < b[0]; });

        priority_queue<int, vector<int>, greater<int>> minHeap;  // ends

        for (const auto& iv : intervals) {
            // If earliest-ending room is free, reuse it
            if (!minHeap.empty() && minHeap.top() <= iv[0]) {
                minHeap.pop();
            }
            minHeap.push(iv[1]);
        }
        return (int)minHeap.size();
    }

    // Approach 3: sweep line — clean & fast
    int minMeetingRoomsSweep(vector<vector<int>>& intervals) {
        // event: (time, +1 start / -1 end). End events tie-break first.
        vector<pair<int,int>> events;
        for (auto& iv : intervals) {
            events.push_back({iv[0], +1});
            events.push_back({iv[1], -1});
        }
        sort(events.begin(), events.end(),
             [](const auto& a, const auto& b){
                 if (a.first != b.first) return a.first < b.first;
                 return a.second < b.second;  // -1 (end) before +1 (start)
             });
        int cur = 0, mx = 0;
        for (auto& [t, d] : events) { cur += d; mx = max(mx, cur); }
        return mx;
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: What if we want which rooms each meeting goes to?
 * A: Pair heap entry as (end, roomId). On reuse, take the popped roomId.
 *
 * Q: Streaming meetings (events arrive online)?
 * A: Maintain a sorted structure of (end → count) — each new start pops all
 *    ends ≤ start; insert end. Heap or balanced BST works.
 *
 * Q: Multiple resources (each meeting needs k rooms)?
 * A: Same idea but increment by k per event. Answer = peak running sum.
 *
 * Q: Atlassian Jira angle — agent / asset booking?
 * A: Replace "rooms" with "agents handling tickets". Sweep gives required
 *    headcount at peak. Used in capacity planning / SLA analysis.
 */

int main() {
    Solution sol;
    vector<vector<int>> v1 = {{0,30},{5,10},{15,20}};
    cout << sol.minMeetingRooms(v1) << endl;       // 2
    cout << sol.minMeetingRoomsSweep(v1) << endl;  // 2

    vector<vector<int>> v2 = {{7,10},{2,4}};
    cout << sol.minMeetingRooms(v2) << endl;       // 1

    vector<vector<int>> v3 = {{1,5},{8,9},{8,9}};
    cout << sol.minMeetingRooms(v3) << endl;       // 2
    return 0;
}
