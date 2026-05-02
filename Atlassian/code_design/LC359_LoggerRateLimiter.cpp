/*
 * 359. Logger Rate Limiter
 * Easy
 * [Atlassian Frequently Asked]
 *
 * Design a logger system that receives a stream of messages along with their timestamps.
 * Each unique message should only be printed at most every 10 seconds.
 *
 * All messages will come in chronological order. Several messages may arrive at the same timestamp.
 *
 * Implement:
 *   shouldPrintMessage(timestamp, message) -> boolean
 *     Returns true if the message should be printed at the given timestamp, otherwise false.
 *
 * Example:
 * logger.shouldPrintMessage(1, "foo");  // true
 * logger.shouldPrintMessage(2, "bar");  // true
 * logger.shouldPrintMessage(3, "foo");  // false (only 2 seconds since last "foo")
 * logger.shouldPrintMessage(8, "bar");  // false (only 6 seconds since last "bar")
 * logger.shouldPrintMessage(10, "foo"); // false (only 9 seconds since last "foo")
 * logger.shouldPrintMessage(11, "foo"); // true (10 seconds have passed)
 */
#include <bits/stdc++.h>
using namespace std;

class Logger {
    // Simple HashMap approach
    // Time: O(1), Space: O(n) where n = unique messages
    unordered_map<string, int> lastPrinted;

public:
    bool shouldPrintMessage(int timestamp, string message) {
        auto it = lastPrinted.find(message);
        if (it == lastPrinted.end() || timestamp - it->second >= 10) {
            lastPrinted[message] = timestamp;
            return true;
        }
        return false;
    }
};

// Follow-up: Memory-bounded version with cleanup
class LoggerBounded {
    unordered_map<string, int> lastPrinted;
    deque<pair<string, int>> q; // for cleanup

public:
    bool shouldPrintMessage(int timestamp, string message) {
        // Cleanup old entries (older than 10 seconds)
        while (!q.empty() && timestamp - q.front().second >= 10) {
            auto& [msg, ts] = q.front();
            auto it = lastPrinted.find(msg);
            if (it != lastPrinted.end() && it->second == ts) {
                lastPrinted.erase(it);
            }
            q.pop_front();
        }

        auto it = lastPrinted.find(message);
        if (it == lastPrinted.end() || timestamp - it->second >= 10) {
            lastPrinted[message] = timestamp;
            q.push_back({message, timestamp});
            return true;
        }
        return false;
    }
};
