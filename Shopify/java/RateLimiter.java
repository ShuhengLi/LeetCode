import java.util.*;
import java.util.function.LongSupplier;

/**
 * Shopify Pair Programming - Rate Limiter
 * Reported across Blind, 2024-2025.
 *
 * Java 17.  Run:  javac RateLimiter.java && java RateLimiter
 *
 * Three algorithms, increasing sophistication - all behind one Limiter interface:
 *   FixedWindow   - cheap, but allows up to 2x burst at a window edge
 *   SlidingWindow - accurate, O(requests-in-window) memory
 *   TokenBucket   - smooth + controlled bursts (Shopify's public API model)
 *
 * Time is INJECTED (LongSupplier of millis) so tests are deterministic - no sleeps.
 */
public class RateLimiter {

    interface Limiter { boolean allow(String key); }

    /** Fixed window: bucket by integer window-id. (No unit mixing - the classic bug.) */
    static class FixedWindow implements Limiter {
        private final int max;
        private final long windowMs;
        private final LongSupplier clock;
        private final Map<String, long[]> state = new HashMap<>(); // key -> {windowId, count}

        FixedWindow(int max, long windowMs, LongSupplier clock) {
            this.max = max; this.windowMs = windowMs; this.clock = clock;
        }
        public boolean allow(String key) {
            long windowId = clock.getAsLong() / windowMs;
            long[] s = state.computeIfAbsent(key, k -> new long[]{windowId, 0});
            if (s[0] != windowId) { s[0] = windowId; s[1] = 0; }
            if (s[1] >= max) return false;
            s[1]++;
            return true;
        }
    }

    /** Sliding window log: exact count over the trailing window. */
    static class SlidingWindow implements Limiter {
        private final int max;
        private final long windowMs;
        private final LongSupplier clock;
        private final Map<String, Deque<Long>> log = new HashMap<>();

        SlidingWindow(int max, long windowMs, LongSupplier clock) {
            this.max = max; this.windowMs = windowMs; this.clock = clock;
        }
        public boolean allow(String key) {
            long now = clock.getAsLong();
            Deque<Long> dq = log.computeIfAbsent(key, k -> new ArrayDeque<>());
            long cutoff = now - windowMs;
            while (!dq.isEmpty() && dq.peekFirst() <= cutoff) dq.pollFirst();
            if (dq.size() >= max) return false;
            dq.addLast(now);
            return true;
        }
    }

    /** Token bucket: refill at a steady rate, spend one token per request. */
    static class TokenBucket implements Limiter {
        private final double ratePerMs;
        private final double capacity;
        private final LongSupplier clock;
        private final Map<String, double[]> buckets = new HashMap<>(); // key -> {tokens, lastRefillMs}

        TokenBucket(double refillPerSecond, double capacity, LongSupplier clock) {
            this.ratePerMs = refillPerSecond / 1000.0;
            this.capacity = capacity;
            this.clock = clock;
        }
        public boolean allow(String key) {
            long now = clock.getAsLong();
            double[] b = buckets.computeIfAbsent(key, k -> new double[]{capacity, now});
            double elapsed = now - b[1];
            b[0] = Math.min(capacity, b[0] + elapsed * ratePerMs);   // refill
            b[1] = now;
            if (b[0] < 1.0) return false;
            b[0] -= 1.0;
            return true;
        }
    }

    /* ---- Staff discussion ----
     * Distributed (100k req/s across N gateways): per-node counters let a user do
     *   N x limit. Options: (A) central Redis INCR+EXPIRE (atomic, ~1ms RTT);
     *   (B) local limit of max/N (no network, approximate); (C) gossip (eventual).
     * Shopify's API uses a leaky/token bucket per API key and returns
     *   X-Shopify-Shop-Api-Call-Limit: used/max.
     * Monitor: throttle rate per merchant (spikes = attack or bad client), Redis p99.
     */

    public static void main(String[] args) {
        long[] now = {0};
        LongSupplier clock = () -> now[0];

        FixedWindow fw = new FixedWindow(3, 1000, clock);
        now[0] = 0;
        check("fw 1-3 allow",   fw.allow("u") && fw.allow("u") && fw.allow("u"));
        check("fw 4 blocked",   !fw.allow("u"));
        now[0] = 1000;                                  // next window
        check("fw new window",  fw.allow("u"));

        SlidingWindow sw = new SlidingWindow(3, 1000, clock);
        now[0] = 0;
        sw.allow("u"); sw.allow("u"); sw.allow("u");
        check("sw 4 blocked",   !sw.allow("u"));
        now[0] = 1001;                                  // first three slide out
        check("sw after slide", sw.allow("u"));

        TokenBucket tb = new TokenBucket(2.0, 5, clock); // 2/sec, burst 5
        now[0] = 0;
        check("tb burst 5", tb.allow("u") && tb.allow("u") && tb.allow("u")
                          && tb.allow("u") && tb.allow("u"));
        check("tb 6 blocked",   !tb.allow("u"));
        now[0] = 1000;                                  // +1s => +2 tokens
        check("tb refill 2",    tb.allow("u") && tb.allow("u"));
        check("tb empty again", !tb.allow("u"));
    }

    static void check(String name, boolean pass) {
        System.out.printf("%-16s %s%n", name, pass ? "PASS" : "FAIL");
    }
}
