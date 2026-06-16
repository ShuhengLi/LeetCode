# Shopify Pair Programming — Layer-by-Layer Interview Logic

How to actually *drive* these problems. The code in this folder is the destination;
this guide is the route. Read this, then practice talking through each file out loud.

---

## The Universal Framework (every Shopify problem follows it)

```
1. CLARIFY (2-4 min)   → ask before typing. Scope, inputs, scale, edge cases.
2. LAYER 1 (15-20 min) → smallest thing that runs. Test it. Get a green light.
3. EXTEND (each layer) → interviewer adds a requirement. Absorb it, don't resist.
4. DISCUSS (staff)     → "would you ship this?" → trade-offs, scale, failure modes.
```

**The single most important behavior: get running, tested code on the screen early.**
Shopify's culture is "ship to learn." A working Layer 1 with one passing test beats a
half-built Layer 3. Run your tests after *every* layer — it shows the interviewer you
verify your own work instead of hoping.

**Narrate constantly.** The interviewer is your pair, not a judge. Say what you're
about to do and why: *"I'll start with a HashMap for O(1) lookup; I'll revisit eviction
once get/put work."* Silence reads as being stuck.

---

## SOLID — applied, not worshipped

Shopify rejects both extremes: spaghetti **and** astronaut architecture. The bar is
*"nearly production-quality"*, which means readable and changeable — not a framework.

| Principle | Apply it as… | Over-design smell (avoid) |
|---|---|---|
| **S**ingle Responsibility | One class = one job (`hint()` is pure; `Game` runs rounds; `Stats` counts) | A `Manager`/`Helper`/`Util` for every verb |
| **O**pen/Closed | The command registry: add a command without editing the executor | A plugin system with reflection for a 3-command robot |
| **L**iskov | Subtypes honor the base contract (rarely needed here — prefer composition) | Deep inheritance trees |
| **I**nterface Segregation | Small interfaces: `Store` has 3 methods, `Limiter` has 1 | A 12-method "service" interface |
| **D**ependency Inversion | Inject the clock / store / RNG so logic is testable | A DI container for one object |

**Rule of thumb for the interview: introduce an abstraction only when a *second*
concrete case exists or is explicitly asked for (YAGNI).** Examples in this folder:
- `UrlShortener` depends on a `Store` interface → because Layer 3 *asks* for swappable persistence. Justified.
- `RobotGrid` does **not** have a `MovementStrategy` interface → there's one grid, one rule set. Adding one would be over-design.
- `RobotExtensibleCommands` **does** use a registry → because the spec *says* "commands keep expanding." The abstraction is the actual requirement.

When you add an interface, say *why* out loud: *"I'm extracting `Store` because you asked
about Redis later — this keeps the LRU policy independent of where data lives."* That one
sentence is the difference between "good design" and "gold-plating" in the interviewer's notes.

---

## Per-Problem Logic

### 1. Robot / Mars Rover — [`RobotGrid.java`](RobotGrid.java)

**Clarify:** Grid bounded or infinite? Start position/facing? What happens at a wall —
stop, wrap, or error? Case-sensitive commands?

| Layer | What you build | What you *say* |
|---|---|---|
| 1 | `execute(commands)` on an infinite grid | "Direction as an enum in clockwise order — then left/right is just `±1 mod 4`, no if-chains." |
| 2 | Bounded grid + obstacles, blocked move returns `false` | "I need a policy for blocked moves. I'll stop and report; alternatives are skip-and-continue or throw — which do you want?" |
| 3 | `serialize()` / `deserialize()` | "Serialization lets us persist a robot between requests — needed the moment this is a service." |
| 4 | *(discuss)* HTTP API, 1000 robots, collisions | "Each robot is independent state → shard by id. Collisions need a grid reservation or it's a race." |

**Design note:** `Position` is immutable (every move returns a new one). Mention it — it
makes undo/history trivial and is thread-safe by construction.

**"Would you ship it?"** → "Layer 1–3 yes for a single instance; for production I'd add
input validation, a blocked-move policy agreed with product, and persistence behind an interface."

---

### 2. Extensible Robot — [`RobotExtensibleCommands.java`](RobotExtensibleCommands.java)

This is a **design test disguised as the robot problem.** The spec literally says commands
will keep growing. If you write `if (c=='F') … else if (c=='B') …`, you've failed the point.

