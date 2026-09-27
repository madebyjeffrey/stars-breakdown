# DoAutomitronAiTurn reconstruction (partial)

`reconstructed/src/do_automitron_ai_turn.c` transfers the Automitron design
policy, planet/fleet dispatch, and finalization phases of `DoAutomitronAiTurn` (NE
`0014:01e0–1937`) into typed C.  It preserves the early/late merge masks,
armada/support potency ramps, turn-dependent design budget, four design-status
groups, split dispatch, shdef maintenance, and owned/unowned planet boundaries.
The unowned-planet counter retains the raw class-above-two and positive
optimal-value gates instead of counting every unowned planet.
It now also performs the turn-dependent `IroEnsureAi` research-order handoff
and typed four-channel resource subtraction used before
production decisions, including queued-production cost, repeated hull-cost
affordability, the scanned-planet and mineral-condition production-entry gate,
and a typed queue callback for the selected IS design.
Fleet dispatch is restricted to the current player, matching the raw fleet
loops. When supplied, the typed fleet visitor receives explicit aggregates for
all 16 ship slots and the raw groups consulted by the policy (`1`, `2..3`,
`6`, and `9..10`), while the legacy visitor remains available as an adapter.

The source probes pass the late-turn setup, research-order handoff, ordered
finalization, ownership/potency filtering, typed fleet-slot aggregation, and
dispatch paths plus the production-entry and four-channel-affordability checks
under strict host C89. A fresh VC++ 1.52c / Win98 checkpoint also passes all 31
native checks without a runtime error dialog. A separate packed integration
probe binds the recovered 56-byte `_planet` and 124-byte `_fleet` views,
including owner, flags, planet, and sixteen ship-count fields, and passes 20
host/native checks over the production, design-status, fleet-policy, and final
callback boundary. Planet service values, production costs, design selections,
and detailed order decisions remain explicit service results rather than
invented raw offsets. Complete colonizer and mineral fleet handling,
remaining packed order integration, and original-behavior evidence remain
open. The first-order repair branch is closed at its instruction-derived
boundary, as described below. The design-status service boundary also preserves the shared
sixteen-byte mask passed through all four `CheckAiShdefStatus` calls and into
`SplitOutShdefs`.

The current fleet boundary also carries `cord`, first-order kind, and an
optional packed `PLORD` view. Its host seam applies the shared kind-6/two-word
first-order repair for a cord-one/empty-kind input before the typed visitor and
can dispatch a surrounding `repair_order` callback. The raw C listing's
apparently contradictory `else` is a control-flow structuring artifact:
instruction listing `analysis/decompiler/export/raw/1098_01e0.asm:1235-129e`
confirms the `rgcsh[6] != 0`, `cord == 1`, empty-first-kind gate and
kind-6/two-word writes before `FLookupFleet`. The readable policy now uses
slot 6 rather than the broader any-ships aggregate. Repair runs in the
owned-fleet loop independently of typed, legacy, or absent visitor callbacks.
A three-order strict-C89 production-source probe and fresh VC++ 1.52c/Win98
replay both pass 22/22 checks, preserving `iordMac`, later orders, and all
bytes outside the three repaired words. The `C:\AUTRE3` native build and link
pass, process exits 0, and no Program Error appears. This closes Step-5 E2
first-order repair only;
see `analysis/compat/do-automitron-ai-turn-multi-order-repair-e2.{md,json}`
and `runs/vmctl-autre3/`. Complete raw route/order coverage and original-turn
differential evidence remain open. A separate typed route
plan/apply seam carries action, target, order kind, append mode, and reason
through the Automitron action families before the visitor. Strict host probes report
`AUTOMITRON_AI_REPAIR_BOUNDARY_CHECKS=8 FAILURES=0 RESULT=PASS` and
`AUTOMITRON_AI_ROUTE_PLAN_CHECKS=14 FAILURES=0 RESULT=PASS`; these are
supporting checkpoints only. Raw target predicates, complete packed-order
mutation, broader native replay, and original-turn differential evidence remain open.

The nine named route callbacks now have a single host matrix; it reports
`AUTOMITRON_AI_ACTION_MATRIX_CHECKS=20 FAILURES=0 RESULT=PASS` for action
mapping, plan payload preservation, owned-fleet filtering, and visitor order.
This remains host supporting evidence until native/original behavior is
validated.
