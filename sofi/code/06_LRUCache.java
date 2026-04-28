import java.util.HashMap;
import java.util.Map;

/**
 * SoFi On-site - 真题（Glassdoor / Blind 多次确认 2024/2025）
 *
 * 题目：Design LRU Cache（设计 LRU 缓存）
 * ─────────────────────────────────────────────────────────────────
 * 设计一个 LRU（Least Recently Used）缓存，支持以下操作：
 *   - LRUCache(int capacity)：初始化容量
 *   - int get(int key)：返回 key 对应的 value，若不存在返回 -1
 *   - void put(int key, int value)：插入或更新 (key, value)
 *
 * 要求：get 和 put 的时间复杂度都是 O(1)。
 * 当缓存达到容量上限时，淘汰最久未使用的元素。
 *
 * 示例：
 *   LRUCache cache = new LRUCache(2);
 *   cache.put(1, 1);          // {1=1}
 *   cache.put(2, 2);          // {1=1, 2=2}
 *   cache.get(1);             // 返回 1，{2=2, 1=1}（1 变为最近使用）
 *   cache.put(3, 3);          // 容量满，淘汰 2 → {1=1, 3=3}
 *   cache.get(2);             // 返回 -1（已淘汰）
 *   cache.put(4, 4);          // 淘汰 1 → {3=3, 4=4}
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路：HashMap + 双向链表
 *
 *   关键洞察：
 *     - HashMap 提供 O(1) 的 key 查找
 *     - 双向链表提供 O(1) 的节点删除和插入（前提是已有节点引用）
 *     - 链表头 = 最近使用，链表尾 = 最久未使用
 *
 *   操作：
 *     get(k)：HashMap 找到 node → 移到链表头 → 返回 value
 *     put(k,v)：
 *       - 若存在：更新 value 并移到链表头
 *       - 若不存在：
 *         - 若已满：删除链表尾节点（同时从 HashMap 删除）
 *         - 创建新节点，加到链表头，加到 HashMap
 *
 *   细节：用 dummy head + dummy tail 简化边界处理
 *
 *   时间复杂度：O(1) per operation
 *   空间复杂度：O(capacity)
 */
public class LRUCache {

    // 双向链表节点
    static class Node {
        int key, value;
        Node prev, next;

        Node(int key, int value) {
            this.key = key;
            this.value = value;
        }
    }

    private final int capacity;
    private final Map<Integer, Node> map;
    private final Node head;  // dummy head, head.next = MRU
    private final Node tail;  // dummy tail, tail.prev = LRU

    public LRUCache(int capacity) {
        this.capacity = capacity;
        this.map = new HashMap<>();
        this.head = new Node(-1, -1);
        this.tail = new Node(-1, -1);
        head.next = tail;
        tail.prev = head;
    }

    public int get(int key) {
        if (!map.containsKey(key)) return -1;
        Node node = map.get(key);
        moveToHead(node);  // 更新为最近使用
        return node.value;
    }

    public void put(int key, int value) {
        if (map.containsKey(key)) {
            // 更新已有值
            Node node = map.get(key);
            node.value = value;
            moveToHead(node);
            return;
        }

        // 容量已满 → 淘汰 LRU
        if (map.size() == capacity) {
            Node lru = tail.prev;
            removeNode(lru);
            map.remove(lru.key);
        }

        // 插入新节点
        Node node = new Node(key, value);
        addToHead(node);
        map.put(key, node);
    }

    // ─── 双向链表辅助方法 ────────────────────────────────────────
    private void addToHead(Node node) {
        node.prev = head;
        node.next = head.next;
        head.next.prev = node;
        head.next = node;
    }

    private void removeNode(Node node) {
        node.prev.next = node.next;
        node.next.prev = node.prev;
    }

    private void moveToHead(Node node) {
        removeNode(node);
        addToHead(node);
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        LRUCache cache = new LRUCache(2);
        cache.put(1, 1);
        cache.put(2, 2);
        System.out.println(cache.get(1));   // 1
        cache.put(3, 3);                    // 淘汰 key=2
        System.out.println(cache.get(2));   // -1
        cache.put(4, 4);                    // 淘汰 key=1
        System.out.println(cache.get(1));   // -1
        System.out.println(cache.get(3));   // 3
        System.out.println(cache.get(4));   // 4
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 为什么不用 LinkedHashMap？
 * A: Java 的 LinkedHashMap(initialCapacity, loadFactor, accessOrder=true) +
 *    覆盖 removeEldestEntry 即可实现 LRU。但面试一般要求手写底层数据结构，
 *    展示对 HashMap + 双向链表的理解。
 *
 * Q: 为什么是双向链表，单链表行不行？
 * A: 不行。get 时需要 O(1) 删除任意节点，单链表删除需要前驱节点（要 O(n) 找）。
 *    双向链表每个节点有 prev 指针，可以直接 O(1) 删除。
 *
 * Q: 线程安全如何处理？
 * A: 三种方案：
 *    1. synchronized 包整个类（简单但性能差）
 *    2. ReentrantReadWriteLock（读多写少时更好）
 *    3. Concurrent LRU：用 ConcurrentHashMap + 分段锁，或者 Caffeine 库
 *    Senior 考察：能讨论性能/正确性 tradeoff
 *
 * Q: 如果要持久化呢？
 * A: 这就是 Redis 的实现思路。可以用 Write-Behind 异步刷盘，
 *    或者 Write-Through 同步写磁盘（牺牲性能换持久性）。
 *
 * Q: LFU vs LRU？
 * A: LFU 按访问频率淘汰，需要额外的 frequency map + min-heap 或 doubly linked list of buckets。
 *    SoFi 也可能问 LFU（LC 460），思路类似但更复杂。
 *
 * 对应 LeetCode：LC 146 LRU Cache (Medium，面试高频前 5)
 */
