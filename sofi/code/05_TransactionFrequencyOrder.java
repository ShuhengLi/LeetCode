import java.util.*;

/**
 * SoFi Tech Screen - 真题（1point3acres 报告，2024/2025）
 *
 * 题目：Transaction Frequency Ordering
 * ─────────────────────────────────────────────────────────────────
 * 给一个交易记录列表（每条记录包含用户ID和金额），
 * 按照以下规则对用户进行排序并输出：
 *   1. 主排序键：交易次数（频率）从高到低
 *   2. 次排序键：交易总金额从高到低
 *   3. 三排序键：用户ID 字典序从小到大（保证稳定性）
 *
 * 变体 A（SoFi 1point3acres 版本）：
 *   Input: List<Transaction>，每个 Transaction 有 userId 和 amount
 *   Output: 按上述规则排序的用户ID列表（去重）
 *
 * 变体 B（更接近实际问题）：
 *   Input: String[] transactions，格式 "userId,amount"
 *   Output: List<String> 排序后的用户ID
 *
 * 示例：
 *   Input transactions:
 *     ["alice,100", "bob,200", "alice,50", "charlie,300", "bob,150", "alice,75"]
 *
 *   统计：
 *     alice:   3次，总金额 225
 *     bob:     2次，总金额 350
 *     charlie: 1次，总金额 300
 *
 *   按规则排序（次数→金额→字典序）：
 *     alice(3次,225) > bob(2次,350) > charlie(1次,300)
 *
 *   Output: ["alice", "bob", "charlie"]
 *
 * 约束：
 *   - 1 <= transactions.length <= 10^5
 *   - userId 只含小写字母和数字
 *   - 1 <= amount <= 10^6
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路：HashMap 聚合统计 + Comparator 自定义排序
 *   1. 遍历所有交易，用 HashMap 统计每个用户的交易次数和总金额
 *   2. 取 entrySet，用自定义 Comparator 排序
 *   3. 提取 userId 输出
 *
 *   时间复杂度：O(n + m log m)，n = 交易数，m = 用户数（m <= n）
 *   空间复杂度：O(m)
 */
public class TransactionFrequencyOrder {

    // ─── 数据模型 ────────────────────────────────────────────────
    static class UserStats {
        String userId;
        int frequency;
        long totalAmount;

        UserStats(String userId) {
            this.userId = userId;
            this.frequency = 0;
            this.totalAmount = 0;
        }

        void addTransaction(long amount) {
            frequency++;
            totalAmount += amount;
        }
    }

    // ─── 核心方法 ────────────────────────────────────────────────
    public static List<String> sortUsersByTransactions(String[] transactions) {
        // Step 1: 聚合统计
        Map<String, UserStats> statsMap = new HashMap<>();

        for (String transaction : transactions) {
            String[] parts = transaction.split(",");
            String userId = parts[0].trim();
            long amount = Long.parseLong(parts[1].trim());

            statsMap.computeIfAbsent(userId, UserStats::new)
                    .addTransaction(amount);
        }

        // Step 2: 转换为列表并排序
        List<UserStats> statsList = new ArrayList<>(statsMap.values());

        statsList.sort(
            // 1. 交易次数降序
            Comparator.comparingInt(UserStats::getFrequency).reversed()
            // 2. 总金额降序
            .thenComparingLong(UserStats::getTotalAmount).reversed()
            // 3. userId 字典序升序
            .thenComparing(s -> s.userId)
        );

        // Step 3: 提取 userId
        List<String> result = new ArrayList<>();
        for (UserStats stats : statsList) {
            result.add(stats.userId);
        }
        return result;
    }

    // Java 8+ Comparator 的另一种写法（更清晰的链式）
    public static List<String> sortUsersByTransactionsV2(String[] transactions) {
        Map<String, int[]> stats = new HashMap<>();
        // stats[userId] = [frequency, totalAmount]（用 long 更安全，这里简化）

        for (String t : transactions) {
            String[] parts = t.split(",");
            String userId = parts[0].trim();
            int amount = Integer.parseInt(parts[1].trim());

            stats.computeIfAbsent(userId, k -> new int[2]);
            stats.get(userId)[0]++;          // frequency
            stats.get(userId)[1] += amount;  // totalAmount
        }

        List<String> users = new ArrayList<>(stats.keySet());

        users.sort((a, b) -> {
            int[] sa = stats.get(a);
            int[] sb = stats.get(b);
            if (sb[0] != sa[0]) return sb[0] - sa[0];     // 频率降序
            if (sb[1] != sa[1]) return sb[1] - sa[1];     // 金额降序
            return a.compareTo(b);                          // 字典序升序
        });

        return users;
    }

    // helper getter（for method reference in Comparator）
    static int getFrequency(UserStats s) { return s.frequency; }
    static long getTotalAmount(UserStats s) { return s.totalAmount; }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        String[] transactions = {
            "alice,100", "bob,200", "alice,50",
            "charlie,300", "bob,150", "alice,75"
        };

        System.out.println("V1: " + sortUsersByTransactions(transactions));
        // 期望: [alice, bob, charlie]

        System.out.println("V2: " + sortUsersByTransactionsV2(transactions));
        // 期望: [alice, bob, charlie]

        // 测试次排序键：次数相同时按金额降序
        String[] transactions2 = {
            "alice,100", "bob,500"
        };
        System.out.println("Equal freq: " + sortUsersByTransactionsV2(transactions2));
        // 期望: [bob, alice]（次数相同，bob 金额更高）

        // 测试三排序键：次数和金额都相同时按字典序
        String[] transactions3 = {
            "alice,100", "bob,100"
        };
        System.out.println("Equal freq & amount: " + sortUsersByTransactionsV2(transactions3));
        // 期望: [alice, bob]（字典序）
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 如果数据量极大（百亿条交易），如何处理？
 * A: 分布式方案：
 *    - MapReduce：Map 阶段按 userId 分片聚合，Reduce 阶段汇总排序
 *    - Streaming：用 Apache Kafka + Flink 做实时流式聚合
 *    - 分桶：用 userId hash 将数据分散到多台机器并行统计，最后归并 Top-K
 *    Senior 考察：能提出分布式思路并讨论 consistency vs. availability tradeoff。
 *
 * Q: 如果需要实时排行榜（实时更新频率排序）？
 * A: 用 Redis Sorted Set（ZADD / ZREVRANGE），score = 交易次数，O(log n) 更新，O(log n + k) 查询 Top-K。
 *
 * Q: 如果 userId 有大小写区分？
 * A: 用 equalsIgnoreCase 或 toLowerCase 统一处理，取决于业务要求。
 *
 * Q: 这里用 computeIfAbsent 而不是 getOrDefault + put，为什么？
 * A: computeIfAbsent 是原子操作（在并发场景下更安全），
 *    同时避免了两次 hash 查找（getOrDefault + put 各一次），代码更简洁。
 *
 * Q: 如何让排序对多线程安全？
 * A: statsList.sort() 不是线程安全的，如果并发写入：
 *    - 用 ConcurrentHashMap 替代 HashMap
 *    - 用 Collections.synchronizedList 或 CopyOnWriteArrayList
 *    - 或者在 reduce 阶段单线程汇总排序
 *
 * 相关 LeetCode：
 *   - LC 347 Top K Frequent Elements
 *   - LC 451 Sort Characters By Frequency
 *   - LC 692 Top K Frequent Words
 */
