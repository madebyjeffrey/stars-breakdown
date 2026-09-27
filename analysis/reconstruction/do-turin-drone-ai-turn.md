# DoTurinDroneAiTurn reconstruction (partial)

`reconstructed/src/do_turin_drone_ai_turn.c` transfers the Turin research,
design-maintenance, planet-annotation, and finalization phases of
`DoTurinDroneAiTurn` (NE `0012:3670–58b9`) into typed C.  It preserves the
`IroEnsureAi` research-order handoff, four conditionally applied merge masks
with an explicit typed “merge needed” predicate,
armada/support potency ramps, turn-dependent design budget, eight
design-status groups with the shared 16-byte status mask (including the raw player-technology gate that disables
the 2–3 design group below technology 7), post-turn split dispatch, shdef maintenance, and the
owned/unowned planet classification boundaries.  The unowned mineral-class
counter preserves the raw class/visibility gate independently of whether the
calculated mineral score is positive; its second pass does not repeat the
optical-value service call and counts every visible unowned annotation.  The
typed planet view now
also carries four resource channels, queued-production cost, and hull cost;
the unowned optical-value service is retained as the original two-pass
classification (viable-world count followed by per-planet routing marking),
including its separate second-pass count;
the source now debits resources and queues up to five hulls one at a time,
retaining an affordable prefix when a later debit fails, and exposes
typed production-preparation, production, fleet-summary, and finalization callbacks.  The fleet
summary preserves the raw friendly-fleet ownership filter, ship-role bands,
and empty-order repair boundary;
the individual order-routing helpers remain callbacks.

The source probes pass the late-turn setup, research-order handoff,
design-status, split, planet classification and its single-call first-pass
optical-value boundary plus unconditional second-pass annotation,
production affordability, merge predicate, production-preparation boundary,
and the typed fleet summary plus ordered basic-task/queue finalization
callbacks under strict host C89. A separate packed-record integration probe
binds the recovered 56-byte `_planet` and 124-byte `_fleet` views, including
the raw owner, visibility, mineral, planet, cord, and sixteen ship-count
fields. Production resources/costs, design selections, and order state remain
typed service results rather than invented raw offsets.

A fresh VC++ 1.52c/Win98 checkpoint in `C:\TUR` compiled and linked the exact
source and returned `TURIN_DRONE_AI_TURN_CHECKS=35 FAILURES=0 RESULT=PASS`
without a runtime error dialog. The packed integration checkpoint in fresh
`C:\TURINT` compiled and linked and returned
`TURIN_DRONE_AI_TURN_INTEGRATION_CHECKS=20 FAILURES=0 RESULT=PASS`, also
without a Program Error dialog. This advances the procedure to Phase 2 step
5/6; detailed production policy, fleet-order mutation, colonizer/transport
routing, complete AI-table integration, and full original-turn differential
remain open.

The host edge-policy matrix now reports
`TURIN_DRONE_AI_TURN_EDGE_CHECKS=7 FAILURES=0 RESULT=PASS`. It checks the
armada/support potency thresholds and caps at turns 0, 130, 131, 200, and
300, the merge-needed short-circuit, all eight design-status calls, rich and
insufficient five-hull affordability, and null-input rejection. This is
host-only evidence and closes no remaining Step-5 unit; direct production
table mutation, fleet routing/order mutation, native replay, and original
turn differential remain open. See
`analysis/compat/do-turin-drone-ai-turn-edge-e3.{md,json}`.

The typed source dispatches a separate `repair_order` service boundary before
its normal fleet visitor for the missing-first-order phase. E4 corrects the
initial broad predicate to the raw gate in
`analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c:675-693`: slots
2, 3, 1, 8, 9, 13, 14, 0, 10, and 11 are zero, slot 12 is nonzero, `cord == 1`,
and the first-order kind is zero. The strict-C89 matrix reports
`TURIN_DRONE_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS`, including
negative cases for the individual relevant slots, cord, order kind, and owner.
The source, packed-integration, route, and edge probes also pass (35, 20, 14,
and 7 checks). Native replay, complete packed route/order mutation, full AI
table integration, and original-turn comparison remain open. Evidence is in
`analysis/compat/do-turin-drone-ai-turn-repair-gate-e4.{md,json}`.

