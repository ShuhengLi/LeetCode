/*
 * 295. Find Median from Data Stream
 * Hard
 * [Atlassian DSA Round — ✅ confirmed Blind/Glassdoor 2024/2025]
 *
 * Design a data structure that supports:
 *   addNum(int num) — adds a number to the stream
 *   findMedian()   — returns the median of all numbers added so far
 *
 * Example:
 *   addNum(1) → median = 1.0
 *   addNum(2) → median = 1.5
 *   addNum(3) → median = 2.0
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach: Two heaps
 *   - maxHeap (left half) : stores the smaller half; top = max of left
 *   - minHeap (right half): stores the larger half;  top = min of right
 *
 *   Invariant: maxHeap.size() == minHeap.size()        (even total)
 *           or maxHeap.size() == minHeap.size() + 1    (odd total)
 *
 *   addNum(n):
 *     1. If n <= maxHeap.top() → push to maxHeap; else → push to minHeap
 *     2. Balance: if diff in sizes > 1, move top of larger heap to smaller
 *
 *   findMedian():
 *     Even  → (maxHeap.top() + minHeap.top()) / 2.0
 *     Odd   → maxHeap.top()
 *
 *   Time: addNum O(log n), findMedian O(1)
 *   Space: O(n)
 *
 * Atlassian context: P50 latency of API calls, Sprint velocity median,
 *   real-time issue resolution time monitoring.
 */
#include <bits/stdc++.h>
using namespace std;

class MedianFinder {
    priority_queue<int> maxHeap;                        // left half (max-heap)
    priority_queue<int, vector<int>, greater<int>> minHeap; // right half (min-heap)

public:
    void addNum(int num) {
        // Step 1: route to correct heap
        if (maxHeap.empty() || num <= maxHeap.top())
            maxHeap.push(num);
        else
            minHeap.push(num);

        // Step 2: rebalance
        if (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top()); maxHeap.pop();
        } else if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top()); minHeap.pop();
        }
    }

    double findMedian() {
        if (maxHeap.size() == minHeap.size())
            return (maxHeap.top() + (double)minHeap.top()) / 2.0;
        return maxHeap.top();
    }
};

/*
 * ── Follow-up (Atlassian onsite) ────────────────────────────────
 * Q: All numbers are in range [0, 100] — can we do better?
 * A: Counting sort array cnt[101]. addNum O(1).
 *    findMedian: prefix-sum scan to find median position O(100) = O(1).
 *    Much faster in practice for bounded integer streams.
 *
 * Q: Sliding window median (LC 480)?
 * A: Two heaps + lazy deletion:
 *    HashMap<int, int> to mark "to-be-deleted" counts.
 *    When popping from heap, skip elements whose count > 0 in the map.
 *    Amortized O(log n) per addNum / removeNum.
 *
 * Q: 99th percentile (P99) instead of median (P50)?
 * A: Generalize: keep a max-heap of size ceil(0.99 × n) and a min-heap
 *    of size floor(0.01 × n). P99 = max-heap.top().
 *    Or use T-Digest / HdrHistogram for production-grade percentile tracking.
 *
 * Q: Thread-safe version?
 * A: Wrap addNum / findMedian with a mutex.
 *    Or use a lock-free approach with CAS on heap nodes (complex).
 */
