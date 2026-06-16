/*
 * Shopify Pair Programming - Most Frequently Reported Problem
 * Robot / Roomba on a Grid
 *
 * Problem (Layer 1):
 *   A robot starts at position (0,0) facing NORTH on a 10x10 grid.
 *   Commands: "L" = turn left, "R" = turn right, "M" = move forward 1 step.
 *   Return final (x, y, direction).
 *
 * Extensions reported in interviews:
 *   Layer 2: Add obstacles. Robot stops and reports collision.
 *   Layer 3: Multiple robots. Serialize/deserialize robot state.
 *   Layer 4 (staff): How would you expose this as an HTTP API?
 *              How would you handle 1000 concurrent robots?
 *              What are the failure modes?
 *
 * Shopify interview tip:
 *   Start with Layer 1 working and running. Volunteer Layer 2 before being asked.
 *   Use clean enums and structs — they probe readability.
 */

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <stdexcept>

// ============================================================
// Layer 1: Basic robot movement on unbounded grid
// ============================================================

enum class Direction { NORTH, EAST, SOUTH, WEST };

std::string dirToStr(Direction d) {
    switch (d) {
        case Direction::NORTH: return "NORTH";
        case Direction::EAST:  return "EAST";
        case Direction::SOUTH: return "SOUTH";
        case Direction::WEST:  return "WEST";
    }
    return "";
}

Direction turnLeft(Direction d) {
    // NORTH -> WEST -> SOUTH -> EAST -> NORTH
    return static_cast<Direction>((static_cast<int>(d) + 3) % 4);
}

Direction turnRight(Direction d) {
    return static_cast<Direction>((static_cast<int>(d) + 1) % 4);
}

struct Position {
    int x, y;
    Direction dir;
};

Position move(Position pos) {
    switch (pos.dir) {
        case Direction::NORTH: pos.y++; break;
        case Direction::EAST:  pos.x++; break;
        case Direction::SOUTH: pos.y--; break;
        case Direction::WEST:  pos.x--; break;
    }
    return pos;
}

Position executeCommands(const std::string& commands) {
    Position pos = {0, 0, Direction::NORTH};
    for (char c : commands) {
        if (c == 'L')      pos.dir = turnLeft(pos.dir);
        else if (c == 'R') pos.dir = turnRight(pos.dir);
        else if (c == 'M') pos = move(pos);
        else throw std::invalid_argument(std::string("Unknown command: ") + c);
    }
    return pos;
}

// ============================================================
// Layer 2: Bounded grid + obstacles
// ============================================================

class Robot {
public:
    Robot(int gridW, int gridH) : gridW(gridW), gridH(gridH), pos({0, 0, Direction::NORTH}) {}

    void addObstacle(int x, int y) { obstacles.insert({x, y}); }

    // Returns false if blocked by obstacle or boundary
    bool execute(const std::string& commands) {
        for (char c : commands) {
            if (c == 'L')      pos.dir = turnLeft(pos.dir);
            else if (c == 'R') pos.dir = turnRight(pos.dir);
            else if (c == 'M') {
                Position next = move(pos);
                if (!inBounds(next) || obstacles.count({next.x, next.y})) {
                    return false; // blocked
                }
                pos = next;
            }
        }
        return true;
    }

    Position getPosition() const { return pos; }

    // Layer 3: Serialize / deserialize state
    std::string serialize() const {
        return std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + dirToStr(pos.dir);
    }

    static Robot deserialize(const std::string& state, int gridW, int gridH) {
        // parse "x,y,DIR"
        Robot r(gridW, gridH);
        size_t p1 = state.find(',');
        size_t p2 = state.find(',', p1 + 1);
        r.pos.x = std::stoi(state.substr(0, p1));
        r.pos.y = std::stoi(state.substr(p1 + 1, p2 - p1 - 1));
        std::string d = state.substr(p2 + 1);
        if (d == "NORTH")      r.pos.dir = Direction::NORTH;
        else if (d == "EAST")  r.pos.dir = Direction::EAST;
        else if (d == "SOUTH") r.pos.dir = Direction::SOUTH;
        else                   r.pos.dir = Direction::WEST;
        return r;
    }

private:
    int gridW, gridH;
    Position pos;
    std::set<std::pair<int,int>> obstacles;

    bool inBounds(const Position& p) const {
        return p.x >= 0 && p.x < gridW && p.y >= 0 && p.y < gridH;
    }
};

// ============================================================
// Staff Layer 4 Discussion Points (not code — talking points)
// ============================================================
/*
 * Q: How would you expose this as an HTTP API?
 *    - POST /robots          -> create robot, return id
 *    - POST /robots/:id/commands  -> body: {"commands": "MMLMR"}
 *    - GET  /robots/:id      -> returns state
 *    State stored in Redis (fast reads), persisted to DB async.
 *
 * Q: 1000 concurrent robots?
 *    - Each robot is independent state — horizontally scalable.
 *    - Robot ID routes to a shard (consistent hashing).
 *    - Command processing is idempotent if we add sequence numbers.
 *
 * Q: Failure modes?
 *    - Lost commands: use command log + replay (event sourcing pattern).
 *    - Stale state: optimistic locking on position updates.
 *    - What if two robots collide? Need a global grid lock or reservation system.
 */

// ============================================================
// Tests (run these during the interview to show TDD mindset)
// ============================================================

void runTests() {
    // Layer 1 tests
    {
        auto pos = executeCommands("MMRMMRMM");
        // Start NORTH, move 2 (y=2), turn right (EAST), move 2 (x=2),
        // turn right (SOUTH), move 2 (y=0)
        std::cout << "Test 1: " << pos.x << "," << pos.y << "," << dirToStr(pos.dir) << std::endl;
        // Expected: 2,0,SOUTH
    }
    {
        auto pos = executeCommands("LMLMLMLMM");
        std::cout << "Test 2: " << pos.x << "," << pos.y << "," << dirToStr(pos.dir) << std::endl;
        // 0 moves net in x, 3 in y, facing NORTH — verify by hand
    }

    // Layer 2 tests
    {
        Robot r(5, 5);
        r.addObstacle(0, 2);
        bool ok = r.execute("MM"); // would hit obstacle at (0,2)
        auto pos = r.getPosition();
        std::cout << "Test 3 (blocked): ok=" << ok << " pos=" << pos.x << "," << pos.y << std::endl;
        // Expected: ok=false, pos=(0,1)
    }

    // Layer 3 serialize/deserialize
    {
        Robot r(10, 10);
        r.execute("MRMM");
        std::string s = r.serialize();
        Robot r2 = Robot::deserialize(s, 10, 10);
        std::cout << "Test 4 (serialize): " << r2.serialize() << std::endl;
        // Should match r.serialize()
    }
}

int main() {
    runTests();
    return 0;
}
