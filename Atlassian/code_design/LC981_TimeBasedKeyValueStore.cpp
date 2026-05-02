/*
 * 981. Time Based Key-Value Store
 * Medium
 * [Atlassian #2 Most Frequently Asked]
 *
 * Design a time-based key-value data structure that can store multiple values for the
 * same key at different time stamps and retrieve the key's value at a certain timestamp.
 *
 * Implement:
 *   set(key, value, timestamp) - Stores key with value at the given timestamp
 *   get(key, timestamp) - Returns value with the largest timestamp <= given timestamp.
 *                          If no such value, returns ""
 *
 * Example:
 * set("foo", "bar", 1);
 * get("foo", 1);       // "bar"
 * get("foo", 3);       // "bar" (largest ts <= 3 is 1)
 * set("foo", "bar2", 4);
 * get("foo", 4);       // "bar2"
 * get("foo", 5);       // "bar2"
 *
 * Constraints:
 * - 1 <= key.length, value.length <= 100
 * - 1 <= timestamp <= 10^7
 * - All timestamps of set are strictly increasing (for same key)
 * - At most 2 * 10^5 calls to set and get
 */
#include <bits/stdc++.h>
using namespace std;

class TimeMap {
    // map<int,string> is sorted by timestamp - use upper_bound for floor query
    // set: O(log n), get: O(log n)
    unordered_map<string, map<int, string>> store;

public:
    void set(string key, string value, int timestamp) {
        store[key][timestamp] = value;
    }

    string get(string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& tmap = it->second;
        auto pos = tmap.upper_bound(timestamp);
        if (pos == tmap.begin()) return "";
        return prev(pos)->second;
    }
};

// Alternative: Binary search on sorted vectors (slightly faster in practice)
class TimeMapBinarySearch {
    unordered_map<string, vector<pair<int, string>>> store;

public:
    void set(string key, string value, int timestamp) {
        store[key].push_back({timestamp, value});
    }

    string get(string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& vec = it->second;
        // Binary search for largest timestamp <= given timestamp
        int lo = 0, hi = (int)vec.size() - 1, result = -1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (vec[mid].first <= timestamp) {
                result = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        return result >= 0 ? vec[result].second : "";
    }
};