The Turin repair boundary now carries the shared raw `PLORD` pointer shape
used by `stars_ai_repair_first_order`; the helper's host checkpoint passes
`AI_PLORD_REPAIR_SOURCE_CHECKS=9 FAILURES=0 RESULT=PASS` through the Robotoid
caller. Turin-specific multi-order routing and native/original-turn evidence
remain open.

The Turin-specific first-order repair subcheck now has original-instruction,
strict-C89, and fresh VC++ 1.52c/Win98 evidence. Original `1088:5811–588A`
reads the packed kind and writes only the first kind plus two repair words.
The source now uses packed PLORD as authoritative when present and executes
repair independently of the typed visitor. Host and native `C:\TURRE1`
matrices pass 18/18 over three-order preservation, stale typed-kind values,
eligibility bypasses, and callback rejection. See
`analysis/compat/do-turin-drone-ai-turn-multi-order-repair-e1.{md,json}` and
`runs/vmctl-turre1/`. This is E1 repair support, not E1 closure: the broader
production design/queue/RNG policy remains unimplemented.

The first selected-design production loop has an instruction-derived
per-item correction. Original raw lines 353–385 debit four resource channels
and call `AddItemToQueue(..., 1)` after each affordable hull; a later shortage
retains earlier queue items. The source no longer treats this as one
all-or-nothing quantity-five callback. Strict-C89 and fresh VC++ 1.52c/Win98
`C:\TURPE1` probes pass 11/11; packed integration and edge probes pass 20/20
and 7/7. See
`analysis/compat/do-turin-drone-ai-turn-partial-production-e1.{md,json}` and
`runs/vmctl-turpe1/`. The current source has since extended this to four
selected-design loops, described below; the `TURPE1` run is historical
evidence for the first per-item correction.

The four selected-design five-item loops now use the original role order
3, 0, 7, 5 (design groups 4–5, 6–7, 10–11, 15), distinct plan-size/count
caps, per-role hull costs, the design-15 inactive gate, and resource carry
between loops. The previous source had incorrectly used role 0 for the first
loop; it now uses role 3 even in the legacy single-cost path. A strict-C89
matrix and fresh VC++ 1.52c/Win98 `C:\TURDQ1` run pass 31/31, including
ordered callbacks, exact cap boundaries, design-15 exclusion, partial stop,
and cross-loop resource debit. See
`analysis/compat/do-turin-drone-ai-turn-design-queue-e1.{md,json}` and
`runs/vmctl-turdq1/`. Turn-zero queue seeding, technology/design-count and
RNG-gated items, support-fleet choices, `FinishProduction`, live table
binding, and original-turn differential still prevent E1 closure.

The subsequent E1 seed/finalization subcheck binds the original existing-
ship queue bypass, turn-zero design-0 seed with its 191 threshold and 100/30
decrements, later base-design inactive/hull/count/built gate, and
`FinishProduction(changed)` call boundary. Strict-C89 and fresh VC++
1.52c/Win98 `C:\TURSF1` pass 21/21; the full host suite passes 535 tests
with 153 expected skips. See
`analysis/compat/do-turin-drone-ai-turn-seed-finish-e1.{md,json}` and
`runs/vmctl-tursf1/`. Technology/RNG and support-fleet branches, live
packed-table binding, actual finalization-service side effects, and
original-turn differential still prevent E1 closure. The `TURDQ1` result
above remains historical evidence for the earlier source milestone.

