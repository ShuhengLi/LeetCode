import java.util.*;
import java.util.function.LongSupplier;

/**
 * Shopify Pair Programming - LRU Cache (LeetCode 146)
 * Confirmed: Jul/Aug 2024, Jan 2026.
 *
 * Java 17.  Run:  javac LRUCache.java && java LRUCache
 *
 * Layer 1: get/put in O(1), evict least-recently-used (manual DLL + HashMap)
 * Layer 2: TTL expiry. The clock is INJECTED (Dependency Inversion) so tests
 *          are deterministic - no Thread.sleep, no flakiness.
 * Layer 3 (discussion): swappable backend, thread-safety.
 *
 * Why a manual doubly-linked list in Layer 1 instead of LinkedHashMap?
 *   It proves you understand the O(1) invariant. Then SAY OUT LOUD that in
 *   production you'd use LinkedHashMap(accessOrder=true) - shown in Layer 2.
 */
public class LRUCache {

    /** Layer 1: generic, type-safe, hand-rolled DLL to demonstrate the mechanics. */
    static class Cache<K, V> {
        private final int capacity;
        private final Map<K, Node<K, V>> map = new HashMap<>();
        private final Node<K, V> head = new Node<>(null, null); // sentinel: most-recent side
        private final Node<K, V> tail = new Node<>(null, null); // sentinel: least-recent side

        Cache(int capacity) {
            if (capacity <= 0) throw new IllegalArgumentException("capacity must be > 0");
            this.capacity = capacity;
            head.next = tail;
            tail.prev = head;
        }

        V get(K key) {
            Node<K, V> n = map.get(key);
            if (n == null) return null;
            moveToFront(n);
            return n.value;
        }

        void put(K key, V value) {
            Node<K, V> n = map.get(key);
            if (n != null) { n.value = value; moveToFront(n); return; }
            if (map.size() == capacity) evictLast();
            Node<K, V> node = new Node<>(key, value);
            map.put(key, node);
            addFront(node);
        }

        int size() { return map.size(); }

        private void addFront(Node<K, V> n) {
            n.prev = head; n.next = head.next;
            head.next.prev = n; head.next = n;
        }
        private void remove(Node<K, V> n) { n.prev.next = n.next; n.next.prev = n.prev; }
        private void moveToFront(Node<K, V> n) { remove(n); addFront(n); }
        private void evictLast() {
            Node<K, V> lru = tail.prev;
            remove(lru);
            map.remove(lru.key);
        }

        private static class Node<K, V> {
            final K key; V value;
            Node<K, V> prev, next;
            Node(K key, V value) { this.key = key; this.value = value; }
        }
    }

    /** Layer 2: TTL + LRU via LinkedHashMap; clock injected for fast tests. */
    static class TtlCache<K, V> {
        private static final class Entry<V> {        // expiresAt == MAX => never expires
            final V value; final long expiresAt;
            Entry(V value, long expiresAt) { this.value = value; this.expiresAt = expiresAt; }
            V value() { return value; }
            long expiresAt() { return expiresAt; }
        }
        private final int capacity;
        private final LongSupplier clock;
        private final LinkedHashMap<K, Entry<V>> map;

        TtlCache(int capacity, LongSupplier clock) {
            this.capacity = capacity;
            this.clock = clock;
            // accessOrder=true makes iteration order = LRU order; hook does the eviction.
            this.map = new LinkedHashMap<>(16, 0.75f, true) {
                @Override protected boolean removeEldestEntry(Map.Entry<K, Entry<V>> e) {
                    return size() > TtlCache.this.capacity;
                }
            };
        }

        void put(K key, V value, long ttlMillis) {
            long expiry = ttlMillis <= 0 ? Long.MAX_VALUE : clock.getAsLong() + ttlMillis;
            map.put(key, new Entry<>(value, expiry));
        }

        Optional<V> get(K key) {
            Entry<V> e = map.get(key);
            if (e == null) return Optional.empty();
            if (e.expiresAt() != Long.MAX_VALUE && clock.getAsLong() > e.expiresAt()) {
                map.remove(key);                 // lazy expiry on read
                return Optional.empty();
            }
            return Optional.of(e.value());
        }
    }

    /* ---- Layer 3 talking points ----
     * Thread-safety: wrap in a ReadWriteLock, or use Caffeine in production.
     * Swappable backend: extract a Store interface (get/put/evict) so the same
     *   LRU policy can sit in front of memory, Redis, etc. (Dependency Inversion).
     * Distributed: prefer consistent hashing (one key -> one node) so you never
     *   need cross-node invalidation; use Redis pub/sub only if you must.
     * Monitor: hit rate, eviction rate, p99 latency.
     */

    public static void main(String[] args) {
        Cache<Integer, Integer> c = new Cache<>(2);
        c.put(1, 1);
        c.put(2, 2);
        check("get(1)", c.get(1) == 1);
        c.put(3, 3);                  // evicts key 2 (LRU)
        check("evict 2", c.get(2) == null);
        c.put(4, 4);                  // evicts key 1
        check("evict 1", c.get(1) == null);
        check("get(3)", c.get(3) == 3);
        check("get(4)", c.get(4) == 4);

        // TTL with a fake clock: deterministic, instant.
        long[] now = {1000};
        TtlCache<String, String> t = new TtlCache<>(3, () -> now[0]);
        t.put("a", "A", 50);          // expires at 1050
        t.put("b", "B", 0);           // never expires
        check("before ttl", "A".equals(t.get("a").orElse(null)));
        now[0] = 1100;                 // advance past expiry
        check("after ttl", t.get("a").isEmpty());
        check("no expiry", "B".equals(t.get("b").orElse(null)));
    }

    static void check(String name, boolean pass) {
        System.out.printf("%-12s %s%n", name, pass ? "PASS" : "FAIL");
    }
}
