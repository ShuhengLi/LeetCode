import java.util.*;

/**
 * SoFi On-site - 真题（图算法/最短路径，2024/2025 候选人报告）
 *
 * 题目：Word Ladder (BFS Shortest Path)
 * ─────────────────────────────────────────────────────────────────
 * 给一个起始单词 beginWord、目标单词 endWord、单词列表 wordList。
 * 每次只能改变一个字符，且每步的中间单词必须在 wordList 中。
 * 返回从 beginWord 到 endWord 的最短转换序列长度（包含 begin 和 end），
 * 如果无法转换，返回 0。
 *
 * 示例 1：
 *   beginWord = "hit", endWord = "cog"
 *   wordList = ["hot","dot","dog","lot","log","cog"]
 *   Output: 5
 *   解释: hit → hot → dot → dog → cog (长度 5)
 *
 * 示例 2：
 *   beginWord = "hit", endWord = "cog"
 *   wordList = ["hot","dot","dog","lot","log"]
 *   Output: 0  (cog 不在列表中)
 * ─────────────────────────────────────────────────────────────────
 *
 * 思路：BFS 求最短路径
 *
 *   把每个单词看作图上的节点。两个单词差一个字符，则相邻。
 *   BFS 第一次到达 endWord 即为最短路径。
 *
 * 关键优化：
 *   朴素：对每个单词比较与所有其他单词是否差一个字符 → O(N^2 * L)
 *   优化：用"通配符模式"（pattern）建立反向索引
 *        例如 "hot" 生成 "*ot", "h*t", "ho*"
 *        所有差一个字符的单词共享一个 pattern
 *        建立 pattern → words 的 map，邻居查找 O(L)
 *
 *   时间复杂度：O(N * L^2)，N = |wordList|, L = 单词长度
 *   空间复杂度：O(N * L^2)
 *
 * 进阶：双向 BFS（Bidirectional BFS）
 *   从 begin 和 end 同时搜索，相遇时返回。
 *   时间复杂度近似 O(N * L^2 / 2)，常数优化但量级相同
 *   实际跑分可快 2-3 倍。
 */
public class WordLadder {

    // ─── 方法一：标准 BFS + Pattern Map ──────────────────────────
    public static int ladderLength(String beginWord, String endWord, List<String> wordList) {
        Set<String> dict = new HashSet<>(wordList);
        if (!dict.contains(endWord)) return 0;

        // 预处理：为每个 pattern 建立索引
        // 例如 "hot" → ["*ot", "h*t", "ho*"]，每个 pattern 映射到所有匹配单词
        Map<String, List<String>> patternMap = new HashMap<>();
        int L = beginWord.length();

        for (String word : dict) {
            for (int i = 0; i < L; i++) {
                String pattern = word.substring(0, i) + "*" + word.substring(i + 1);
                patternMap.computeIfAbsent(pattern, k -> new ArrayList<>()).add(word);
            }
        }

        // BFS
        Queue<String> queue = new LinkedList<>();
        queue.offer(beginWord);
        Set<String> visited = new HashSet<>();
        visited.add(beginWord);
        int level = 1;

        while (!queue.isEmpty()) {
            int size = queue.size();
            for (int s = 0; s < size; s++) {
                String word = queue.poll();
                if (word.equals(endWord)) return level;

                for (int i = 0; i < L; i++) {
                    String pattern = word.substring(0, i) + "*" + word.substring(i + 1);
                    for (String neighbor : patternMap.getOrDefault(pattern, Collections.emptyList())) {
                        if (!visited.contains(neighbor)) {
                            visited.add(neighbor);
                            queue.offer(neighbor);
                        }
                    }
                }
            }
            level++;
        }

        return 0;
    }

    // ─── 方法二：双向 BFS（Senior 加分） ─────────────────────────
    public static int ladderLengthBidirectional(String beginWord, String endWord, List<String> wordList) {
        Set<String> dict = new HashSet<>(wordList);
        if (!dict.contains(endWord)) return 0;

        Set<String> beginSet = new HashSet<>();
        Set<String> endSet = new HashSet<>();
        beginSet.add(beginWord);
        endSet.add(endWord);

        Set<String> visited = new HashSet<>();
        int level = 1;

        while (!beginSet.isEmpty() && !endSet.isEmpty()) {
            // 永远从较小的一端扩展（关键优化）
            if (beginSet.size() > endSet.size()) {
                Set<String> tmp = beginSet;
                beginSet = endSet;
                endSet = tmp;
            }

            Set<String> nextLevel = new HashSet<>();
            for (String word : beginSet) {
                char[] chars = word.toCharArray();
                for (int i = 0; i < chars.length; i++) {
                    char old = chars[i];
                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == old) continue;
                        chars[i] = c;
                        String next = new String(chars);

                        // 双方相遇 → 找到最短路径
                        if (endSet.contains(next)) return level + 1;

                        if (dict.contains(next) && !visited.contains(next)) {
                            visited.add(next);
                            nextLevel.add(next);
                        }
                    }
                    chars[i] = old;
                }
            }

            beginSet = nextLevel;
            level++;
        }

        return 0;
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        List<String> wordList = Arrays.asList("hot","dot","dog","lot","log","cog");

        System.out.println(ladderLength("hit", "cog", wordList));            // 5
        System.out.println(ladderLengthBidirectional("hit", "cog", wordList)); // 5

        List<String> wordList2 = Arrays.asList("hot","dot","dog","lot","log");
        System.out.println(ladderLength("hit", "cog", wordList2));            // 0

        System.out.println(ladderLength("a", "c", Arrays.asList("a","b","c"))); // 2
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 为什么用 BFS 而非 DFS？
 * A: BFS 在无权图上保证第一次到达即最短路径。DFS 需遍历所有路径才知道最短。
 *
 * Q: 为什么用 pattern map？
 * A: 朴素邻居判断需要遍历所有单词 O(N*L)，整体 O(N^2*L)。
 *    Pattern map 把"差一字符的邻居"压缩为 hash 查找 O(L) 找到候选集，整体 O(N*L^2)。
 *    当 N >> L 时（字典很大），优化效果明显。
 *
 * Q: 双向 BFS 为什么快？
 * A: BFS 节点数随层数指数增长。从两端各走一半，
 *    总节点数 = 2 * b^(d/2) 远小于 b^d（b 是分支因子，d 是路径长度）。
 *    实际可快几个数量级。
 *
 * Q: 如何返回路径本身（不只是长度）？
 * A: BFS 时记录每个节点的 parent，结束后从 endWord 回溯。
 *    LC 126 Word Ladder II 要求所有最短路径，需要 BFS 建层 + DFS 回溯，更复杂。
 *
 * Q: SoFi 业务场景？
 * A: - 风控规则推导：从一个用户状态到另一个状态需要的最少步骤
 *    - 客户支持流程：从 issue A 解决到 issue B 的最短工单流转
 *    - 推荐系统：从一个产品到另一个产品的相似性路径
 *
 * Q: 如何处理超大字典（百万级）？
 * A: - 预构建 pattern map 并持久化（避免每次重算）
 *    - 用 Trie 优化模式匹配
 *    - 分布式 BFS：每层节点分片到多机，跨机通信合并
 *
 * 相关 LeetCode：
 *   - LC 127 Word Ladder (本题)
 *   - LC 126 Word Ladder II (返回所有最短路径)
 *   - LC 433 Minimum Genetic Mutation (类似但字母表更小)
 */