The next E1 subcheck models the design-8..9 technology queue at raw lines
280–294. `CheckAiShdefStatus(8,9,...)` returns a saturated total of
existing eligible designs, not a Boolean. The source takes that typed
service result plus AI/player table scalars, applies the primary and
expanded count targets, and calls `Random(4)` only in the expanded band.
Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURTQ2` pass 21/21. See
`analysis/compat/do-turin-drone-ai-turn-tech-queue-e1.{md,json}` and
`runs/vmctl-turtq2/`. The design-1, minelayer, and support branches, full
queue request shape, live bindings, and original-turn differential remain.

The subsequent E1 milestone restores the first viable-world pass before
owned-planet production, four design-1 queue requests, and the design-12
minelayer fleet/RNG path. The typed fleet count no longer truncates to eight
bits and the full queue callback carries all four `AddItemToQueue` arguments.
Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURMI1` pass 32/32, including
late-array viable worlds, first matching local fleet, count 256, exact RNG
bounds, and rejection propagation. See
`analysis/compat/do-turin-drone-ai-turn-minelayer-e1.{md,json}` and
`runs/vmctl-turmi1/`. Service effects before the minelayer decision,
support-fleet choices, live binding, and original-turn differential remain.

The subsequent E1 support-fleet subcheck models the first local owned
fleet for which `FPotentRobWarFleet(fleet,2)` is true, the slot-13+14
support-potency threshold, exact `(selected,4,2,1)` queue request, and the
early skip to `FinishProduction(changed)`. A weak first potent fleet stops
the search even if a later fleet is stronger. Strict-C89 and fresh VC++
1.52c/Win98 `C:\TURSP1` pass 23/23. See
`analysis/compat/do-turin-drone-ai-turn-support-e1.{md,json}` and
`runs/vmctl-tursp1/`. The real service binding, live tables, production
side effects, and original-turn differential still prevent E1 closure.

The next E1 checkpoint decodes actual packed planet/fleet/PLPROD bytes in
production source and uses the recovered `FPotentRobWarFleet(fleet,2)`
slot-weight formula as the default support-fleet service. Strict-C89 and
fresh VC++ 1.52c/Win98 `C:\TURPK1` pass 17/17, including queue-type/design
classification, count 256, truncation rejection, and an end-to-end decoded
support queue/finish call. See
`analysis/compat/do-turin-drone-ai-turn-packed-service-e1.{md,json}` and
`runs/vmctl-turpk1/`. This does not bind live AI/design/production memory
or establish original-turn output equivalence; E1 remains open.

The typed source now separates Turin fleet action planning from application.
`plan_fleet_action` receives the fleet policy and turn, while
`apply_fleet_action` receives only a non-empty plan before the typed fleet
visitor. Named action callbacks are selected before the generic applier
fallback, and the probe exercises the specialized colonize callback. The
strict host probe passes
`TURIN_DRONE_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`, covering
repair-before-plan ordering, policy and turn forwarding, no-op suppression,
payload preservation, and planner/applier failure propagation. Exact route
predicates, multi-order packed `PLORD` mutation, native replay, and
original-turn differential remain open. Evidence is in
`analysis/compat/do-turin-drone-ai-turn-route-plan-e3.{md,json}`.

The next E1 source checkpoint optionally binds the recovered 16-byte-per-ID
AI-planet table. The original second pass sets byte `+9` on every planet,
the capped mineral score at `+1` for visible unowned worlds, and byte `+2`
for owned worlds with negative desirability. Source prevalidation prevents
partial table writes on an invalid ID or short buffer. Strict-C89 and fresh
VC++ 1.52c/Win98 `C:\TURAI1` pass 26/26; see
`analysis/compat/do-turin-drone-ai-turn-ai-planet-e1.{md,json}` and
`runs/vmctl-turai1/`. This binds a table shape but does not yet enumerate
the live guest table or close Turin E1.

The owned-planet production gate now preserves the raw negative-desirability
branch: it marks the bad-world AI byte but skips queueing and
`FinishProduction` even when a typed production slot is marked open. The
regression checks this with packed AI bytes and an open slot. Strict-C89
and fresh VC++ 1.52c/Win98 `C:\TURDE1` pass 27/27; see
`analysis/compat/do-turin-drone-ai-turn-desirability-e1.{md,json}` and
`runs/vmctl-turde1/`. Live services and original-turn comparison still
prevent E1 closure.
