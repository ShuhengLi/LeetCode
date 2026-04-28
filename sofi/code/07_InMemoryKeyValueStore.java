import java.util.HashMap;
import java.util.Map;

/**
 * SoFi On-site - 真题（Aug 2025 Glassdoor 报告确认）
 *
 * 题目：In-Memory Key-Value Store
 * ─────────────────────────────────────────────────────────────────
 * 设计并实现一个内存键值存储，支持以下 API：
 *   - void add(String key, String value)：添加 (key, value)
 *   - String get(String key)：返回 key 的 value，若不存在返回 null
 *   - void remove(String key)：删除 key
 *   - String getLast()：返回最近一次 add 的 value（如果该 key 还存在）
 *
 * 要求：所有操作都是 O(1) 时间复杂度。
 *
 * 示例：
 *   store.add("a", "1");   // last = "1"
 *   store.add("b", "2");   // last = "2"
 *   store.getLast();       // "2"
 *   store.remove("b");     // last 指向的 "b" 被删除
 *   store.getLast();       // 应该返回 "1"（"b" 被删了，下一个最近的）
 *   store.add("c", "3");   // last = "3"
 *   store.getLast();       // "3"
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路：HashMap + 双向链表（与 LRU 类似但语义不同）
 *
 *   关键设计：
 *     - HashMap<key, Node> 提供 O(1) 查找
 *     - 双向链表维护"添加顺序"，head 是最近添加，tail 是最早
 *     - getLast() 返回 head 节点的 value
 *     - remove 时如果该节点是 head，head 自动指向下一个
 *
 *   为什么用双向链表？
 *     remove 操作可能删除"最近添加的元素"，需要 O(1) 找到下一个最近。
 *     如果用单链表或栈，删除中间元素是 O(n)。
 *
 *   时间复杂度：O(1) per operation
 *   空间复杂度：O(n)
 */
public class InMemoryKeyValueStore {

    static class Node {
        String key, value;
        Node prev, next;

        Node(String key, String value) {
            this.key = key;
            this.value = value;
        }
    }

    private final Map<String, Node> map = new HashMap<>();
    private final Node head;  // dummy head, head.next = 最近添加
    private final Node tail;  // dummy tail

    public InMemoryKeyValueStore() {
        head = new Node(null, null);
        tail = new Node(null, null);
        head.next = tail;
        tail.prev = head;
    }

    public void add(String key, String value) {
        if (map.containsKey(key)) {
            // 已存在：更新 value 并移到 head（视为重新添加）
            Node node = map.get(key);
            node.value = value;
            removeFromList(node);
            addToHead(node);
        } else {
            Node node = new Node(key, value);
            addToHead(node);
            map.put(key, node);
        }
    }

    public String get(String key) {
        Node node = map.get(key);
        return node == null ? null : node.value;
    }

    public void remove(String key) {
        Node node = map.remove(key);
        if (node != null) {
            removeFromList(node);
        }
    }

    public String getLast() {
        // head.next 是最近添加；若链表空（head.next == tail），返回 null
        return head.next == tail ? null : head.next.value;
    }

    // ─── 双向链表辅助方法 ────────────────────────────────────────
    private void addToHead(Node node) {
        node.prev = head;
        node.next = head.next;
        head.next.prev = node;
        head.next = node;
    }

    private void removeFromList(Node node) {
        node.prev.next = node.next;
        node.next.prev = node.prev;
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        InMemoryKeyValueStore store = new InMemoryKeyValueStore();

        store.add("a", "1");
        store.add("b", "2");
        System.out.println(store.getLast());   // "2"

        store.remove("b");
        System.out.println(store.getLast());   // "1"

        store.add("c", "3");
        System.out.println(store.getLast());   // "3"

        System.out.println(store.get("a"));    // "1"
        System.out.println(store.get("b"));    // null
        System.out.println(store.get("c"));    // "3"

        store.remove("c");
        store.remove("a");
        System.out.println(store.getLast());   // null（空）

        // 测试更新已有 key
        store.add("x", "10");
        store.add("y", "20");
        store.add("x", "100");                 // x 重新成为 last
        System.out.println(store.getLast());   // "100"
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 与 LRU Cache 有何区别？
 * A: LRU 在 get 时也更新"最近使用"，本题只在 add 时更新"最近添加"。
 *    本题没有容量限制，没有自动淘汰。但底层结构（HashMap+双向链表）一样。
 *
 * Q: 如果改成 getFirst()（最早添加）？
 * A: 链表设计已经支持，返回 tail.prev.value 即可。
 *
 * Q: 如果加 getKthLast(k)？
 * A: 需要从 head 走 k 步，O(k)。如果要 O(log n)，需用平衡树（TreeMap+索引）
 *    或跳表（Skip List）。
 *
 * Q: 为什么不用 LinkedHashMap？
 * A: 可以！LinkedHashMap 内部就是 HashMap+双向链表。
 *    但面试要求展示对底层的理解，手写更能体现 Senior 水平。
 *    生产代码可以直接用 LinkedHashMap 简化。
 *
 * Q: 并发场景？
 * A: 这种结构本身不是线程安全的。简单方案：
 *    - 加全局 ReentrantLock
 *    - 或用 ConcurrentLinkedDeque + ConcurrentHashMap，但 getLast 一致性难保证
 *    更复杂方案：用 CAS + 版本号，参考 Caffeine。
 *
 * Q: 与栈（Stack）有何区别？
 * A: 栈也是 LIFO，但栈不能 O(1) 删除中间元素，所以 remove(key) 后 getLast 会返回错误结果。
 *    如果只需要 add 和 getLast 不需要 remove，用 Stack 即可。
 *
 * 这道题是 Bloomberg、SoFi 等公司常考的 LLD 经典题，
 * 考察对 HashMap + LinkedList 组合的灵活运用。
 */
