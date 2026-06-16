# Shopify System Design — Staff Interview Prep

Ten real, recurring Shopify system-design prompts, each worked end-to-end at **staff level**.
Built as a small visual website — **open [`index.html`](index.html) in a browser** to start.

> Why HTML: every question has an architecture diagram, color-coded steps, comparison tables,
> and collapsible follow-ups that read far more clearly rendered than as plain markdown.
> All pages share [`assets/style.css`](assets/style.css); each page is also print-to-PDF friendly.

## How each question is structured (the 8-step framework)

`1 Clarify → 2 Estimate → 3 API → 4 Data model → 5 Architecture → 6 Deep dives → 7 Trade-offs → 8 Follow-ups`

Every page ends with a **"Staff-level signal"** box: the specific things to *say* to demonstrate
staff scope (the bottleneck, the consistency split, the blast-radius/isolation story).

## The questions

| # | Question | Tier | Why Shopify asks it |
|---|---|---|---|
| [01](01_flash_sale.html) | Flash Sale / BFCM Traffic Surge | 1 · Signature ★★★★★ | 1000× spike on one hot key + never oversell |
| [02](02_inventory_management.html) | Global Inventory Management | 1 · Signature ★★★★★ | The correctness problem: prevent overselling |
| [03](03_checkout_payments.html) | Checkout & Payment Processing | 1 · Signature ★★★★★ | Idempotency, never double-charge, PSP outage |
| [04](04_multi_tenant_sharding.html) | Multi-Tenant Architecture (Pods) | 1 · Signature ★★★★☆ | Millions of shops, sharding, isolation |
| [05](05_shopping_cart.html) | Shopping Cart Service | 2 · Core ★★★★☆ | Anonymous + merge, AP over CP |
| [06](06_product_search.html) | Product Catalog, Search & Filtering | 2 · Core ★★★★☆ | Per-store search, facets, freshness |
| [07](07_webhook_delivery.html) | Webhook & Event Delivery | 3 · Ecosystem ★★★★☆ | App ecosystem: at-least-once + retries + DLQ |
| [08](08_rate_limiter.html) | Rate Limiting API | 3 · Ecosystem ★★★☆☆ | REST bucket + GraphQL calculated cost |
| [09](09_notification_system.html) | Notification System | 3 · Ecosystem ★★★☆☆ | Multi-channel, failover, dedupe at spike |
| [10](10_analytics_pipeline.html) | Real-time Merchant Analytics | 3 · Ecosystem ★★★☆☆ | Stream aggregation, exact vs approximate |

The index page also covers the **interview process** and an **operational curveball**
("checkout p99 just spiked 5× — debug it live") that sometimes replaces the greenfield prompt.

## Sourcing

Questions were cross-referenced across public candidate reports and Shopify-specific interview
guides (Blind, interviewing.io, InterviewQuery, techinterview.org, System Design Handbook,
AlgoMaster). Architectures reflect publicly documented Shopify patterns (Pods/sharding, BFCM
scaling, GraphQL cost-based limits). Full source links are listed on each page and the index.

These are **preparation prompts**, not leaked questions — the round tests reasoning, so practice
the framework rather than memorizing answers.
