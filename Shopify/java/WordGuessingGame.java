import java.util.*;

/**
 * Shopify Pair Programming - Word Guessing Game (Wordle-like)
 * Confirmed: Aug 2025, Jan 2026 (PracHub).
 *
 * Java 11.  Run:  javac WordGuessingGame.java && java WordGuessingGame
 *
 * Hint per letter:  '1' = right letter & spot, '0' = right letter wrong spot,
 *                   '-' = letter absent.
 * The hard part is DUPLICATE letters: match exact positions first, then mark
 * misplaced only while an unmatched occurrence remains in the target. A naive
 * "contains()" check over-reports '0' for repeated letters - the classic bug.
 *
 * `hint` is a pure function: trivially unit-testable, the heart of the problem.
 */
public class WordGuessingGame {

    static String hint(String guess, String target) {
        if (guess.length() != target.length())
            throw new IllegalArgumentException("length mismatch");
        int n = target.length();
        char[] out = new char[n];
        Arrays.fill(out, '-');
        Map<Character, Integer> remaining = new HashMap<>();

        // Pass 1: exact matches; everything else becomes an "available" target letter.
        for (int i = 0; i < n; i++) {
            if (guess.charAt(i) == target.charAt(i)) out[i] = '1';
            else remaining.merge(target.charAt(i), 1, Integer::sum);
        }
        // Pass 2: misplaced, consuming available occurrences so duplicates can't over-count.
        for (int i = 0; i < n; i++) {
            if (out[i] == '1') continue;
            char c = guess.charAt(i);
            if (remaining.getOrDefault(c, 0) > 0) {
                out[i] = '0';
                remaining.merge(c, -1, Integer::sum);
            }
        }
        return new String(out);
    }

    static final class Result {
        private final boolean won;
        private final int attemptsUsed;
        private final List<String> hints;
        Result(boolean won, int attemptsUsed, List<String> hints) {
            this.won = won; this.attemptsUsed = attemptsUsed; this.hints = hints;
        }
        boolean won() { return won; }
        int attemptsUsed() { return attemptsUsed; }
        List<String> hints() { return hints; }
    }

    static class Game {
        private final Set<String> dictionary;
        private final int wordLen;
        private final int maxAttempts;

        Game(Set<String> dictionary, int wordLen, int maxAttempts) {
            this.dictionary = dictionary;
            this.wordLen = wordLen;
            this.maxAttempts = maxAttempts;
        }

        Result play(String target, List<String> guesses) {
            String t = target.toUpperCase();
            if (!dictionary.contains(t)) throw new IllegalArgumentException("target not in dictionary");

            List<String> hints = new ArrayList<>();
            int used = 0;
            boolean won = false;
            for (String raw : guesses) {
                if (won || used >= maxAttempts) break;
                String g = raw.toUpperCase();
                if (g.length() != wordLen || !dictionary.contains(g)) {
                    hints.add("INVALID");          // reported, but does NOT consume an attempt
                    continue;
                }
                used++;
                String h = hint(g, t);
                hints.add(h);
                if (h.chars().allMatch(ch -> ch == '1')) won = true;
            }
            return new Result(won, used, hints);
        }
    }

    /** Layer 2: stats with streak tracking (persist to a file in the full CLI variant). */
    static class Stats {
        int played, wins, streak, best;
        void record(boolean won) {
            played++;
            if (won) { wins++; streak++; best = Math.max(best, streak); }
            else streak = 0;
        }
        @Override public String toString() {
            return String.format("played=%d wins=%d streak=%d best=%d", played, wins, streak, best);
        }
    }

    public static void main(String[] args) {
        // Pure hint logic - including the tricky duplicate cases:
        check("ABBA/BALE", hint("ABBA", "BALE").equals("00--"), hint("ABBA", "BALE"));   // both misplaced
        check("exact",     hint("BALE", "BALE").equals("1111"), hint("BALE", "BALE"));
        check("none",      hint("XXXX", "BALE").equals("----"), hint("XXXX", "BALE"));
        // AABB vs ABCD: first A exact; 2nd A absent (only one A in target);
        //               first B misplaced; 2nd B absent (only one B in target).
        check("AABB/ABCD", hint("AABB", "ABCD").equals("1-0-"), hint("AABB", "ABCD"));

        Set<String> dict = new HashSet<>(Arrays.asList("BALE", "ABBA", "WORD", "TEST"));
        Game game = new Game(dict, 4, 5);

        Result win = game.play("BALE", Arrays.asList("BALE"));
        check("win", win.won() && win.attemptsUsed() == 1, win.hints().toString());

        Result inv = game.play("BALE", Arrays.asList("ZZZZ", "BALE"));   // ZZZZ not in dict
        check("invalid skip", inv.hints().get(0).equals("INVALID") && inv.attemptsUsed() == 1,
              inv.hints().toString());

        Stats stats = new Stats();
        stats.record(true); stats.record(true); stats.record(false); stats.record(true);
        check("stats", stats.best == 2 && stats.streak == 1, stats.toString());
    }

    static void check(String name, boolean pass, String actual) {
        System.out.printf("%-14s %s  (%s)%n", name, pass ? "PASS" : "FAIL", actual);
    }
}
