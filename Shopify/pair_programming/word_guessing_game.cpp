/*
 * Shopify Pair Programming - Word Guessing Game (Wordle-Like)
 * Confirmed: Aug 2025 (PracHub, SWE/MLE), Jan 2026 (PracHub)
 *
 * Problem (Layer 1):
 *   Implement a 4-letter word guessing game:
 *   - Player has 5 attempts
 *   - Each guess must be exactly 4 letters AND exist in a provided dictionary
 *     - Invalid: return "INVALID", don't consume an attempt, don't give hints
 *   - Hint format per letter:
 *       '1' = correct letter, correct position
 *       '0' = correct letter, wrong position
 *       '-' = letter not in target at all
 *   - Wordle-style duplicate handling:
 *       Mark exact matches first, then mark misplaced ONLY if unmatched
 *       occurrence remains in target
 *   - Return: (won: bool, attempts_used: int, results: list of hint strings)
 *
 * Layer 2 (MLE variant):
 *   - Full CLI app: load dictionary from file
 *   - Persist statistics: games played, wins, current/max streak
 *   - Unit tests for hint logic, validation, and statistics
 *
 * This is the trickiest part: duplicate letter handling.
 * Example: target = "BALE", guess = "ABBA"
 *   Step 1 exact matches: pos 0 (A vs B) no, pos 1 (B vs A) no, pos 2 (B vs L) no, pos 3 (A vs E) no
 *   -> no exact matches
 *   Step 2 misplaced: A is in target (A at pos 1), mark pos 0 as '0', consume that A
 *                     B is in target (B at pos 0), mark pos 1 as '0', consume that B
 *                     B: no more unmatched B in target -> mark pos 2 as '-'
 *                     A: no more unmatched A in target -> mark pos 3 as '-'
 *   Result: "00--"
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <fstream>

// ============================================================
// Core: Hint generation (the hard part)
// ============================================================

std::string getHint(const std::string& guess, const std::string& target) {
    int n = target.size();
    std::string hint(n, '-');
    std::unordered_map<char, int> remaining; // unmatched chars in target

    // Pass 1: exact matches
    for (int i = 0; i < n; i++) {
        if (guess[i] == target[i]) {
            hint[i] = '1';
        } else {
            remaining[target[i]]++;
        }
    }

    // Pass 2: misplaced (only consume unmatched target chars)
    for (int i = 0; i < n; i++) {
        if (hint[i] == '1') continue;
        if (remaining.count(guess[i]) && remaining[guess[i]] > 0) {
            hint[i] = '0';
            remaining[guess[i]]--;
        }
    }

    return hint;
}

// ============================================================
// Layer 1: Game engine
// ============================================================

struct GameResult {
    bool won;
    int attemptsUsed;
    std::vector<std::string> hints; // hint per valid attempt
};

class WordGame {
public:
    explicit WordGame(std::unordered_set<std::string> dictionary, int maxAttempts = 5)
        : dict(std::move(dictionary)), maxAttempts(maxAttempts) {}

    GameResult play(const std::string& target, const std::vector<std::string>& guesses) {
        std::string t = toUpper(target);
        if (!dict.count(t)) throw std::invalid_argument("Target not in dictionary: " + target);

        GameResult result = {false, 0, {}};
        int attempts = 0;

        for (const auto& rawGuess : guesses) {
            if (result.won || attempts >= maxAttempts) break;

            std::string guess = toUpper(rawGuess);

            // Validate: must be right length and in dictionary
            if ((int)guess.size() != (int)t.size() || !dict.count(guess)) {
                result.hints.push_back("INVALID");
                continue; // does NOT consume an attempt
            }

            attempts++;
            std::string hint = getHint(guess, t);
            result.hints.push_back(hint);

            if (hint == std::string(t.size(), '1')) {
                result.won = true;
            }
        }

        result.attemptsUsed = attempts;
        return result;
    }

private:
    std::unordered_set<std::string> dict;
    int maxAttempts;

    static std::string toUpper(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }
};

// ============================================================
// Layer 2: Statistics persistence
// ============================================================

struct Stats {
    int gamesPlayed = 0;
    int wins = 0;
    int currentStreak = 0;
    int maxStreak = 0;

    void recordGame(bool won) {
        gamesPlayed++;
        if (won) {
            wins++;
            currentStreak++;
            maxStreak = std::max(maxStreak, currentStreak);
        } else {
            currentStreak = 0;
        }
    }

    void saveTo(const std::string& path) const {
        std::ofstream f(path);
        f << gamesPlayed << "\n" << wins << "\n"
          << currentStreak << "\n" << maxStreak << "\n";
    }

    static Stats loadFrom(const std::string& path) {
        Stats s;
        std::ifstream f(path);
        if (!f.is_open()) return s;
        f >> s.gamesPlayed >> s.wins >> s.currentStreak >> s.maxStreak;
        return s;
    }

    void print() const {
        std::cout << "Games: " << gamesPlayed
                  << " | Wins: " << wins
                  << " | Streak: " << currentStreak
                  << " | Best: " << maxStreak << std::endl;
    }
};

// ============================================================
// Staff Discussion Points
// ============================================================
/*
 * Q: How do you test the hint logic?
 *    - Property: hint of correct guess is always "1111"
 *    - Property: hint never has more '0'/'1' for a letter than occurrences in target
 *    - Table-driven tests for duplicate edge cases (the trickiest part)
 *    - Fuzz: random guess vs random target, check hint symmetry properties
 *
 * Q: How would you scale this to a multiplayer daily challenge?
 *    - One target word per day (seeded by date), same for all users
 *    - Store guesses in Redis per user per day (TTL = 24h)
 *    - Leaderboard: sorted set in Redis, score = attempts used (lower is better)
 *
 * Q: 100,000 word dictionary lookup — is unordered_set fast enough?
 *    - Yes: O(1) average lookup, ~4MB for 100k 10-char words
 *    - Alternative: Bloom filter for valid word check (less memory, false positives)
 *      For this game, false positives (accepting an invalid word) are unacceptable
 *      -> stick with hash set
 */

