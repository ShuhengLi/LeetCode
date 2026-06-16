import java.util.*;

/**
 * Shopify Pair Programming - Robot / Mars Rover on a Grid
 * Confirmed: Aug 2024, Apr 2025 (JoinTaro).
 *
 * Java 11.  Run:  javac RobotGrid.java && java RobotGrid
 *
 * Layer 1: single robot, L/R/M commands, report (x, y, dir)
 * Layer 2: bounded grid + obstacles, blocked moves
 * Layer 3: serialize / deserialize state
 * Layer 4 (staff, discussion only): HTTP API, concurrency, failure modes
 */
public class RobotGrid {

    /** Clockwise order (N,E,S,W) makes turning trivial via ordinal arithmetic. */
    enum Direction {
        NORTH(0, 1), EAST(1, 0), SOUTH(0, -1), WEST(-1, 0);
        final int dx, dy;
        Direction(int dx, int dy) { this.dx = dx; this.dy = dy; }
        Direction left()  { return values()[(ordinal() + 3) % 4]; }
        Direction right() { return values()[(ordinal() + 1) % 4]; }
    }

    /** Immutable value type. (Java 17+ would make this a record.) */
    static final class Position {
        private final int x, y;
        private final Direction dir;
        Position(int x, int y, Direction dir) { this.x = x; this.y = y; this.dir = dir; }
        int x() { return x; }
        int y() { return y; }
        Direction dir() { return dir; }
        Position move()  { return new Position(x + dir.dx, y + dir.dy, dir); }
        Position left()  { return new Position(x, y, dir.left()); }
        Position right() { return new Position(x, y, dir.right()); }
        @Override public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Position)) return false;
            Position p = (Position) o;
            return x == p.x && y == p.y && dir == p.dir;
        }
        @Override public int hashCode() { return Objects.hash(x, y, dir); }
        @Override public String toString() { return "(" + x + "," + y + "," + dir + ")"; }
    }

    // ---- Layer 1: unbounded movement ----
    static Position execute(String commands) {
        Position p = new Position(0, 0, Direction.NORTH);
        for (char c : commands.toCharArray()) {
            switch (c) {
                case 'L': p = p.left();  break;
                case 'R': p = p.right(); break;
                case 'M': p = p.move();  break;
                default:  throw new IllegalArgumentException("Unknown command: " + c);
            }
        }
        return p;
    }

    // ---- Layer 2 + 3: bounded grid, obstacles, serialization ----
    static final class Cell {
        private final int x, y;
        Cell(int x, int y) { this.x = x; this.y = y; }
        @Override public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Cell)) return false;
            Cell c = (Cell) o;
            return x == c.x && y == c.y;
        }
        @Override public int hashCode() { return Objects.hash(x, y); }
    }

    static class Robot {
        private final int width, height;
        private final Set<Cell> obstacles = new HashSet<>();
        private Position pos = new Position(0, 0, Direction.NORTH);

        Robot(int width, int height) {
            if (width <= 0 || height <= 0)
                throw new IllegalArgumentException("Grid dimensions must be positive");
            this.width = width;
            this.height = height;
        }

        void addObstacle(int x, int y) { obstacles.add(new Cell(x, y)); }

        /** Returns false if a move was blocked by a wall or obstacle (stops there). */
        boolean execute(String commands) {
            for (char c : commands.toCharArray()) {
                switch (c) {
                    case 'L': pos = pos.left();  break;
                    case 'R': pos = pos.right(); break;
                    case 'M':
                        Position next = pos.move();
                        if (!inBounds(next) || obstacles.contains(new Cell(next.x(), next.y())))
                            return false;
                        pos = next;
                        break;
                    default: throw new IllegalArgumentException("Unknown command: " + c);
                }
            }
            return true;
        }

        Position position() { return pos; }

        private boolean inBounds(Position p) {
            return p.x() >= 0 && p.x() < width && p.y() >= 0 && p.y() < height;
        }

        String serialize() { return pos.x() + "," + pos.y() + "," + pos.dir(); }

        static Robot deserialize(String state, int width, int height) {
            String[] parts = state.split(",");
            Robot r = new Robot(width, height);
            r.pos = new Position(Integer.parseInt(parts[0]),
                                 Integer.parseInt(parts[1]),
                                 Direction.valueOf(parts[2]));
            return r;
        }
    }

    /* ---- Layer 4 talking points (no code) ----
     * HTTP API:  POST /robots, POST /robots/{id}/commands, GET /robots/{id}
     * 1000 robots: each robot is independent state -> shard by id (consistent hashing).
     * Idempotency: attach a sequence number to each command batch.
     * Failure modes: lost commands -> command log + replay (event sourcing);
     *                two robots same cell -> reservation/lock on the grid.
     */

    public static void main(String[] args) {
        // Layer 1
        Position p = execute("MMRMMRMM");
        check("Layer1", p.equals(new Position(2, 0, Direction.SOUTH)), p.toString());

        // Layer 2: blocked by an obstacle, position frozen at the wall
        Robot r = new Robot(5, 5);
        r.addObstacle(0, 2);
        boolean ok = r.execute("MM");
        check("Blocked", !ok && r.position().equals(new Position(0, 1, Direction.NORTH)),
              r.position().toString());

        // Layer 3: serialize round-trip
        Robot r2 = new Robot(10, 10);
        r2.execute("MRMM");
        Robot restored = Robot.deserialize(r2.serialize(), 10, 10);
        check("Serialize", restored.serialize().equals(r2.serialize()), restored.serialize());
    }

    static void check(String name, boolean pass, String actual) {
        System.out.printf("%-10s %s  (%s)%n", name, pass ? "PASS" : "FAIL", actual);
    }
}
