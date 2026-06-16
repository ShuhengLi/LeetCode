# Shopify Staff Engineer Interview Prep

## Interview Process (Staff / Senior Staff Level)

| Stage | Format | Duration |
|---|---|---|
| Recruiter Screen | Phone | 30 min |
| Async CoderPad Screen | Coding | 45-60 min |
| Pair Programming Round 1 | Live coding w/ engineer | 75-90 min |
| **Pair Programming Round 2** (staff-specific) | Live coding w/ staff engineer | 75 min |
| Life Story Interview | Behavioral/narrative | 60 min |
| Technical Deep Dive | Architecture/past projects | 60 min |
| **System Design Round** (staff-specific) | Architecture design | 60 min |
| Hiring Manager Round | Values/scope/cross-team | 30-45 min |

---

## Solutions & Study Materials

- **C++ solutions:** [`pair_programming/`](pair_programming/) — each file has Layers 1→4, discussion points, and tests.
- **Java 11 solutions:** [`java/`](java/) — same problems, compile + pass on Java 11 (`javac X.java && java X`).
- **Layer-by-layer interview logic:** [`java/INTERVIEW_GUIDE.md`](java/INTERVIEW_GUIDE.md) — how to *drive* each problem, what to say at each layer, SOLID-without-over-design, and the "would you ship it?" answers.

### Review notes (defects found & fixed in the C++ originals)
| File | Defect | Fix |
|---|---|---|
| `lru_cache.cpp` | `std::this_thread::sleep_for` used but `<thread>` not included → won't compile | added `#include <thread>` |
| `rate_limiter.cpp` | Fixed-window mixed units: `nanosecond_ticks % windowMs` → meaningless boundaries | bucket by `nowMs / windowMs` (integer window id) |
| `secret_santa.cpp` | `std::iota` used but `<numeric>` not included | added `#include <numeric>` |
| `word_guessing_game.cpp` | Test comment wrong: `getHint("CCCC","CRANE")` is a length mismatch | changed to same-length `"CCCCC"` → `"1----"` |

Open design notes (not bugs, but raised in the Java versions): money as `double` should be cents/`BigDecimal`; most structures aren't thread-safe (fine for interview, mention it); the URL-shortener's hand-rolled JSON parser is fragile — use a real library in prod. The Java ports inject the clock/RNG so tests are deterministic (no `sleep`).

---

## Confirmed Round 1 Pair Programming Problems (2024–2026)

All problems below have been verified with sources and dates.

| Problem | File | Last Seen | Frequency |
|---|---|---|---|
| Robot / Mars Rover (basic) | [robot_grid.cpp](pair_programming/robot_grid.cpp) | Apr 2025 | ★★★★★ |
| Robot with Extensible Commands | [robot_extensible_commands.cpp](pair_programming/robot_extensible_commands.cpp) | Dec 2025 | ★★★★☆ |
| LRU Cache | [lru_cache.cpp](pair_programming/lru_cache.cpp) | Jan 2026 | ★★★★★ |
| LRU Cache + Secret Santa | [secret_santa.cpp](pair_programming/secret_santa.cpp) | Oct 2025 | ★★★☆☆ |
| URL Shortener | [url_shortener.cpp](pair_programming/url_shortener.cpp) | Jan 2026 | ★★★★☆ |
| Word Guessing Game (Wordle) | [word_guessing_game.cpp](pair_programming/word_guessing_game.cpp) | Jan 2026 | ★★★☆☆ |
| Rate Limiter | [rate_limiter.cpp](pair_programming/rate_limiter.cpp) | multiple | ★★★☆☆ |
| Discount / Pricing Calculator | [discount_calculator.cpp](pair_programming/discount_calculator.cpp) | multiple | ★★★☆☆ |

---

## Problem Sourcing Details

### Robot / Mars Rover
- Aug 2024 — JoinTaro, SWE Toronto (rejected)
- Apr 2025 — JoinTaro, Senior SWE Canada (rejected)
- Dec 2025 — PracHub, MLE Technical Screen

**Layer 1:** Single robot, `L`/`R`/`M` commands, report final `(x, y, direction)`  
**Layer 2:** Multiple robots — `CREATE <id>`, `DELETE <id>`, `SELECT <id>`; collision avoidance  
**Layer 3:** Serialize/deserialize robot state  
**Layer 4 (staff):** HTTP API design, 1000 concurrent robots, failure modes  

