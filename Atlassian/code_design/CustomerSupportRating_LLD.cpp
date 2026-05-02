/*
 * Customer Support Rating System
 * [Atlassian Code Design Round — confirmed in 2025 LLD reports]
 *
 * Design a system that:
 *   1. submitRating(employeeId, rating, timestamp)   — add a rating (1–5)
 *   2. averageRating(employeeId)                     — overall avg, current
 *   3. averageRating(employeeId, year, month)        — avg for that month
 *   4. bestRated()                                   — employee with highest avg
 *   5. bestRated(year, month)                        — best in given month
 *
 * Follow-ups asked in interview:
 *   - "Best rated within month" (drives data model choice)
 *   - Tie-breaking when multiple have the same avg
 *
 * ─────────────────────────────────────────────────────────────────
 * Data model:
 *   per employee:
 *      total sum, count                  → O(1) avg
 *      map<YYYYMM, (sum, count)>         → O(1) per-month avg
 *
 *   bestRated() — choose between:
 *      Option A: scan all employees, O(E)
 *      Option B: maintain a global priority structure (multiset by avg)
 *                — but avg changes on every submit; need to re-key. Doable
 *                with a multiset<pair<double,string>> + map<string, iterator>.
 *
 * For interview, start with Option A (clear), then offer B as scaling story.
 * ─────────────────────────────────────────────────────────────────
 */
#include <bits/stdc++.h>
using namespace std;

class RatingSystem {
    struct Stat {
        long long sum = 0;
        long long cnt = 0;
        double avg() const { return cnt == 0 ? 0.0 : (double)sum / cnt; }
    };

    // employeeId -> overall
    unordered_map<string, Stat> overall;
    // employeeId -> (yyyymm -> stat)
    unordered_map<string, unordered_map<int, Stat>> monthly;

    static int yyyymm(int year, int month) { return year * 100 + month; }

public:
    void submitRating(const string& employeeId, int rating, int year, int month) {
        if (rating < 1 || rating > 5)
            throw invalid_argument("rating out of range [1,5]");

        auto& g = overall[employeeId];
        g.sum += rating; g.cnt += 1;

        auto& m = monthly[employeeId][yyyymm(year, month)];
        m.sum += rating; m.cnt += 1;
    }

    double averageRating(const string& employeeId) const {
        auto it = overall.find(employeeId);
        if (it == overall.end()) return 0.0;
        return it->second.avg();
    }

    double averageRating(const string& employeeId, int year, int month) const {
        auto it = monthly.find(employeeId);
        if (it == monthly.end()) return 0.0;
        auto it2 = it->second.find(yyyymm(year, month));
        if (it2 == it->second.end()) return 0.0;
        return it2->second.avg();
    }

    // Returns (employeeId, avg). Tie-break: lexicographically smallest id.
    pair<string,double> bestRated() const {
        string bestId;
        double bestAvg = -1.0;
        for (auto& [id, st] : overall) {
            double a = st.avg();
            if (a > bestAvg || (a == bestAvg && (bestId.empty() || id < bestId))) {
                bestAvg = a; bestId = id;
            }
        }
        return {bestId, bestAvg};
    }

    pair<string,double> bestRated(int year, int month) const {
        string bestId;
        double bestAvg = -1.0;
        int key = yyyymm(year, month);
        for (auto& [id, mm] : monthly) {
            auto it = mm.find(key);
            if (it == mm.end()) continue;
            double a = it->second.avg();
            if (a > bestAvg || (a == bestAvg && (bestId.empty() || id < bestId))) {
                bestAvg = a; bestId = id;
            }
        }
        return {bestId, bestAvg};
    }
};

/*
 * Follow-up Q&A (Atlassian-style — asked in real interview):
 *
 * Q: bestRated for arbitrary date range, not just one month?
 * A: Need cumulative sums by day or month. Or fenwick tree over dates per
 *    employee. Or precompute monthly aggregates and sum. For arbitrary
 *    range, switch to OLAP / OLAP-like analytical store (not LLD scope).
 *
 * Q: Concurrency — multiple submitRating calls from different threads?
 * A: Wrap with mutex (simple). For higher throughput, shard by employeeId
 *    and use per-shard locks. Or use atomic counters per stat field.
 *
 * Q: Persistence?
 * A: Append every (employeeId, rating, ts) to durable log (Kafka/DB);
 *    in-memory aggregates rebuildable from log. Periodic snapshots.
 *
 * Q: Scaling to millions of employees, billions of ratings?
 * A: Move per-employee stats to a key-value store (e.g., DynamoDB / Cassandra)
 *    with employeeId as partition key. bestRated requires either a precomputed
 *    leaderboard (Redis sorted set) or batch jobs.
 *
 * Q: Tie-breaking & exposing top-K, not just best?
 * A: Maintain a Redis ZSET keyed by avg; updates on every submit (lazy or
 *    eager). top-K = ZREVRANGE.
 *
 * Q: How to test?
 * A: Unit tests around boundaries (1 rating, all same, ties), property tests
 *    (avg invariants), concurrency tests (thread sanitizer).
 *
 * Atlassian Jira/JSM angle:
 *   This IS literally JSM (Jira Service Management) agent rating —
 *   tickets close with CSAT 1-5, leaderboards, monthly reports.
 */

int main() {
    RatingSystem rs;
    rs.submitRating("alice", 5, 2026, 4);
    rs.submitRating("alice", 4, 2026, 4);
    rs.submitRating("bob",   3, 2026, 4);
    rs.submitRating("bob",   5, 2026, 4);
    rs.submitRating("alice", 3, 2026, 3);  // March, not April

    cout << "alice overall: " << rs.averageRating("alice") << endl;        // (5+4+3)/3 = 4.0
    cout << "alice 2026-04: " << rs.averageRating("alice", 2026, 4) << endl; // 4.5
    cout << "bob 2026-04:   " << rs.averageRating("bob",   2026, 4) << endl; // 4.0

    auto [id, avg] = rs.bestRated(2026, 4);
    cout << "Best in 2026-04: " << id << " avg=" << avg << endl;          // alice 4.5
    return 0;
}
