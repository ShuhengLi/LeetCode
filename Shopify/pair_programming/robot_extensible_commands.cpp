/*
 * Shopify Pair Programming - Robot with Extensible Command System
 * Confirmed: Dec 2025 (PracHub, MLE Technical Screen)
 *
 * This is a VARIANT of the basic robot grid problem.
 * The KEY DIFFERENCE: the problem spec explicitly tells you
 * "valid commands will keep expanding" (examples given: backward, jump, undo).
 *
 * This means the interviewer is testing whether you will:
 *   a) Hard-code if/else chains (wrong — doesn't scale)
 *   b) Design a command registry / command-object pattern (correct)
 *
 * Staff-level expectation: you should proactively propose the extensible
 * design BEFORE being asked, not wait for the interviewer to say "now add undo."
 *
 * Problem:
 *   Robot starts at (x0, y0, dir0).
 *   Execute a string of commands. Return final state.
 *   Commands: F (forward), L (turn left), R (turn right)
 *   More commands WILL be added.
 *
 * Layer 2: Invalid command policy (ignore / raise / collect failures)
 * Layer 3: Testing discussion — unit, property-based, edge cases
 * Layer 4: Add UNDO command (undo last non-UNDO move)
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <vector>
#include <stack>
#include <cassert>

// ============================================================
// Core types
// ============================================================

enum class Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

std::string dirStr(Direction d) {
    switch (d) {
        case Direction::NORTH: return "N";
        case Direction::EAST:  return "E";
        case Direction::SOUTH: return "S";
        case Direction::WEST:  return "W";
    }
    return "?";
}

struct State {
    int x, y;
    Direction dir;

    bool operator==(const State& o) const {
        return x == o.x && y == o.y && dir == o.dir;
    }
};

// ============================================================
// Command registry: new commands are registered, not hard-coded
// ============================================================

using CommandFn = std::function<State(State)>;

class CommandRegistry {
public:
    void registerCommand(char symbol, CommandFn fn) {
        commands[symbol] = std::move(fn);
    }

    bool has(char symbol) const { return commands.count(symbol) > 0; }

    State execute(char symbol, State state) const {
        return commands.at(symbol)(state);
    }

private:
    std::unordered_map<char, CommandFn> commands;
};

// ============================================================
// Default commands
// ============================================================

CommandRegistry makeDefaultRegistry() {
    CommandRegistry reg;

    reg.registerCommand('F', [](State s) {
        switch (s.dir) {
            case Direction::NORTH: s.y++; break;
            case Direction::EAST:  s.x++; break;
            case Direction::SOUTH: s.y--; break;
            case Direction::WEST:  s.x--; break;
        }
        return s;
    });

    reg.registerCommand('L', [](State s) {
        s.dir = static_cast<Direction>((static_cast<int>(s.dir) + 3) % 4);
        return s;
    });

    reg.registerCommand('R', [](State s) {
        s.dir = static_cast<Direction>((static_cast<int>(s.dir) + 1) % 4);
        return s;
    });

    // Layer 4: Backward command (easy to add without changing anything else)
    reg.registerCommand('B', [](State s) {
        switch (s.dir) {
            case Direction::NORTH: s.y--; break;
            case Direction::EAST:  s.x--; break;
            case Direction::SOUTH: s.y++; break;
            case Direction::WEST:  s.x++; break;
        }
        return s;
    });

    return reg;
}

// ============================================================
// Robot executor with configurable invalid-command policy
// ============================================================

enum class InvalidPolicy { IGNORE, RAISE, COLLECT };

struct ExecutionResult {
    State finalState;
    std::vector<char> invalidCommands; // only populated with COLLECT policy
};

ExecutionResult execute(
    const std::string& commands,
    State initial,
    const CommandRegistry& registry,
    InvalidPolicy policy = InvalidPolicy::IGNORE)
{
    State state = initial;
    std::vector<char> invalid;

    for (char c : commands) {
        if (!registry.has(c)) {
            if (policy == InvalidPolicy::RAISE) {
                throw std::invalid_argument(std::string("Unknown command: ") + c);
            } else if (policy == InvalidPolicy::COLLECT) {
                invalid.push_back(c);
            }
            // IGNORE: do nothing
            continue;
        }
        state = registry.execute(c, state);
    }

    return {state, invalid};
}

// ============================================================
// Layer 4: UNDO support via history stack
// ============================================================

class RobotWithUndo {
public:
    explicit RobotWithUndo(State initial, CommandRegistry registry)
        : state(initial), registry(std::move(registry)) {}

    void execute(char c) {
        if (c == 'U') {
            undo();
            return;
        }
        if (!registry.has(c)) return; // IGNORE policy
        history.push(state);
        state = registry.execute(c, state);
    }

    void execute(const std::string& commands) {
        for (char c : commands) execute(c);
    }

    State getState() const { return state; }

private:
    State state;
    CommandRegistry registry;
    std::stack<State> history;

    void undo() {
        if (history.empty()) return; // nothing to undo
        state = history.top();
        history.pop();
    }
};

// ============================================================
// Tests
// ============================================================

void runTests() {
    auto registry = makeDefaultRegistry();

    // Basic movement
    {
        auto [s, _] = execute("FFRFF", {0, 0, Direction::NORTH}, registry);
        assert(s.x == 2 && s.y == 2);
        std::cout << "FFRFF: (" << s.x << "," << s.y << "," << dirStr(s.dir) << ") PASS" << std::endl;
    }

    // Invalid command — ignore policy
    {
        auto [s, inv] = execute("FXF", {0, 0, Direction::NORTH}, registry, InvalidPolicy::IGNORE);
        assert(s.y == 2); // X ignored, 2 forward steps
        std::cout << "Ignore invalid: PASS" << std::endl;
    }

    // Invalid command — collect policy
    {
        auto [s, inv] = execute("FXF", {0, 0, Direction::NORTH}, registry, InvalidPolicy::COLLECT);
        assert(inv.size() == 1 && inv[0] == 'X');
        std::cout << "Collect invalid: PASS (" << inv.size() << " invalid)" << std::endl;
    }

    // Invalid command — raise policy
    {
        try {
            execute("FXF", {0, 0, Direction::NORTH}, registry, InvalidPolicy::RAISE);
            std::cout << "FAIL: should have thrown" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "Raise on invalid: PASS" << std::endl;
        }
    }

    // Backward command (dynamically registered, no core changes)
    {
        auto [s, _] = execute("FB", {0, 0, Direction::NORTH}, registry);
        assert(s.y == 0); // F up 1, B down 1 = net 0
        std::cout << "Forward + Backward: PASS (" << s.x << "," << s.y << ")" << std::endl;
    }

    // Undo
    {
        RobotWithUndo robot({0, 0, Direction::NORTH}, makeDefaultRegistry());
        robot.execute("FF");      // (0,2,N)
        robot.execute("U");       // undo -> (0,1,N)
        robot.execute("R");       // (0,1,E)
        robot.execute("U");       // undo -> (0,1,N)
        auto s = robot.getState();
        assert(s.x == 0 && s.y == 1 && s.dir == Direction::NORTH);
        std::cout << "Undo test: (" << s.x << "," << s.y << "," << dirStr(s.dir) << ") PASS" << std::endl;
    }
}

int main() {
    runTests();
    return 0;
}
