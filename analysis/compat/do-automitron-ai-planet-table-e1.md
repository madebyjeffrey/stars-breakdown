# DoAutomitronAiTurn AU-E1 — live AI-planet table indexing

The original turn writes `vlpbAiPlanet[id * 0x10 + field]`, not an array
position corresponding to the current planet-loop index. The production
source now accepts an optional live 16-byte-per-ID table and resolves each
record by `planet.id`. A supplied per-planet record remains an explicit
override. Before writing any record, it rejects undersized overrides and
out-of-range/negative table IDs; Win16 near-pointer indexing is bounded to
the 64 KiB segment. When no live binding is supplied, the older callback
boundary remains unchanged.

Strict-C89 host probe `tests/do_automitron_ai_planet_table_e1_probe.c`
passes 8/8 checks: out-of-order IDs, owned/foreign markers, override
precedence, and preflight rejection before any table mutation. The prior
per-planet record probe still passes 14/14. Fresh r5 Win16 packet includes
both. It has **not** been transferred or executed; live guest/original-turn
evidence and remaining mineral/action branches keep AU-E1 open.
