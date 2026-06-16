import java.util.*;

/**
 * Shopify Pair Programming - Secret Santa (random derangement)
 * Confirmed: Oct 2025 (PracHub) - given right after the LRU Cache.
 *
 * Java 11.  Run:  javac SecretSanta.java && java SecretSanta
 *
 * Input CSV:  name,email
 * Output CSV: giver_name,giver_email,receiver_name,receiver_email
 * Rules: nobody assigned to themselves; perfect bijection (everyone gives one,
 *        receives one); assignment must be TRULY random (a +1 shift is wrong -
 *        the receiver becomes predictable, defeating the "secret").
 */
public class SecretSanta {

    static final class Participant {
        private final String name, email;
        Participant(String name, String email) { this.name = name; this.email = email; }
        String name() { return name; }
        String email() { return email; }
        @Override public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Participant)) return false;
            Participant p = (Participant) o;
            return Objects.equals(name, p.name) && Objects.equals(email, p.email);
        }
        @Override public int hashCode() { return Objects.hash(name, email); }
    }

    static final class Assignment {
        private final Participant giver, receiver;
        Assignment(Participant giver, Participant receiver) { this.giver = giver; this.receiver = receiver; }
        Participant giver() { return giver; }
        Participant receiver() { return receiver; }
    }

    static List<Participant> parseCsv(String csv) {
        List<Participant> out = new ArrayList<>();
        String[] lines = csv.strip().split("\n");
        for (int i = 1; i < lines.length; i++) {           // skip header row
            String line = lines[i].strip();
            if (line.isEmpty()) continue;
            String[] f = line.split(",", 2);
            if (f.length < 2) throw new IllegalArgumentException("Malformed line: " + line);
            out.add(new Participant(f[0].strip(), f[1].strip()));
        }
        return out;
    }

    /**
     * Shuffle until the permutation is a derangement (no fixed point).
     * P(random permutation is a derangement) -> 1/e, so expected attempts ~ 2.7.
     * Effectively O(n). Random is injected so tests are reproducible.
     */
    static List<Assignment> assign(List<Participant> people, Random rng) {
        int n = people.size();
        if (n < 2) throw new IllegalArgumentException("Need >= 2 participants, got " + n);

        List<Integer> idx = new ArrayList<>();
        for (int i = 0; i < n; i++) idx.add(i);
        do { Collections.shuffle(idx, rng); } while (hasFixedPoint(idx));

        List<Assignment> result = new ArrayList<>(n);
        for (int i = 0; i < n; i++)
            result.add(new Assignment(people.get(i), people.get(idx.get(i))));
        return result;
    }

    private static boolean hasFixedPoint(List<Integer> idx) {
        for (int i = 0; i < idx.size(); i++) if (idx.get(i) == i) return true;
        return false;
    }

    static String toCsv(List<Assignment> assignments) {
        StringBuilder sb = new StringBuilder("giver_name,giver_email,receiver_name,receiver_email\n");
        for (Assignment x : assignments)
            sb.append(x.giver().name()).append(',').append(x.giver().email()).append(',')
              .append(x.receiver().name()).append(',').append(x.receiver().email()).append('\n');
        return sb.toString();
    }

    /* ---- Discussion ----
     * Alternative: Sattolo's algorithm guarantees a derangement in one O(n) pass
     *   (swap index i with a random j in [0, i-1]). It only produces single-cycle
     *   permutations - slightly less uniform, but fine for Secret Santa.
     * Concurrency on a shared pool: re-run is cheap (n is small). For huge n, log
     *   the result so a crash mid-email doesn't double-assign.
     */

    public static void main(String[] args) {
        Random rng = new Random(42);  // seeded -> reproducible tests

        String csv = "name,email\nAlice,a@x.com\nBob,b@x.com\nCarol,c@x.com\nDave,d@x.com\n";
        List<Participant> people = parseCsv(csv);
        List<Assignment> result = assign(people, rng);
        System.out.print(toCsv(result));

        boolean noSelf = result.stream().noneMatch(a -> a.giver().equals(a.receiver()));
        check("no self-assign", noSelf);

        Set<Participant> receivers = new HashSet<>();
        result.forEach(a -> receivers.add(a.receiver()));
        check("bijection", receivers.size() == people.size());

        try {
            assign(parseCsv("name,email\nSolo,s@x.com\n"), rng);
            check("too few", false);
        } catch (IllegalArgumentException e) {
            check("too few", true);
        }

        List<Assignment> two = assign(parseCsv("name,email\nA,a\nB,b\n"), rng);
        check("n=2 swaps", two.stream().noneMatch(a -> a.giver().equals(a.receiver())));
    }

    static void check(String name, boolean pass) {
        System.out.printf("%-14s %s%n", name, pass ? "PASS" : "FAIL");
    }
}
