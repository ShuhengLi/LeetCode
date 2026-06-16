/*
 * Shopify Pair Programming - LRU Cache
 * Also: LeetCode 146
 *
 * Problem (Layer 1):
 *   Implement an LRU Cache with get(key) and put(key, value).
 *   Both operations must be O(1).
 *   When capacity is exceeded, evict the least recently used entry.
 *
 * Extensions reported in interviews:
 *   Layer 2: Add TTL (time-to-live) expiry per entry.
 *   Layer 3: Thread-safe version. Swappable storage backend (memory vs. Redis).
 *   Layer 4 (staff): Distributed cache invalidation across multiple nodes.
 *              What consistency model do you need for Shopify's checkout?
 *
 * Core data structure: doubly-linked list + hash map
 *   - Hash map: key -> node pointer for O(1) lookup
 *   - Linked list: maintains LRU order, head=most recent, tail=least recent
 */

#include <iostream>
#include <unordered_map>
#include <list>
#include <chrono>
#include <optional>
#include <stdexcept>
#include <thread>   // FIX: std::this_thread::sleep_for used in runTests()

// ============================================================
// Layer 1: Basic LRU Cache
// ============================================================

class LRUCache {
public:
    explicit LRUCache(int capacity) : capacity(capacity) {
        if (capacity <= 0) throw std::invalid_argument("Capacity must be > 0");
    }

    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) return -1;
        // Move to front (most recently used)
        list.splice(list.begin(), list, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = value;
            list.splice(list.begin(), list, it->second);
            return;
        }
        if ((int)list.size() == capacity) {
            // Evict least recently used (tail)
            map.erase(list.back().first);
            list.pop_back();
        }
        list.emplace_front(key, value);
        map[key] = list.begin();
    }

private:
    int capacity;
    std::list<std::pair<int,int>> list;      // {key, value}
    std::unordered_map<int, std::list<std::pair<int,int>>::iterator> map;
};

// ============================================================
// Layer 2: LRU Cache with TTL
// ============================================================

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;

struct Entry {
    int value;
    TimePoint expiry;
};

class LRUCacheWithTTL {
public:
    explicit LRUCacheWithTTL(int capacity) : capacity(capacity) {}

    std::optional<int> get(int key) {
        auto it = map.find(key);
        if (it == map.end()) return std::nullopt;
        if (isExpired(it->second->second.expiry)) {
            evict(it);
            return std::nullopt;
        }
        list.splice(list.begin(), list, it->second);
        return it->second->second.value;
    }

    // ttlMs = milliseconds, 0 = no expiry
    void put(int key, int value, int ttlMs = 0) {
        TimePoint expiry = ttlMs > 0
            ? Clock::now() + std::chrono::milliseconds(ttlMs)
            : TimePoint::max();

        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = {value, expiry};
            list.splice(list.begin(), list, it->second);
            return;
        }
        if ((int)list.size() == capacity) {
            map.erase(list.back().first);
            list.pop_back();
        }
        list.emplace_front(key, Entry{value, expiry});
        map[key] = list.begin();
    }

private:
    int capacity;
    std::list<std::pair<int, Entry>> list;
    std::unordered_map<int, decltype(list)::iterator> map;

    bool isExpired(const TimePoint& expiry) const {
        return expiry != TimePoint::max() && Clock::now() > expiry;
    }

    void evict(std::unordered_map<int, decltype(list)::iterator>::iterator it) {
        list.erase(it->second);
        map.erase(it);
    }
};

// ============================================================
// Staff Layer 4 Discussion Points
// ============================================================
/*
 * Q: Thread-safe LRU?
 *    - Option A: Global mutex (simple, but all reads/writes serialized)
 *    - Option B: Read-write lock (concurrent reads, exclusive writes)
 *    - Option C: Striped locks per key bucket (best throughput)
 *    - For Shopify's use case: Option B is usually sufficient unless
 *      the cache is on the hot path of checkout.
 *
 * Q: Distributed cache (multiple nodes)?
 *    - Problem: eviction on node A doesn't notify node B.
 *    - Solution 1: Consistent hashing — each key lives on exactly one node.
 *                  No cross-node invalidation needed.
 *    - Solution 2: Redis pub/sub for invalidation events.
 *    - Shopify checkout: needs strong consistency for inventory/pricing.
 *                        Use Redis with write-through, not LRU for those.
 *    - Shopify product catalog: eventual consistency OK, LRU fine.
 *
 * Q: What would you monitor in production?
 *    - Cache hit rate (target > 90% for Shopify product pages)
 *    - Eviction rate (if too high, capacity is undersized)
 *    - Memory pressure
 *    - p99 latency of get/put
 */

// ============================================================
// Tests
// ============================================================

void runTests() {
    {
        LRUCache cache(2);
        cache.put(1, 1);
        cache.put(2, 2);
        std::cout << cache.get(1) << std::endl;  // 1
        cache.put(3, 3);                          // evicts key 2
        std::cout << cache.get(2) << std::endl;  // -1 (evicted)
        cache.put(4, 4);                          // evicts key 1
        std::cout << cache.get(1) << std::endl;  // -1 (evicted)
        std::cout << cache.get(3) << std::endl;  // 3
        std::cout << cache.get(4) << std::endl;  // 4
    }

    {
        LRUCacheWithTTL cache(3);
        cache.put(1, 100, 50); // expires in 50ms
        cache.put(2, 200, 0);  // no expiry
        auto v1 = cache.get(1);
        std::cout << "Before TTL: " << (v1 ? std::to_string(*v1) : "null") << std::endl; // 100

        std::this_thread::sleep_for(std::chrono::milliseconds(60));
        v1 = cache.get(1);
        std::cout << "After TTL: " << (v1 ? std::to_string(*v1) : "null") << std::endl;  // null
        auto v2 = cache.get(2);
        std::cout << "No expiry: " << (v2 ? std::to_string(*v2) : "null") << std::endl;  // 200
    }
}

int main() {
    runTests();
    return 0;
}
