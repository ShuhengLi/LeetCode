/*
752. Open the Lock

You have a lock in front of you with 4 circular wheels. Each wheel has 10 slots:
'0', '1', '2', ..., '9'. The wheels can rotate freely and wrap around: for
example we can turn '9' to be '0', or '0' to be '9'. Each move consists of
turning one wheel one slot.

The lock initially starts at '0000', a string representing the state of the
4 wheels.

You are given a list of deadends dead ends, meaning if the lock displays any
of these codes, the wheels of the lock will stop turning and you will be unable
to open it.

Given a target representing the value of the wheels that will unlock the lock,
return the minimum total number of turns required to open the lock, or -1 if
it is impossible.

Example 1:
  Input: deadends = ["0201","0101","0102","1212","2002"], target = "0202"
  Output: 6
  Explanation: "0000" -> "1000" -> "1100" -> "1200" -> "1201" -> "1202" -> "0202"

Example 2:
  Input: deadends = ["8888"], target = "0009"
  Output: 1

Example 3:
  Input: deadends = ["8887","8889","8878","8898","8788","8988","7888","9888"],
         target = "8888"
  Output: -1

Constraints:
  - 1 <= deadends.length <= 500
  - deadends[i].length == 4
  - target.length == 4
  - target will not be in the list deadends.
  - target and deadends[i] consist of digits only.
*/
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

// Standard BFS on state space graph. Each state is a 4-digit string,
// each turn produces 8 neighbors (4 wheels x 2 directions).
// Time: O(10^4 * 4), Space: O(10^4)
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> dead(deadends.begin(), deadends.end());
        if (dead.count("0000")) return -1;

        queue<string> q;
        q.push("0000");
        unordered_set<string> visited;
        visited.insert("0000");
        int steps = 0;

        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                string cur = q.front(); q.pop();
                if (cur == target) return steps;

                for (int i = 0; i < 4; i++) {
                    for (int d : {-1, 1}) {
                        string next = cur;
                        next[i] = (cur[i] - '0' + d + 10) % 10 + '0';
                        if (!dead.count(next) && !visited.count(next)) {
                            visited.insert(next);
                            q.push(next);
                        }
                    }
                }
            }
            steps++;
        }
        return -1;
    }
};

// ===================== Improved Solution =====================
// Bidirectional BFS - search from "0000" and target simultaneously.
// Meets in the middle, significantly reducing explored states.
// Time: O(10^4 * 4), but much faster in practice
// Space: O(10^4)
class Solution_v2 {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> dead(deadends.begin(), deadends.end());
        if (dead.count("0000")) return -1;
        if (target == "0000") return 0;

        unordered_set<string> front = {"0000"};
        unordered_set<string> back = {target};
        unordered_set<string> visited = {"0000", target};
        int steps = 0;

        while (!front.empty() && !back.empty()) {
            if (front.size() > back.size()) swap(front, back);

            unordered_set<string> nextFront;
            for (const string& cur : front) {
                for (int i = 0; i < 4; i++) {
                    for (int d : {-1, 1}) {
                        string next = cur;
                        next[i] = (cur[i] - '0' + d + 10) % 10 + '0';
                        if (back.count(next)) return steps + 1;
                        if (!dead.count(next) && !visited.count(next)) {
                            visited.insert(next);
                            nextFront.insert(next);
                        }
                    }
                }
            }
            front = nextFront;
            steps++;
        }
        return -1;
    }
};
