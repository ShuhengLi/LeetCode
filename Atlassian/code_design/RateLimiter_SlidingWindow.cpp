/*
 * Rate Limiter - Sliding Window
 * [Atlassian Classic - Coding Design Round]
 *
 * Problem: Imagine we are building an application that is used by many different customers.
 * We want to avoid one customer being able to overload the system by sending too many
 * requests, so we enforce a per-customer rate limit.
 *
 * The rate limit is defined as:
 *   "Each customer can make at most X requests per Y seconds"
 *
 * Implement a RateLimiter that:
 *   - rateLimit(customerId) -> boolean: returns true if the request is allowed
 *
 * Follow-ups:
 *   1. What if we want different rate limits per customer?
 *   2. How would you handle this in a distributed system?
 *   3. What about memory cleanup for inactive customers?
 *
 * Related LeetCode: 359. Logger Rate Limiter, 362. Design Hit Counter
 */
#include <bits/stdc++.h>
using namespace std;

// Approach 1: Sliding Window Log (exact, uses deque of timestamps)
class RateLimiterSlidingLog {
    int maxRequests;
    long long windowMillis;
    unordered_map<string, deque<long long>> customerRequests;

public:
    RateLimiterSlidingLog(int maxRequests, int windowSeconds)
        : maxRequests(maxRequests), windowMillis(windowSeconds * 1000LL) {}

    bool rateLimit(const string& customerId) {
        long long now = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count();
        auto& timestamps = customerRequests[customerId];

        // Evict expired timestamps
        while (!timestamps.empty() && now - timestamps.front() >= windowMillis) {
            timestamps.pop_front();
        }

        if ((int)timestamps.size() < maxRequests) {
            timestamps.push_back(now);
            return true; // allowed
        }
        return false; // rate limited
    }
};

// Approach 2: Fixed Window Counter (simpler, but has boundary burst issue)
class RateLimiterFixedWindow {
    int maxRequests;
    long long windowMillis;
    unordered_map<string, pair<long long, int>> customerWindows; // {windowStart, count}

public:
    RateLimiterFixedWindow(int maxRequests, int windowSeconds)
        : maxRequests(maxRequests), windowMillis(windowSeconds * 1000LL) {}

    bool rateLimit(const string& customerId) {
        long long now = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count();
        auto& window = customerWindows[customerId];

        if (window.first == 0 || now - window.first >= windowMillis) {
            window = {now, 1};
            return true;
        }

        if (window.second < maxRequests) {
            window.second++;
            return true;
        }
        return false;
    }
};

// Approach 3: Token Bucket (smooth rate, allows controlled bursts)
class RateLimiterTokenBucket {
    int maxTokens;
    double refillRate; // tokens per millisecond
    unordered_map<string, pair<double, long long>> customerBuckets; // {tokens, lastRefillTime}

public:
    RateLimiterTokenBucket(int maxTokens, int refillPerSecond)
        : maxTokens(maxTokens), refillRate(refillPerSecond / 1000.0) {}

    bool rateLimit(const string& customerId) {
        long long now = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count();
        auto it = customerBuckets.find(customerId);

        if (it == customerBuckets.end()) {
            customerBuckets[customerId] = {(double)(maxTokens - 1), now};
            return true;
        }

        auto& [tokens, lastRefill] = it->second;
        double elapsed = now - lastRefill;
        tokens = min((double)maxTokens, tokens + elapsed * refillRate);
        lastRefill = now;

        if (tokens >= 1.0) {
            tokens -= 1.0;
            return true;
        }
        return false;
    }
};
