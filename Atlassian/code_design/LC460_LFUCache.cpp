/*
 * 460. LFU Cache
 * Hard
 * [Atlassian Code Design Round — ✅ confirmed as LRU follow-up 2024/2025]
 *
 * Design a Least Frequently Used (LFU) cache with capacity k:
 *   - get(key)        : return value or -1; update frequency
 *   - put(key, value) : insert/update; evict the LFU key when at capacity
 *                       (tie-break: evict the LRU among least-frequent keys)
 *
 * Example:
 *   LFUCache(2)
 *   put(1,1), put(2,2)
 *   get(1)           → 1   (freq[1]=2, freq[2]=1)
 *   put(3,3)         → evicts 2 (lowest freq)
 *   get(2)           → -1
 *   get(3)           → 3
 *   put(4,4)         → evicts 1 (freq[1]=2, freq[3]=2, but 3 more recent... wait:
 *                       actually both freq=2; tie-break LRU → evict 1 as older)
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach: keyMap + freqMap + minFreq tracker
 *   keyMap  : key → {value, freq}
 *   freqMap : freq → doubly-linked list of keys (most recent at front)
 *   minFreq : current minimum frequency (for O(1) eviction)
 *
 *   get(key):
 *     Look up keyMap. Remove key from freqMap[freq], add to freqMap[freq+1].
 *     If freqMap[minFreq] is now empty → minFreq++.
 *
 *   put(key, value):
 *     If exists → update value, call get logic to update freq.
 *     If new:
 *       If at capacity → evict LRU from freqMap[minFreq] (list's back).
 *       Insert into keyMap with freq=1. Add to freqMap[1].
 *       minFreq = 1.
 *
 *   Time: O(1) per get and put   Space: O(capacity)
 *
 * Note: This is the most common "LRU follow-up" at Atlassian.
 *       Interviewers often say: "Now change eviction from LRU to LFU."
 */
#include <bits/stdc++.h>
using namespace std;

class LFUCache {
    int cap, minFreq;
    unordered_map<int, pair<int,int>> keyMap;          // key → {val, freq}
    unordered_map<int, list<int>> freqMap;             // freq → list of keys (MRU front)
    unordered_map<int, list<int>::iterator> iterMap;   // key → iterator in its freq-list

    void touch(int key) {
        int freq = keyMap[key].second;
        keyMap[key].second++;

        freqMap[freq].erase(iterMap[key]);
        if (freqMap[freq].empty()) {
            freqMap.erase(freq);
            if (minFreq == freq) minFreq++;
        }

        freqMap[freq + 1].push_front(key);
        iterMap[key] = freqMap[freq + 1].begin();
    }

public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}

    int get(int key) {
        if (!keyMap.count(key)) return -1;
        touch(key);
        return keyMap[key].first;
    }

    void put(int key, int value) {
        if (cap <= 0) return;

        if (keyMap.count(key)) {
            keyMap[key].first = value;
            touch(key);
            return;
        }

        if ((int)keyMap.size() == cap) {
            // Evict LRU among minFreq keys (back of freqMap[minFreq] list)
            int evict = freqMap[minFreq].back();
            freqMap[minFreq].pop_back();
            if (freqMap[minFreq].empty()) freqMap.erase(minFreq);
            keyMap.erase(evict);
            iterMap.erase(evict);
        }

        keyMap[key] = {value, 1};
        freqMap[1].push_front(key);
        iterMap[key] = freqMap[1].begin();
        minFreq = 1;
    }
};

/*
 * ── Follow-up (Atlassian onsite) ────────────────────────────────
 * Q: LFU vs LRU — when to use which?
 * A: LRU: better for temporal locality (recently used ≈ soon-to-be-used).
 *    LFU: better for frequency locality (popular content stays longer).
 *    Real caches: W-TinyLFU (Caffeine) = LFU for long-term + LRU window.
 *
 * Q: Minimum frequency can be 0 after eviction — how to reset?
 * A: After eviction and new insert, minFreq is always reset to 1
 *    because the new element starts with freq=1.
 *    So minFreq = 1 at end of every new put().
 *
 * Q: What if multiple keys have same minFreq and same last-access time?
 * A: The list (freqMap[minFreq]) maintains insertion order with MRU at front.
 *    We always evict from the back (LRU among equal-freq keys).
 *    This is guaranteed by push_front on access and pop_back on eviction.
 *
 * Q: Thread-safe version?
 * A: Use a single mutex around get/put, or use segment locking on key ranges.
 *    Production: Caffeine uses lock-free ring buffers + striped counters.
 */