| Layer | What you build | What you *say* |
|---|---|---|
| 1 | `CommandRegistry` (map of `char → State→State`) + F/L/R | "Since commands will expand, I'll register them as small functions. Adding one never touches the executor — that's Open/Closed." |
| 2 | Invalid-command policy (ignore / raise / collect) | "Unknown commands need a defined behavior. I'll make it a parameter so callers choose." |
| 3 | *(discuss)* testing strategy | "Unit-test each command in isolation; property test that F then B is identity; table-test edge cases." |
| 4 | `UndoableRobot` (history stack) | "Undo isn't a pure command — it needs history. I'll keep it in the executor, not the registry. Commands move; the executor remembers." |

**The money moment:** in `main`, a brand-new `'J'` (jump) command is registered *at runtime*
and just works. Demo that and say *"I didn't modify any existing code to add J."* That sentence
is what staff-level looks like.

**Don't over-design:** a `Command` is a one-line lambda, not a class hierarchy with a
`CommandFactory`. Resist it. If asked to undo, *then* add the stack — not before.

---

### 3. LRU Cache — [`LRUCache.java`](LRUCache.java)

The most common problem. They've seen it 100 times — your *clarity* and *follow-ups* separate you.

**Clarify:** Key/value types? Capacity fixed at construction? Thread-safe needed? Is `null` a valid value?

| Layer | What you build | What you *say* |
|---|---|---|
| 1 | Manual doubly-linked list + HashMap, O(1) | "DLL gives O(1) move-to-front; the map gives O(1) lookup. I'll hand-roll it to show the invariant, but in prod I'd use `LinkedHashMap(accessOrder=true)`." |
| 2 | Configurable capacity, then TTL | "TTL needs a clock. I'll inject it (`LongSupplier`) so tests don't `Thread.sleep` — deterministic and fast." |
| 3 | *(discuss)* swappable backend, thread-safety | "Extract a `Store` interface to sit in front of memory or Redis. For threads: a ReadWriteLock, or Caffeine in production." |
| 4 | *(discuss)* distributed invalidation | "Prefer consistent hashing — one key, one node — so I never need cross-node invalidation." |

**Two implementations on purpose:** `Cache` (hand-rolled DLL) proves you understand the
mechanics; `TtlCache` (LinkedHashMap) shows you know the standard-library shortcut. Showing
both = "I can build it *and* I know when not to."

**"Would you ship it?"** (this is the actual Aug 2024 follow-up) → "The core yes. Before
prod: thread-safety, metrics on hit/eviction rate, a max-memory bound not just max-count,
and I'd benchmark vs Caffeine before hand-rolling."

---

### 4. Secret Santa — [`SecretSanta.java`](SecretSanta.java)

Usually paired *after* LRU. The trap is randomness.

**Clarify:** Can someone get themselves? (No.) Must it be uniform random, or just not
self-assigned? Re-run allowed if someone drops out?

| Layer | What you build | What you *say* |
|---|---|---|
| 1 | Parse CSV → assign → emit CSV | "I'll separate parsing, assignment, and formatting — three responsibilities, three functions." |
| 2 | Derangement (shuffle until no fixed point) | "A `+1` shift isn't random — the receiver is predictable, which defeats the secret. I'll shuffle and reject fixed points." |
| 3 | Edge cases: `<2` people, n=2 | "Fewer than two is an error, not a crash. n=2 must swap — my shuffle-retry handles it." |

**The interviewer *will* ask "is this truly random?"** Answer: yes — every derangement is
reachable; expected retries ≈ e ≈ 2.7, so it's effectively O(n). Mention Sattolo's algorithm
as the guaranteed-O(n) alternative (single pass) and note its slight non-uniformity. That
shows depth without you needing to implement it.

**Inject the `Random`** so tests are reproducible (seeded). Same lesson as the clock in LRU.

---

### 5. URL Shortener — [`UrlShortener.java`](UrlShortener.java)

**Clarify:** Custom aliases allowed? Same URL → same code (dedup) or new each time?
Code length/charset? Expiry?

| Layer | What you build | What you *say* |
|---|---|---|
| 1 | `shorten` / `resolve`, two maps | "Two maps: code→url for resolve, url→code for dedup. Trades memory for O(1) both ways." |
| 2 | Validation, dedup, collision retry | "I hash + base62, and on a collision I re-salt and retry. `Integer.toUnsignedLong` avoids the `Math.abs(MIN_VALUE)` bug." |
| 3 | *(discuss)* persistence behind `Store` | "Logic depends on a `Store` interface, so swapping memory→DB doesn't touch shortener code." |
| 4 | *(discuss)* scale | "Reads ≫ writes → cache at the CDN edge and redirect there. Shard by hash with consistent hashing." |

