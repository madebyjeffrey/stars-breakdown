# DoRobotoidAiTurn reconstruction (partial)

`reconstructed/src/do_robotoid_ai_turn.c` transfers the Robotoid design
maintenance and table-annotation phases of `DoRobotoidAiTurn` (NE
`0012:0312–1f33`) into typed C.  It preserves the late-turn merge sequence,
armada/support potency ramps, turn-dependent design budgets, five design-status
groups with their shared 16-byte mask, the raw inactive `0x1d` filter for
entries 14–15, four split dispatches (including the
raw 11/12/13 mask), design maintenance, attack-design-gated
war-fleet counting,
friendly-fleet ownership filtering, hostile planet marking with the raw
packet-strength threat band and starbase increment, the scanned-planet
and mineral-condition production-entry gate, five-hull production-resource affordability, typed fleet-role
classification, the turn-dependent research-order handoff, production/fleet
dispatch, and the ordered basic-task/production-queue finalization phases.

The source probes pass the late-turn setup, dispatch, research-order handoff,
the inactive `0x1d` status-mask filter,
the production-entry threshold and visibility boundaries, hostile threat-band
marking, ordered finalization paths, and null-table entry rejection under
strict host C89. A packed integration adapter now binds the recovered 56-byte
`_planet` and 124-byte `_fleet` layouts, including packet-strength bits and
the sixteen 16-bit ship-count entries, and checks the status/split,
production, threat, fleet-role, and finalization boundary. Production costs
and design selections remain explicit service results because the original
procedure calls `GetResourcesAvailable`, `GetProdQCost`, `GetTrueHullCost`,
and `CheckAiShdefStatus` rather than loading invented direct fields.

A host-only fleet-gate matrix now exercises the typed inputs at the still-open
order seam: empty, colonizer, transport, already ordered armed, separate
support/attack bins, `warship_count` fallback, and non-owned fleets. It passes
`ROBOTOID_AI_FLEET_GATE_CHECKS=10 FAILURES=0 RESULT=PASS`. The matrix is an
input-boundary diagnostic, not an implementation of the missing route action;
it closes no Step-5 unit.

E4 tightens the repair seam to the raw late-turn deterministic eligibility:
owned fleet, slot 0 nonzero, turn at least 41, `cord == 1`, and empty first
order. It also moves repair after fleet route planning; a selected action
bypasses the repair, matching the original path that attempts a random nearby
route first. The raw `Random(5)`/nearby-planet selection and exact RNG draws are
still unmodeled and must be represented by the route policy before claiming
full fidelity. The host repair probe reports
`ROBOTOID_AI_REPAIR_BOUNDARY_CHECKS=7 FAILURES=0 RESULT=PASS`; the route probe
reports `ROBOTOID_AI_ROUTE_PLAN_BOUNDARY_CHECKS=18 FAILURES=0 RESULT=PASS` and
checks both route-selected bypass and no-op-then-repair ordering.

Fleet-order repair, colonizer/transport routing, and native VC1.52c validation
were previously covered by the source checkpoint in fresh `C:\ROB`. The new
packed VC++ 1.52c/Win98 checkpoint in fresh `C:\ROBINT` returns
`ROBOTOID_AI_TURN_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS` without a
runtime error dialog. This advances the procedure to Phase 2 step 5/6, not
promotion; raw `PLORD` mutation, complete AI-table integration, and full
original-turn differential remain open.

The shared `stars_ai_repair_first_order` helper applies the recovered
four-byte-header/18-byte-record repair writes. The first host checkpoint was a
single-order probe (`AI_PLORD_REPAIR_SOURCE_CHECKS=9`). A subsequent original-
instruction audit found that the helper's count reset was wrong: the Robotoid
branch at `1088:130D–136F` writes the first kind/repair words but never writes
`iordMac`. The corrected helper preserves both header counts and every later
order. Fresh strict-C89 and VC++ 1.52c/Win98 `C:\ROBRE1` replays pass the
three-order 14-check matrix with exact whole-buffer comparison, route bypass,
ineligible/foreign exclusions, and callback order. This closes the named
Step-5 E1 repair unit; it does not close route targeting or original full-turn
evidence. See `analysis/compat/do-robotoid-ai-turn-multi-order-repair-e1.md`.

The typed source now separates Robotoid fleet action planning from action
application. `plan_fleet_action` receives the fleet policy and turn and can
select the recovered clear-task, move, colonize, wormhole, starbase, attack,
armada, transport, and freighter action families; `apply_fleet_action` receives
the resulting plan only for a non-empty action. Named action callbacks are
selected before the generic applier fallback, and the probe exercises the
specialized colonize callback. The host route probe covers the updated
planner-before-repair order, no-op repair, action suppression, payload
preservation, and planner/applier failure propagation. The random-route branch,
multi-order packed `PLORD` mutation, complete AI-table state, native replay,
and original-turn differential remain open. Evidence is in
`analysis/compat/do-robotoid-ai-turn-repair-gate-e4.{md,json}`.
