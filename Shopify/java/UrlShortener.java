import java.util.*;

/**
 * Shopify Pair Programming - URL Shortener
 * Confirmed: Apr 2025 (JoinTaro), Jan 2026 (PracHub), Glassdoor (recurring).
 *
 * Java 17.  Run:  javac UrlShortener.java && java UrlShortener
 *
 * Layer 1: shorten / resolve, in-memory
 * Layer 2: URL validation, dedup, collision handling
 * Layer 3 (discussion): persistence, sharding, CDN edge redirect
 *
 * The store is behind an interface (Dependency Inversion) so the in-memory map
 * can be swapped for a file or DB without touching shortener logic. That is the
 * ONLY abstraction here - resist adding more until a second backend exists (YAGNI).
 */
public class UrlShortener {

    /** SOLID seam: the shortener depends on this, not on a concrete map. */
    interface Store {
        void save(String code, String url);
        Optional<String> findUrl(String code);
        Optional<String> findCode(String url);
    }

    static class InMemoryStore implements Store {
        private final Map<String, String> codeToUrl = new HashMap<>();
        private final Map<String, String> urlToCode = new HashMap<>();
        public void save(String code, String url) { codeToUrl.put(code, url); urlToCode.put(url, code); }
        public Optional<String> findUrl(String code) { return Optional.ofNullable(codeToUrl.get(code)); }
        public Optional<String> findCode(String url)  { return Optional.ofNullable(urlToCode.get(url)); }
    }

    private static final String ALPHABET =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    private static final int CODE_LENGTH = 7;

    private final String base;
    private final Store store;

    UrlShortener(String base, Store store) { this.base = base; this.store = store; }

    String shorten(String longUrl) {
        validate(longUrl);
        // Dedup: the same URL always maps to the same code.
        Optional<String> existing = store.findCode(longUrl);
        if (existing.isPresent()) return base + existing.get();

        String code = generateCode(longUrl, 0);
        store.save(code, longUrl);
        return base + code;
    }

    String resolve(String shortUrl) {
        String code = shortUrl.startsWith(base) ? shortUrl.substring(base.length()) : shortUrl;
        return store.findUrl(code)
            .orElseThrow(() -> new NoSuchElementException("Unknown short URL: " + shortUrl));
    }

    private void validate(String url) {
        if (url == null || !(url.startsWith("http://") || url.startsWith("https://")))
            throw new IllegalArgumentException("URL must start with http:// or https://: " + url);
    }

    private String generateCode(String url, int attempt) {
        // toUnsignedLong avoids the Math.abs(Integer.MIN_VALUE) trap (stays negative!).
        long h = Integer.toUnsignedLong((url + "#" + attempt).hashCode());
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < CODE_LENGTH; i++) { sb.append(ALPHABET.charAt((int)(h % 62))); h /= 62; }
        String code = sb.toString();
        // Collision: a different URL hashed to the same code -> re-salt and retry.
        Optional<String> owner = store.findUrl(code);
        if (owner.isPresent() && !owner.get().equals(url)) return generateCode(url, attempt + 1);
        return code;
    }

    /* ---- Layer 3 talking points ----
     * Persistence: write-through to the Store; on boot, hydrate the in-memory index.
     *   (Don't hand-roll a JSON parser in production - use Jackson; here, a DB row.)
     * Scale beyond one box: shard by hash(longUrl) with consistent hashing.
     *   Reads >> writes -> cache code->url in Redis/CDN, redirect at the edge.
     * Concurrency: two nodes shortening the same URL -> accept two codes (benign)
     *   OR use SETNX for a single canonical code when it matters (paid links).
     * Analytics: emit click events to Kafka; never block the redirect on a write.
     */

    public static void main(String[] args) {
        UrlShortener us = new UrlShortener("https://sho.rt/", new InMemoryStore());

        String s1 = us.shorten("https://example.com/very/long/path?q=1");
        check("resolve",       us.resolve(s1).equals("https://example.com/very/long/path?q=1"));

        String s2 = us.shorten("https://example.com/very/long/path?q=1");
        check("dedup",         s1.equals(s2));

        String s3 = us.shorten("https://example.com/other");
        check("distinct",      !s1.equals(s3));

        try { us.shorten("not-a-url");                 check("reject invalid", false); }
        catch (IllegalArgumentException e)           { check("reject invalid", true); }

        try { us.resolve("https://sho.rt/zzzzzzz");    check("reject unknown", false); }
        catch (NoSuchElementException e)             { check("reject unknown", true); }
    }

    static void check(String name, boolean pass) {
        System.out.printf("%-16s %s%n", name, pass ? "PASS" : "FAIL");
    }
}