**Subtle bug worth naming:** `Math.abs(Integer.MIN_VALUE)` is *still negative*. Using
`Integer.toUnsignedLong(...)` sidesteps it. Spotting traps like this reads as production maturity.

**Don't over-design:** one `Store` interface is enough. Don't add a `CodeGenerationStrategy`
interface unless they ask for pluggable algorithms.

---

### 6. Word Guessing Game — [`WordGuessingGame.java`](WordGuessingGame.java)

The whole problem is **one tricky function**: duplicate-letter hints.

**Clarify:** Word length fixed? Case-sensitive? Does an invalid guess cost an attempt? (No.)
Wordle-style duplicate rules?

| Layer | What you build | What you *say* |
|---|---|---|
| 1 | `hint(guess, target)` — two passes | "Two passes: exact matches first, then misplaced — consuming a counter so duplicates can't over-count. The naive `contains()` is the classic bug." |
| 1 | `Game` loop: 5 attempts, validation | "Invalid guesses are reported but don't burn an attempt — that's a product rule I'll honor." |
| 2 | Stats: streaks, persistence | "Stats is its own class — single responsibility, easy to persist and test separately." |

**Walk the duplicate case on paper for the interviewer:** `ABBA` vs `BALE`. Pass 1: no
exact matches, so target letters {B,A,L,E} are all "available." Pass 2: guess's A and first
B each consume one → `00--`. The trailing B and A find nothing left → `-`. This is the case
candidates get wrong; narrating it earns the round.

**Test the function before the game.** `hint` is pure → table-driven tests are trivial and
prove the hard part works before you build the loop around it.

---

### 7. Rate Limiter — [`RateLimiter.java`](RateLimiter.java)

**Clarify:** Per-user or global? Hard limit or smooth? Allowed to burst? Distributed?

| Layer | What you build | What you *say* |
|---|---|---|
| 1 | Fixed window (bucket by window-id) | "Cheapest option. Caveat: it allows up to 2× at a window boundary — fine for coarse limits." |
| 2 | Sliding window log | "Accurate — I keep timestamps and evict the old ones. Costs O(requests-in-window) memory." |
| 3 | Token bucket | "Smooth refill + controlled bursts. This is what Shopify's public API actually uses." |
| 4 | *(discuss)* distributed | "Per-node counters let a user do N×limit. Central Redis `INCR` is atomic and simple; or local `max/N`; or gossip for soft limits." |

**The trap (and a real bug I found in the C++ version):** fixed-window math must not mix
time units. Compute `nowMillis / windowMs` for the window id — never modulo raw nanosecond
ticks. Naming this trade-off ("fixed window is cheap but bursty; sliding is exact but heavier")
is exactly the trade-off reasoning they grade.

**Inject the clock** → the tests advance time instantly with no sleeps.

---

### 8. Discount Calculator — [`DiscountCalculator.java`](DiscountCalculator.java)

Most e-commerce-flavored — very on-brand for Shopify.

**Clarify:** Bundle = same day only? Multiple bundles per day? Stackable promos? Per-customer limits?

| Layer | What you build | What you *say* |
|---|---|---|
| 1 | Bundle pricing, group by day | "Group purchases by day, then pair popcorn+soda greedily into bundles, rest at list price." |
| 2 | Quantities (multiple bundles/day) | "`min(popcorn, soda)` bundles, remainder individually. Scales to any quantity." |
| 3 | Promo codes + per-customer usage limit | "Promo is *composition* — `PricingService` wraps the calculator, so pricing logic stays clean." |
| 4 | *(discuss)* configurable rule engine | "Rules in a DB (type, priority, stackable) so marketing edits without a deploy. Fail *closed* if the store is down — protect revenue." |

**The maturity signal:** say *"money should be integer cents or `BigDecimal`, not `double` —
floating-point rounding is a real correctness bug for currency. I'm using `double` for speed
here, but I'd flag it before shipping."* Knowing the right answer while making a pragmatic
interview choice **and saying so** is precisely the staff-level move.

---

## A 10-Minute Pre-Interview Checklist

- [ ] I will **ask 2-4 clarifying questions before typing.**
- [ ] I will get **Layer 1 running and tested** before extending.
- [ ] I will **run my tests after every layer.**
- [ ] I will **narrate** each decision and its trade-off.
- [ ] I will **inject** clocks / RNG / stores so my code is testable.
- [ ] I will add an abstraction **only when a second case exists or is asked for.**
- [ ] When asked **"would you ship it?"** I have a real answer: validation, metrics,
      thread-safety, the data-type caveat — not "yes it works."
- [ ] I will treat the interviewer as my **pair**, accept redirects without resisting,
      and keep the keyboard moving.
```
