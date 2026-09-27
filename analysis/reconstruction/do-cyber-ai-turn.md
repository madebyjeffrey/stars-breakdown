# DoCyberAiTurn reconstruction (partial)

## Multi-order packed PLORD mutation E12

The shared movement writer now has host/native coverage of the broader
`FMoveAiFleet` append and replacement behavior across multi-record queues,
including last-endpoint replacement, first-endpoint truncation, non-append
replacement, and the helper's full-capacity guard (not the original
`LpplReAlloc` integration). Both host and Win98 VC++ 1.52c
reports pass 15/15, and the full host suite passes 526 tests. This is supporting
E3 evidence, not a named Step-5 closure; Cyber personality predicates and the
complete AI-table/original-turn comparison remain open. See
`analysis/compat/ai-plord-multi-order-e12.{md,json}` and
`runs/vmctl-ai-plord-multi-e12/`.

`reconstructed/src/do_cyber_ai_turn.c` transfers the Cyber setup and table
preparation phases of `DoCyberAiTurn` (NE `0016:002a–1a77`) into typed C.  It
preserves the `IroEnsureAi` research-order handoff, five merge masks, Cyber packet-pressure ramp, armada/support
potency ramps, four turn-dependent design budgets, three design-status groups
with a shared sixteen-byte status mask, the first split consuming that mask,
and the exact late split masks `{0:2}`, `{1:2}`, and `{2:2,3:2}`, shdef
maintenance, rate-aware low-mineral
classification (current-player scanned-planet gate, signed 32-bit production
rate plus unsigned 32-bit concentration), the
strict four-resource production affordability gate (equal fixed-point resource
values are sufficient; a negative remainder is not), friendly-fleet ownership
filter, typed fleet-role
classification (including raw auxiliary design slots 10/11), hostile-planet
packet-strength threat bands and starbase increments, production/fleet dispatch, and the ordered basic-task, packet,
and production-queue finalization phases.

The source probes pass the late-turn setup, research-order handoff, shared
design-status-mask consumption and exact split-mask dispatch, mineral-table,
hostile threat marking, equal-resource and underfunded production-gate, typed
fleet-policy, and ordered finalization paths, including null-table entry
rejection, under strict host C89. A fresh VC++ 1.52c / Win98 checkpoint also
passes all 32 native checks without a runtime error dialog. A fresh packed
integration checkpoint now binds the recovered 56-byte `_planet` and 124-byte
`_fleet` views and passes 25 host/native checks over merge/status/split masks,
threat and production gates, fleet-role aggregates, and ordered finalization.
Packet routing and freighter behavior now have a full-caller edge matrix;
detailed fleet-order repair, direct packed `PLORD` mutation beyond the tested
first order, complete AI-table integration, and original-behavior evidence
remain open.

The typed source now dispatches a separate `repair_order` service boundary
before its normal fleet visitor for the raw missing-first-order phase. The
strict host probe passes `CYBER_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0
RESULT=PASS`, including preservation of the five merge-mask calls,
ordered/non-owned bypasses, potency forwarding, ordering, and failure
propagation. Packed `PLORD` mutation, packet/freighter routing, native replay,
and original-turn comparison remain open.

The Cyber repair boundary now carries the shared raw `PLORD` pointer shape
used by `stars_ai_repair_first_order`; the helper's host checkpoint passes
`AI_PLORD_REPAIR_SOURCE_CHECKS=9 FAILURES=0 RESULT=PASS` through the Robotoid
caller. Cyber packet/freighter routing, multi-order mutation, and
native/original-turn evidence remain open.

The typed source now separates Cyber fleet action planning from application.
`plan_fleet_action` receives the fleet policy and armada potency, while
`apply_fleet_action` receives only a non-empty plan before the typed visitor
and legacy fleet callback. Named action callbacks are selected before the
generic applier fallback, and the probe exercises the specialized colonize
callback. The strict host probe passes
`CYBER_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`, covering
repair-before-plan ordering, policy and potency forwarding, no-op suppression,
payload preservation, and planner/applier failure propagation. Exact route
predicates, packet/freighter behavior, multi-order packed `PLORD` mutation,
native replay, and original-turn differential remain open. Evidence is in
`analysis/compat/do-cyber-ai-turn-route-plan-e3.{md,json}`.

The E4 packet-service checkpoint adds an optional typed packet callback to the
turn boundary. It receives the converted planet table, its count, player id,
and computed packet pressure; the legacy player-only callback remains the
fallback. The strict C89 packed integration probe passes
`CYBER_AI_TURN_INTEGRATION_CHECKS=30 FAILURES=0 RESULT=PASS`, including the
packed 501 packet-strength value, write-back to 401, and final callback ordering. This closes no
named Step-5 unit and is not native validation. Direct `DoCyberPackets` /
`DoCyberFreighter` composition, packed `PLORD` mutation, AI-table integration,
original-turn comparison, and promotion remain open. Evidence is in
`analysis/compat/do-cyber-ai-turn-packet-bridge-e4.{md,json}`.

