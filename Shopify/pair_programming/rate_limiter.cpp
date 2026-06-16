/*
 * Shopify Pair Programming - Rate Limiter
 *
 * Problem (Layer 1):
 *   Implement a rate limiter that allows at most N requests per time window.
 *   isAllowed(userId) -> bool
 *
 * Extensions reported in interviews:
 *   Layer 2: Sliding window (not fixed window).
 *   Layer 3: Per-user AND global rate limits. Redis-backed implementation.
 *   Layer 4 (staff): Distributed rate limiting across multiple API gateway nodes.
 *              Shopify serves Black Friday: 100k req/s. How does this scale?
 *
 * Two main algorithms:
 *   Fixed Window Counter  — simple, cheap, but allows 2x burst at window boundary
 *   Sliding Window Log    — accurate but memory O(requests)
 *   Token Bucket          — smooth, allows controlled bursts, Shopify API uses this
 */

#include <iostream>
#include <unordered_map>
#include <deque>
#include <chrono>
#include <mutex>
#include <string>

using Clock = std::chrono::steady_clock;
using Ms = std::chrono::milliseconds;

// ============================================================
// Layer 1: Fixed Window Counter
// ============================================================

class FixedWindowRateLimiter {
public:
    // maxRequests per windowMs milliseconds
    FixedWindowRateLimiter(int maxRequests, int windowMs)
        : maxRequests(maxRequests), windowMs(windowMs) {}

    bool isAllowed(const std::string& userId) {
        // FIX: the old version computed `time_since_epoch().count() % windowMs`,
        // mixing steady_clock's native ticks (nanoseconds) with a millisecond
        // window -> meaningless boundaries. Convert to ms first, then bucket.
        long long nowMs = std::chrono::duration_cast<Ms>(
            Clock::now().time_since_epoch()).count();
        long long windowId = nowMs / windowMs;

        auto& state = counters[userId];
        if (state.windowId != windowId) {
            state.windowId = windowId;
            state.count = 0;
        }
        if (state.count >= maxRequests) return false;
        state.count++;
        return true;
    }

private:
    int maxRequests, windowMs;
    struct WindowState {
        long long windowId = -1;
        int count = 0;
    };
    std::unordered_map<std::string, WindowState> counters;
};

// ============================================================
// Layer 2: Sliding Window Log (accurate)
// ============================================================

class SlidingWindowRateLimiter {
public:
    SlidingWindowRateLimiter(int maxRequests, int windowMs)
        : maxRequests(maxRequests), windowMs(windowMs) {}

    bool isAllowed(const std::string& userId) {
        auto now = Clock::now();
        auto& log = requestLog[userId];

        // Evict entries outside the window
        auto cutoff = now - Ms(windowMs);
        while (!log.empty() && log.front() <= cutoff) {
            log.pop_front();
        }

        if ((int)log.size() >= maxRequests) return false;
        log.push_back(now);
        return true;
    }

private:
    int maxRequests, windowMs;
    std::unordered_map<std::string, std::deque<Clock::time_point>> requestLog;
};

// ============================================================
// Layer 3: Token Bucket (allows controlled bursts — Shopify API model)
// ============================================================

class TokenBucketRateLimiter {
public:
    // refillRate = tokens per second, bucketSize = max burst
    TokenBucketRateLimiter(double refillRatePerSec, int bucketSize)
        : refillRatePerSec(refillRatePerSec), bucketSize(bucketSize) {}

    bool isAllowed(const std::string& userId, int tokens = 1) {
        std::lock_guard<std::mutex> lock(mu);
        auto now = Clock::now();
        auto& bucket = buckets[userId];

        if (bucket.lastRefill == Clock::time_point{}) {
            bucket.tokens = bucketSize;
            bucket.lastRefill = now;
        }

        double elapsed = std::chrono::duration<double>(now - bucket.lastRefill).count();
        bucket.tokens = std::min((double)bucketSize, bucket.tokens + elapsed * refillRatePerSec);
        bucket.lastRefill = now;

        if (bucket.tokens < tokens) return false;
        bucket.tokens -= tokens;
        return true;
    }

private:
    double refillRatePerSec;
    int bucketSize;
    std::mutex mu;
    struct Bucket {
        double tokens = 0;
        Clock::time_point lastRefill;
    };
    std::unordered_map<std::string, Bucket> buckets;
};

// ============================================================
// Staff Layer 4 Discussion Points
// ============================================================
/*
 * Q: Shopify API Gateway, 100k req/s, multiple nodes. How do you rate limit?
 *
 * Problem: if each node has its own counter, a user can hit N*numNodes requests.
 *
 * Solution A: Centralized Redis counter
 *   - INCR key (atomic), EXPIRE on first set
 *   - Adds ~1ms network RTT per request — acceptable for API calls
 *   - Redis single-threaded for counters = no race conditions
 *   - Risk: Redis becomes SPOF -> use Redis Cluster or Sentinel
 *
 * Solution B: Approximate local rate limiting
 *   - Each node limits to N/numNodes
 *   - No network overhead, but requires knowing numNodes (use service discovery)
 *   - Allows some over-limit when nodes unbalanced
 *
 * Solution C: Gossip protocol (eventual consistency)
 *   - Nodes share counter updates asynchronously
 *   - Very low latency, slightly over-allows during propagation delay
 *   - Good for "soft" rate limits (analytics ingestion)
 *
 * Q: Shopify's actual approach (public knowledge):
 *   - Uses leaky bucket / token bucket model
 *   - API key gets a "bucket" of call credits
 *   - Bucket refills at a rate based on plan tier
 *   - Headers returned: X-Shopify-Shop-Api-Call-Limit: current/max
 *
 * Q: What to monitor?
 *   - Rate limit hit rate per merchant (spikes = possible attack or bad client)
 *   - Redis latency p99
 *   - False positive rate (legitimate users throttled)
 */

// ============================================================
// Tests
// ============================================================

void runTests() {
    {
        SlidingWindowRateLimiter limiter(3, 1000); // 3 req per second
        std::cout << "Sliding window:" << std::endl;
        for (int i = 0; i < 4; i++) {
            bool ok = limiter.isAllowed("user1");
            std::cout << "  Request " << (i+1) << ": " << (ok ? "ALLOWED" : "DENIED") << std::endl;
        }
        // First 3 allowed, 4th denied
    }

    {
        TokenBucketRateLimiter limiter(2.0, 5); // 2 tokens/sec, burst up to 5
        std::cout << "Token bucket (burst test):" << std::endl;
        for (int i = 0; i < 6; i++) {
            bool ok = limiter.isAllowed("user1");
            std::cout << "  Request " << (i+1) << ": " << (ok ? "ALLOWED" : "DENIED") << std::endl;
        }
        // First 5 allowed (full bucket), 6th denied
    }
}

int main() {
    runTests();
    return 0;
}
