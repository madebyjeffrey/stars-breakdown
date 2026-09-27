# DoRototillAiTurn RT-E1 — packed production and AI-planet source

Original NE 20:1E22-~24A0 first counts unowned planets whose low
overlap-4 class byte exceeds two and whose PctPlanetOptValue is positive.
The reconstructed typed planet now carries that class separately from
the scanned/visibility word; the previous source conflated the two.
In the second pass, it writes the 16-byte AI-planet visit marker,
unowned mineral score (capped at 127), bad-owned flag, and foreign
visibility/optimal markers in the original branch order.

For production-eligible owned or unowned planets, the new source path
requires explicit live inputs and a bounded raw PLPROD allocation.
The original four-byte item scan bypasses the queue when type 2 has a
design index of at least 17. Otherwise turn zero queues two design-0
items and still calls FinishProduction(0), matching the original local
flag; later turns queue at most one design-1 item across planets when
the original existence/unowned-count gate holds. It preserves the
InitProduction/bypass/queue/FinishProduction order and exposes
per-turn item/bypass counters.

The strict-C89 host probe passes 15/15: complete pre-production census,
packed AI markers, one-per-turn design-1 queue, turn-zero double queue,
the bypass threshold, finish flags, count gate, and malformed raw queue
preflight. Corrected legacy source and packed-integration probes also
pass. The full host suite passes 564 tests with 153 expected skips and
zero failures.

This initial host result did not bind callbacks to live game memory.
The r1 VC1.52c package was prepared but never run; the current r9 native
result is recorded below. An original production-turn comparison remains
open, as do RT-E1 and RT-P.

## Packed planet and AI table continuation

The current source can bind each original 56-byte `_planet` record. It
decodes ID and owner at offsets 0/2, the separate class and visibility
fields at 4/4–5, the three concentrations at 9–11, and the production
mineral predicate from low/high words at 40/42. These packed fields
supersede contradictory typed snapshots in both census and production
passes. Optional `vlpbAiPlanet` storage is selected by `id * 16`; an
explicit per-planet record takes precedence. All supplied records and
table indices are checked before the first AI-table mutation. Callback
planet views are transient and must not be retained.

The expanded strict-C89 probe passes 20/20, including an out-of-order
planet ID, packed ownership/class/visibility/mineral precedence, ID-indexed
AI marker, and short/mismatched planet and short AI-table rejection before
writes. The r1 packet and its 15-check report are stale for this source;
the fresh r2 packet is prepared for `C:\RTE1N2` and remains unrun. Live
service binding and a native/original production-turn comparison still
keep RT-E1 and RT-P open.

Verification at 2026-09-23 05:43 MDT: full host suite 564 tests,
153 expected skips, zero failures; baseline inventory and `git diff --check`
pass. These are host checks, not guest or original acceptance.

## Post-InitProduction live PLPROD continuation

The original branch calls `InitProduction(rgprod)` before reading the
global `lpplProdGlob` allocation. A per-planet pre-init snapshot can be
stale if initialization replaces that allocation. The source now supports
`current_prod_queue`, a callback invoked immediately after init. Its
returned bounded PLPROD header/items supersede the snapshot for the
type-2/design-17 bypass and for queue/finish callbacks. When this binding
is present, stale pre-init queue metadata is deliberately not validated;
the returned active allocation is validated before any queue scan or add.

The new strict-C89 probe passes 7/7: an otherwise invalid stale snapshot
is ignored, a live bypass prevents additions, a live open queue receives
the opening two items, callbacks observe init → bind → add/finish order,
and short or failing live bindings abort before queue/finish. A queue
reallocation after `AddItemToQueue` and actual global-pointer identity
remain for native/integration validation. Fresh r6 Win16 packet
`compat/vc152/rototill-production-e1-r6/` targets `C:\RTE1N6` and is
unrun; earlier r1–r5 packets are stale for current source.

Verification at 2026-09-23 06:04 MDT: full host suite 566 tests,
153 expected skips, zero failures; baseline inventory and diff checks pass.
These do not establish live Win16 global identity or original-turn behavior.

## Post-add queue relocation continuation

`AddItemToQueue` may replace `lpplProdGlob` as it grows. The source now
reacquires and bounds the active PLPROD after each successful addition:
between the two opening-turn design-0 calls and again before
`FinishProduction`; the later design-1 call is likewise followed by a
refresh before finish. The per-turn added-item diagnostic increments
immediately after each successful add, even if a later rebind fails.

The expanded strict-C89 live-queue probe passes 12/12. It simulates
first- and second-add pointer changes, verifies that each subsequent
callback receives the new allocation, tests the one-add later-turn
branch and changed flag, and rejects a malformed post-add allocation
before the next add or finish. This closes the host-side stale-pointer
subproblem, not the live Win16 global identity or original-turn boundary.
Fresh r7 packet `compat/vc152/rototill-production-e1-r7/` targets
`C:\RTE1N7`; r1–r6 are stale for current source.

Verification at 2026-09-23 06:08 MDT: full host suite 566 tests,
153 expected skips, zero failures; baseline inventory and diff checks pass.
The guest agent did not connect during the r7 listener check, so native
and original-turn acceptance remain unverified.

## Focused Win16 execution, 2026-09-26

The fresh r9 package built and ran with VC++ 1.52c in StarDev. Its
[native return](../../runs/rototill-production-e1-r9/RTE1RES.TXT) reports
20 production, 17 packed-fleet, and 12 live-queue checks passing (49/49),
with `COMPILE=PASS` and `LINK=PASS` in the returned build status. The
[packet record](do-rototill-production-e1-native-packet.md) documents the
r8 linker failure and r9 repair. The guest run exercises the bounded source
and synthetic callback allocations. Live original global identity, exact
queue costs, and original-turn comparison are still needed for RT-E1.

## Source queue-writer composition, 2026-09-26

The raw Rototill calls `AddItemToQueue(0,1,2,1)` twice on turn zero and
`AddItemToQueue(1,1,2,1)` at most once later. The recovered shared
`AddItemToQueue` packs quantity in bits 0–9, design in 10–16, and group in
17–19. An [11-check strict-C89 caller composition](../../tests/do_rototill_queue_writer_e1_probe.c)
now passes these requests into production `stars_add_item_to_queue`, with
a new PLPROD allocation after each add. It verifies callback order and
post-add rebinding, and the resulting item bytes `01 00 04 00` twice on
turn zero or `01 04 04 00` for the later design-1 branch. The bypass item
still suppresses additions. This is the pre-finish workspace; it does not
prove persisted queue contents, costs, live `lpplProdGlob` identity, or an
original-turn result. RT-E1 remains open.