The E5 freighter-service checkpoint adds an optional typed `run_freighter_ex`
callback to the owned-fleet pass. It receives the converted fleet, the derived
fleet policy, and armada potency after first-order repair and before fleet
action planning. The strict C89 packed integration probe passes
`CYBER_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS`, including packed
transport-count forwarding and callback ordering. The native replay is
supporting evidence;
fresh VC++ 1.52c / Win98 replay in `C:\\CYBPE5` compiled and linked, returned
the same 31-check report, exited normally, and showed no Program Error dialog.
Direct `DoCyberFreighter` composition, packed `PLORD` mutation, AI-table
integration, original-turn comparison, and promotion remain open. Evidence is
in
`analysis/compat/do-cyber-ai-turn-freighter-bridge-e5.{md,json}`.

The E6 direct-composition checkpoint now invokes the real typed
`stars_do_cyber_freighter` implementation from the complete `DoCyberAiTurn`
caller. The adapter covers owned-planet transfer direction, target selection,
exact `0x11b0` movement, packed first-order replacement, and ordering before
the typed fleet visitor. The strict host C89 and fresh VC++ 1.52c / Win98
replays both pass
`CYBER_AI_TURN_FREIGHTER_COMPOSITION_CHECKS=18 FAILURES=0 RESULT=PASS`, with
normal exit and no Program Error dialog. This closes no named Step-5 unit:
complete packet/freighter predicates, raw fleet-cargo behavior, AI-table
integration, original-turn comparison, and promotion remain open. Evidence is
in `analysis/compat/do-cyber-ai-turn-freighter-composition-e6.{md,json}`.

The E7 packet/freighter-composition checkpoint invokes both real
`stars_do_cyber_freighter` and `stars_do_cyber_packets` from the complete AI
turn. The strict host C89 and fresh VC++ 1.52c / Win98 replays both pass
`CYBER_AI_TURN_PACKET_FREIGHTER_COMPOSITION_CHECKS=20 FAILURES=0 RESULT=PASS`,
covering freighter-before-basic-task order, packet queueing after basic tasks,
packet-strength writeback from 501 to 389, exact `0x11b0` packed movement, and
finalization order. This is supporting evidence and closes no named Step-5
unit: complete raw cargo predicates, packet/freighter edge branches,
AI-table integration, original-turn comparison, and promotion remain open.
Evidence is in
`analysis/compat/do-cyber-ai-turn-packet-freighter-composition-e7.{md,json}`.

The E8 packet/freighter edge checkpoint invokes both real services from the
complete AI-turn caller across five scenarios: high-stock owned-planet
drop-off, low-stock owned-planet pickup, enemy ordinary-planet starbase
fallback, open-space short circuit, and packet-target rejection. The strict
host C89 and fresh VC++ 1.52c / Win98 replays both pass
`CYBER_AI_TURN_PACKET_FREIGHTER_EDGE_CHECKS=45 FAILURES=0 RESULT=PASS`, with
compile/link and process exit passing and no Program Error dialog. The matrix
checks transfer direction, exact `0x11b0`/`0x11b1` movement, packed first-order
mutation, lookup short circuits, packet queue/strength writeback, suppression
of rejected packets, and final callback ordering. Together with E5's native
freighter gates and E7's accepted packet/freighter composition, this closes
the named packet/freighter behavior Step-5 unit. Detailed order repair/order
writes, complete Cyber AI-table integration, original-turn differential, and
Step-6 promotion remain open. Evidence is in
`analysis/compat/do-cyber-ai-turn-packet-freighter-edge-e8.{md,json}` and
`runs/vmctl-vc152cybfe8/`.

The E9 order-write checkpoint combines the shared first-order repair with the
typed route-plan/action boundary and the packed `PLORD` writer in one complete
caller path. An owned colonizer fleet takes a same-endpoint replacement while
an owned armed fleet appends a movement order; foreign fleets are bypassed.
The strict host C89 and fresh VC++ 1.52c / Win98 replays both pass
`CYBER_AI_TURN_ORDER_WRITE_CHECKS=24 FAILURES=0 RESULT=PASS`, with compile/link
and process exit passing and no Program Error dialog. The matrix closes the
named detailed repair/order-write Step-5 unit. Direct raw personality branch
coverage beyond these typed action families, complete Cyber AI-table
integration, original-turn differential, and Step-6 promotion remain open.
Evidence is in
`analysis/compat/do-cyber-ai-turn-order-write-e9.{md,json}` and
`runs/vmctl-vc152cybowe9/`.

The E10 action-dispatch checkpoint exercises all nine named Cyber fleet action
families through the complete caller: clear task, move, colonize, wormhole,
starbase, attack, armada, transport, and freighter targeting. The movement
families route seeded first-order records through the shared packed writer.
The strict host C89 and fresh VC++ 1.52c / Win98 replays both pass
`CYBER_AI_TURN_ACTION_MATRIX_CHECKS=34 FAILURES=0 RESULT=PASS`, with
compile/link and process exit passing and no Program Error dialog. This is
supporting evidence only; raw personality predicates, complete AI-table
integration, and the original-turn differential remain open. Evidence is in
`analysis/compat/do-cyber-ai-turn-action-matrix-e10.{md,json}` and
`runs/vmctl-vc152cybame10/`.
