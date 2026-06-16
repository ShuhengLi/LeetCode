/*
 * Shopify Pair Programming - URL Shortener
 * Confirmed: Apr 2025 (JoinTaro, Senior SWE Canada), Jan 2026 (PracHub)
 * Also mentioned on Glassdoor (recurring)
 *
 * Problem (Layer 1):
 *   Implement a local URL shortener on a single machine.
 *   shorten(long_url)  -> short_code
 *   resolve(short_code) -> original_url or error
 *   In-memory storage only.
 *
 * Layer 2 — Production Hardening:
 *   - Validate well-formed URLs, reject invalid
 *   - Deduplication: same long URL always returns same short code
 *   - Handle collision in hash-based encoding
 *
 * Layer 3 — Persistence:
 *   - Persist mapping to a JSON file
 *   - Reload on program start (no external DB/Redis)
 *
 * Layer 4 — Discussion:
 *   - Memory growth, what changes when data exceeds one machine?
 *   - How do you handle high read vs. write ratio?
 *
 * Apr 2025 candidate note:
 *   "A full front-to-back solution was expected. Interviewer seemed distracted
 *    but still expected complete working code."
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <functional>

// ============================================================
// Layer 1 + 2: In-memory URL shortener with validation and dedup
// ============================================================

class UrlShortener {
public:
    explicit UrlShortener(std::string baseUrl = "https://sho.rt/")
        : baseUrl(std::move(baseUrl)) {}

    std::string shorten(const std::string& longUrl) {
        validateUrl(longUrl);

        // Deduplication: return existing code if URL already shortened
        auto it = longToShort.find(longUrl);
        if (it != longToShort.end()) return baseUrl + it->second;

        std::string code = generateCode(longUrl);
        longToShort[longUrl] = code;
        shortToLong[code] = longUrl;
        return baseUrl + code;
    }

    std::string resolve(const std::string& shortUrl) {
        std::string code = extractCode(shortUrl);
        auto it = shortToLong.find(code);
        if (it == shortToLong.end()) {
            throw std::invalid_argument("Unknown short URL: " + shortUrl);
        }
        return it->second;
    }

    // Layer 3: persist to / load from JSON file
    void saveTo(const std::string& path) const {
        std::ofstream f(path);
        f << "{\n";
        bool first = true;
        for (const auto& [code, url] : shortToLong) {
            if (!first) f << ",\n";
            f << "  \"" << escape(code) << "\": \"" << escape(url) << "\"";
            first = false;
        }
        f << "\n}";
    }

    void loadFrom(const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) return; // first run, no file yet

        // Minimal JSON parser for our own format (key:value pairs)
        std::string line;
        while (std::getline(f, line)) {
            auto colon = line.find("\": \"");
            if (colon == std::string::npos) continue;
            std::string code = line.substr(3, colon - 3);
            std::string url  = line.substr(colon + 4);
            if (!url.empty() && url.back() == '"') url.pop_back();
            if (!url.empty() && url.back() == ',') url.pop_back();
            if (!url.empty() && url.back() == '"') url.pop_back();
            shortToLong[code] = url;
            longToShort[url] = code;
        }
    }

private:
    std::string baseUrl;
    std::unordered_map<std::string, std::string> shortToLong;
    std::unordered_map<std::string, std::string> longToShort;

    static void validateUrl(const std::string& url) {
        if (url.size() < 7) throw std::invalid_argument("URL too short: " + url);
        if (url.substr(0, 7) != "http://" && url.substr(0, 8) != "https://") {
            throw std::invalid_argument("URL must start with http:// or https://: " + url);
        }
    }

    std::string generateCode(const std::string& url, int attempt = 0) {
        // Base62 encode a hash of the URL
        static const std::string chars =
            "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        size_t h = std::hash<std::string>{}(url + std::to_string(attempt));
        std::string code;
        for (int i = 0; i < 7; i++) {
            code += chars[h % 62];
            h /= 62;
        }
        // Collision: try again with a different salt
        if (shortToLong.count(code) && shortToLong[code] != url) {
            return generateCode(url, attempt + 1);
        }
        return code;
    }

    std::string extractCode(const std::string& shortUrl) const {
        if (shortUrl.substr(0, baseUrl.size()) == baseUrl) {
            return shortUrl.substr(baseUrl.size());
        }
        return shortUrl; // treat as raw code
    }

    static std::string escape(const std::string& s) {
        std::string out;
        for (char c : s) {
            if (c == '"' || c == '\\') out += '\\';
            out += c;
        }
        return out;
    }
};

// ============================================================
// Layer 4 Discussion Points
// ============================================================
/*
 * Q: Memory growth beyond single machine?
 *    - Shard by hash of long URL -> route to specific node
 *    - Use consistent hashing so adding nodes doesn't rehash everything
 *    - Separate read replicas (reads >> writes for URL shorteners)
 *
 * Q: High read ratio?
 *    - Cache short->long in Redis with CDN in front
 *    - Redirect at edge (Cloudflare Worker) before hitting origin
 *    - Write path: sync to DB; read path: cache-aside with TTL
 *
 * Q: What if the same URL is shortened concurrently on two machines?
 *    - Option A: Accept two codes for same URL (benign, wastes space)
 *    - Option B: Write-through to central store; use SETNX (atomic Redis)
 *    - Shopify context: slight inconsistency acceptable for link shortener;
 *      not acceptable for payment URLs (use SETNX there)
 *
 * Q: Analytics (clicks per short URL)?
 *    - Log redirect events to Kafka, consume async into ClickHouse
 *    - Never block the redirect on analytics write
 */

// ============================================================
// Tests
// ============================================================

void runTests() {
    UrlShortener us;

    // Basic shorten + resolve
    auto short1 = us.shorten("https://example.com/very/long/path?q=1");
    std::cout << "Shortened: " << short1 << std::endl;
    std::cout << "Resolved:  " << us.resolve(short1) << std::endl;

    // Deduplication
    auto short2 = us.shorten("https://example.com/very/long/path?q=1");
    std::cout << "Dedup same: " << (short1 == short2 ? "PASS" : "FAIL") << std::endl;

    // Different URL -> different code
    auto short3 = us.shorten("https://example.com/other");
    std::cout << "Different: " << (short1 != short3 ? "PASS" : "FAIL") << std::endl;

    // Invalid URL
    try {
        us.shorten("not-a-url");
        std::cout << "FAIL: should have thrown" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid URL rejected: PASS" << std::endl;
    }

    // Unknown short code
    try {
        us.resolve("https://sho.rt/xxxxxxx");
        std::cout << "FAIL: should have thrown" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "Unknown code rejected: PASS" << std::endl;
    }

    // Persistence
    us.saveTo("/tmp/urls.json");
    UrlShortener us2;
    us2.loadFrom("/tmp/urls.json");
    std::cout << "Persisted resolve: " << us2.resolve(short1) << std::endl;
}

int main() {
    runTests();
    return 0;
}
