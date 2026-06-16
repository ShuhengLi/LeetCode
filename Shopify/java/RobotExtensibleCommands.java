import java.util.*;
import java.util.function.UnaryOperator;

/**
 * Shopify Pair Programming - Robot with an Extensible Command System
 * Confirmed: Dec 2025 (PracHub, MLE Technical Screen).
 *
 * Java 11.  Run:  javac RobotExtensibleCommands.java && java RobotExtensibleCommands
 *
 * KEY: the spec explicitly says "valid commands will keep expanding"
 * (backward, jump, undo...). The whole point is the Open/Closed Principle:
 * add a command by REGISTERING it, never by editing the executor's switch.
 *
 * SOLID, not over-engineered: a Command is just State -> State. No class
 * explosion, no visitor, no DI container - just a map of small functions.
 */
public class RobotExtensibleCommands {

    enum Direction {
        NORTH(0, 1), EAST(1, 0), SOUTH(0, -1), WEST(-1, 0);
        final int dx, dy;
        Direction(int dx, int dy) { this.dx = dx; this.dy = dy; }
        Direction left()  { return values()[(ordinal() + 3) % 4]; }
        Direction right() { return values()[(ordinal() + 1) % 4]; }
    }

    static final class State {
        private final int x, y;
        private final Direction dir;
        State(int x, int y, Direction dir) { this.x = x; this.y = y; this.dir = dir; }
        int x() { return x; }
        int y() { return y; }
        Direction dir() { return dir; }
        @Override public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof State)) return false;
            State s = (State) o;
            return x == s.x && y == s.y && dir == s.dir;
        }
        @Override public int hashCode() { return Objects.hash(x, y, dir); }
        @Override public String toString() { return "(" + x + "," + y + "," + dir + ")"; }
    }

    /** A command is a pure State transformation. Named for readability. */
    @FunctionalInterface
    interface Command extends UnaryOperator<State> {}

    /** Open for extension (register), closed for modification (no switch to edit). */
    static class CommandRegistry {
        private final Map<Character, Command> commands = new HashMap<>();
        CommandRegistry register(char symbol, Command cmd) { commands.put(symbol, cmd); return this; }
        boolean has(char symbol) { return commands.containsKey(symbol); }
        State apply(char symbol, State s) { return commands.get(symbol).apply(s); }
    }

    enum InvalidPolicy { IGNORE, RAISE, COLLECT }

    static final class ExecutionResult {
        private final State finalState;
        private final List<Character> invalid;
        ExecutionResult(State finalState, List<Character> invalid) {
            this.finalState = finalState; this.invalid = invalid;
        }
        State finalState() { return finalState; }
        List<Character> invalid() { return invalid; }
    }

    static CommandRegistry defaultRegistry() {
        return new CommandRegistry()
            .register('F', s -> new State(s.x() + s.dir().dx, s.y() + s.dir().dy, s.dir()))
            .register('B', s -> new State(s.x() - s.dir().dx, s.y() - s.dir().dy, s.dir()))
            .register('L', s -> new State(s.x(), s.y(), s.dir().left()))
            .register('R', s -> new State(s.x(), s.y(), s.dir().right()));
    }

    static ExecutionResult execute(String commands, State initial,
                                   CommandRegistry registry, InvalidPolicy policy) {
        State state = initial;
        List<Character> invalid = new ArrayList<>();
        for (char c : commands.toCharArray()) {
            if (!registry.has(c)) {
                switch (policy) {
                    case RAISE:   throw new IllegalArgumentException("Unknown command: " + c);
                    case COLLECT: invalid.add(c); break;
                    case IGNORE:  break;
                }
                continue;
            }
            state = registry.apply(c, state);
        }
        return new ExecutionResult(state, invalid);
    }

    /**
     * Layer 4: UNDO. Note UNDO is NOT a pure Command - it needs history, which
     * is executor state. Keeping it out of the registry is a deliberate boundary
     * (Single Responsibility): commands transform position; the executor owns history.
     */
    static class UndoableRobot {
        private final CommandRegistry registry;
        private final Deque<State> history = new ArrayDeque<>();
        private State state;

        UndoableRobot(State initial, CommandRegistry registry) {
            this.state = initial;
            this.registry = registry;
        }
        void execute(char c) {
            if (c == 'U') { undo(); return; }
            if (!registry.has(c)) return;          // IGNORE unknowns
            history.push(state);
            state = registry.apply(c, state);
        }
        void execute(String s) { for (char c : s.toCharArray()) execute(c); }
        private void undo() { if (!history.isEmpty()) state = history.pop(); }
        State state() { return state; }
    }

    public static void main(String[] args) {
        CommandRegistry reg = defaultRegistry();

        ExecutionResult r1 = execute("FFRFF", new State(0, 0, Direction.NORTH), reg, InvalidPolicy.IGNORE);
        check("Basic", r1.finalState().equals(new State(2, 2, Direction.EAST)), r1.finalState().toString());

        ExecutionResult r2 = execute("FXF", new State(0, 0, Direction.NORTH), reg, InvalidPolicy.COLLECT);
        check("Collect", r2.invalid().equals(Collections.singletonList('X')) && r2.finalState().y() == 2,
              r2.invalid().toString());

        try {
            execute("FXF", new State(0, 0, Direction.NORTH), reg, InvalidPolicy.RAISE);
            check("Raise", false, "no throw");
        } catch (IllegalArgumentException e) {
            check("Raise", true, e.getMessage());
        }

        // Extend at RUNTIME with a brand-new 'J' (jump 2) command - executor unchanged.
        reg.register('J', s -> new State(s.x() + 2 * s.dir().dx, s.y() + 2 * s.dir().dy, s.dir()));
        ExecutionResult r3 = execute("J", new State(0, 0, Direction.NORTH), reg, InvalidPolicy.IGNORE);
        check("Extend(J)", r3.finalState().equals(new State(0, 2, Direction.NORTH)), r3.finalState().toString());

        UndoableRobot robot = new UndoableRobot(new State(0, 0, Direction.NORTH), defaultRegistry());
        robot.execute("FF");  // (0,2,N)
        robot.execute("U");   // (0,1,N)
        robot.execute("R");   // (0,1,E)
        robot.execute("U");   // (0,1,N)
        check("Undo", robot.state().equals(new State(0, 1, Direction.NORTH)), robot.state().toString());
    }

    static void check(String name, boolean pass, String actual) {
        System.out.printf("%-10s %s  (%s)%n", name, pass ? "PASS" : "FAIL", actual);
    }
}
