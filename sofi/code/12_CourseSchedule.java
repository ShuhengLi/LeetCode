import java.util.*;

/**
 * SoFi On-site - 真题（图算法高频题，2024/2025 候选人报告）
 *
 * 题目：Course Schedule II（拓扑排序 / 任务依赖）
 * ─────────────────────────────────────────────────────────────────
 * 共有 numCourses 门课程，编号 0 ~ numCourses-1。
 * prerequisites[i] = [a, b] 表示要修课程 a 必须先修课程 b。
 *
 * 返回一个学习顺序数组（任意一个有效顺序）。
 * 如果无法完成（存在循环依赖），返回空数组。
 *
 * 示例 1：
 *   numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
 *   依赖图：
 *     0 → 1 → 3
 *     0 → 2 → 3
 *   合法顺序：[0,1,2,3] 或 [0,2,1,3]
 *
 * 示例 2：
 *   numCourses = 2, prerequisites = [[1,0],[0,1]]
 *   有循环 → 返回 []
 * ─────────────────────────────────────────────────────────────────
 *
 * SoFi 业务场景类比：
 *   - 部署任务依赖（service A 要先部署 service B）
 *   - 数据 ETL 管道
 *   - 风控规则链：先验证身份 → 再验证账户 → 再批准交易
 *
 * 思路：拓扑排序
 *
 * 方法一：BFS（Kahn's algorithm）← 推荐面试默认
 *   1. 构建邻接表 graph 和入度数组 inDegree
 *   2. 把所有入度为 0 的节点加入队列
 *   3. 从队列取出节点 u，加入结果；遍历 u 的邻居 v，inDegree[v]--，
 *      若 inDegree[v] == 0，加入队列
 *   4. 若结果长度 < numCourses → 有环
 *
 * 方法二：DFS + 三色标记法
 *   状态：0=未访问，1=访问中（在当前 DFS 路径上），2=已完成
 *   若 DFS 中遇到状态 1 的节点 → 有环
 *   完成的节点逆序加入结果
 *
 *   时间复杂度：O(V + E)
 *   空间复杂度：O(V + E)
 */
public class CourseSchedule {

    // ─── 方法一：BFS / Kahn's Algorithm ──────────────────────────
    public static int[] findOrderBFS(int numCourses, int[][] prerequisites) {
        // 构建邻接表 + 入度
        List<List<Integer>> graph = new ArrayList<>();
        for (int i = 0; i < numCourses; i++) graph.add(new ArrayList<>());

        int[] inDegree = new int[numCourses];
        for (int[] p : prerequisites) {
            // p = [a, b] 表示 b → a（b 是 a 的前置）
            graph.get(p[1]).add(p[0]);
            inDegree[p[0]]++;
        }

        // 入度 0 的入队
        Queue<Integer> queue = new LinkedList<>();
        for (int i = 0; i < numCourses; i++) {
            if (inDegree[i] == 0) queue.offer(i);
        }

        int[] result = new int[numCourses];
        int idx = 0;
        while (!queue.isEmpty()) {
            int u = queue.poll();
            result[idx++] = u;

            for (int v : graph.get(u)) {
                if (--inDegree[v] == 0) {
                    queue.offer(v);
                }
            }
        }

        // 若不能全部完成，有环
        return idx == numCourses ? result : new int[0];
    }

    // ─── 方法二：DFS + 三色标记 ───────────────────────────────────
    public static int[] findOrderDFS(int numCourses, int[][] prerequisites) {
        List<List<Integer>> graph = new ArrayList<>();
        for (int i = 0; i < numCourses; i++) graph.add(new ArrayList<>());
        for (int[] p : prerequisites) {
            graph.get(p[1]).add(p[0]);
        }

        int[] color = new int[numCourses];  // 0/1/2
        Deque<Integer> stack = new ArrayDeque<>();

        for (int i = 0; i < numCourses; i++) {
            if (color[i] == 0 && hasCycle(i, graph, color, stack)) {
                return new int[0];
            }
        }

        // stack 顶是依赖最少的（最先学），逆序输出
        int[] result = new int[numCourses];
        int idx = 0;
        while (!stack.isEmpty()) {
            result[idx++] = stack.pop();
        }
        return result;
    }

    private static boolean hasCycle(int u, List<List<Integer>> graph, int[] color, Deque<Integer> stack) {
        color[u] = 1;  // 访问中
        for (int v : graph.get(u)) {
            if (color[v] == 1) return true;  // 环
            if (color[v] == 0 && hasCycle(v, graph, color, stack)) return true;
        }
        color[u] = 2;       // 完成
        stack.push(u);      // 后完成的依赖先完成的，所以 stack 顶 = 最先学的
        return false;
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        System.out.println(Arrays.toString(
            findOrderBFS(4, new int[][]{{1,0},{2,0},{3,1},{3,2}})));
        // 期望: [0, 1, 2, 3] 或 [0, 2, 1, 3]

        System.out.println(Arrays.toString(
            findOrderBFS(2, new int[][]{{1,0},{0,1}})));
        // 期望: [] (有环)

        System.out.println(Arrays.toString(
            findOrderDFS(4, new int[][]{{1,0},{2,0},{3,1},{3,2}})));

        System.out.println(Arrays.toString(
            findOrderBFS(1, new int[][]{})));
        // 期望: [0]

        System.out.println(Arrays.toString(
            findOrderBFS(6, new int[][]{{1,0},{2,1},{3,2},{4,3},{5,4}})));
        // 期望: [0,1,2,3,4,5] (链状)
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: BFS vs DFS 怎么选？
 * A: BFS（Kahn's）：直观，检测环顺便给出顺序，迭代不会栈溢出
 *    DFS：递归更短，但深图可能栈溢出（可改迭代 DFS）
 *    生产代码推荐 BFS。
 *
 * Q: 如何检测哪些课程在环里？
 * A: BFS 完成后，inDegree > 0 的节点就在环里（或被环阻塞）。
 *    若要找环本身，需要 DFS 记录路径。
 *
 * Q: 如何并行执行？（Senior 拓展）
 * A: 拓扑排序的层级输出：每一轮取所有入度 0 的节点，可以并行执行。
 *    类似 Apache Airflow / Argo Workflows 的 DAG 调度。
 *
 *   修改 BFS：
 *     while (!queue.isEmpty()) {
 *       int size = queue.size();
 *       List<Integer> levelTasks = new ArrayList<>();
 *       for (int i = 0; i < size; i++) levelTasks.add(queue.poll());
 *       // 这一层的所有 task 可以并行执行
 *       parallelExecute(levelTasks);
 *       // 更新入度，加入下一层
 *     }
 *
 * Q: 如果加权（每个任务有耗时，求最早完成时间）？
 * A: 关键路径方法（Critical Path Method, CPM），
 *    动态规划 finish[v] = max(finish[u] for u in deps(v)) + duration[v]
 *
 * Q: 如何处理"软依赖"（环允许，但要尽量满足）？
 * A: SCC（Tarjan 算法）找强连通分量，每个 SCC 缩点后做拓扑排序。
 *    Senior+ 加分项。
 *
 * 相关 LeetCode：
 *   - LC 207 Course Schedule (检测是否能完成)
 *   - LC 210 Course Schedule II (本题)
 *   - LC 269 Alien Dictionary (从单词推断字母顺序)
 *   - LC 310 Minimum Height Trees
 */
