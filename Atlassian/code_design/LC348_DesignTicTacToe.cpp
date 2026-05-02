/*
 * 348. Design Tic-Tac-Toe
 * Medium
 * [Atlassian Code Design Round - LLD classic]
 *
 * Design n x n Tic-Tac-Toe game with two players. Each move is at (row, col)
 * by player p (1 or 2). Return:
 *   0 - no one wins yet
 *   1 - player 1 wins
 *   2 - player 2 wins
 *
 * Optimal: O(1) per move (vs naive O(n)).
 *
 * ─────────────────────────────────────────────────────────────────
 * Key idea:
 *   Track per-row, per-col, diagonal, anti-diagonal counts.
 *   Player 1 increments by +1, player 2 by -1.
 *   When |count| == n, that player wins.
 * ─────────────────────────────────────────────────────────────────
 */
#include <bits/stdc++.h>
using namespace std;

class TicTacToe {
    int n;
    vector<int> rows;     // size n
    vector<int> cols;     // size n
    int diag = 0;         // top-left to bottom-right
    int antiDiag = 0;     // top-right to bottom-left

public:
    TicTacToe(int n) : n(n), rows(n, 0), cols(n, 0) {}

    int move(int row, int col, int player) {
        int delta = (player == 1) ? +1 : -1;
        int target = (player == 1) ? n : -n;

        rows[row] += delta;
        cols[col] += delta;
        if (row == col)        diag += delta;
        if (row + col == n-1)  antiDiag += delta;

        if (rows[row] == target || cols[col] == target
            || diag == target || antiDiag == target) {
            return player;
        }
        return 0;
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: Add support for k-in-a-row instead of n-in-a-row?
 * A: Each move check sliding window in 4 directions; not O(1) anymore but
 *    O(k). Or maintain "longest streak so far" per cell.
 *
 * Q: 3 players? Players in a circle?
 * A: Counters can't be ±1 anymore. Track per-(row/col/diag) per-player counts
 *    in 2D map. O(p) per move where p = #players.
 *
 * Q: Undo move?
 * A: Apply -delta to the four buckets; reset winner detection.
 *
 * Q: Spectator API — return current board state?
 * A: Add explicit grid[][]; counters stay for fast win check.
 *
 * Q: How to test?
 * A: Parameterized tests: random sequences, deterministic seeds, edge cases
 *    (filled board → tie, single move, illegal move → throw or ignore).
 *
 * Q: Atlassian angle — collaborative drawing / canvas?
 * A: Same pattern: each cell has owner, multi-user updates via WebSocket.
 *    But concurrency/conflict resolution is the harder problem there.
 */

int main() {
    TicTacToe game(3);
    cout << game.move(0, 0, 1) << endl;  // 0
    cout << game.move(0, 2, 2) << endl;  // 0
    cout << game.move(2, 2, 1) << endl;  // 0
    cout << game.move(1, 1, 2) << endl;  // 0
    cout << game.move(2, 0, 1) << endl;  // 0
    cout << game.move(1, 0, 2) << endl;  // 0
    cout << game.move(2, 1, 1) << endl;  // 1 — player 1 wins on row 2
    return 0;
}
