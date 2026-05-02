/*
 * 362. Design Hit Counter
 * Medium
 * [Atlassian Frequently Asked - Related to Rate Limiter]
 *
 * Design a hit counter which counts the number of hits received in the past 5 minutes
 * (i.e., the past 300 seconds).
 *
 * Your system should accept a timestamp parameter (in seconds granularity), and you may
 * assume that calls are being made to the system in chronological order (i.e., timestamp
 * is monotonically increasing). Several hits may arrive at roughly the same time.
 *
 * Implement:
 *   hit(timestamp) - Record a hit
 *   getHits(timestamp) - Return hits in past 300 seconds
 *
 * Example:
 * Input: ["HitCounter","hit","hit","hit","getHits","hit","getHits","getHits"]
 *        [[],[1],[2],[3],[4],[300],[300],[301]]
 * Output: [null,null,null,null,3,null,4,3]
 *
 * Follow up: What if the number of hits per second could be very large?
 */
#include <bits/stdc++.h>
using namespace std;

class HitCounter {
    // Approach: Circular buffer of 300 slots
    // Time: O(1) amortized for hit, O(300) = O(1) for getHits, Space: O(300) = O(1)
    int times[300];
    int hits[300];

public:
    HitCounter() {
        memset(times, 0, sizeof(times));
        memset(hits, 0, sizeof(hits));
    }

    void hit(int timestamp) {
        int idx = timestamp % 300;
        if (times[idx] != timestamp) {
            times[idx] = timestamp;
            hits[idx] = 1;
        } else {
            hits[idx]++;
        }
    }

    int getHits(int timestamp) {
        int total = 0;
        for (int i = 0; i < 300; i++) {
            if (timestamp - times[i] < 300) {
                total += hits[i];
            }
        }
        return total;
    }
};

// Alternative: Queue-based (simpler, but O(n) space for heavy traffic)
class HitCounterQueue {
    deque<int> q;

public:
    void hit(int timestamp) {
        q.push_back(timestamp);
    }

    int getHits(int timestamp) {
        while (!q.empty() && timestamp - q.front() >= 300) {
            q.pop_front();
        }
        return q.size();
    }
};
