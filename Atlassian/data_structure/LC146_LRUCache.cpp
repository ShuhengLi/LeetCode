/*
 * 146. LRU Cache
 * Medium
 * [Atlassian Code Design Round — confirmed in 2025 phone screen reports]
 *
 * Design a Least Recently Used (LRU) cache supporting:
 *   - LRUCache(int capacity)
 *   - int  get(int key)        — return value or -1; mark MRU
 *   - void put(int key, int v) — insert or update; evict LRU if over capacity
 *
 * Both operations must be O(1).
 *
 * ─────────────────────────────────────────────────────────────────
 * Key insight: HashMap + Doubly Linked List
 *   - HashMap<key, list_iterator> for O(1) lookup
 *   - Doubly linked list maintains LRU order:
 *       front = most recently used   |   back = least recently used
 *   - On get: splice node to front
 *   - On put new key when full: pop back, erase from map; push front
 * ─────────────────────────────────────────────────────────────────
 */
#include <bits/stdc++.h>
using namespace std;

class LRUCache {
    int cap;
    list<pair<int,int>> dll;                                    // (key, value)
    unordered_map<int, list<pair<int,int>>::iterator> map_;     // key -> node

public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        auto it = map_.find(key);
        if (it == map_.end()) return -1;
        // splice to front (O(1))
        dll.splice(dll.begin(), dll, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            // Update existing
            it->second->second = value;
            dll.splice(dll.begin(), dll, it->second);
            return;
        }
        // Insert new
        if ((int)dll.size() == cap) {
            // Evict LRU (back)
            auto& [k, v] = dll.back();
            map_.erase(k);
            dll.pop_back();
        }
        dll.emplace_front(key, value);
        map_[key] = dll.begin();
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: Why std::list and not std::deque?
 * A: list iterators are NOT invalidated by other inserts/erases, and we store
 *    iterators in the map. deque iterators ARE invalidated. Hence list.
 *
 * Q: Can we use std::unordered_map alone with timestamps?
 * A: Yes (TS-based eviction) but eviction becomes O(n) to find min ts.
 *    Or std::map<ts, key> + map<key, ts> = O(log n). list+hash is O(1).
 *
 * Q: Thread safety?
 * A: Wrap with std::mutex (simple) or finer-grained per-bucket lock.
 *    For prod-grade, use Caffeine (Java) / RocksDB-like W-TinyLFU.
 *
 * Q: LRU vs LFU vs TinyLFU?
 * A: LRU evicts old; LFU evicts low-freq; TinyLFU does freq sketch + LRU
 *    window — best hit rate in practice.
 *
 * Q: TTL support?
 * A: Add expireAt to each entry. On get, check expiry → erase if dead.
 *    Background sweep for proactive cleanup.
 *
 * Q: Atlassian product angle?
 * A: Permission caching for Jira projects, rendered Confluence page caching,
 *    rate limiter token bucket per user (see RateLimiter_SlidingWindow.cpp).
 */

int main() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << endl;   // 1
    cache.put(3, 3);                // evicts key 2
    cout << cache.get(2) << endl;   // -1
    cache.put(4, 4);                // evicts key 1
    cout << cache.get(1) << endl;   // -1
    cout << cache.get(3) << endl;   // 3
    cout << cache.get(4) << endl;   // 4
    return 0;
}