// ============================================================
// Tests — run these at the start to prove correctness
// ============================================================

void runTests() {
    std::unordered_set<std::string> dict = {"BALE", "ABBA", "WORD", "TEST", "CRANE"};
    WordGame game(dict);

    // Basic win
    {
        auto r = game.play("BALE", {"BALE"});
        std::cout << "Win on first: " << (r.won && r.attemptsUsed == 1 ? "PASS" : "FAIL") << std::endl;
    }

    // Hint correctness: ABBA vs BALE
    {
        std::string hint = getHint("ABBA", "BALE");
        std::cout << "ABBA vs BALE hint: " << hint << " (expected: 00--)" << std::endl;
    }

    // All wrong
    {
        std::string hint = getHint("XXXX", "BALE");
        std::cout << "All wrong: " << hint << " (expected: ----)" << std::endl;
    }

    // Exact match
    {
        std::string hint = getHint("BALE", "BALE");
        std::cout << "Exact match: " << hint << " (expected: 1111)" << std::endl;
    }

    // Invalid guess (not in dict) doesn't consume attempt
    {
        auto r = game.play("BALE", {"ZZZZ", "BALE"});
        std::cout << "Invalid skipped: hints=" << r.hints.size()
                  << " attempts=" << r.attemptsUsed
                  << " " << (r.hints[0] == "INVALID" && r.attemptsUsed == 1 ? "PASS" : "FAIL")
                  << std::endl;
    }

    // Run out of attempts
    {
        std::vector<std::string> guesses(5, "WORD");
        auto r = game.play("BALE", guesses);
        std::cout << "Max attempts: won=" << r.won
                  << " attempts=" << r.attemptsUsed
                  << (r.attemptsUsed == 5 && !r.won ? " PASS" : " FAIL") << std::endl;
    }

    // Duplicate letter edge: target "CRANE", guess "CCCCC" (same length!)
    // Only 1 C in target -> first C is exact '1', remaining C's are absent '-'
    {
        std::string hint = getHint("CCCCC", "CRANE");
        std::cout << "Dup letter: " << hint << " (expected: 1----)" << std::endl;
    }

    // Stats
    {
        Stats s;
        s.recordGame(true);
        s.recordGame(true);
        s.recordGame(false);
        s.recordGame(true);
        s.print(); // Games:4 Wins:3 Streak:1 Best:2
    }
}

int main() {
    runTests();
    return 0;
}
