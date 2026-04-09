/*
1197. Minimum Knight Moves (Premium)

In an infinite chess board with coordinates from -infinity to +infinity, you
have a knight at square [0, 0].

A knight has 8 possible moves it can make. Each move is two squares in a
cardinal direction, then one square in an orthogonal direction.

Return the minimum number of steps needed to move the knight to the square
[x, y]. It is guaranteed the answer exists.

Example 1:
  Input: x = 2, y = 1
  Output: 1
  Explanation: [0, 0] -> [2, 1]

Example 2:
  Input: x = 5, y = 5
  Output: 4
  Explanation: [0, 0] -> [2, 1] -> [4, 2] -> [3, 4] -> [5, 5]

Constraints:
  - -300 <= x <= 300
  - -300 <= y <= 300
  - 0 <= |x| + |y| <= 300
*/
#include <queue>
#include <unordered_set>
#include <cmath>
using namespace std;

// BFS on infinite board. Optimize by using symmetry (first quadrant only)
// and bounding the search space to nr >= -2 && nc >= -2.
// Time: O(|x|*|y|), Space: O(|x|*|y|)
class Solution {
public:
    int minKnightMoves(int x, int y) {
        x = abs(x); y = abs(y);
        int dirs[][2] = {{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2}};

        queue<pair<int,int>> q;
        q.push({0, 0});
        unordered_set<long long> visited;
        visited.insert(0);
        int steps = 0;

        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                auto [r, c] = q.front(); q.pop();
                if (r == x && c == y) return steps;

                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    long long key = (long long)(nr + 310) * 1000 + (nc + 310);
                    if (nr >= -2 && nc >= -2 && !visited.count(key)) {
                        visited.insert(key);
                        q.push({nr, nc});
                    }
                }
            }
            steps++;
        }
        return -1;
    }
};

// ===================== Improved Solution =====================
// Bidirectional BFS - search from (0,0) and (x,y) simultaneously.
// Dramatically reduces the number of explored states.
// Time: O(sqrt(|x|*|y|)), Space: O(sqrt(|x|*|y|))
class Solution_v2 {
public:
    int minKnightMoves(int x, int y) {
        x = abs(x); y = abs(y);
        int dirs[][2] = {{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2}};

        auto encode = [](int r, int c) { return (long long)(r + 310) * 1000 + (c + 310); };

        unordered_map<long long, int> fromStart, fromEnd;
        fromStart[encode(0, 0)] = 0;
        fromEnd[encode(x, y)] = 0;

        queue<pair<int,int>> qStart, qEnd;
        qStart.push({0, 0});
        qEnd.push({x, y});

        auto expand = [&](queue<pair<int,int>>& q, unordered_map<long long, int>& cur,
                          unordered_map<long long, int>& other) -> int {
            int sz = q.size();
            while (sz--) {
                auto [r, c] = q.front(); q.pop();
                long long key = encode(r, c);
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    long long nkey = encode(nr, nc);
                    if (nr < -2 || nc < -2 || cur.count(nkey)) continue;
                    cur[nkey] = cur[key] + 1;
                    if (other.count(nkey)) return cur[nkey] + other[nkey];
                    q.push({nr, nc});
                }
            }
            return -1;
        };

        while (true) {
            int res = expand(qStart, fromStart, fromEnd);
            if (res != -1) return res;
            res = expand(qEnd, fromEnd, fromStart);
            if (res != -1) return res;
        }
    }
};