### Robot with Extensible Commands (distinct variant)
- Dec 2025 — PracHub, MLE Technical Screen

**Key twist:** Problem spec says "commands will keep expanding." You must use a **command registry / command-object pattern** — not if/else chains. If you don't design it this way proactively, the interviewer will add `UNDO` and expose the flaw.

### LRU Cache
- Jul 2024 — JoinTaro, Staff SWE Toronto (rejected next day)
- Aug 2024 — JoinTaro, SWE Toronto (rejected)
- Jan 2026 — PracHub

**Layer 1:** `get(key)` / `put(key, value)`, O(1), capacity eviction  
**Layer 2:** Configurable capacity as constructor param  
**Layer 3:** Abstract storage backend (in-memory / Redis swappable)  
**Layer 4:** Named tiers ("free"/"pro"/"enterprise" → capacity integers)  
**Follow-up question (Aug 2024):** *"Would you push this code to production? Why or why not?"*

### LRU Cache + Secret Santa (two-part screen)
- Oct 2025 — PracHub, Technical Screen

Part 1 is the LRU Cache. Part 2 is Secret Santa:  
CSV in `(name, email)` → CSV out `(giver_name, giver_email, receiver_name, receiver_email)`  
Rules: no self-assignment, each person gives and receives exactly once, truly randomized (not position-shift).  
**Key insight:** This is a random derangement. Fisher-Yates shuffle loop is correct; simple `+1 mod n` is wrong.

### URL Shortener
- Apr 2025 — JoinTaro, Senior SWE Canada
- Jan 2026 — PracHub
- Glassdoor (recurring, multiple years)

**Layer 1:** `shorten(url)` / `resolve(code)`, in-memory  
**Layer 2:** URL validation, deduplication, collision handling  
**Layer 3:** Persist to JSON file, reload on startup  
**Layer 4:** Scale beyond single machine — sharding, read replicas, CDN redirect  

### Word Guessing Game (Wordle-Like)
- Aug 2025 — PracHub, SWE/MLE
- Jan 2026 — PracHub

**Layer 1:** 4-letter words, 5 attempts, hint format `1`/`0`/`-`, Wordle duplicate handling  
**Hardest part:** Duplicate letter hint logic. Mark exact matches first, then misplaced only for unmatched target chars.  
**Layer 2 (MLE):** Full CLI app, dictionary file loading, statistics persistence (streak tracking)

---

## Pair Programming Format

**Platform:** CoderPad or your own IDE + screen share  
**AI tools:** Explicitly **allowed and expected** since Q4 2024. Narrate your AI usage.  
**Duration:** 75–90 min  

### Universal Problem Arc (every problem follows this)
1. **Layer 1** — Write a simple working implementation (aim: running code in ~20 min)
2. **Layer 2** — Extend it: new feature, edge cases
3. **Layer 3** — Production concern: caching, persistence, error handling
4. **Layer 4** (staff) — Scale / architecture discussion; interviewer may ask "would you ship this?"

---

## What Gets Evaluated

### Technical
- Working code at each layer (not just pseudocode)
- **Nearly production-quality** — an Aug 2024 candidate was rejected partly for "messy nested classes"
- Clean naming, clear structure, awareness of edge cases

### Communication (Critical)
- **Thinking out loud** while coding
- **Proactive clarification** before touching keyboard
- **Incremental delivery** — ship Layer 1 then extend, don't design everything upfront
- **Absorbing redirects** — interviewer adds requirements mid-problem, stay composed

### Staff-Specific
- Proactively raise production concerns without being prompted
- Natural transition to system design at Layer 4
- Cross-team influence examples in Life Story

---

## Preparation Priority Order

1. **LRU Cache** — highest frequency, tests fundamental data structure skills
2. **Robot Grid (basic + extensible)** — highest frequency, tests OOP design
3. **URL Shortener** — frequent, tests system thinking at Layer 3+
4. **Word Guessing Game** — rising frequency, tests edge case handling (duplicate letters)
5. **Secret Santa** — medium frequency, tests algorithmic thinking (derangement)

### For each problem, practice this flow:
1. Clarify: "Should this handle unicode? What's the expected scale? Thread safety?"
2. Write Layer 1 working in ~20 minutes
3. Voluntarily propose Layer 2 before being asked
4. Run tests inline as you go

### Language: Use Python or Ruby
Shopify's primary language is Ruby/Rails. Python is acceptable. Avoid Java/C++.
