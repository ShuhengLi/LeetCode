import java.util.*;

/**
 * Shopify Pair Programming - Discount / Pricing Calculator (e-commerce)
 *
 * Java 11.  Run:  javac DiscountCalculator.java && java DiscountCalculator
 *
 * Layer 1+2: bundle pricing (popcorn $2.50, soda $8.00, both same day $9.00) + quantities
 * Layer 3:   promo codes with a per-customer usage limit
 * Layer 4 (discussion): a configurable rule engine the marketing team edits without deploys
 *
 * NOTE on money: this uses double for interview speed, but say out loud that
 * production money should be integer cents or BigDecimal - floating point rounding
 * is a real correctness bug for currency. (That awareness is the signal.)
 *
 * Promo support is added by COMPOSITION (PricingService wraps OrderCalculator),
 * not inheritance - cleaner separation of concerns.
 */
public class DiscountCalculator {

    static final class Purchase {
        private final String date, item;
        private final int quantity;
        Purchase(String date, String item, int quantity) {
            this.date = date; this.item = item; this.quantity = quantity;
        }
        Purchase(String date, String item) { this(date, item, 1); }
        String date() { return date; }
        String item() { return item; }
        int quantity() { return quantity; }
    }

    static class OrderCalculator {
        private static final double POPCORN = 2.50, SODA = 8.00, BUNDLE = 9.00;

        double total(List<Purchase> purchases) {
            // Group quantities by date, then by item.
            Map<String, Map<String, Integer>> byDate = new HashMap<>();
            for (Purchase p : purchases)
                byDate.computeIfAbsent(p.date(), d -> new HashMap<>())
                      .merge(p.item(), p.quantity(), Integer::sum);

            double sum = 0;
            for (Map<String, Integer> day : byDate.values()) sum += dayTotal(day);
            return sum;
        }

        private double dayTotal(Map<String, Integer> items) {
            int popcorn = items.getOrDefault("popcorn", 0);
            int soda    = items.getOrDefault("soda", 0);
            int bundles = Math.min(popcorn, soda);          // greedily pair into bundles
            return bundles * BUNDLE
                 + (popcorn - bundles) * POPCORN
                 + (soda    - bundles) * SODA;
        }
    }

    static final class PromoCode {
        private final String code;
        private final double discountPct;
        private final int maxUsesPerCustomer;
        PromoCode(String code, double discountPct, int maxUsesPerCustomer) {
            this.code = code; this.discountPct = discountPct; this.maxUsesPerCustomer = maxUsesPerCustomer;
        }
        String code() { return code; }
        double discountPct() { return discountPct; }
        int maxUsesPerCustomer() { return maxUsesPerCustomer; }
    }

    static class PricingService {
        private final OrderCalculator calculator = new OrderCalculator();
        private final Map<String, PromoCode> promos = new HashMap<>();
        private final Map<String, Integer> usage = new HashMap<>(); // "customer:code" -> count

        void addPromo(PromoCode p) { promos.put(p.code(), p); }

        double checkout(List<Purchase> purchases, String code, String customerId) {
            double base = calculator.total(purchases);
            if (code == null || code.trim().isEmpty()) return base;

            PromoCode promo = promos.get(code);
            if (promo == null) throw new IllegalArgumentException("Invalid promo: " + code);

            String key = customerId + ":" + code;
            if (usage.getOrDefault(key, 0) >= promo.maxUsesPerCustomer())
                throw new IllegalStateException("Promo usage limit reached");

            usage.merge(key, 1, Integer::sum);
            return base * (1.0 - promo.discountPct() / 100.0);
        }
    }

    /* ---- Layer 4 talking points ----
     * Configurable rules: store rule rows (type, params, priority, stackable) in a DB;
     *   evaluate in priority order at request time (cache ~1min). Marketing edits via
     *   admin UI -> cache invalidates. Rule types: flat, percent, bundle, threshold.
     * Stacking: a `stackable` flag + priority; "best deal wins" vs "all apply".
     * Failure: if the rule store is down, fail CLOSED (charge list price) to protect revenue.
     * Concurrency on promo counts: atomic Redis INCR, reject if result > max.
     */

    public static void main(String[] args) {
        OrderCalculator calc = new OrderCalculator();

        check("bundle",       eq(calc.total(Arrays.asList(
            new Purchase("d1", "popcorn"), new Purchase("d1", "soda"))), 9.00));

        check("bundle+extra", eq(calc.total(Arrays.asList(
            new Purchase("d1", "popcorn"), new Purchase("d1", "popcorn"),
            new Purchase("d1", "soda"))), 11.50));

        check("multi-day",    eq(calc.total(Arrays.asList(
            new Purchase("d1", "popcorn"), new Purchase("d1", "soda"),
            new Purchase("d2", "popcorn"))), 11.50));

        PricingService svc = new PricingService();
        svc.addPromo(new PromoCode("SAVE10", 10.0, 1));
        List<Purchase> order = Arrays.asList(new Purchase("d1", "popcorn"), new Purchase("d1", "soda"));
        check("promo",        eq(svc.checkout(order, "SAVE10", "c1"), 8.10));

        try { svc.checkout(order, "SAVE10", "c1"); check("promo reuse", false); }
        catch (IllegalStateException e)          { check("promo reuse", true); }
    }

    static boolean eq(double a, double b) { return Math.abs(a - b) < 1e-9; }
    static void check(String name, boolean pass) {
        System.out.printf("%-14s %s%n", name, pass ? "PASS" : "FAIL");
    }
}
