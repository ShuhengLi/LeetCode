/*
 * Shopify Pair Programming - Secret Santa (Derangement)
 * Confirmed: Oct 2025 (PracHub, Technical Screen — given AFTER LRU Cache)
 *
 * Problem:
 *   Given a CSV with columns: name, email (minimum 2 participants)
 *   Produce a CSV with columns: giver_name, giver_email, receiver_name, receiver_email
 *   Rules:
 *     - No person is assigned to themselves
 *     - Each person gives exactly one gift
 *     - Each person receives exactly one gift (perfect derangement)
 *     - Assignment must be randomized (no simple position shift)
 *     - Handle < 2 participants with clear error
 *     - Up to 10^4 participants
 *
 * Key insight: this is a random derangement (permutation with no fixed points).
 * A Fisher-Yates shuffle + fix-up is a clean O(n) solution.
 *
 * Common mistake: simple position shift (+1 mod n) is NOT random.
 * Shopify interviewer will ask "is this truly random?" — answer must be yes.
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <numeric>   // FIX: std::iota used in generateAssignments()
#include <random>
#include <cassert>

struct Participant {
    std::string name;
    std::string email;
};

struct Assignment {
    Participant giver;
    Participant receiver;
};

// ============================================================
// CSV parsing
// ============================================================

std::vector<Participant> parseCSV(const std::string& csv) {
    std::vector<Participant> result;
    std::istringstream stream(csv);
    std::string line;

    bool header = true;
    while (std::getline(stream, line)) {
        if (header) { header = false; continue; } // skip header row
        if (line.empty()) continue;

        auto comma = line.find(',');
        if (comma == std::string::npos) {
            throw std::invalid_argument("Malformed CSV line: " + line);
        }
        result.push_back({line.substr(0, comma), line.substr(comma + 1)});
    }
    return result;
}

std::string toCSV(const std::vector<Assignment>& assignments) {
    std::ostringstream out;
    out << "giver_name,giver_email,receiver_name,receiver_email\n";
    for (const auto& a : assignments) {
        out << a.giver.name << "," << a.giver.email << ","
            << a.receiver.name << "," << a.receiver.email << "\n";
    }
    return out.str();
}

// ============================================================
// Derangement: Fisher-Yates shuffle with derangement fix-up
// ============================================================
// This runs in O(n) expected time. The fix-up handles the case
// where someone ends up giving to themselves.

std::vector<Assignment> generateAssignments(std::vector<Participant> participants) {
    int n = participants.size();
    if (n < 2) {
        throw std::invalid_argument("Need at least 2 participants, got " + std::to_string(n));
    }

    std::mt19937 rng(std::random_device{}());

    // Shuffle until we get a derangement
    // Expected iterations: e (~2.718), so this is effectively O(n)
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);

    while (true) {
        std::shuffle(indices.begin(), indices.end(), rng);

        bool isDerangement = true;
        for (int i = 0; i < n; i++) {
            if (indices[i] == i) { isDerangement = false; break; }
        }
        if (isDerangement) break;
    }

    std::vector<Assignment> result;
    result.reserve(n);
    for (int i = 0; i < n; i++) {
        result.push_back({participants[i], participants[indices[i]]});
    }
    return result;
}

// ============================================================
// Main entry point
// ============================================================

std::string runSecretSanta(const std::string& inputCSV) {
    auto participants = parseCSV(inputCSV);
    auto assignments = generateAssignments(participants);
    return toCSV(assignments);
}

// ============================================================
// Discussion Points
// ============================================================
/*
 * Q: Why not just shift everyone by 1 (i+1 mod n)?
 *    - Not random: given giver, receiver is perfectly predictable.
 *    - Doesn't generalize to n=2 when you want non-trivial randomness.
 *    - Shopify values randomness here to preserve the "secret" in Secret Santa.
 *
 * Q: Is shuffle-until-derangement efficient for n=10^4?
 *    - Yes. P(random permutation is a derangement) ≈ 1/e ≈ 37%.
 *      Expected iterations = e ≈ 2.7. Total expected work = O(n).
 *
 * Q: Alternative: Sattolo's algorithm (guaranteed derangement in O(n))
 *    - In Fisher-Yates, when processing index i, swap with j in [0, i-1].
 *    - Sattolo: swap with j in [0, i-1] \ {i} (exclude i itself).
 *    - Produces a derangement guaranteed, but only generates cyclic permutations.
 *    - Slightly less uniform distribution; acceptable for Secret Santa.
 *
 * Q: What if a participant is added last-minute?
 *    - Re-run the full assignment (simple, n is small for Secret Santa).
 *    - Or: single-element insertion: swap new person with a random existing pair.
 *
 * Q: Error handling for < 2 participants?
 *    - Return an error response, not a panic.
 *    - In production: log the error with participant count, return HTTP 422.
 */

// ============================================================
// Tests
// ============================================================

void verifyDerangement(const std::vector<Assignment>& assignments) {
    for (const auto& a : assignments) {
        assert(a.giver.name != a.receiver.name && "Someone assigned to themselves!");
    }

    // Check bijection: each person appears exactly once as receiver
    std::vector<std::string> receivers;
    for (const auto& a : assignments) receivers.push_back(a.receiver.name);
    std::sort(receivers.begin(), receivers.end());
    for (int i = 1; i < (int)receivers.size(); i++) {
        assert(receivers[i] != receivers[i-1] && "Duplicate receiver!");
    }
}

void runTests() {
    // Basic case
    {
        std::string csv = "name,email\nAlice,alice@x.com\nBob,bob@x.com\nCarol,carol@x.com\n";
        auto output = runSecretSanta(csv);
        std::cout << "3 participants:\n" << output << std::endl;

        auto participants = parseCSV(csv);
        auto assignments = generateAssignments(participants);
        verifyDerangement(assignments);
        std::cout << "Derangement check: PASS" << std::endl;
    }

    // Too few participants
    {
        try {
            std::string csv = "name,email\nAlice,alice@x.com\n";
            runSecretSanta(csv);
            std::cout << "FAIL: should have thrown" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "< 2 participants error: PASS (" << e.what() << ")" << std::endl;
        }
    }

    // Large input: verify no self-assignments
    {
        std::string csv = "name,email\n";
        for (int i = 0; i < 100; i++) {
            csv += "Person" + std::to_string(i) + ",p" + std::to_string(i) + "@x.com\n";
        }
        auto participants = parseCSV(csv);
        auto assignments = generateAssignments(participants);
        verifyDerangement(assignments);
        std::cout << "100 participants derangement: PASS" << std::endl;
    }

    // n=2 edge case (must swap)
    {
        std::string csv = "name,email\nAlice,a@x.com\nBob,b@x.com\n";
        auto assignments = generateAssignments(parseCSV(csv));
        verifyDerangement(assignments);
        std::cout << "n=2 edge case: PASS" << std::endl;
    }
}

int main() {
    runTests();
    return 0;
}
