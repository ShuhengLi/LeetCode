/*
 * Shopify Pair Programming - Discount / Pricing Calculator
 * (E-commerce flavored, very Shopify-specific)
 *
 * Problem (Layer 1):
 *   Given a list of (date, item) purchase records, compute total cost.
 *   Pricing:
 *     - Popcorn: $2.50 individually
 *     - Soda:    $8.00 individually
 *     - Bundle (popcorn + soda bought on same day): $9.00
 *
 * Example:
 *   Input: [("2024-01-01", "popcorn"), ("2024-01-01", "soda"), ("2024-01-02", "popcorn")]
 *   Output: 9.00 + 2.50 = $11.50
 *
 * Extensions reported in interviews:
 *   Layer 2: Multiple units per item per day. Stack additional bundles.
 *   Layer 3: Promotion codes (e.g. "SAVE10" = 10% off, max 1 use per customer).
 *   Layer 4 (staff): Stackable rule engine. How would you make this configurable
 *                    so the marketing team can define rules without deploying code?
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// ============================================================
// Layer 1 + 2: Bundle pricing with multiple quantities
// ============================================================

struct Purchase {
    std::string date;
    std::string item;
    int quantity = 1;
};

struct PricingRule {
    std::vector<std::string> items;  // items in bundle (sorted)
    double bundlePrice;
    double individualPrice;          // fallback if not in bundle
};

class OrderCalculator {
public:
    OrderCalculator() {
        // Default Shopify interview pricing
        rules["popcorn"] = {2.50};
        rules["soda"] = {8.00};
        bundlePrice = 9.00;
    }

    double calculate(const std::vector<Purchase>& purchases) {
        // Group by date
        std::unordered_map<std::string, std::unordered_map<std::string, int>> byDate;
        for (const auto& p : purchases) {
            byDate[p.date][p.item] += p.quantity;
        }

        double total = 0.0;
        for (const auto& [date, items] : byDate) {
            total += calculateDay(items);
        }
        return total;
    }

private:
    struct ItemPrice { double price; };
    std::unordered_map<std::string, ItemPrice> rules;
    double bundlePrice;

    double calculateDay(const std::unordered_map<std::string, int>& items) {
        int popcornCount = items.count("popcorn") ? items.at("popcorn") : 0;
        int sodaCount    = items.count("soda")    ? items.at("soda")    : 0;

        int bundles = std::min(popcornCount, sodaCount);
        double cost = bundles * bundlePrice;
        cost += (popcornCount - bundles) * rules["popcorn"].price;
        cost += (sodaCount    - bundles) * rules["soda"].price;

        // Any other items at individual price
        for (const auto& [item, qty] : items) {
            if (item != "popcorn" && item != "soda") {
                if (rules.count(item)) cost += qty * rules[item].price;
            }
        }
        return cost;
    }
};

// ============================================================
// Layer 3: Promotion codes with usage limits
// ============================================================

struct PromoCode {
    std::string code;
    double discountPct;
    int maxUsesPerCustomer;
};

class OrderCalculatorWithPromo : public OrderCalculator {
public:
    void addPromoCode(const PromoCode& promo) {
        promoCodes[promo.code] = promo;
    }

    // customerId for tracking per-customer usage
    double calculateWithPromo(const std::vector<Purchase>& purchases,
                              const std::string& promoCode,
                              const std::string& customerId) {
        double total = calculate(purchases);
        if (promoCode.empty()) return total;

        auto it = promoCodes.find(promoCode);
        if (it == promoCodes.end()) {
            throw std::invalid_argument("Invalid promo code: " + promoCode);
        }

        std::string usageKey = customerId + ":" + promoCode;
        int uses = promoUsage[usageKey];
        if (uses >= it->second.maxUsesPerCustomer) {
            throw std::runtime_error("Promo code already used maximum times");
        }

        promoUsage[usageKey]++;
        return total * (1.0 - it->second.discountPct / 100.0);
    }

private:
    std::unordered_map<std::string, PromoCode> promoCodes;
    std::unordered_map<std::string, int> promoUsage; // customerId:code -> count
};

// ============================================================
// Staff Layer 4 Discussion Points
// ============================================================
/*
 * Q: How do you make pricing rules configurable without code deploys?
 *    - Store rules in a database / config service (JSON schema).
 *    - Rule types: flat discount, percentage, bundle match, quantity threshold.
 *    - Rule evaluation engine reads rules at request time (with caching, ~1min TTL).
 *    - Marketing team edits via admin UI -> writes to DB -> cache invalidates.
 *
 * Q: How do you handle stackable promotions?
 *    - Define rule priority and stackability flag.
 *    - Evaluate rules in priority order; if not stackable, stop after first match.
 *    - Document clearly: "best deal wins" vs. "all apply" — Shopify uses both modes.
 *
 * Q: What are the failure modes in production?
 *    - Rule config DB is down: fail open (apply no promos) or fail closed?
 *      For pricing, fail closed (show list price) is safer to avoid revenue loss.
 *    - Race condition on promo code usage count:
 *      Use atomic increment with Redis INCR, reject if result > maxUses.
 *    - Flash sale (Black Friday): promo code traffic spike.
 *      Cache aggressively; use rate limiting per customer on promo validation.
 *
 * Q: How would you test the discount engine?
 *    - Unit test each rule in isolation.
 *    - Property-based testing: total with discount always <= total without.
 *    - Snapshot tests for complex stacking scenarios.
 *    - Canary deploy new rule engine against shadow traffic.
 */

// ============================================================
// Tests
// ============================================================

void runTests() {
    OrderCalculator calc;

    // Basic bundle
    {
        std::vector<Purchase> purchases = {
            {"2024-01-01", "popcorn"},
            {"2024-01-01", "soda"},
        };
        double total = calc.calculate(purchases);
        std::cout << "Bundle test: $" << total << " (expected $9.00)" << std::endl;
    }

    // Bundle + leftover
    {
        std::vector<Purchase> purchases = {
            {"2024-01-01", "popcorn"},
            {"2024-01-01", "popcorn"},
            {"2024-01-01", "soda"},
        };
        double total = calc.calculate(purchases);
        std::cout << "Bundle + leftover: $" << total << " (expected $11.50)" << std::endl;
    }

    // Multi-day: one bundle day, one standalone day
    {
        std::vector<Purchase> purchases = {
            {"2024-01-01", "popcorn"},
            {"2024-01-01", "soda"},
            {"2024-01-02", "popcorn"},
        };
        double total = calc.calculate(purchases);
        std::cout << "Multi-day: $" << total << " (expected $11.50)" << std::endl;
    }

    // Promo code
    {
        OrderCalculatorWithPromo calc2;
        calc2.addPromoCode({"SAVE10", 10.0, 1}); // 10% off, max 1 use
        std::vector<Purchase> purchases = {
            {"2024-01-01", "popcorn"},
            {"2024-01-01", "soda"},
        };
        double total = calc2.calculateWithPromo(purchases, "SAVE10", "customer_123");
        std::cout << "With promo: $" << total << " (expected $8.10)" << std::endl;

        try {
            calc2.calculateWithPromo(purchases, "SAVE10", "customer_123"); // second use
            std::cout << "ERROR: should have thrown" << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Second use blocked: " << e.what() << std::endl;
        }
    }
}

int main() {
    runTests();
    return 0;
}
