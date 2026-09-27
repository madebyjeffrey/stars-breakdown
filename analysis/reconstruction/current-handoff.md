# Reconstruction continuation handoff

Last updated: 2026-09-26

**Current status is maintained in [TODO7](../../TODO7.md) and the numbered
[EVIDENCE](../../EVIDENCE.md) index.** `SatisfyOrders` is promoted 6/6 after
the [Step-6 audit](../compat/satisfy-orders-step6-promotion.md). The older
121/152 and 122/152 counts in the checkpoints below predate later reopening
and closure work. Checkpoint headings and counts below are historical records,
not live status.

## Latest checkpoint — ExecuteReportClick E1 packed pointer tables

The production report-click dispatcher now resolves optional packed
`vlprgidFleet` rows through the `rglpfl` far-pointer table, and composes
starbase far pointers from the player's ship-definition table and the
packed planet design nibble times the original 0x93-byte stride. It
rejects missing table entries and preserves caller-supplied fallbacks
when the raw tables are unavailable. Strict-C89 and fresh VC++ 1.52c/
Win98 `C:\GPDP4` pass 30/30; build, process, and report pass without
Program Error. The full host suite passes 540 tests (153 expected skips).

These are instruction-derived formulas, not live original pointer values.
ER-E1 remains open pending original fleet/starbase/defense-part capture and
differential. Phase 2 stays **122/152 evidence units closed, 30 open,
10 promotions open, 40 practical items**. Evidence:
`analysis/compat/execute-report-click-pointer-tables-e1.{md,json}`;
returns: `runs/vmctl-gpdp4/`.

## Previous checkpoint — ExecuteReportClick E1 resource union

The original resource-cell popup writes `GlobalPD`'s first union word as
the selected planet ID, and its second as the resource/part alias. The
typed source had overwritten the ID with an unrelated caller-supplied
word. It now preserves the planet ID and derives the alias from resources;
the popup consumer's title lookup corroborates the original ownership.
Strict-C89 and fresh VC++ 1.52c/Win98 `C:\GPDE3` pass 20/20; build,
process, and report pass without Program Error. The full host suite passes
540 tests (153 expected skips).

This supports but does not close ER-E1: live far-pointer/part values across
other click kinds, USER/GDI, and full caller layout remain. Phase 2 stays
**122/152 evidence units closed, 30 open, 10 promotions open, 40 practical
items**. Evidence:
`analysis/compat/execute-report-click-resource-union-e1.{md,json}`;
returns: `runs/vmctl-gpde3/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 desirability gate

The original owned-planet loop skips production completely after a
negative `PctPlanetDesirability` result. The typed source previously marked
the bad world but could still enter an open production slot. It now carries
the result through the second pass and skips queueing and finalization for
that world. Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURDE1` pass 27/27,
including an open-slot negative-world regression. Build, process, and report
pass without Program Error. The full host suite passes 539 tests (153
expected skips).

This is Turin E1 supporting evidence, not closure. Live tables/services
and an original-turn differential remain. Phase 2 stays **122/152
evidence units closed, 30 open, 10 promotions open, 40 practical items**.
Evidence: `analysis/compat/do-turin-drone-ai-turn-desirability-e1.{md,json}`;
returns: `runs/vmctl-turde1/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 packed AI-planet writes

The production Turin source now optionally binds the original
16-byte-per-planet AI table and writes the three recovered second-pass
fields: universal byte `+9`, visible-unowned mineral score at `+1`, and
negative-owned desirability flag at `+2`. It prevalidates IDs and extents
before any table mutation. Strict-C89 and fresh VC++ 1.52c/Win98
`C:\TURAI1` pass 26/26; build, process, and report pass with no Program
Error. The full host suite passes 539 tests (153 expected skips).

This is Turin E1 supporting evidence, not a closure. Live table enumeration,
other writes and service effects, and original-turn comparison remain.
Phase 2 stays **122/152 evidence units closed, 30 open, 10 promotions open,
40 practical items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-ai-planet-e1.{md,json}`;
returns: `runs/vmctl-turai1/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 packed-service bridge

The production source now decodes recovered 56-byte planet, 124-byte fleet,
and variable-length PLPROD queue records, including the original category-2
and design-below-16 queued-ship gate. A default support-potency decision
implements the original `FPotentRobWarFleet(fleet,2)` slot-2–7 formula when
no external callback is supplied. Strict-C89 and fresh VC++ 1.52c/Win98
`C:\TURPK1` pass 17/17 for packed widths, queue classification/truncation,
and decoded fleet/planet data flowing through support queue and
finalization. Compile/link/process pass, no Program Error appears. The
full host suite passes 539 tests (153 expected skips), 2,241 layout
assertions, and 846 declarations.

This supports but does not close Turin E1: live VM AI/design/production
table enumeration, growth/resource and queue/finalization service effects,
remaining design inputs, and original-turn comparison remain. Phase 2 stays
**122/152 evidence units closed, 30 open, 10 promotions open, 40 practical
items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-packed-service-e1.{md,json}`;
returns: `runs/vmctl-turpk1/`; package:
`compat/vc152/turin-packed-service-e1/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 support-fleet production

The Turin support-fleet branch at original raw lines 328–352 uses the
selected group-13–14 design and the first local owned fleet for which
`FPotentRobWarFleet(fleet,2)` is nonzero. A slot-13+14 count at or above
the support-potency byte queues `(design,4,2,1)` and goes directly to
`FinishProduction(changed)`, skipping the ordinary hull loops. A weak first
potent fleet stops the search; a later stronger one is not used. The typed
source now models this with an explicit potent-fleet service callback and
the full queue request. Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURSP1`
pass 23/23; compile/link/process pass and no Program Error appears. The
full host suite passes 538 tests (153 expected skips), 2,241 layout
assertions, and 846 declarations.

This remains E1 support only: the actual potent-fleet service, live AI/
design/production tables, preceding growth/resource calls, actual queue/
finalization side effects, and original-turn comparison are not bound.
Phase 2 stays **122/152 evidence units closed, 30 open, 10 promotions open,
40 practical items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-support-e1.{md,json}`; returns:
`runs/vmctl-tursp1/`; package: `compat/vc152/turin-support-e1/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 design-1/minelayer production

The Turin source now performs the original first visible-unowned-world pass
before any owned-planet production. Design 1 queues four exact `(1,1,2,1)`
requests when the complete viable count is nonzero and its existing count
is below two. A full four-argument queue callback and signed 16-bit fleet
counts support the design-12 minelayer path: `Random(3)`, first qualifying
same-owner/same-planet fleet, optional `Random(8)` for 10–16 local hulls,
then `Random(2*cFr+1)` and `(12,3,2,1)` on zero. Strict-C89 and fresh
VC++ 1.52c/Win98 `C:\TURMI1` pass 32/32, including count 256 without
truncation. Compile/link and process exit pass; no Program Error window.
The full host suite passes 537 tests (153 expected skips), 2,241 layout
assertions, and 846 declarations.

This advances but does not close Turin E1. Second-pass optical/growth/
resource service effects, support-fleet choices and early finalization,
live packed table binding, actual queue/finalization side effects, and an
original-turn differential remain. Phase 2 stays **122/152 evidence units
closed, 30 open, 10 promotions open, 40 practical items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-minelayer-e1.{md,json}`; returns:
`runs/vmctl-turmi1/`; package: `compat/vc152/turin-minelayer-e1/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 group-8..9 technology queue

Original `CheckAiShdefStatus` returns the eligible group's existing ship
total, saturated at 32,000. Turin stores that scalar for designs 8–9 and,
at raw lines 280–294, compares it with the greater of twice an AI-data word
or one-tenth of the player's planet count. Technology byte 2 must exceed
four and a group-8..9 design must be selected. Below the primary target it
queues directly; below `target*10/7` it consumes `Random(4)` and queues only
on zero. The typed source now models that decision before later loops and
fails explicitly if the relevant queue or RNG callback is absent.

Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURTQ2` pass 21/21, including
threshold and callback-failure cases. Build/link and process exit pass;
no Program Error appears. One broad-suite run passes 536 tests (153 expected
skips), 2,241 layout assertions, and 846 declarations; two other attempts
exited 138 early, cause not established. This supports but does not close
Turin E1. Design-1 queueing, minelayer RNG/fleet scan, support choices,
full four-argument queue requests, live bindings, production side effects,
and original-turn comparison remain. Phase 2 stays **122/152 evidence units
closed, 30 open, 10 promotions open, 40 practical items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-tech-queue-e1.{md,json}`; returns:
`runs/vmctl-turtq2/`; package:
`compat/vc152/turin-tech-queue-e1-r2/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 seed/finalization milestone

The original Turin production path at raw lines 230–278 checks for an
existing ship queue before new construction, seeds design 0 on turn zero
with the 191 threshold and 100/30 decrements, and applies a separate
base-design count/built gate on later turns. The constructed path calls
`FinishProduction(changed)` at raw line 490. The readable source now models
these gates and the finalization call boundary. Strict-C89 and fresh VC++
1.52c/Win98 `C:\TURSF1` pass 21/21, including accepted-prefix queue failure
and finalization rejection. Build and link pass, the process exits 0, and
the post-run window list has no Program Error. The full host suite passes
535 tests (153 expected skips), 2,241 layout assertions, and 846 declarations.

This advances but does not close Turin E1. Technology/design-count and
RNG-gated items, support-fleet selection, live packed production/AI/design
tables, actual production-service side effects, and original-turn comparison
remain. Phase 2 stays **122/152 evidence units closed, 30 open, 10 promotions
open, 40 practical items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-seed-finish-e1.{md,json}`; returns:
`runs/vmctl-tursf1/`; package: `compat/vc152/turin-seed-finish-e1/`.

A read-only Terra handoff audited the next E1 branch without editing source
or using the VM. Raw lines 280–294 can consume `Random(4)` for the selected
8–9 design before the minelayer path at lines 308–327 consumes `Random(3)`,
sometimes `Random(8)`, then `Random(2*cFr+1)`. The latter scans for the first
same-owner, same-planet fleet with a positive slot-12 ship count and can
queue `AddItemToQueue(12,3,2,1)`. The present Turin queue callback retains
only design and quantity, so the four-argument request needs a richer typed
boundary; the typed fleet count is also only eight bits. Before implementing
this slice, reconcile the scalar returned by `CheckAiShdefStatus(8,9,...)`
at raw line 140 with the current Boolean callback contract. These are source
audit findings and the next implementation targets, not closed evidence.

## Previous checkpoint — DoTurinDroneAiTurn E1 four-role design queue milestone

Original status outputs map selected roles 3, 0, 7, and 5 to design groups
4–5, 6–7, 10–11, and 15. Their five-item loops run in that order with
distinct `cPlanMax`-based existing-count caps and a design-15 gate. The
readable source previously used role 0 for the first loop; it now uses role
3, accepts typed count/cost/table inputs, and carries queued resource costs
between all four loops. The strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURDQ1`
matrix passes 31/31 across ordered calls, cap boundaries, design-15
exclusion, partial stop, and resource carry. Build/link and process exit pass;
no Program Error appears. The full host suite passes 534 tests (153 expected
skips), 2,241 layout assertions, and 846 declarations.

This advances Turin E1 but does not close it. Turn-zero queue seeding,
technology/design-count and RNG-gated choices, support-fleet decisions,
`FinishProduction`, live table binding, and original-turn differential remain.
Phase 2 stays **122/152 evidence units closed, 30 open, 10 promotions open,
40 practical items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-design-queue-e1.{md,json}`; returns:
`runs/vmctl-turdq1/`; package: `compat/vc152/turin-design-queue-e1/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 per-item production milestone

The original Turin production loop at raw lines 353–385 debits four resource
channels and calls `AddItemToQueue(design, 1, 2, 1)` after each affordable
hull, up to five. A later shortage retains earlier queued items. The typed
source previously required all five to be affordable before one
quantity-five callback; it now issues per-item callbacks and records the
accepted prefix. Strict-C89 passes 11/11 across full and partial batches,
initial shortage, and callback rejection after two accepted items. The packed
integration and edge probes pass 20/20 and 7/7. Fresh VC++ 1.52c/Win98
`C:\TURPE1` compiles, links, exits 0, and reports 11/11 without Program Error.
The full host suite passes 533 tests (153 expected skips), 2,241 layout
assertions, and 846 declarations.

This advances Turin E1 but does not close it: other selected-design loops,
population/design-count gates, RNG-gated items, and `FinishProduction`
interaction remain. Phase 2 stays **122/152 evidence units closed, 30 open,
10 promotions open, 40 practical items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-partial-production-e1.{md,json}`;
returns: `runs/vmctl-turpe1/`; package:
`compat/vc152/turin-partial-production-e1/`.

## Previous checkpoint — DoTurinDroneAiTurn E1 repair subcheck

Original Turin `1088:5811–588A` reads the packed first-order kind, gates on
slot 12 and `cord == 1` after the preceding zero-slot tests, and writes only
kind 6 and two `0x0005` words. The readable source now uses packed PLORD
when available and executes repair in the owned-fleet loop regardless of
whether a typed visitor is installed. A strict-C89 matrix and fresh
VC++ 1.52c/Win98 `C:\TURRE1` replay both pass 18/18, including stale typed
kind values and exact three-order preservation; the process exits 0 and no
Program Error appears. The full host suite passes 532 tests (153 expected
skips), 2,241 layout assertions, and 846 declarations.

This is a supporting subcheck, not Turin E1 closure. The original production
phase still contains design selection, per-item queue writes, RNG gates, and
population/resource thresholds not represented by the current batch seam.
E2 routing, E3 broader PLORD mutation, E4 original-turn comparison, and
promotion remain. Phase 2 stays **122/152 evidence units closed, 30 open,
10 promotions open, 40 practical items**. Evidence:
`analysis/compat/do-turin-drone-ai-turn-multi-order-repair-e1.{md,json}`;
returns: `runs/vmctl-turre1/`; package:
`compat/vc152/turin-multi-order-repair-e1/`.

## Previous checkpoint — DoAutomitronAiTurn E2 multi-order repair closure

The original Automitron `1098:1225–129E` instructions require nonzero ship
slot 6, `cord == 1`, and an empty first-order kind. They write kind 6 and
two `0x0005` words to the first PLORD order without changing `iordMac` or
later orders. A follow-up audit moved repair out of the typed-visitor-only
branch to match that original loop. The strict-C89 production-source probe
passes 22/22 for exact three-order mutation, bypass cases, callback
order/failure, and typed/legacy/no-visitor execution. Fresh VC++ 1.52c/Win98
`C:\AUTRE3` compiles and links the current source, exits 0, and reports 22/22
with no Program Error. The full host suite passes 531 tests
(153 expected skips), 2,241 layout assertions, and 846 declarations.

This closes Automitron Step-5 E2 only. E1 colonizer/mineral fleet handling,
E3 route/broader packed-order integration, E4 original-turn behavior, and
Step-6 promotion remain. Phase 2 is **122/152 evidence units closed, 30 open,
10 promotions open, 40 practical items**; 41/51 procedures remain promoted.
Evidence: `analysis/compat/do-automitron-ai-turn-multi-order-repair-e2.{md,json}`;
returns: `runs/vmctl-autre3/`; package:
`compat/vc152/automitron-multi-order-repair-e2-r2/`. The prior 17-check
return in `runs/vmctl-autre2/` is retained.

## Previous checkpoint — DoRobotoidAiTurn E1 multi-order repair closure

The shared `stars_ai_repair_first_order` helper previously forced PLORD
`iordMac` to one. The original Robotoid instruction sequence at
`1088:130D–136F` writes only the first kind nibble and two repair words,
leaving the count and later orders intact. The analogous repair blocks in
Cyber, Automitron, Turin Drone, and Macinti have the same write boundary.
The corrected helper preserves the count; existing host repair/route probes
for all five personalities pass after their fixtures start with a valid
occupied first order. A new 14-check strict-C89 Robotoid probe tests exact
three-order mutation and route bypass, and fresh VC++ 1.52c/Win98 `C:\ROBRE1`
compiles, links, exits 0, and reports the same 14/14 with no Program Error.
The full host suite passes 530 tests (153 expected skips), 2,241 layout
assertions, and 846 declarations.
Artifacts are retained in `runs/vmctl-robre1/`; package:
`compat/vc152/robotoid-multi-order-repair-e1/`.

This closes Robotoid Step-5 E1 only; E2 exact routing, E3 broader packed
order mutation, E4 AI-table/original-turn differential, and Step-6 promotion
remain. The Phase 2 burn-down is **121/152 evidence units closed, 31 open,
10 promotions open, 41 practical items**; 41/51 procedures remain promoted.
Evidence: `analysis/compat/do-robotoid-ai-turn-multi-order-repair-e1.{md,json}`
and the updated `TODO6.md` scorecard.

## Previous checkpoint — SatisfyOrders E3 original batch and decoded replay

A verified clean 2400 snapshot and pinned original executable were copied to
isolated Win98 directory `C:\SYORIG1`, avoiding the user's live game. The
original batch run produced turn-1 HST/M1/M2 files despite process `EXIT 1`;
its backup files match the input hashes. The retained reconstructed file XOR
now decodes the output: M1's message, fleet, and order records match both
earlier original batch runs in plaintext, with variation limited to BOF salt
and one planet record. This is original-versus-original stability evidence,
not a production-source differential; SatisfyOrders E3 remains open. See
`analysis/compat/satisfy-orders-full-turn-e3-original-oracle.{md,json}` and
`runs/vmctl-syorig1/`.

## Previous checkpoint — Cyber AI multi-order packed PLORD mutation E12

E12 tests the packed mutation semantics from original `FMoveAiFleet` NE
`19:3D0A` with queues longer than two orders. The strict-C89 and fresh Win98
VC++ 1.52c reports pass 15/15 across append-to-third, last-endpoint
replacement, first-endpoint truncation, non-append replacement, and the
helper's full-capacity guard (not original `LpplReAlloc` integration). The
native process exits 0 without a Program Error.
The full host suite passes 526 tests (153 expected skips), 2,241 layout
assertions, and 846 declarations. Returns: `runs/vmctl-ai-plord-multi-e12/`;
package: `compat/vc152/ai-plord-multi-order-e12/`.

This is supporting evidence and closes no named Step-5 unit. DoCyberAiTurn
remains 2/4 at 5/6; raw personality predicates and complete AI-table/original-
turn evidence remain. Phase 2 remains 120/152 evidence units closed, 32 open,
10 promotions open, and 42 practical open items. Evidence:
`analysis/compat/ai-plord-multi-order-e12.{md,json}`.

## Previous checkpoint — FAttack complex-beam live hull damage E60

E60 extends the raw complex-beam live-target probe through shield and hull
damage: with a 408-point beam against two ships, 6 shield damage is absorbed,
both ships are killed at 10 hull DP each, the live count reaches zero, and the
remaining beam is consumed. Strict-C89 raw damage passes 15/15, related probes
pass, and the full suite passes 525 tests (153 expected skips), 2,241 layout
assertions, and 846 declarations. Fresh Win98 VC++ 1.52c build/link and all
three reports pass; the segment matches and no Program Error appears. Returns:
`runs/vmctl-fapraw-e60/`; package:
`compat/vc152/f-attack-raw-residual-e60/`.

This remains supporting evidence and closes no named Step-5 unit. FAttack is
0/3 at 5/6; complete raw service acceptance, broader original damage
differential, full native battle replay, and promotion remain. Project-wide
counters remain 120/152 evidence units closed, 32 open, 10 promotions open,
and 42 practical open items. Evidence:
`analysis/compat/f-attack-complex-beam-hull-e60.{md,json}`.

## Previous checkpoint — FAttack large-intermediate shield-ratio E59

E59 extends the E58 two-target shield-ratio case to `dpMain=408,000` using
original beam item 21 (`dp=204`), two components, and 1,000 attacker ships.
This exercises the production 64-bit multiply/divide path: the two observed
shield passes are `163200→163194` and `163193→163187`, reflecting the capped
residual update between passes. Strict-C89 and fresh VC++ 1.52c/Win98 probes
pass, the native guest has no Program Error, and the returned segment matches.
Return: `runs/vmctl-fapraw-e59/`; package:
`compat/vc152/f-attack-raw-residual-e59/`.

This is supporting evidence only; no Step-5 named unit closes. FAttack remains
0/3 and 5/6, with 32 evidence units and 10 promotions open project-wide.
Broader original damage differential, complete battle replay, and promotion
remain. Evidence: `analysis/compat/f-attack-complex-beam-residual-e59.{md,json}`.

## Previous checkpoint — FAttack complex-beam shield-ratio oracle E58

E58 extends E55's live two-target mode-priority regression with an exact
`dpMain` shield-ratio oracle. Original beam item 21 has `dp=204` (E34); with
two installed components and one attacker ship, the initial pool is 408. The
host and native probes observe target 1 first with damage 163→157 after
shield-only FDamageTok, giving `floor(408*157/163)=392`; mode-1 is then
ineligible, so mode 0 selects target 0 with damage 156→150. The residual path
terminates after the shields are depleted. Strict-C89 and fresh E58
VC++1.52c/Win98 reports pass, the guest has no Program Error dialog, and the
copied segment matches. Return: `runs/vmctl-fapraw-e58/`; package:
`compat/vc152/f-attack-raw-residual-e58/`.

This is still supporting evidence: no named Step-5 unit closes. FAttack stays
0/3 at 5/6; broader original damage differentials, full battle replay, and
promotion remain. Phase 2 counters remain 120/152 closed, 32 evidence units
open, 10 promotions open, and 42 practical open items. Evidence:
`analysis/compat/f-attack-complex-beam-residual-e58.{md,json}`.

The full host suite after E58 passes 525 tests, 153 expected skips, 2,241
layout assertions, and 846 declarations.

## Previous checkpoint — FAttack raw complex-beam residual rescan E55

The E54 API limitation is addressed for the raw complex group-`0x10` beam
path. `StarsAttackOps.read_target` provides a post-callback live-target read,
and `StarsAttackInput.attacker_ship_count` supplies the `_tok.csh` factor. The
new residual loop seeds its pool with low-32-bit
`raw_part_dp * part_count * attacker_ship_count`, selects mode 1 before mode
0, then rescans through the live-state callback after each raw-damage call. The
residual update uses the observed `min(dpMain - 1, dpMain * dp / dpT)` shape.
The legacy typed/fixed-pass path remains as a fallback when raw inputs are not
available.

Strict-C89 raw-damage/rescan, beam-scaling, packed-part, and torpedo-dispatch
tests pass. Fresh native package E57 compiles and links with VC++ 1.52c in
Win98. The packed-part report passes 24/24, raw torpedo dispatch 17/17, and the
12-case raw damage/rescan report passes mode-0 fallback after live shield
depletion. All processes exit 0, the copied NE segment matches, and the guest
window inventory has no Program Error. Returns are preserved in
`runs/vmctl-fapraw-e57/`, package in
`compat/vc152/f-attack-raw-residual-e57/`. E56's earlier 11-case pass and
E55's wrapper-link failure are retained separately.

The post-change host suite passes 525 tests, 153 expected skips, 2,241 layout
assertions, and 846 declarations; package/JSON and diff checks pass as well.

This closes **0 named Step-5 units**: FAttack remains 0/3 named units and 5/6
overall; TODO6 remains 120/152 closed, 32 evidence units open, 10 promotions
open, and 42 practical open work items. A fuller retarget sequence, an
end-to-end original damage differential, broader damage acceptance, full
native battle replay, and promotion remain. Evidence:
`analysis/compat/f-attack-complex-beam-residual-e55.{md,json}`.

## Previous checkpoint — FAttack `dpMain` arithmetic and callback alias audit E53

E53 traces more of the original attack loop. `dpMain` is initialized on the
weapon-kind `0x10` path as the low-32-bit product of selected-part
`_beam.dp` at offset `0x36`, installed component count, and attacker
`_tok.csh` at offset `0x13`; the count is read from the current hull slot
although CodeView names its temporary `dxRangeCur`. The alternate path zeros
it. The raw decompiler's `_hul *` /
`wtFuelMax` spelling is an overlapping-type alias for this group-`0x10`
part access. In the later
target loop, the integer and x87 paths produce a
quotient `dpMain * dp / dpT`, capped at `dpMain - 1`. CodeView maps `dp` and
`dpT` to the operands at BP `-0x66` and `-0x58` hexadecimal. `dpT` snapshots
`dp` immediately before shield-only `FDamageTok`; the call mutates `dp`, so
the ratio is the remaining beam damage over the pre-shield value.
`cTorpsLeft` is the separate local at BP `-0x3a`; do not conflate it with
`dpT`. The BP `-0x74` base-pointer type for offset `0x36` and end-to-end
original behavior still need verification before implementing the calculation.

At the hull `FDamageTok` call, `&cTorpsLeft` is passed as `pcTorp`. The
original damage routine can subtract casualties from that pointed long with
a zero floor; after return, `FAttack` separately subtracts the selected split
terms. The readable source does not yet represent this proven mutation order
or independent `dpMain` state. No source behavior changed and no named Step-5
unit closes. FAttack remains 0/3 named Step-5 units and 5/6 overall; TODO6
remains 120/152 closed, 32 open, 10 promotion items, and 42 practical open
work items. Evidence: `analysis/compat/f-attack-dpmain-loop-arithmetic-e53.{md,json}`.

Next model `dpMain` and the shield ratio explicitly, then add
original-executable cases for the shield-ratio update and callback-plus-caller
residual mutation.

## Previous checkpoint — FAttack torpedo loop condition raw branch audit E52

The retained original assembly plus CodeView locals resolves the FAttack
loop condition to two named 4-byte `long` locals. At
`10f0:7c84–7cb2`, the group-0x20 path repeats at `10f0:6e04` if either signed
`dpMain` (BP -36 decimal) or `cTorpsLeft` (BP -58 decimal) is positive. The
second is the remaining-torpedo quantity, initialized on the non-`0x10`
weapon-kind path and decremented by the selected split terms. `dpMain` is
computed on the `0x10` path, zeroed on the alternate path, and adjusted during
target selection; its full update semantics remain unresolved. The raw
decompiler variables `lVar19` / `uStack_26` / `lVar16` do not match the machine
stack slots at the branch, so they are not a safe source mapping.

The readable source still uses `remaining_packets > 0` plus a
`made_progress` guard; it has not been changed because `dpMain`'s full
calculation and its interactions with callback mutations are not yet mapped
to source state.
The two focused host regressions pass, but this instruction audit closes **0
named Step-5 units**. FAttack remains 0/3 named Step-5 units and 5/6 overall;
TODO6 counters remain 120/152 evidence units closed, 32 open, 10 promotion
units, and 42 practical open work items. Evidence:
`analysis/compat/f-attack-outer-loop-assembly-e52.{md,json}`.

## Previous checkpoint — FAttack packed raw-part resolver native integration E51

Fresh package `compat/vc152/f-attack-packed-record-native-e51/` compiles and
links with VC++ 1.52c in the running Win98 guest. Packed FAttack exercises the
E47 `stars_lookup_part_record` resolver over the original unrelocated NE
segment-2 image and reports `F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0
RESULT=PASS`; the companion raw torpedo record/residual executable reports
`F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=0 RESULT=PASS`. Both exit 0.
The guest window inventory after execution has no Program Error dialog. The
returned `SEG2.BIN` matches the package byte-for-byte, SHA-256
`52b2fb4ebc7c8ea4527b2d277fcaf3218c916053d9e295e33900cc637a753698`. Reports,
compiler/linker logs, executables, maps, objects, screenshot, segment, and
agent log are preserved in `runs/vmctl-fapne51/`. Both focused host tests and
`git diff --check` pass; full-suite evidence remains E47's 524 tests, 153
expected skips, 2,241 layout assertions, and 846 declarations.

This proves native execution over copied static original bytes and scripted
services, not a live Stars! CS mapping, complete lookup race/hidden/technology
services, original-executable damage differential, or whole-battle replay. It
closes **0 named Step-5 units**; `FAttack` remains 0/3 named Step-5 units and
5/6 overall. TODO6 counters remain 120/152 evidence units closed, 32 open, 10
promotion units, and 42 practical open work items. Evidence:
`analysis/compat/f-attack-packed-record-native-e51.{md,json}`.

## Previous checkpoint — FAttack raw torpedo record and residual sequence E46

The multi-pass/residual probe now reads group-0x20 item 8 from the original
segment-2 record at `0x2360` (`range=5`, `dp=85`, `init=0`). With its scripted
hit sequence, native dispatch reports damage 85 then 42, preserves the
target/callback flags/hit count, and consumes residual packet state from one
to zero. Host result is
`F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=0 RESULT=PASS`.

Fresh E46 Win98/VC++ 1.52c compile/link pass. The packed-record executable
passes 24/24 and the raw torpedo record/residual executable passes 17/17;
both exit 0 and the post-run screen has no Program Error. Full host suite
passes 524 tests, 153 expected skips, 2,241 layout assertions, and 846
declarations; baseline and diff checks pass. Evidence:
`analysis/compat/f-attack-torpedo-record-residual-native-e46.{md,json}`,
`compat/vc152/f-attack-packed-record-native-e46/`, and
`runs/vmctl-vc152fapne46/`.

This uses reconstructed code over a copied static segment and scripted random
inputs; it is not a full original-executable differential or whole-battle
replay. No named Step-5 unit closes; `FAttack` remains 0/3 at Step 5 and 5/6
overall. Broader damage differentials and complete native battle replay
remain.

## Previous checkpoint — FAttack original beam and torpedo record integration E45

The raw part-record callback is now exercised for both weapon groups. The
strict-C89 regression loads the original item-7 beam and item-4 torpedo
records. The torpedo's original `range=5`, `dp=90`, and `init=2` produce 45
hull damage for one guaranteed hit; changing `dp` to 120 produces 60, while
range zero or initiative one blocks dispatch. The test uses production
`c_torp_hit.c` and passes 24/24 checks.

Fresh `C:\FAPNE45` VC++ 1.52c/Win98 compile/link pass; native report is
`F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0 RESULT=PASS`, process exit is 0,
and the post-run screen has no Program Error. E43's wrapper-path and E44's
stale report-count attempts are preserved but not counted. Evidence:
`analysis/compat/f-attack-original-part-record-native-e45.{md,json}`,
`compat/vc152/f-attack-packed-record-native-e45/`, and
`runs/vmctl-vc152fapne45/`.
The full host suite passes 524 tests, 153 expected skips, 2,241 layout
assertions, and 846 declarations; baseline and diff checks pass.

This is stronger binding to copied original static bytes, not live Stars! CS
mapping or full battle replay. No named Step-5 unit closes; `FAttack` remains
0/3 at Step 5 and 5/6 overall. Broader original damage differentials and
complete native battle replay remain.

## Previous checkpoint — FAttack original part-record integration E42

`stars_f_attack` now has an optional part-record resolver and decodes the
selected beam/torpedo's original range, damage-points, and initiative fields
from offsets 52, 54, and 56 of the returned record. The host probe loads the
unrelocated NE segment-2 image from `target/stars.exe`, verifies the ordinary
path, mutates each field to prove it affects dispatch, and rejects a 57-byte
record. It passes `F_ATTACK_PACKED_PART_CHECKS=20 FAILURES=0 RESULT=PASS`.

Fresh E42 VC++ 1.52c/Win98 compile/link pass and the executable returns the
same 20/20 report with process exit 0. The returned screen has no Program
Error dialog. E40's missing helper symbol and E41's duplicate stub are
preserved as failed attempts. Evidence:
`analysis/compat/f-attack-original-part-record-native-e42.{md,json}`,
`compat/vc152/f-attack-packed-record-native-e42/`, and
`runs/vmctl-vc152fapne42/`.
The full host suite passes 524 tests, 153 expected skips, 2,241 layout
assertions, and 846 declarations; baseline and diff checks pass.

This binds the semantic FAttack path to copied original static bytes; it does
not prove a live Stars! process's CS mapping, complete `FLookupPart` services,
whole-battle behavior, or full native replay. No named Step-5 unit closes.
`FAttack` remains 0/3 at Step 5 and 5/6 overall. The next evidence gate is a
broader original damage differential and complete native battle replay.

## Previous checkpoint — FLookupPart original-segment record binding E47

`stars_lookup_part_record` now validates the recovered current-CS marker,
family base/stride, computed item address, and complete record extent before
returning a borrowed pointer into a caller-supplied segment image. The strict
host-C89 probe uses the original unrelocated NE segment-2 bytes extracted from
`target/stars.exe` (input SHA-256
`ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59`). It
passes 129 checks: all 16 family record starts; all 24 beam and 12 torpedo
records' `dRangeMax`, `dp`, and `init` words; truncated-span and forged
metadata rejection. The targeted packed-FAttack regression also passes. Full
suite (`PYTHONMALLOC=malloc python3 -m unittest discover -s tests -q`): 524
tests passed, 153 expected skips, 2,241 layout assertions, and 846 declarations;
baseline and `git diff --check` pass.

Fresh VC++ 1.52c packet E50 compiles/links and returns the same 129/129 report
in Win98. The returned `SEG2.BIN` is byte-identical to the packaged original
segment. The first E49 build failed at the linker because its response file
omitted the explicit map-file argument; that failed log/object/segment return
is preserved in `runs/vmctl-faptae49/`, and E50 corrects the argument list.
Passing native evidence is in `compat/vc152/f-lookup-part-memory-native-e50/`
and `runs/vmctl-faptae50/`.

This is binding to original static executable bytes, **not** live Win98 CS
mapping evidence. E50 exercises the resolver against a copied segment image,
not a running Stars! process's CS. Next wire borrowed records into packed
FAttack's weapon service. No named
Step-5 unit is closed; complete lookup gates, attack acceptance/differential,
full replay, and promotion remain open. Evidence:
`analysis/compat/f-attack-original-segment-binding-e47.{md,json}`.

## Previous checkpoint — FLookupPart current-CS table pointers E46

Raw disassembly confirms all sixteen valid `FLookupPart` families compute a
literal family base/stride and store current CS into the returned far pointer.
Since `FLookupPart` is NE `02:524E`, its tables are CS-relative to NE segment 2.
Ghidra selector `0x10e0` is synthetic NE29, not DGROUP or the part-table
selector. CodeView globals with matching offsets were misleading overlays.
The readable source now marks every family as current-CS and checks its exact
CS-relative base, stride, and selected-item offset against the instruction
immediates. ZIP's byte stride is `0x36`.

The strict-C89 table probe covers all 16 valid groups plus an unsupported
group and passes `LOOKUP_PART_TABLE_CHECKS=66 FAILURES=0 RESULT=PASS`; the
separate source-gate probe passes `LOOKUP_PART_CHECKS=8`. Fresh
`C:\FAPTA48` VC++ 1.52c/Win98 compile/link pass and the native report passes
66/66; process exit is 0 and no Program Error appears. Full suite: 523 tests,
153 expected skips, 2,241 layout assertions, and 846 declarations; baseline
and `git diff --check` pass. The packed-FAttack regression now asserts current
CS and zero literal segment (the first full-suite attempt found its stale
`0x10e0` assertion; targeted test and subsequent full suite pass). The E44
ZIP-stride and E45 selector/overlay
interpretations are superseded; their reports and VM returns remain preserved
but are not counted as original-fidelity evidence. Earlier failed `_printf`
build attempts are preserved separately. Evidence:
`analysis/compat/f-attack-current-cs-tables-e46.{md,json}` and
`runs/vmctl-faptae48/`.

This closes **0 named Step-5 units**: exact current-CS-relative offsets are
known, but they still need binding to live part-table bytes; complete `FLookupPart`
race/visibility/technology services, FAttack damage acceptance, original
differential, full replay, and promotion remain open.

## Previous checkpoint — FLookupPart absolute-offset model E45 (superseded)

E45 resolved same-offset CodeView names in DGROUP and treated `0x10e0` as the
table selector. Raw assembly shows `MOV DX,CS` for every family; selector
`0x10e0` identifies NE29 in Ghidra's synthetic map. The numeric offsets were
consistent with NE2 table bases, but the selector/anchor explanation was not.
E46 replaces that inference with instruction-derived current-CS addressing.
E45 is retained for history but is not source-fidelity evidence.

## Previous checkpoint — FLookupPart live-anchor schema E44 (superseded)

E44's native 50/50 report modeled ZIP as `0x1b` bytes per item. Raw machine
code uses stride `0x36`. E45 corrected that numeric stride but assigned it the
wrong selector; E46 supersedes both address interpretations.

## Previous checkpoint — FAttack raw DGROUP hull far-pointer tables E43

Added `stars_lpshdef_from_tok_globals` for the original raw DGROUP arrays:
`rglpshdef` at NE `0025:00FE` and `rglpshdefSB` at `0025:014C`. Each contains
sixteen four-byte far pointers encoded offset-word then segment-word. The
selector applies the original design partition and `0x93` record stride, then
passes the resulting segment:offset to an explicit resolver. The production
FAttack adapter composes this with token context, `_hul.rghs[16]`/`chs` and the
real attack callback.

The integrated native/host cases resolve alternate player-1/design-`0x12` to
`1234:2126` and normal player-3/design-5 to `2345:12DF`; the alternate hull's
raw part reaches lookup/damage. Strict-C89 and fresh `C:\FCBATE43` VC++
1.52c/Win98 reports pass 44/44; build/link pass, process exits 0, and no
Program Error appears. The complete suite passes 523 tests, 153 expected
skips, 2,241 layout assertions, and 846 declarations; baseline/diff pass.
Evidence: `analysis/compat/f-attack-dgroup-hull-far-tables-e43.{md,json}` and
`runs/vmctl-fcbate43/`.

This closes **0 named Step-5 units**. Raw global table selection is exercised,
but the final segmented-address resolver remains a service; this does not
claim linkage to the complete game's live DGROUP/hull memory. Full native
`FLookupPart`, damage-service acceptance, original differential, battle replay,
and promotion remain.

## Previous checkpoint — FAttack design-to-hull normal/alternate table branches E42

Extended E41's design/hull callback to validate both branches of the original
`LpshdefFromTok` selector. The integrated battle callback selects player 1's
alternate table for design `0x12` (index 2); the selected 123-byte hull's
`rghs[16]` at offset 58 and `chs` at offset 122 feed raw slot lookup and damage.
A second adapter case selects player 3's normal table for design 5 (index 5)
and verifies the zero-slot hull path. Strict-C89 host and fresh
`C:\FCBATE42` VC++ 1.52c/Win98 reports pass 44/44, build/link pass, process
exits 0, and the post-run guest has no Program Error. The full suite passes
523 tests, 153 expected skips, 2,241 layout assertions, and 846 declarations;
baseline/diff checks pass. Evidence:
`analysis/compat/f-attack-design-hull-table-branches-e42.{md,json}` and
`runs/vmctl-fcbate42/`.

This closes **0 named Step-5 units**. FAttack remains 0/3 at Step 5 and 5/6
overall. Exact native per-player table globals/far pointers, complete native
part services, beam/torpedo damage acceptance, original differential, full
battle replay, and promotion remain.

## Previous checkpoint — FAttack token design-to-hull slot resolution E41

Added `stars_attack_bind_cool_battle_design_hull`. It derives the original
token player/design bytes from the `FDoCoolBattle` token, calls the existing
`stars_lpshdef_from_tok` branch, then reads the selected 123-byte `_hul` record:
the 16 inline four-byte `_hs` entries start at byte 58 and active `chs` is at
byte 122. Counts beyond the fixed slot array are rejected before binding into
FAttack. The selected table-view callback remains explicit; this does not
claim native `_tok`/`vrgtok` globals or original far-table addresses.

The callback integration selects player 1's alternate design table for design
byte `0x12` (index 2), then verifies the returned hull's raw group `0x10`, item
7, count 2 overrides contradictory typed section metadata through lookup and
damage. Host and fresh `C:\FCBATE41` VC++ 1.52c/Win98 reports pass 38/38;
compile/link pass, process exits 0, and no Program Error appears. Full suite:
523 tests, 153 expected skips, 2,241 layout assertions, 846 declarations;
baseline/diff pass. Evidence:
`analysis/compat/f-attack-design-hull-resolution-e41.{md,json}` and
`runs/vmctl-fcbate41/`.

This closes **0 named Step-5 units**. FAttack remains 0/3 at Step 5 and 5/6
overall. Actual native per-player far-table binding, complete native
`FLookupPart` table/service/race/visibility, raw damage acceptance, original
differential, full battle replay, and promotion remain.

## Previous checkpoint — FAttack battle callback raw hull-slot binding E40

`stars_attack_bind_cool_battle_hull` composes E38's token context adapter
with E39's raw four-byte `_hs` decoder. The `FDoCoolBattle` → attack callback
probe supplies raw group `0x10`, item 7, count 2 while deliberately providing
contradictory typed group `0x20`, item 4, count 1. The composed adapter feeds
the decoded values to the actual FAttack lookup and damage path; target
mutation, battle lifecycle, reporting, and cleanup are also checked. The
callback still owns selecting the actual design/hull and supplying its raw
slot source; no `LpshdefFromTok` or native global binding is claimed.

Strict-C89 host and fresh `C:\FCBATE40` VC++ 1.52c/Win98 report pass 33/33;
compile/link pass, process exits 0, and there is no Program Error dialog. The
full suite passes 523 tests, 153 expected skips, 2,241 layout assertions, and
846 declarations; baseline/diff checks pass. Evidence is in
`analysis/compat/f-attack-cool-battle-hull-e40.{md,json}` and
`runs/vmctl-fcbate40/`.

This is stronger supporting evidence but closes **0 named Step-5 units** for
FAttack or FDoCoolBattle. FAttack remains 0/3 at Step 5 and 5/6 overall; its
callback-owned design-to-hull selection, complete `FLookupPart` table/service/
race/visibility behavior, beam/torpedo damage-service acceptance, original
differential, full battle replay, and promotion remain. FDoCoolBattle still
needs exact native `_tok`/`vrgtok` globals and callers, full behavior replay,
and original differential.

## Previous checkpoint — FAttack raw `_hs` slot decoding bridge E39

Added `stars_attack_bind_raw_hull_slots`, which explicitly decodes each
four-byte little-endian `_hs` record (group word, item byte, count byte) into
FAttack's packed-part input. It validates source stride and destination
capacity without type-punning packed storage. The updated packed-part probe
starts from raw bytes and verifies that they override contradictory typed
section metadata; invalid item, zero count, bad stride, and short destination
cases are included.

Host and fresh Win98 reports pass packed-part 16/16, beam scale 86/86, raw
damage 9/9, torpedo target 5/5, and raw torpedo dispatch 17/17. Compile/link
pass; processes exit 0 and the post-run window inventory has no Program Error.
The full verbose suite passes 523 tests, 153 expected skips, 2,241 layout
assertions, and 846 declarations; baseline and diff checks pass. The packet and
return files are retained in `compat/vc152/f-attack-complex-native-e39/` and
`runs/vmctl-vc152fapne39/`. Evidence:
`analysis/compat/f-attack-raw-hs-slot-bridge-e39.{md,json}`.

This closes **0 Step-5 units**: the adapter is not yet connected to the actual
battle callback's packed hull-slot source. FAttack remains 0/3 at Step 5 and
5/6 overall. Complete native `FLookupPart` table/service and race/visibility
binding, integrated attacker/target/design-to-hull resolution, raw beam and
torpedo damage-service acceptance, E2 original differentials, E3 complete
battle replay, and promotion remain.

## Previous checkpoint — FAttack context from packed battle token E38

Added `stars_attack_bind_cool_battle_token`, a production adapter for the
FDoCoolBattle per-token attack callback. It transfers token index, player,
raw `grobj`, battle coordinate, current initiative, and survivor mask into
`StarsAttackInput`; section, target, packed-hull, and service resolution remain
callback-owned. The E38 integration enters through the actual 29-byte
packed-token load/store bridge and validates these fields through the FAttack
callback: strict-C89 host and fresh native Win98 report 29/29. VC++ 1.52c
compile/link pass, process exits 0, and no Program Error appears. Full verbose
suite passes 523 tests, 153 expected skips, 2,241 layout assertions, and 846
declarations; baseline/diff checks pass. The initial packet's missing-header
failure is preserved separately from the passing run. Evidence:
`analysis/compat/f-attack-cool-battle-context-e38.{md,json}` and
`runs/vmctl-vc152fcbate4-success/`.

This closes **0 Step-5 units** for FAttack or FDoCoolBattle. FAttack remains
0/3 at Step 5 and 5/6 overall; complete packed weapon/hull/target and
damage-service acceptance, E2 broader differentials, E3 full battle replay,
and promotion remain. FDoCoolBattle still needs exact native `_tok`/`vrgtok`
global/caller binding and its complete original battle differential.

## Previous checkpoint — FAttack original group-0x20 torpedo initiative/range E37

E37 recovers all twelve original `_torp.init` and `_torp.dRangeMax` fields
(record offsets 56 and 52) from `rgtorp` at NE segment 2:`0x2180`, then binds
them into the successful packed group-0x20 FAttack path. The initiative comes
from `initBase + part.init`; range uses the raw inclusive distance comparison
and the `grobj == 1` allowance. Host integration verifies item 9 initiative 4,
distance 6 admitted, and distance 7 rejected; dispatch matrix 17/17. Fresh
`C:\\FAPNE37` VC++ 1.52c compile/link and native reports pass: beam 86/86,
packed part 13/13, raw damage 9/9, torpedo target 5/5, and torpedo dispatch
17/17. Processes exit 0; no Program Error dialog. Full verbose suite passes
522 tests with 153 expected skips, 2,241 layout assertions, and 846
declarations; baseline/diff checks pass. Evidence:
`analysis/compat/f-attack-torpedo-fields-e37.{md,json}` and
`runs/vmctl-vc152fapne37/`.

E37 is supporting evidence only: FAttack stays 0/3 at Step 5 and 5/6 overall.
Complete packed battle-token/object-kind mapping and beam/torpedo service
acceptance (including hull/resource/shield/jam/capacity), E2 original damage
differentials, E3 full battle replay, and Step-6 promotion remain.

## Previous checkpoint — FAttack original group-0x10 beam range gate E36

Recovered all 24 original `_beam.dRangeMax` words (record byte 52) from NE
segment 2:`0x2450`. Raw FAttack computes the allowed range as
`DzFromBrcBrc(...) <= (grobj == 1) + dRangeMax`; packed group-0x10 source now
uses that inclusive coordinate gate and exposes the attacker object kind in
`StarsAttackInput`. Host checks all `dRangeMax`, `dp`, and `init` entries,
invalid item, scaling cases, and range edges (86/86). A planet-kind attacker
gets the raw +1 allowance; a fleet-kind attacker does not.

Fresh `C:\\FAPNE36` VC++ 1.52c compile/link passes. Native reports pass beam
86/86, packed part 13/13, raw damage 9/9, torpedo target 5/5, and torpedo
dispatch 11/11; tested processes exit 0, with no warnings or Program Error.
Full verbose suite passes 521 tests with 153 expected skips, 2,241 layout
assertions, and 846 declarations; baseline and diff checks pass.

This is supporting evidence only and closes **0 named Step-5 units**. FAttack
remains 0/3 at Step 5 and 5/6 overall. Complete packed battle-token/object-kind
mapping and beam/torpedo service acceptance, E2 original damage differentials,
E3 full-battle replay, and Step-6 promotion remain. Evidence:
`analysis/compat/f-attack-beam-range-e36.{md,json}` and
`runs/vmctl-vc152fapne36/`. The earlier full-suite quiet-mode signal-10
intermittent remains documented, despite successful full verbose runs.

## Previous checkpoint — FAttack original group-0x10 beam initiative binding E35

The original `_beam` table has `dRangeMax` at byte 52, `dp` at byte 54, and
`init` at byte 56 in its 60-byte record. E34 wired `dp` into FAttack; E35
adds all 24 executable `init` values from NE segment 2:`0x2450` and makes
packed group-0x10 FAttack replace the resolver's `init_delta` with the actual
part initiative. The raw unsigned `initBase + part.init` then feeds the
existing cap/initiative comparison. Host table/scaling test: 58/58; host
packed initiative integration: 13/13; raw damage: 9/9.

Fresh `C:\\FAPNE35` VC++ 1.52c compile/link and Win98 replay pass: native
beam/table 58/58, packed-part 13/13, raw damage 9/9, torpedo target 5/5, and
torpedo dispatch 11/11. Tested processes exit 0, compiler logs are warning-
free, and no Program Error appears. The full verbose suite passes 521 tests,
153 expected skips, 2,241 layout assertions, and 846 declarations; baseline
and diff checks pass. Two previous full quiet-mode runs exited on signal 10
before summary, although a verbose full run and isolated common-source test
pass; this intermittent remains under observation.

E35 is supporting evidence only and closes **0 named Step-5 units**. FAttack
remains 0/3 at Step 5 and 5/6 overall. Complete packed beam/torpedo field and
service acceptance (including range and hull/resource/shield/jam/capacity),
E2 original damage differentials, E3 full-battle replay, and Step-6 promotion
remain. Evidence is in `analysis/compat/f-attack-beam-table-init-e35.{md,json}`
and `runs/vmctl-vc152fapne35/`.

## Previous checkpoint — FAttack original group-0x10 beam table integration E34

Recovered all 24 original `_beam.dp` values from NE segment 2:`0x2450` using
the CodeView `_beam` record size (60 bytes) and `dp` field offset (54). The
readable `stars_lookup_beam_dp` now supplies FAttack's group-0x10 raw part
damage after successful packed lookup, rather than trusting resolver data.
The strict-C89 host beam probe checks the full table, invalid item, and eight
scaling/branch cases (33/33); the raw damage integration checks table-driven
damage classes with a deliberately incorrect resolver value (9/9). Fresh
`C:\\FAPNE34` VC++ 1.52c compile/link passes, as do native beam/table (33/33),
raw damage (9/9), torpedo target (5/5), and raw torpedo dispatch (11/11).
Full host suite passes 520 tests with 153 expected skips, 2,241 layout
assertions, and 846 declarations. Baseline and scoped diff checks pass.
The full verbose discovery is the passing regression evidence; two full
quiet-mode invocations exited on signal 10 before reporting results, while
`test_common_source -q` passes 221/221 by itself. Keep that runner-level
intermittent visible until localized.

This is supporting evidence only and closes **0 named Step-5 units**. FAttack
remains 0/3 at Step 5 and 5/6 overall; complete E1 beam/torpedo acceptance,
E2 broader original damage differentials, E3 full-battle replay, and Step-6
promotion remain open. The failed first build and raw expectation are retained
alongside the final successful run evidence in `runs/vmctl-vc152fapne34/`.
See `analysis/compat/f-attack-beam-table-e34.{md,json}`.

## Previous checkpoint — FAttack group-0x10 broadcast falloff correction E33

Raw original group-0x10 broadcast/simple beams apply attacker `pctCap` and
target `pctBeamDef` but not the complex-beam distance-loss multiplier. The
readable implementation had incorrectly shared that falloff. It now enables
distance loss only for complex target selection. Eight host/native checks
include a paired case with identical values yielding 39 complex-path damage
versus 40 broadcast-path damage. The fresh `C:\FAPNE33` VC++ 1.52c build and
link pass; the report passes, process exits 0, and the post-run window list is
free of Program Error. This corrects source but closes no named Step-5 unit:
FAttack remains 0/3 at Step 5 and 5/6 overall. E1 complete raw beam/torpedo
acceptance, E2 broader original damage differentials, E3 full-battle replay,
and promotion remain. Evidence is in
`analysis/compat/f-attack-beam-broadcast-e33.{md,json}` and
`runs/vmctl-vc152fapne33/`.

## Previous checkpoint — FAttack group-0x10 beam scale host probe E32

The strict-C89 `tests/f_attack_beam_scale_probe.c` now exercises seven
production-path beam cases derived from the raw FAttack instruction sequence:
nonzero/zero attacker `pctCap`, target `pctBeamDef`, truncating distance
falloff, the raw `dp == 200` damage-class boundary, zero effective damage,
and falloff above 100 percent. Host and fresh VC++ 1.52c/Win98 validation pass;
the native process exits 0 with no Program Error dialog. This is supporting
evidence only and closes no named Step-5 unit. Complete E1 beam/torpedo scaling
and table-service acceptance, E2 broader original damage differentials, E3
full-battle replay, and Step-6 promotion remain open. FAttack remains 0/3 named
Step-5 units and 5/6 overall. Evidence is in
`analysis/compat/f-attack-beam-scaling-e32.{md,json}`.

## Previous checkpoint — FAttack original group-0x20 torpedo table E29/E30/E31

The group-0x20 torpedo lookup is now backed by original executable data.
`FLookupPart`'s accepted disassembly forms `CS:(0x2180 + item*0x3c)`;
CodeView places `rgtorp` at NE 2:`0x2180`, and `_torp` has size 60 with `dp`
at byte 54. The twelve extracted original `dp` values are
`5, 12, 26, 48, 90, 169, 316, 60, 85, 150, 280, 525`. This corrects E22's
mistaken DGROUP/string-overlay inference: it mixed a same-offset string from
NE autodata segment 37 with the segment-2 torpedo table.

`stars_lookup_torpedo_dp` now supplies those values to the FAttack group-0x20
path after successful packed-part resolution. The host dispatch probe checks
all twelve entries and proves item 8 dispatches with `dp=85`, replacing the
fixture value 16. Fresh E29 VC++ 1.52c/Win98 passes compile/link and raw damage
9/9, torpedo target 5/5, raw torpedo dispatch 11/11, packed part 13/13, lookup
metadata 34/34, complex score 9/9, and complex selection 3/3. E30 separately
replays the current `tests/f_attack_source_probe.c` with current production
FAttack/lookup sources and passes 34/34. Both processes exit 0 with no Program
Error dialog. E31 corrects metadata to explicitly mark group 0x20's current-CS
selector (rather than a literal 0x10e0 value); the fresh native lookup matrix
passes 38/38, with raw damage 9/9, target 5/5, torpedo dispatch 11/11, current
source 34/34, and packed-part 13/13. Compile/link and process exit pass, with
no Program Error. Full host suite passes 519 tests, with 153 expected skips,
2,241 layout assertions, and 846 declarations. Evidence:
`analysis/compat/f-attack-torpedo-table-native-e29.{md,json}` and
`runs/vmctl-vc152fapne29/`, `runs/vmctl-vc152fapne30/`,
`runs/vmctl-vc152fapne31/`.

This is supporting evidence only: it closes **0 named Step-5 units**.
FAttack remains 0/3 at Step 5 and 5/6 overall pending complete raw complex-beam
battle-part scaling/table acceptance, broader original damage differentials,
and full native battle replay. TODO6 counters remain 120/152 evidence units,
32 open, 10 promotions, and 42 practical open work items.

## Previous checkpoint — FAttack residual torpedo loop native E25/E27

Current production `FAttack` now passes fresh Win98/VC++ 1.52c evidence for
the raw torpedo allocation/dispatch path: 9/9 raw beam-damage checks, 5/5
torpedo target checks, and 9/9 raw torpedo dispatch checks. The same E25 packet
also passes packed-part lookup 13/13, fixed-table metadata 34/34, complex
score 9/9, and complex selection 3/3. E25 compile/link status passes and the
test process exits 0 with no Program Error dialog. The current 34-check
`tests/f_attack_source_probe.c` was then built and run separately in E27 and
passes natively, addressing a stale E25 copy that had expected pre-residual
loop counts. E24's `_printf` link failure, E25's stale fixture report, and
E26's missing-`TORP.C` packaging error are all preserved and explicitly not
credited as passes.

The full host suite passes 519 tests with 153 expected skips, 2,241 layout
assertions, and 846 declarations; VC152 package tests pass 3/3 and
`git diff --check` passes. These are implementation and native-regression
milestones only: **0 named Step-5 units close**. E29/E30/E31 subsequently resolve
the group-0x20 table values; FAttack still needs complete raw complex-beam
scaling, broader original-behavior differentials, and complete native battle
replay across packed state. The TODO6
top-level counters remain **120/152 closed, 32 open, 10 promotions, and 42
practical open work items**. Full evidence:
`analysis/compat/f-attack-torpedo-residual-native-e25.{md,json}`.

## Previous checkpoint — FAttack raw beam damage class E23

Raw group-0x10 beam dispatch now derives FDamageTok's class directly from the
resolved record's offset-54 word: `raw_part_dp < 200` yields class 1, otherwise
class 2, matching raw FAttack's threshold and `_beam.dp` layout. The strict-C89
raw-damage regression verifies values 50 and 200 (9 checks total); the full
suite passes 519 tests with 153 expected skips, 2,241 layout assertions, and
846 declarations. `git diff --check` passes. Native replay is still pending:
port 2122 has a listener but no established guest session. This is host source
evidence only and closes no Step-5 unit; counters remain **120/152 closed, 32
open, 10 promotion units, and 42 practical open work items**. Detailed record:
`analysis/compat/f-attack-beam-damage-class-e23.md`.

## Superseded checkpoint — FAttack group-0x20 torpedo record address E22

The E22 address inference from Ghidra's `0x10e0` selector and M6108 DGROUP
string is superseded. The original instructions store `CS` as the selected
far-pointer segment; CodeView and segment bytes identify the group-0x20 table
as `rgtorp` in NE segment 2. The actual `dp` words and native dispatch are
recorded in E29/E30/E31 above. E22 remains useful as a historical record of the
initial hypothesis, not as current table-value evidence.
Detailed record: `analysis/compat/f-attack-torpedo-record-address-e22.md`.

## Previous checkpoint — FAttack raw torpedo allocation E21

The readable raw torpedo path now computes the two allocation terms before
damage dispatch. It preserves the `_dv` hull-capacity reduction, item 8–11
power doubling when shields are absent, the raw ceiling/truncation sequence,
the `/8` shield term and `/2` hull term, and the capacity-threshold scan. The
resolved local section now takes group/item/count from the packed `_hs` slot.
The strict-C89 dispatch probe passes 9 cases and focused source/target/raw
damage regressions pass 34/5/7. The full suite after E21 changes passes 519
tests (153 expected skips), with 2,241 generated layout assertions and 846
function declarations. A fresh 43-file `C:\FAPNE21` packet passes
syntax/ASCII-CRLF checks; native execution is unverified.

The residual loop and state feedback, actual group-0x20 record population in
the packed resolver, beam class derivation, original differentials, full native
battle replay, and promotion remain open. This closes zero Step-5 units;
scorecard remains **120/152 closed, 32 open, 10 promotion units, 42 practical
open work items**. Evidence:
`analysis/compat/f-attack-torpedo-allocation-e21.{md,json}`.

## Previous checkpoint — FAttack torpedo split/residual source map E20

The raw FAttack torpedo branch is now traced through allocation and damage
dispatch (`10f0_69ac_FAttack.c:620-769`): it keeps launched budget separate
from successful hits, scans a shield/hull split under capacity thresholds,
dispatches distinct `/8` shield and `/2` hull terms, and loops on residual
budget plus a second counter after mutable `FDamageTok` calls. The readable
path still lacks that split and same-target state feedback. The uncertain
meaning of `lVar19`/`uStack_26` is called out rather than guessed. This is
analysis only; no tests or Step-5 closure are claimed. Counters remain
**120/152 closed, 32 open, 10 promotion units, 42 practical open work items**.
Evidence and the scoped implementation/test plan are in
`analysis/compat/f-attack-torpedo-split-analysis-e20.md`.

## Previous checkpoint — FAttack launched torpedo packet budget E19

The raw `FDamageTok` packet pointer receives the torpedo allocation budget,
not `CTorpHit`'s successful-hit count. The original FAttack initializes
`uStack42` from the launched budget, partitions it into two terms, then passes
their sum through `pcTorp`; readable `FAttack` now forwards the launched
section budget. The strict-C89 seven-check probe makes two of three torpedoes
hit and verifies that the callback receives 3 packets and can write back 2.
The fresh 43-file `C:\FAPNE19` packet passes packaged-source syntax and
ASCII/CRLF checks; guest replay is not recorded.

This does not implement the `uStack34`/`uStack38` shield/hull allocation or
the same-target `lVar19`/`lVar16` residual loop, which also depends on mutated
target state. The loop must be reconstructed from those inputs rather than
approximated by repeated callback calls. No Step-5 unit closes; counters remain
**120/152 closed, 32 open, 10 promotion units, and 42 practical open work
items**. Evidence: `analysis/compat/f-attack-torpedo-packet-budget-e19.{md,json}`.

## Previous checkpoint — FAttack torpedo damage-class selection E18

The raw torpedo `grfWeapon` class now follows the original item-index rule:
class 4 normally, class 12 for packed items 8–11, and `0x80` added for the
shields-only dispatch. The strict-C89 dispatch probe passes 7 assertions
covering items 7 and 8; the E16 target/RNG probe still passes 5. A fresh
43-file packet (42 ASCII/CRLF text files plus the DOS stub) is prepared for
`C:\FAPNE18`, and packaged-source strict-C89 syntax checks pass. Native replay
is not yet verified. Exact shield/hull partitioning, residual-torpedo
iteration, beam damage-class derivation, broader original differential, full
battle replay, and promotion remain open. This closes zero Step-5 units; the
scorecard remains **120/152 closed, 32 open, 10 promotion units, 42 practical
open work items**. Evidence: `analysis/compat/f-attack-torpedo-damage-class-
e18.{md,json}`.

## Previous checkpoint — FAttack raw torpedo damage-call bridge E17

The readable raw-damage callback now mirrors the recovered `FDamageTok`
argument contract for torpedo shield and hull dispatch: beam damage by
reference, `dpTorp`, weapon damage-class flags, shields-only state, and mutable
remaining packets. Host strict-C89 probes pass the new 6-case dispatch matrix,
the E15 7-case packed raw-damage matrix, and the E16 5-case target/RNG matrix.

A fresh 43-file packet (42 ASCII/CRLF text files plus the DOS stub) is
prepared at `compat/vc152/f-attack-complex-native-e17/` for `C:\FAPNE17`; its
packaged sources pass strict-C89 syntax checking against the reduced header.
Guest compile/run evidence is not yet recorded. In particular, production
packed-part resolution still does not derive the original `grfWeapon` class
from the chosen weapon and target state, and the raw shield/hull split and
residual-torpedo iteration remain incomplete. This closes zero named Step-5
units; the current scorecard stays at **120/152 closed, 32 open, 10 promotion
units, 42 practical open work items**. Evidence: `analysis/compat/f-attack-raw-
torpedo-dispatch-e17.{md,json}`.

## Previous checkpoint — FAttack torpedo candidate ranking E16 host support

The raw `FAttack` torpedo selector at
`analysis/decompiler/export/raw/10f0_69ac_FAttack.c:383-515` ranks eligible
targets deterministically by resource value per expected shield/hull torpedoes
before calling `CTorpHit`. The readable source previously rolled hit RNG for
every candidate, changing both target selection and random-stream consumption.
It now computes the raw score/truncation sequence and consumes hit RNG only
for the selected target. The new strict-C89 probe passes 4/4; focused FAttack
source/score/selection/packed/raw-damage probes pass 34/9/3/13/7. The full
suite passes 519 tests (153 expected skips), with 2,241 layout assertions and
846 declarations; baseline/diff checks pass.

This is host support only and closes no named Step-5 unit. The E15 packet has
an older `f_attack.c`; a fresh 42-file native packet is prepared in
`compat/vc152/f-attack-complex-native-e16/` for `C:\FAPNE16`, with all 41 text
files in ASCII CRLF; packaged sources pass strict-C89 syntax against the
packet's `CORE.H`. Native replay, raw torpedo shield/hull damage splitting and
residual-torpedo behavior, original differential, full battle replay, and
promotion remain open. The Phase 2 counters remain **120/152 closed, 32 open,
10 promotion units, 42 practical open work items**. Evidence is in
`analysis/compat/f-attack-torpedo-target-e16.{md,json}`.

## Latest checkpoint — DoRobotoidAiTurn repair eligibility/order E4 host support

The readable Robotoid policy now reflects the raw deterministic repair gate:
owned fleet, slot 0 nonzero, turn >= 41, `cord == 1`, empty first-order kind.
It asks the route planner first and applies repair only when the route is a
no-op. Host repair and route probes pass 7 and 18 checks; source/phase/
production/integration/fleet-gate/typed-fleet probes pass 23/6/12/18/10/2.
The complete test suite passes (519 tests, 153 expected skips), with 2,241
layout assertions and 846 declarations; baseline/diff checks pass. The raw
`Random(5)` nearby-route branch and exact RNG draw behavior remain open, so
this closes no Step-5 unit. Native replay and original-turn differential are
also open. Evidence: `analysis/compat/do-robotoid-ai-turn-repair-gate-e4.{md,json}`.

## Latest checkpoint — DoTurinDroneAiTurn exact repair gate E4 host support

Corrected Turin's repair predicate from any nonempty fleet to the raw slot
matrix at `analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c:675-693`:
slots 2, 3, 1, 8, 9, 13, 14, 0, 10, and 11 are zero; slot 12 is nonzero;
`cord == 1`; and first-order kind is empty. The strict-C89 repair probe passes
6 checks, including excluded-slot cases. Turin source/integration/route/edge
probes pass 35/20/14/7 checks. Full suite: 519 passed, 153 expected skips;
2,241 layout assertions and 846 declarations. Baseline and diff checks pass.
This is host-only and closes no Step-5 unit; native replay, complete packed
routing, and original-turn comparison remain open. The guest agent is
disconnected. Evidence: `analysis/compat/do-turin-drone-ai-turn-repair-gate-e4.{md,json}`.

## Latest checkpoint — AI PLORD first-order kind writes E11 host support

The raw Robotoid listing contains repeated first-order branches that perform
`(word & 0xfff0) | 5` or `(word & 0xfff0) | 6`. The shared
`stars_ai_set_first_order_kind` helper now models that exact low-nibble write
without resetting the packed queue count or overwriting unrelated order
bytes. The strict host C89 probe reports
`AI_PLORD_FIRST_KIND_SOURCE_CHECKS=12 FAILURES=0 RESULT=PASS`, and the
existing `AI_PLORD_REPAIR_SOURCE_CHECKS=9 FAILURES=0 RESULT=PASS` regression
remains green.

The surrounding Robotoid/PLORD probes also pass, and the full repository
regression reports `Ran 519 tests`, `OK (skipped=153)`; generated headers
remain at 2,241 layout assertions and 846 function declarations.
`baseline.py --check` and `git diff --check` pass.

This is supporting host evidence only: no named Step-5 unit or promotion count
changes. Native replay, complete Robotoid-family route selection, multi-order
packed mutation, and original-turn differentials remain open because the
VMCTL1 listener still has no guest connection/banner. Evidence is in
`analysis/compat/ai-plord-first-kind-e11.{md,json}`.

## Audit resolution — DoAutomitronAiTurn E3 repair branch confirmed in assembly

The E3 host probe reports 8/8 checks for a cord-one, empty-kind packed repair.
The raw decompiler C appears to place `cord == 1` in the `else` of `cord < 2`,
but the instruction listing at
`analysis/decompiler/export/raw/1098_01e0.asm:1235-129e` confirms a reachable
path: it checks `rgcsh[6]` at fleet offset `0x18` is nonzero, checks fleet
`[BX+0x62] == 1`, tests the first-order low nibble at
`[PLORD+0x0a]`, writes kind 6 and two `0x0005` words, then calls `FLookupFleet`.
Treat the contradiction as a decompiler structuring artifact. The readable
repair gate now uses slot 6 rather than any nonzero ship total; its probe
confirms a slot-2-only fleet does not repair. Repair/source/production/
integration/route/action-matrix probes pass at 8/31/12/20/14/20 checks. The
full suite passes (519 tests, 153 expected skips), with 2,241 layout assertions
and 846 declarations; baseline and diff checks pass. This adds no native
evidence and changes no Phase 2 counter.

## Prior checkpoint — DoAutomitronAiTurn packed repair boundary E3 host support

The readable Automitron fleet view carries `cord`, first-order kind, and an
optional packed `PLORD` pointer. The seam applies the shared kind-6/two-word
repair before the typed fleet visitor for cord-one/empty-kind input and then
dispatches an optional repair callback. Instruction-level listing
`analysis/decompiler/export/raw/1098_01e0.asm:1235-129e` confirms the raw
slot-6/cord-one/empty-kind write path; the contradictory structured C is a
decompiler artifact. The typed repair policy uses slot 6 (not total ship
count), and the probe includes a slot-2-only non-repair case. The strict host C89 boundary reports
`AUTOMITRON_AI_REPAIR_BOUNDARY_CHECKS=8 FAILURES=0 RESULT=PASS`.

The existing Automitron source, production, and packed integration probes
remain green at 31, 12, and 20 checks. This is supporting host evidence only:
no named Step-5 unit or promotion count changes. Native replay, complete
Automitron route/order behavior, and original-turn differentials remain open.
The VMCTL1 listener still has no guest connection/banner. Evidence is in
`analysis/compat/do-automitron-ai-turn-repair-boundary-e3.{md,json}`.

The full repository regression after the public-header/source change reports
`Ran 519 tests`, `OK (skipped=153)`; generated headers remain at 2,241 layout
assertions and 846 function declarations.

## Latest checkpoint — DoAutomitronAiTurn route plan/apply boundary E4 host support

The readable Automitron caller now separates its fleet route plan from route
application and carries action, target, order kind, append mode, and reason
through the recovered action-family boundary. A strict host C89 probe reports
`AUTOMITRON_AI_ROUTE_PLAN_CHECKS=14 FAILURES=0 RESULT=PASS`, covering
owned-fleet filtering, colonize/move payloads, callback ordering, and planner /
applier failure propagation.

This remains supporting host evidence only: no named Step-5 unit or promotion
count changes. Raw Automitron target predicates, complete packed-order
mutation, native replay, and original-turn differentials remain open. Evidence
is in `analysis/compat/do-automitron-ai-turn-route-plan-e4.{md,json}`.

The full repository regression after this public-header/source change reports
`Ran 519 tests`, `OK (skipped=153)`; generated headers remain at 2,241 layout
assertions and 846 function declarations.

## Latest checkpoint — DoAutomitronAiTurn action dispatch matrix E5 host support

A strict-C89 full-caller matrix selects all nine Automitron fleet action
families and checks the intended named callback, complete plan payload,
owned-fleet filtering, and post-action visitor. It reports
`AUTOMITRON_AI_ACTION_MATRIX_CHECKS=20 FAILURES=0 RESULT=PASS`.

This is host-only support and closes no named Step-5 unit. Native replay, raw
target predicates, packed order writes, original-turn differential, and
promotion remain open. Evidence is in
`analysis/compat/do-automitron-ai-turn-action-matrix-e5.{md,json}`.

## Latest checkpoint — Phase 2 host regression after combat/order corrections

The focused strict-C89 `FAttack` source boundary reports
`F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS`, and the complete four-pass
`SatisfyOrders`/`DoOrders` integration reports
`SATISFY_ORDERS_FULL_TURN_E3_CHECKS=18 FAILURES=0 RESULT=PASS`. The full
project regression reports 519 tests passed with 153 expected skips; generated
headers remain at 2,241 layout assertions and 846 function declarations.
`python3 tools/baseline.py --check` and `git diff --check` pass.

This is host-only regression evidence. The VMCTL1 listener is still waiting
without a guest connection/banner, so the prepared FAttack E15 native packet
and the remaining native replays are still pending manual `C:\VMCTL1\STARTAGT`
startup. No named Step-5 unit or Step-6 promotion count changes here. Evidence
is in `analysis/compat/phase2-host-regression-20260922.{md,json}`.

## Latest checkpoint — DoMacintiAiTurn policy boundary E2 host support

A focused strict-C89 matrix now reports
`MACINTI_AI_POLICY_BOUNDARY_CHECKS=11 FAILURES=0 RESULT=PASS`. It covers the
readable Macinti per-planet policy seam (ownership, mineral viability,
queue/batch affordability, packet threat, and starbase increment) and the
owned repair/transport/colonizer fleet-role predicates, while excluding a
foreign fleet from the typed turn visit.

This is supporting host evidence only; it closes no named Step-5 unit. Native
replay, exact original retarget/colonizer/transport predicates, broader
packed-order mutation, and the original-turn differential remain open. Evidence
is in `analysis/compat/do-macinti-ai-turn-policy-boundary-e2.{md,json}` and
`tests/do_macinti_ai_turn_policy_boundary_probe.c`.

## Latest checkpoint — DoCyberAiTurn action dispatch matrix E10

The complete readable Cyber caller now has a host/native matrix over all nine
named action families: clear task, move, colonize, wormhole, starbase, attack,
armada, transport, and freighter targeting. The movement families route
seeded first-order records through the shared packed writer. Host and fresh
VC++ 1.52c / Win98 `C:\CYBAME10` validation both report
`CYBER_AI_TURN_ACTION_MATRIX_CHECKS=34 FAILURES=0 RESULT=PASS`; compile/link
and process exit passed, and no Program Error dialog appeared.

This is supporting evidence, not a named-unit closure: raw Cyber personality
predicates, complete AI-table integration, the original-turn differential, and
promotion remain open. Evidence is in
`analysis/compat/do-cyber-ai-turn-action-matrix-e10.{md,json}`,
`compat/vc152/do-cyber-ai-turn-action-matrix-e10/`, and
`runs/vmctl-vc152cybame10/`.

## Latest checkpoint — DoCyberAiTurn repair and packed order write E9

The complete readable Cyber caller now combines the shared first-order repair,
typed route planning/action dispatch, and packed `PLORD` writing in one
matrix. An owned colonizer fleet takes a same-endpoint replacement, an owned
armed fleet appends a movement order, and a foreign fleet is bypassed. The
strict host C89 and fresh VC++ 1.52c / Win98 `C:\CYBOWE9` replays both report
`CYBER_AI_TURN_ORDER_WRITE_CHECKS=24 FAILURES=0 RESULT=PASS`; compile/link and
process exit passed, and no Program Error dialog appeared. Planner and applier
failure propagation are also checked.

This closes the named detailed repair/order-write Step-5 unit. Direct raw
personality branch coverage beyond these typed action families, complete Cyber
AI-table integration, the original-turn differential, and Step-6 promotion
remain open. Evidence is in
`analysis/compat/do-cyber-ai-turn-order-write-e9.{md,json}`,
`compat/vc152/do-cyber-ai-turn-order-write-e9/`, and
`runs/vmctl-vc152cybowe9/`.

The authoritative project counters are now **120/152 evidence units closed,
32 open, 10 promotion units, and 42 practical open work items**.

## Latest checkpoint — DoMacintiAiTurn packed first-order repair support

The readable Macinti fleet view now accepts an optional packed `PLORD`
allocation. When the recovered fleet policy requires first-order repair, the
turn applies the shared raw repair before typed task/action planning. The
strict-C89 host probe reports
`MACINTI_AI_PLORD_REPAIR_CHECKS=5 FAILURES=0 RESULT=PASS`, checking the
repaired count, order-kind nibble, repair words, callback ordering, and
preservation of unrelated bytes. This is host supporting evidence only; a
fresh VC++ 1.52c replay, broader Macinti packed-order mutation, exact routing
predicates, and the original full-turn differential remain open. Evidence is
in `analysis/compat/do-macinti-ai-turn-integration.{md,json}` and
`tests/do_macinti_ai_turn_plord_repair_probe.c`.

## Latest checkpoint — DoCyberAiTurn packet/freighter edge matrix E8

The complete readable `DoCyberAiTurn` caller now composes the real packet and
freighter services across five full-caller cases: high-stock owned-planet
drop-off, low-stock owned-planet pickup, enemy ordinary-planet starbase
fallback, open-space short circuit, and packet-target rejection. The strict
host C89 and fresh VC++ 1.52c / Win98 `C:\CYBFE8` replays both report
`CYBER_AI_TURN_PACKET_FREIGHTER_EDGE_CHECKS=45 FAILURES=0 RESULT=PASS`;
compile/link and process exit passed, and no Program Error dialog appeared.

This closes the named DoCyberAiTurn packet/freighter behavior Step-5 unit
when combined with E5's native freighter gate/progress matrix and E7's native
accepted packet/freighter composition. Detailed order-repair/order-write
coverage, complete Cyber AI-table integration, the original-turn differential,
and Step-6 promotion remain open. Evidence is in
`analysis/compat/do-cyber-ai-turn-packet-freighter-edge-e8.{md,json}`,
`compat/vc152/do-cyber-ai-turn-packet-freighter-edge-e8/`, and
`runs/vmctl-vc152cybfe8/`.

The authoritative project counters are now **119/152 evidence units closed,
33 open, 10 promotion units, and 43 practical open work items**.

## Latest checkpoint — DrawPopup original consumer E4 completion and Step-6 promotion

The final named DrawPopup E4 boundary is now closed from retained original
Win98 consumer captures. The evidence covers fleet summary, starbase/base,
planet mineral/status, planet population/report, Planet Summary Report, and
restored-frame behavior without a Program Error dialog. This is an original
consumer-boundary comparison; it does not claim pixel identity.

The original captures are combined with the authoritative native E1 38-check
payload, E2 21-check content, E3 25-check geometry/style, 7-check GDI
state-order, and 26-check production caller replays. The review record,
reconstruction note, TODO5/TODO6, and coverage ledger now record `DrawPopup`
as **6/6**, with **4/4** named Step-5 units closed and Step 6 promoted.
Evidence is in `analysis/compat/draw-popup-original-e4.{md,json}`,
`runs/vmctl-vc152current/`, and the native E1–E3 run directories recorded in
`tracking/reviews.json`.

The authoritative project counters are **118/152 evidence units closed, 34
open, 10 promotion units, and 44 practical open work items**.

## Latest checkpoint — FAttack packed raw-damage integration E15

The host probe now composes the packed `_hs` attacker-part slot with the raw
complex-beam scaling and stateful `FDamageTok` callback. It passes 7/7 checks:
group/item/count forwarding, local resolved-section ownership, the 39-point
scaled beam, shield/hull record order, casualty mutation, and FAttack's
hit/battle-record fields. A fresh native packet is prepared for `C:\FAPNE15`
but cannot yet be replayed because the Win98 agent stopped reconnecting after
the read-only report-window transport interruption. The VM itself remains
running in 86Box; no reset or shutdown was performed. The packet has since
been verified and normalized to ASCII CRLF text files so DOS batch, response,
and linker parsing is not an additional variable. The host listener is still
waiting for `STARSAGT 0.4.2`; no guest connection has arrived yet. This closes
no named Step-5 unit. Evidence is in
`analysis/compat/f-attack-packed-raw-damage-e15.{md,json}` and
`compat/vc152/f-attack-complex-native-e15/`.

## Latest checkpoint — DoCyberAiTurn packet-service bridge E4

The readable Cyber turn now exposes an optional typed packet callback with the
prepared mutable planet table, planet count, player id, and computed packet pressure;
the player-only callback remains a compatibility fallback. The strict C89
integration probe passes 30/30, including the packed planet's 501 packet
strength, write-back to 401, and final callback ordering. Native replay is pending the Win98 agent
reconnection, and this closes no named Step-5 unit. Direct packet/freighter
composition, packed `PLORD` mutation, complete AI-table integration, and the
original-turn differential remain open. Evidence is in
`analysis/compat/do-cyber-ai-turn-packet-bridge-e4.{md,json}`.

## Latest checkpoint — SatisfyOrders E3 original-instruction boundary audit

The original `DoOrders` export now has an explicit machine-readable
orchestrator oracle covering the exact pre-/post-movement callback order,
inhabited-planet flag update, battle-id formula, post-movement planet-turn
clear, and pre-movement-only `TransferToOthers` gate. The readable source and
fresh native 18-check fixture agree with it. This strengthens the original
instruction boundary but is not claimed as the missing original executable
full-turn state/message differential; SatisfyOrders remains **2/3 named units
at 5/6** and Step-6 promotion remains open. Evidence is in
`analysis/compat/satisfy-orders-full-turn-e3-original-oracle.{md,json}`.

## Latest checkpoint — ExecuteReportClick packed mineral-status E6 native support

The packed `_planet` bridge now preserves the original `overlap_4` status word
at offset `0x04` and propagates bit 10 to the mineral popup's raw
`GlobalPD.iPlanVal` offset `0x06`. The strict-C89 and fresh VC++ 1.52c/Win98
`C:\EXINT6` matrices both pass 34/34; compile/link and process exit pass, and
no Program Error dialog appears. This is supporting packed/native evidence,
not a named-unit closure. Evidence is in
`analysis/compat/execute-report-click-packed-e6-native.{md,json}`,
`compat/vc152/execute-report-click-packed-e6-native/`, and
`runs/vmctl-vc152exint6/`.

## Latest checkpoint — SatisfyOrders E2 closure

The existing production-source/native packets now collectively close the
SatisfyOrders E2 boundary: technology-table/result binding, real research and
salvage services, created-player fleet/design materialization, special-order
gates, packed special dispatch, and every recovered `FSendPlrMsg`/
`FSendPlrMsg2` message family. The corrected raw sweep passes 141/141 checks;
the fuel and failure packets pass 40 and 44 exact-payload checks. This is an
E2 closure only. The complete four-pass/original full-turn differential is
still E3, and final promotion remains open.

SatisfyOrders is now **2/3 authoritative Step-5 units closed at 5/6**. The
authoritative project counters are **117/152 evidence units closed, 35 open,
10 promotion units, and 45 practical open work items**. The closure packet is
`analysis/compat/satisfy-orders-e2-closure.{md,json}`.

The closure audit also corrected a typed-callback regression in the foreign
fleet-colonist branch. The exact raw `0x0155` frame is retained when the raw
callback is present, while the legacy typed callback is preserved otherwise.
The fresh `C:\SYOE24` replay passes 12/12 with compile/link PASS, exit 0, and
no Program Error dialog; its evidence is in
`analysis/compat/satisfy-orders-foreign-colonist-fallback-e2-native.{md,json}`.

## Latest checkpoint — SatisfyOrders E3 full-turn integration support

The turn-sequencing harness was expanded into a four-pass pre-/post-movement
state and message matrix bound through the real `DoOrders`/`SatisfyOrders`
production sources. It checks sequencing, battle/thing callbacks, planet and
fleet mutation, order retirement, mine/route/player-transfer dispatch, and
generic/player message order. The strict-C89 host probe reports
`SATISFY_ORDERS_FULL_TURN_E3_CHECKS=18 FAILURES=0 RESULT=PASS`.

The fresh VC++ 1.52c/Win98 replay in `C:\SYFTE4` reports the same 18/18 pass,
with compile/link PASS, process exit 0, and no Program Error dialog. Evidence
is preserved in
`analysis/compat/satisfy-orders-full-turn-e3-native.{md,json}`,
`compat/vc152/satisfy-orders-full-turn-e3-r2/`, and
`runs/vmctl-vc152syfte4/`.

This is a supporting E3 integration subcheck, not closure of the named E3
unit. The original executable's complete full-turn state/message differential
and Step-6 promotion remain open. The authoritative counters remain
**117/152 evidence units closed, 35 open, 10 promotion units, and 45 practical
open work items**; SatisfyOrders remains **2/3 named Step-5 units closed at
5/6**.

## Latest checkpoint — DoCyberFreighter native gate/progress E5 support

The corrected `DoCyberFreighter` boundary now has a fresh combined native
matrix covering the owned-planet mineral-rate/stock threshold, pickup/drop-off
direction, carrying-only progress-bit advancement, no-destination rollback,
enemy ordinary-planet starbase fallback, special-planet carrying, open-space
short circuit, exact `0x11b0` movement, and packed PLORD first-order
replacement. Host and fresh VC++ 1.52c/Win98 `C:\CYBFE5` reports both show
`DO_CYBER_FREIGHTER_E5_CHECKS=26 FAILURES=0 RESULT=PASS`; compile/link and
process exit passed, and no Program Error dialog appeared.

This strengthens source-boundary evidence but does not close a named
DoCyberAiTurn unit. Complete raw fleet-cargo predicates, packet/freighter
interaction, complete AI-table integration, original-turn differential, and
promotion remain open. Evidence is in
`analysis/compat/ai-cyber-freighter-native-e5.{md,json}`,
`compat/vc152/do-cyber-freighter-e5-native-r1/`, and
`runs/vmctl-vc152cybfe5/`.

## Latest checkpoint — ExecuteReportClick raw GlobalPD E1 native support

The production `ExecuteReportClick` boundary now preserves the complete
22-byte overlapping `GlobalPD` / `_popupdata` view alongside typed popup
fields.  The raw support covers fleet and starbase far-pointer words, the
eight-byte defense `_part` union, the resource part word, and the five
overlapping `rgi` long views; pointer words remain opaque and are never
dereferenced by the host reconstruction.

The strict host probe and corrected fresh VC++ 1.52c/Win98 replay in `C:\GPDE2`
both report `EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=20 FAILURES=0 RESULT=PASS`.
Compile/link passed, the process exited 0, and the final guest window list
contained no Program Error dialog.  This is supporting E1 boundary evidence,
not closure of the named Step-5 unit. Exact original pointer/part values and
differentials, live USER/GDI report behavior, complete report-layout caller
coverage, and promotion remain open. Evidence is in
`analysis/compat/execute-report-click-globalpd-e1-native.{md,json}`,
`compat/vc152/execute-report-click-globalpd-e1-native-r2/`, and
`runs/vmctl-vc152gpde2/`. The earlier `GPDE1` replay is retained as
superseded evidence because its starbase oracle predated the raw
`iPlanVal=1` correction.

## Latest checkpoint — DrawPopup detailed GDI/caller E3 closure

The readable `DrawPopup` text boundary now carries explicit x/y coordinates,
left/right/center anchors, normal/bold font selection, opaque background mode,
and optional measurement-driven label offsets. A strict-C89 geometry probe
passes `DRAW_POPUP_GEOMETRY_E3_CHECKS=25 FAILURES=0 RESULT=PASS` across popup
kinds 1–4, hull rows, separator placement, row spacing, and the short-client
fallback. The final VC++ 1.52c/Win98 replay in `C:\DPE9` reports the same
25-check pass; compile/link and process exit passed and no Program Error
dialog appeared. The first wrapper-ending failure is retained as diagnostic
evidence; correcting the mixed CRLF/LF wrappers produced the authoritative
run.

Together with the native 7-check state-order probe and the fresh native
26-check `ExecuteReportClick -> Popup -> DrawPopup` caller replay, this closes
the named E3 detailed GDI/caller unit. DrawPopup is now **3/4 named Step-5
units closed at 5/6**; E4 original completion and Step-6 promotion remain.
The authoritative counters are now **116/152 evidence units closed, 36 open,
10 promotion units, and 46 practical open work items**. Evidence is in
`analysis/compat/draw-popup-detailed-e3.{md,json}`,
`analysis/compat/draw-popup-geometry-e3-{host,native}.{md,json}`,
`compat/vc152/draw-popup-geometry-e3-r2/`,
`runs/vmctl-vc152drawpopupgeometrye3r2/`, and
`runs/vmctl-vc152popintp/`.

## Latest checkpoint — DrawPopup localization/hull formatting E2 native replay

The readable `DrawPopup` text-segment boundary is now natively validated.
The fresh `C:\DPE7` VC++ 1.52c packet reports
`DRAW_POPUP_CONTENT_E2_CHECKS=21 FAILURES=0 RESULT=PASS`; compile and link
passed, the process exited 0, and the final guest window inventory contained
no Program Error dialog. The final screen and complete build/map artifacts are
preserved in `runs/vmctl-vc152drawpopupcontente2-r2/`. The first compile-only
link failure is retained in `runs/vmctl-vc152drawpopupcontente2/`; it was a
packet-only unresolved host `printf`, removed under `TARGET_NATIVE` while the
host probe remains unchanged.

This closes DrawPopup E2, so the procedure is now **2/4 named Step-5 units
closed at 5/6**. E3 detailed GDI/caller differential, E4 original completion,
and Step-6 promotion remain. The authoritative counters are now **115/152
evidence units closed, 37 open, 10 promotion units, and 47 practical open
work items**. Evidence is in
`analysis/compat/draw-popup-content-e2-native.{md,json}` and
`runs/vmctl-vc152drawpopupcontente2-r2/`.

## Supporting checkpoint — DrawPopup localization/hull formatting E2 host boundary

The readable `DrawPopup` source now emits typed semantic text segments for
popup kinds 1–4, including localized string IDs, numeric formatting kinds,
fleet-name and planet-name seams, hull visibility filtering, hull decoration,
damage/count formatting, and coordinate labels. Optional raw 124-byte fleet
and 147-byte hull-definition views keep the recovered packed offsets explicit
without dereferencing raw far-pointer words in the 22-byte popup union.

The strict-C89 host probe passes
`DRAW_POPUP_CONTENT_E2_CHECKS=21 FAILURES=0 RESULT=PASS`, and the common-source
suite (221 tests), baseline regeneration/check, and `git diff --check` are
clean. The native E2 closure is recorded in the checkpoint above; this host
entry is retained as the independent host-side evidence.

## Latest checkpoint — DrawPopup complete packed `GlobalPD` payload E1

The production `DrawPopup` bridge now materializes the complete recovered
22-byte `_popupdata` union before dispatch. `StarsDrawPopupPayload` preserves
the raw bytes and every recorded overlay: all five `rgi` long values,
planet/player and production-counter views, near/far pointer words,
damage/display flags, the eight-byte `_part` view, and token-summary fields.
Pointer words remain raw and are not dereferenced by the dispatcher.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\DPE6` both
report `DRAW_POPUP_PACKED_CHECKS=38 FAILURES=0 RESULT=PASS`; compile/link and
process exit passed, and the final guest window inventory contained no Program
Error dialog. The first diagnostic replay is retained separately because it
identified and corrected a test-oracle mismatch before the final pass.

This closes DrawPopup E1, so the procedure is now **1/4 named Step-5 units
closed at 5/6**. E2 localization/hull filtering/formatting, E3 detailed GDI
and caller differential, E4 original completion, and Step-6 promotion remain.
The authoritative counters are **114/152 evidence units closed, 38 open, 10
promotion units, and 48 practical open work items**. Evidence is in
`analysis/compat/draw-popup-globalpd-e1.{md,json}`,
`compat/vc152/draw-popup-packed-e6-payload/`, and
`runs/vmctl-vc152drawpopupe6/`.

## Latest checkpoint — DumpPlanets Step-6 promotion

The original Win98 executable behavior boundary is now closed. The actual
`Reports > Dump to Text File > Planet Information` path wrote the 334-byte
`GAME.pla`, displayed its success message, and completed without a Program
Error; the hash and screenshots are preserved in
`analysis/compat/report-dump-integration.json`. E1 localized normal
serialization, E2 complete header/two-row differential, and E3 packed service
resolution are also closed. The review ledger, TODO5/TODO6, coverage, and
reconstruction notes now record `DumpPlanets` as **6/6 and promoted**.

The authoritative counters are **113/152 evidence units closed, 39 open, 10
promotion units, and 49 practical open work items**. Evidence is in
`analysis/compat/dump-planets-original-behavior-e4.{md,json}`,
`runs/vmctl-vc152current/`, and `runs/vmctl-vc152dpsor2/`.

## Latest checkpoint — DumpPlanets localized normal serialization E1

The production `stars_dump_planets` coordinator now exposes the recovered
twenty-column normal report through formatter callbacks while retaining its
raw callback path. Strict-C89 host and fresh VC++ 1.52c/Win98 validation in
`C:\DPSER1` both report
`DUMP_PLANETS_SERIALIZATION_CHECKS=12 FAILURES=0 RESULT=PASS`. The matrix
covers two-row raw/formatted field order, localized planet/owner/starbase/
queue/value/defence text, the preserved `GAME.pla` tab/CRLF oracle, and
invalid callback rejection. Compile/link and process exit passed, and the
final guest window inventory contained no Program Error dialog.

This closes one named `DumpPlanets` Step-5 evidence unit. Its packed-service
E3 unit is already closed; detailed/service-column serialization, complete
original export differential, and Step-6 promotion remain. `DumpPlanets`
remains at **5/6**, now **2/4** named Step-5 units closed. The authoritative
counters are **111/152 evidence units closed, 41 open, 11 promotion units,
and 52 practical open work items**. Evidence is in
`analysis/compat/dump-planets-serialization-e1.{md,json}`,
`compat/vc152/dump-planets-serialization-e1/`, and
`runs/vmctl-vc152dpser1/`.

## Latest checkpoint — DumpPlanets complete original-output E2

An independent full-stream differential now exercises the same production
formatter path. Strict-C89 host and fresh VC++ 1.52c/Win98 validation in
`C:\DPSOR2` both report
`DUMP_PLANETS_SERIALIZATION_CHECKS=14 FAILURES=0 RESULT=PASS`, including the
preserved `GAME.pla` header, both rows, tabs, and CRLF terminators byte-for-byte.
Compile/link and process exit passed, and the final guest window inventory
contained no Program Error dialog.

Together with E1 localized normal serialization and E3 packed-service
integration, this closes **3/4** named `DumpPlanets` Step-5 units. The
procedure remains at **5/6** for its remaining original executable behavior
boundary and final promotion. The authoritative counters are **112/152
evidence units closed, 40 open, 11 promotion units, and 51 practical open
work items**. Evidence is in
`analysis/compat/dump-planets-original-output-e2.{md,json}`,
`compat/vc152/dump-planets-original-output-e2/`, and
`runs/vmctl-vc152dpsor2/`.

## Latest checkpoint — DumpFleets complete original-output E4

The formatted `stars_dump_fleets` boundary now materializes all six fleet rows
from the preserved original export and compares the complete header/data/CRLF
stream byte-for-byte with `runs/vmctl-vc152current/GAME.fle`. The strict-C89
host probe and fresh VC++ 1.52c/Win98 validation in `C:\DFSER4` both report
`DUMP_FLEETS_SERIALIZATION_CHECKS=26 FAILURES=0 RESULT=PASS`. Compile/link and
process exit passed, and the final guest window inventory had no Program Error
dialog.

This closes DumpFleets E4. Its E1 localized scanner/task, E2 serialized
field-order, and E3 packed service-resolution units are also closed, so all
four Step-5 evidence units are complete. Step-6 ledger promotion is now
recorded, and the procedure is at 6/6. The authoritative counters are now
**110/152 evidence units closed, 42 open, 11 promotion units, and 53 practical
open work items**. The remaining Phase 2 matrix contains 11 procedures at 5/6
and 40 promoted procedures at 6/6. Evidence is in
`analysis/compat/dump-fleets-serialization-e4.{md,json}`,
`compat/vc152/dump-fleets-serialization-e4/`, and
`runs/vmctl-vc152dfser4/`.

## Latest checkpoint — DumpFleets localized formatting/original-row E1

The production `stars_dump_fleets` path now exercises both its raw field-order
and formatted text boundaries. The strict-C89 host probe and fresh VC++
1.52c/Win98 validation in `C:\DFSER4` both report
`DUMP_FLEETS_SERIALIZATION_CHECKS=23 FAILURES=0 RESULT=PASS`. The matrix
checks the preserved original `GAME.fle` normal-row text oracle plus localized
fleet/planet/destination/battle-plan values and detailed task/scanner fields.
Compile/link and process exit passed, and the final guest window inventory had
no Program Error dialog.

This closes the named localized scanner/task serialization E1 unit. The
complete original export differential and final promotion remain open;
`DumpFleets` remains at 5/6. The authoritative counters are now **109/152
evidence units closed, 43 open, 13 promotion units, and 56 practical open
work items**. Evidence is in
`analysis/compat/dump-fleets-serialization-e4.{md,json}`,
`compat/vc152/dump-fleets-serialization-e4/`, and
`runs/vmctl-vc152dfser4/`.

## Latest checkpoint — DumpFleets serialized field-order E2

`stars_dump_fleets` now owns the recovered tabular field sequence while
leaving localization and numeric formatting at the callback boundary. The
normal report emits 12 fields; detailed mode emits 29, including route and
scanner/task services, mine/terraform values, the other-hull bucket, and all
four hull-class buckets.

Strict-C89 host and fresh VC++ 1.52c/Win98 validation in `C:\DFSER4` both
report `DUMP_FLEETS_SERIALIZATION_CHECKS=14 FAILURES=0 RESULT=PASS`.
Compile/link and process exit passed, and the final guest window inventory had
no Program Error dialog. This closes the named serialization field-order
unit, while localized text formatting, original export comparison, and
promotion remain open. Evidence is in
`analysis/compat/dump-fleets-serialization-e4.{md,json}`,
`compat/vc152/dump-fleets-serialization-e4/`, and
`runs/vmctl-vc152dfser4/`.

The authoritative counters are now **108/152 evidence units closed, 44 open,
13 promotion units, and 57 practical open work items**. `DumpFleets` remains
at 5/6 because localization and original-output evidence remain.

## Latest checkpoint — DumpPlanets and DumpFleets packed service-resolution E3

Both production report bridges now expose their recovered live-service seam.
`stars_dump_planets_packed` calls `resolve_services` after decoding the
56-byte `_planet` record, and `stars_dump_fleets_packed` calls the matching
hook after decoding the 124-byte `_fleet` record; the ordinary coordinators
then retain their existing detail and visibility gates.

The strict-C89 host probes and fresh VC++ 1.52c/Win98 replays both pass:
`DUMP_PLANETS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS` in `C:\DPSE3` and
`DUMP_FLEETS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS` in `C:\DFSE3`.
Both compile and link, exit 0, and finish with no Program Error dialog. This
closes one named packed-service integration unit for each procedure; localized
field formatting, complete serialized output, original row differentials,
and promotion remain open. Evidence is in
`analysis/compat/dump-planets-services-e3.{md,json}` and
`analysis/compat/dump-fleets-services-e3.{md,json}`, the matching packets,
and `runs/vmctl-vc152dpse3/` plus `runs/vmctl-vc152dfse3/`.

The authoritative counters are now **107/152 evidence units closed, 45 open,
13 promotion units, and 58 practical open work items**. `DumpPlanets` and
`DumpFleets` remain at 5/6 because their localization/serialization and
original-output units are still open.

## Latest checkpoint — ExecuteReportClick production packed bridge E5 native replay

Fresh VC++ 1.52c/Win98 validation in `C:\EXINT5` compiled and linked the
current production `execute_report_click.c`, packed probe, and `core.h`. The
native report is `EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`;
the process exited 0 and the final guest window list contained no Program
Error dialog. This confirms production decoding of the 124-byte `_fleet`,
56-byte `_planet`, and 14-byte `_btldata` records, all tested popup unions,
resource-exempt handling, and modal guards.

This closes the native production-lookup subcheck, not a named ExecuteReportClick
Step-5 unit. Exact original `GlobalPD` owner/part-pointer cases, live USER/GDI
handles, complete report caller/layout integration, original behavior, and
promotion remain open. Current counters remain **105/152 evidence units
closed, 47 evidence units open, 13 promotion units, and 60 practical open work
items**. Evidence is in
`analysis/compat/execute-report-click-packed-e5-native.{md,json}`,
`compat/vc152/execute-report-click-packed-e5-native/`, and
`runs/vmctl-vc152exint5/`.

## Latest checkpoint — DrawPopup packed-selector E5 native replay

Fresh production-source VC++ 1.52c/Win98 validation in `C:\DPE5R` reports
`DRAW_POPUP_PACKED_CHECKS=37 FAILURES=0 RESULT=PASS`; compile/link passed, the
process exited 0, and the final guest window list contained no Program Error
dialog. The matrix covers the current 22-byte `_popupdata` selector
precedence, every dispatcher kind, kind-14 slot/hull aliasing, geometry,
GDI-state callback order, and short-record fallback. A first compile-only
attempt in `runs/vmctl-vc152dpe5/` is preserved as superseded evidence because
the transfer omitted lowercase `core.h`; `runs/vmctl-vc152dpe5r/` is the
authoritative corrected replay.

This closes the native replay subcheck, not a named DrawPopup Step-5 unit.
Complete `GlobalPD` payloads, localization, hull filtering/formatting,
detailed GDI rendering, original caller differential, and promotion remain
open. Current counters remain **105/152 evidence units closed, 47 evidence
units open, 13 promotion units, and 60 practical open work items**. Evidence is
in `analysis/compat/draw-popup-packed-e5-native.{md,json}`,
`compat/vc152/draw-popup-packed-e5r-native/`, and
`runs/vmctl-vc152dpe5r/`.

## Latest checkpoint — SatisfyOrders raw transfer-failure E2 native replay

The production source now exposes `stars_satisfy_raw_failure_message` for the
remaining exact transfer-failure frames recovered from `turn3.c`: set-amount
`0x0121`, set-colonists `0x0122`, deep-space colonist rejection `0x0165`, and
futile mineral-packet transfer `0x011e`. The builder preserves the complete
eight-word `FSendPlrMsg` frames and the exact three-word `FSendPlrMsg2` frames;
the packed action union and selection conditions remain caller-resolved.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\SYOE22`
both report `SATISFY_ORDERS_RAW_FAILURE_CHECKS=44 FAILURES=0 RESULT=PASS`.
Compile/link passed, the process exited 0, and the final guest window list
contained no Program Error dialog. This is supporting E2 ABI evidence, not
named E2 closure: the original full-turn transfer differential, route/fuel
integration, E3 replay, and Step-6 promotion remain open. Current counters
remain **105/152 evidence units closed, 47 evidence units open, 13 promotion
units, and 60 practical open work items**. Evidence is in
`analysis/compat/satisfy-orders-raw-failure-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-failure-e2-native/`, and
`runs/vmctl-vc152syoe22/`.

## Latest checkpoint — SatisfyOrders technology-table/result binding E2 native replay

The resource-resolution technology branch now exposes the signed result from
the production `stars_itech_learn_a_tech` implementation through
`stars_bind_satisfy_technology`. Point destinations select the recovered
message pair `0x013c`/`0x013d`; planet destinations select `0x013e`/`0x013f`.
Negative results map to trader-part IDs and positive results map to the next
battle-technology ID. The strict-C89 host probe and fresh VC++ 1.52c/Win98
replay in `C:\SYOE11` report
`SATISFY_ORDERS_TECHNOLOGY_BINDING_E2_CHECKS=12 FAILURES=0 RESULT=PASS`;
the native wrapper records `EXIT=0`, compile/link passed, and the final guest
window inventory contained no Program Error dialog. The final packet uses
the normal `/O` setting. A malformed response file initially omitted
`SYO11.DEF`, causing linker warning L4021 and the earlier Win98 “not enough
space for arguments” dialog; adding the definition file fixed the packet.

This closes the signed technology-result/message-code boundary as supporting
E2 evidence, not named E2 closure. Complete raw message layouts, original
transfer differential, and E3 full-turn/original differential remain open.
Evidence is in
`analysis/compat/satisfy-orders-technology-binding-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-technology-binding-e2-native/`, and
`runs/vmctl-vc152syoe11/`.

## Latest checkpoint — SatisfyOrders player-design materialization E2 native replay

The created player-transfer branch now invokes a production `materialize_designs`
boundary. The helper reconstructs the original `IshFindSimilarDesign` and
record-copy sequence: equivalent active designs are reused, unmatched designs
take the first hidden destination slot, the complete 0x93-byte design record is
copied, destination identity flags are rewritten, the two accumulated design
totals are cleared, and per-design fleet count/status plus both 32-bit totals
are transferred. Host and fresh VC++ 1.52c/Win98 validation in `C:\SYOE10`
report `SATISFY_ORDERS_DESIGN_MATERIALIZATION_E2_CHECKS=15 FAILURES=0
RESULT=PASS EXIT=0`; compile/link passed and the final guest window inventory
contained no Program Error dialog. The packet uses production `common.c` and
`satisfy_orders.c`, with `/O` only to stay below the VC++ 1.52c code-segment
limit. This closes the player-specific design-record materialization/copying
boundary as supporting E2 evidence, not named E2 closure. Original
technology-table/result binding, complete raw message layouts, original
transfer differential, and E3 full-turn/original differential remain open.
Evidence is in
`analysis/compat/satisfy-orders-design-materialization-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-design-materialization-e2-native/`, and
`runs/vmctl-vc152syoe10/`.

## Latest checkpoint — SatisfyOrders fleet/design copy E2 native replay

The created player-transfer branch now has a distinct fleet/design service
boundary. The callback invokes the production `stars_find_similar_design`
helper for matching and no-match selection, then invokes
`stars_duplicate_fleet` from `common.c` and verifies the fixed 124-byte fleet
state, destination allocation, source preservation, and the variable 18-byte
order record tail. Host and fresh VC++ 1.52c/Win98 validation in `C:\SYOE9`
report `SATISFY_ORDERS_FLEET_COPY_E2_CHECKS=10 FAILURES=0 RESULT=PASS EXIT=0`;
compile/link passed, and the final guest window inventory contained no Program
Error dialog. The native packet uses small stubs only for unrelated
`common.c` helpers because full `utilgen.c` exceeds the VC++ 1.52c code
segment limit; the tested fleet-copy function is the production implementation.

This closes the design-selection helper and a concrete fleet-state-copy
boundary but not named E2 closure. Player-specific ship-design record
materialization/copying, original technology-table/result binding, complete
raw message layouts, original transfer differential, and E3 full-turn/original
differential remain open. Current counters remain **105/152 evidence
units closed, 47 evidence units open, 13 promotion units, and 60 practical open
work items**. Evidence is in
`analysis/compat/satisfy-orders-fleet-copy-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-fleet-copy-e2-native/`, and
`runs/vmctl-vc152syoe9/`.

## Latest checkpoint — SatisfyOrders concrete production-service E2 replay

The next SatisfyOrders replay bound the technology and point-drop callbacks to
the existing production services. The typed path invokes
`stars_itech_learn_a_tech` and checks research/event/result state; its point
drop invokes `stars_drop_salvage` and checks the actual two-object mineral
split, values, and coordinates. The packed path repeats the technology
service through raw 124-byte `_fleet` and 18-byte `_order` records. Host and
fresh VC++ 1.52c/Win98 validation in `C:\SYOE8` report
`SATISFY_ORDERS_PRODUCTION_E2_CHECKS=18 TYPED=12 PACKED=6 FAILURES=0
RESULT=PASS`; the native wrapper records `EXIT=0`. Compile/link passed and
the final guest window inventory contained no Program Error dialog.

This is stronger supporting E2 evidence, not named E2 closure. Original
technology-table/result binding, actual fleet/ship-design copying, complete
raw message argument layouts, original destruction-to-salvage caller
comparison, and E3 full-turn/original differential remain open. Current
counters remain **105/152 evidence units closed, 47 evidence units open, 13
promotion units, and 60 practical open work items**. Evidence is in
`analysis/compat/satisfy-orders-production-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-production-e2-native/`, and
`runs/vmctl-vc152syoe8/`.

## Latest checkpoint — SatisfyOrders advanced E2 native boundary replay

The production `SatisfyOrders` source now exposes explicit technology,
created-transfer ship-copy, point-drop salvage, and richer message-layout
service boundaries. The strict-C89 host and fresh VC++ 1.52c/Win98 replay in
`C:\SYOE7` both report
`SATISFY_ORDERS_ADVANCED_E2_CHECKS=20 FAILURES=0 RESULT=PASS`. Compile/link
and process exit passed, and the final guest window inventory contained no
Program Error dialog. This is supporting E2 evidence, not closure of the
named E2 unit: original technology-table/result binding, actual fleet/design
copying, concrete salvage object creation, complete raw message layouts, and
the E3 full-turn/original differential remain open.

Current counters remain **105/152 evidence units closed, 47 evidence units
open, 13 promotion units, and 60 practical open work items**. Evidence is in
`analysis/compat/satisfy-orders-advanced-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-advanced-e2-native/`, and
`runs/vmctl-vc152syoe7/`.

## Latest checkpoint — SatisfyOrders packed resource E1 native replay

Fresh host and VC++ 1.52c/Win98 production-source validation in `C:\SYOE6`
both report `SATISFY_ORDERS_PACKED_RESOURCE_E1_CHECKS=21 FAILURES=0
RESULT=PASS`. The raw 124-byte `_fleet`/18-byte `_order` matrix covers
source/capacity limiting, target-kind restrictions, exact transfer and
shortage messages, stat-percent mode, active/turn-flag write-back, and
pass-1 battle-plan dispatch. Compile/link and process exit passed, and the
final guest window inventory contained no Program Error dialog.

This closes SatisfyOrders E1 packed battle-plan/resource accounting. The
procedure is now **1/3 named Step-5 units closed** while remaining at **5/6**;
E2 technology/ship-copy/salvage/message-layout work and E3 full-turn/original
differential remain. Current counters are **105/152 evidence units closed,
47 evidence units open, 13 promotion units, and 60 practical open work items**.
Evidence is preserved in
`analysis/compat/satisfy-orders-packed-resource-e1-native.{md,json}`,
`compat/vc152/satisfy-orders-packed-resource-e1-native/`, and
`runs/vmctl-vc152syoe6/`.

## Latest checkpoint — DrawPlanShipBitmap planet-window E4 native replay and promotion

The current production `PlanetWndProc` WM_PAINT path now drives
`DrawPlanShipBitmap` through the recovered begin-paint/planet-paint/end-paint
sequence in fresh guest directory `C:\DPSBE4`. The 12-check matrix covers
packed fleet and planet selection, exact localized tile names, fleet dispatch,
ten planet-frame pattern operations, DIB source coordinates, and the
no-buttons positioning boundary. Host and native reports both pass
`DRAW_PLAN_SHIP_BITMAP_PLANET_WINDOW_E4_CHECKS=12 FAILURES=0 RESULT=PASS`;
compile/link and process exit passed, and the final guest window inventory
contained no Program Error dialog.

Together with the original Win98 planet/fleet frame and map-selection refresh
observation, this closes the named E4 planet-window/original differential
integration unit. The E4 packet, reconstruction note, review record, coverage
ledger, TODO5/TODO6, and this handoff are synchronized, so `DrawPlanShipBitmap`
is now **6/6 and promoted**.

The current counters are **38/51 procedures at 6/6, 13 at 5/6, 104/152
evidence units closed, 48 evidence units open, 13 promotion units, and 61
practical open work items**. Evidence is preserved in
`analysis/compat/draw-plan-ship-bitmap-planet-window-e4-native.{md,json}`,
`compat/vc152/draw-plan-ship-bitmap-planet-window-e4-native/`, and
`runs/vmctl-vc152dpsbe4/`.

## Latest checkpoint — DrawPlanShipBitmap localized-name E3 native replay

The current production `DrawPlanShipBitmap` source now preserves exact
localized fleet and planet names through the packed selection boundary. Fresh
host and Win98 validation in `C:\DPSBE3` both pass
`DRAW_PLAN_SHIP_BITMAP_LOCALIZED_E3_CHECKS=11 FAILURES=0 RESULT=PASS`;
compile/link and process exit passed, and the final guest window inventory
contained no Program Error dialog. This closes E3; only E4 complete
planet-window/original differential integration and promotion remain.

The current counters at the preceding E3 checkpoint were **37/51 procedures
at 6/6, 14 at 5/6, 103/152 evidence units closed, 49 evidence units open,
14 promotion units, and 63 practical open work items**; the current counters
are recorded in the newer E4 checkpoint above.

## Latest checkpoint — DrawPlanShipBitmap packed-selection E2 native replay

The current production `DrawPlanShipBitmap` source now consumes the complete
226-byte `_sel` view directly, decoding `grobj`/`id` at offsets 4/8 and
rejecting undersized records before dispatch. The strict-C89 and fresh Win98
replay in `C:\DPSBE2` both pass
`DRAW_PLAN_SHIP_BITMAP_PACKED_E2_CHECKS=11 FAILURES=0 RESULT=PASS`;
compile/link and process exit passed, and the final guest window inventory
contained no Program Error dialog. This closes the named E2 unit; E3
localized-name behavior and E4 complete planet-window/original differential
integration remain open.

The E2 checkpoint had **102/152 evidence units closed, 50 evidence units
open, 14 promotion units, and 64 practical open work items**; the current
counters are recorded in the newer E3 checkpoint above.

## Latest checkpoint — ReportDlg Step-6 promotion

With ReportDlg E1 packed-view loading, E2 populated rows/live USER-GDI-menu
payloads, and E3 native/original caller integration all closed, the review
record is now synchronized as **manually reviewed**. TODO5, TODO6, the
coverage ledger, and the reconstruction note now record ReportDlg as **6/6
and promoted**.

TODO5 now renders **833 reviewed / 13 partial / 0 open** game procedures;
that source-level count remains distinct from the stricter Phase 2 gate below.

The ReportDlg promotion checkpoint had **101/152 evidence units closed, 51
evidence units open, 14 promotion units, and 65 practical open work items**;
the current counters are recorded in the newer DrawPlanShipBitmap checkpoint
above.

## Latest checkpoint — ReportDlg populated E2 native replay

The current production `ReportDlg` plus `DrawReport` and `DrawReportItem`
sources now pass a fresh 43-check VC++ 1.52c/Win98 replay in `C:\RPE2`:
`REPORT_DLG_POPULATED_E2_CHECKS=43 FAILURES=0 RESULT=PASS`. The matrix drives
packed-view loading, creation, paint, and destruction for populated planet,
fleet, scan-fleet, and battle rows, while recording scrollbar/DC/paint,
header/text/GDI, row-lookup, and frame-menu cleanup payloads. Compile/link
passed, the process exited 0, and the final guest window inventory contained
no Program Error dialog.

This closes ReportDlg E2. Together with the already closed E1 and E3 units,
ReportDlg is now **3/3 named Step-5 units closed** but remains at the `5/6`
procedure gate until final Step-6 promotion. The authoritative burn-down is
now **101/152 evidence units closed, 51 open, 15 promotion units, 66 practical
open work items**. Evidence is preserved in
`analysis/compat/report-dlg-populated-e2-native.{md,json}`,
`compat/vc152/report-dlg-populated-e2-native/`, and
`runs/vmctl-vc152rptpe2/`.

## Latest checkpoint — ReportDlg populated caller-chain E3 native replay

The current production `ReportDlg` → `DrawReport` → `DrawReportItem` chain
now passes a fresh six-check VC++ 1.52c/Win98 replay in `C:\RPTDINT`:
`REPORT_DLG_DRAW_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS`. Compile/link
passed, the process exited 0, and the final guest window inventory contained
no Program Error dialog. The replay carries a populated `New Eden` row through
`WM_PAINT` into the report renderers. Combined with the original Reports-menu
→ Planet Summary Report → `Planet Name` header-popup observation, this closes
ReportDlg E3 native/original caller integration. ReportDlg is now **2/3 named
Step-5 units closed**; E2 exact populated report unions and live USER/GDI/menu
payloads, then final promotion, remain.

The authoritative burn-down is now **100/152 evidence units closed, 52 open,
15 promotion units, 67 practical open work items**. Evidence is preserved in
`analysis/compat/report-dlg-draw-integration-e3-native.{md,json}`,
`compat/vc152/report-dlg-draw-integration-e3-native/`, and
`runs/vmctl-vc152rptdint/`.

## Latest checkpoint — EstimateItemProdSched E1 original per-turn differential and promotion

The original-instruction-derived 8-check per-turn trace now passes in fresh
VC++ 1.52c / Win98 directory `C:\EIPOD` with
`ESTIMATE_ITEM_PROD_SCHED_ORIGINAL_DIFFERENTIAL_CHECKS=8 FAILURES=0 RESULT=PASS`.
It covers queue order, phase-5 turn break/resumption, first/last completion
turns, mineral/resource callback counts, population advance, special-item
rejection, and the 100-turn cutoff. Compile/link passed, the process exited 0,
and the final guest window inventory contained no Program Error dialog.

This closes E1 and synchronizes Step-6 promotion for `EstimateItemProdSched`;
its E2 packed mutation and E3 resource/queue units were already closed. The
authoritative burn-down is now **99/152 evidence units closed, 53 open, 15
promotion units, 68 practical open work items**. Evidence is preserved in
`analysis/compat/estimate-item-prod-sched-original-differential-e1-native.{md,json}`,
`compat/vc152/estimate-item-prod-sched-original-differential-native/`, and
`runs/vmctl-vc152eipod/`.

## Latest checkpoint — SatisfyOrders special-order dispatch native replay

The existing 16-case special-order matrix now passes through the production
source in fresh VC++ 1.52c / Win98 directory `C:\SYOE4` with
`SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=0 RESULT=PASS`. The replay covers
battle-plan pass-1 dispatch, mine capability/race gates and repeat counts,
same-owner starbase auto-route, merge ownership/callback gates, and
player-transfer source/recipient message pairs. Compile/link passed, the
process exited 0, and the final guest window inventory contained no Program
Error dialog.

This upgrades the special-order matrix from host-only to native evidence but
closes **0 named Step-5 units**. Packed battle-plan/resource accounting,
technology discovery, fleet/ship-design copying, salvage creation, complete
message layouts, and the full-turn/original differential remain open. The
authoritative burn-down remains **99/152 evidence units closed, 53 open, 15
promotion units, 68 practical open work items**. Evidence is preserved in
`analysis/compat/satisfy-orders-special-e4-native.{md,json}`,
`compat/vc152/satisfy-orders-special-native-e4/`, and
`runs/vmctl-vc152syoe4/`.

## Latest checkpoint — SatisfyOrders packed special-order bridge native replay

The production packed bridge now carries source-coordinate and auto-route
message fields and passes a fresh raw-record replay in `C:\SYOP5`:
`SATISFY_ORDERS_PACKED_SPECIAL_CHECKS=10 FAILURES=0 RESULT=PASS`. The packet
exercises 124-byte `_fleet` and 18-byte `_order` records through packed
battle-plan, mine, merge, pass-four auto-route, and player-transfer
creation/message paths. Compile/link passed, the process exited 0, and the
final guest window inventory contained no Program Error dialog.

This is stronger packed/native integration evidence but closes **0 named
Step-5 units**; technology discovery, fleet/ship-design copying, salvage,
complete message layouts, and the full-turn/original differential remain open.
The authoritative burn-down remains **99/152 evidence units closed, 53 open,
15 promotion units, 68 practical open work items**. Evidence is preserved in
`analysis/compat/satisfy-orders-packed-special-e5-native.{md,json}`,
`compat/vc152/satisfy-orders-packed-special-e5/`, and
`runs/vmctl-vc152syop5/`.

## Latest checkpoint — DrawPlanetShipList E4 complete integration and promotion

The current production source now passes the combined complete-integration
trace in fresh VC++ 1.52c / Win98 directory `C:\DLP4E` with
`DRAW_PLANET_SHIP_LIST_E4_CHECKS=12 FAILURES=0 RESULT=PASS`. The trace covers
packed selection/transfer decoding, tile-change hide/reset cleanup, exact
combo geometry and cached-width flags, fleet detail rectangle and callback
order, the `1,2,0` button mapping, thing-mode gating, unavailable-transfer
suppression, and failed-tile cleanup. Compile/link passed, the process exited
0, and the final guest window inventory contained no Program Error dialog.

Together with the original fleet-frame evidence, this closes E4 complete
drawing/transfer/original integration. `DrawPlanetShipList` is now promoted to
`6/6`. The authoritative burn-down is **98/152 evidence units closed, 54
open, 16 promotion units, 70 practical open work items**. Evidence is
preserved in `analysis/compat/draw-planet-ship-list-e4-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-e4-production-native/`, and
`runs/vmctl-vc152dlep4/`.

## Latest checkpoint — DrawPlanetShipList E3 detail-render replay

The current production detail-render path now passes in fresh VC++ 1.52c /
Win98 directory `C:\DLD3` with
`DRAW_PLANET_SHIP_LIST_DETAIL_RENDER_CHECKS=9 FAILURES=0 RESULT=PASS`. The
trace verifies fleet/thing detail dispatch, the exact detail rectangle,
three-button order and geometry, callback-before-button sequencing,
unavailable-transfer suppression, and ship-mode suppression. Compile and link
passed, the process exited 0, and the final guest window inventory contained
no Program Error dialog.

The original fleet-frame capture shows the corresponding composition,
fuel/cargo, and waypoint controls. This closes `DrawPlanetShipList` E3; E4
complete drawing/transfer integration, broader original differential coverage,
and promotion remain. The authoritative burn-down is now **97/152 evidence
units closed, 55 open, 17 promotion units, 72 practical open work items**.
Evidence is preserved in
`analysis/compat/draw-planet-ship-list-detail-render-e3-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-details-e3-production-native/`, and
`runs/vmctl-vc152dld3p/`.

## Latest checkpoint — DrawPlanetShipList E1 production packed replay

The current production `DrawPlanetShipList` source and packed-record probe
now pass in fresh VC++ 1.52c / Win98 directory `C:\DLP4` with
`DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`. The replay
covers the 226-byte selection view, 128-byte transfer view, raw identity/kind/
owner precedence, fleet and thing modes, owner enablement, visibility/layout,
and no-object suppression. Compile and link passed, the process exited 0, and
the final guest window inventory contained no Program Error dialog.

This closes `DrawPlanetShipList` E1. E2 exact controls/geometry is already
closed; E3 localized text/gauges/detail rendering, E4 complete
drawing/transfer integration, original differential coverage, and promotion
remain. The authoritative burn-down is now **96/152 evidence units closed,
56 open, 17 promotion units, 73 practical open work items**. Evidence is
preserved in
`analysis/compat/draw-planet-ship-list-packed-production-e4-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-packed-production-e4-native/`, and
`runs/vmctl-vc152dlp4/`.

## Latest checkpoint — Planet information display E3 native replay

The shared nine-case `PtDisplayPlanetPopInfo` and
`PtDisplayPlanetStateInfo` branch harness now passes in fresh VC++ 1.52c /
Win98 directory `C:\PBR3` with
`PLANET_INFO_BRANCH_CHECKS=9 FAILURES=0 RESULT=PASS`. It exercises the
complete stream/wrap callback seam: owned, unowned, and foreign headers,
negative desirability, population limits, growth warnings, known and unknown
player limits, environment/desirability services, signed differences,
wrapping, and returned layout. Compile and link passed, the process exited 0,
and the final guest window inventory contained no Program Error dialog.

The original `Oop Be Gone` Small Screen capture supplies the visible
comparison: gravity, temperature, radiation, and mineral rows in the state
pane, plus value, population, and current-report rows in the population pane.
This closes E3 for both procedures and completes their Step-6 ledger
promotions. Both rows are now `6/6`. The authoritative burn-down is now
**95/152 evidence units closed, 57 open, 17 promotion units, 74 practical
open work items**; TODO5 reports **829 reviewed and 17 partial**. Evidence is preserved in
`analysis/compat/planet-info-branch-e3-native.{md,json}`,
`compat/vc152/planet-info-branch-e3-native/`, and
`runs/vmctl-vc152pbr3/`.

## Latest checkpoint — EstimateItemProdSched E2 packed mutation

The strict-C89 packed mutation probe now covers complete queue consumption,
partial quantity write-back, and failed-object resource rollback. It preserves
packed type/object bits, checks all four resource lanes, and verifies that a
failed object creation sends no completion message. Fresh VC++ 1.52c / Win98
validation in `C:\EIPM2` reports
`ESTIMATE_ITEM_PROD_SCHED_MUTATION_CHECKS=3 FAILURES=0 RESULT=PASS`; compile
and link passed, the process exited 0, and the final guest window inventory
contained no Program Error dialog.

This closes the `EstimateItemProdSched` E2 production-record/resource
mutation unit. E1's independent original per-turn/complete schedule
differential remains open; E3 resource/queue integration is already closed,
and the procedure remains `5/6` until E1 and the separate Step-6 promotion are
complete. The authoritative burn-down is now **93/152 evidence units closed,
59 open, 19 promotion units, 78 practical open work items**. Evidence is
preserved in `analysis/compat/estimate-item-prod-sched-mutation-e2.{md,json}`,
`compat/vc152/estimate-item-prod-sched-mutation-e2/`, and
`runs/vmctl-vc152eipm2/`.

## Latest checkpoint — DrawPlanShipBitmap native boundary replays

With `STARSAGT 0.4.2` restored, fresh VC++ 1.52c / Win98 packets replayed the
fleet-forwarding boundary in `C:\DPSBFLE4` and the tile-failure/hidden-button
boundary in `C:\DPSBFAI4`. The native reports are respectively
`DRAW_PLAN_SHIP_BITMAP_FLEET_CHECKS=6 FAILURES=0 RESULT=PASS` and
`DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=6 FAILURES=0 RESULT=PASS`. Both compiled
and linked, exited 0, and the final guest window inventory contained no
Program Error dialog.

This upgrades both focused boundaries from host-only to host/native evidence,
but closes 0 named Step-5 units. Exact resource handles, complete
packed-selection ownership, localized names, planet-window ownership/redraw,
original differential coverage, and promotion remain open. The authoritative
burn-down remains **92/152 evidence units closed, 60 open, 19 promotion units,
79 practical open work items**. Evidence is preserved in
`analysis/compat/draw-plan-ship-bitmap-native-boundaries-e4.{md,json}`,
`compat/vc152/draw-plan-ship-bitmap-{fleet-e4,failure-e4}/`, and
`runs/vmctl-vc152dpsbfleet-e4/` plus
`runs/vmctl-vc152dpsbfailure-e4/`.

## Latest checkpoint — DrawPlanetShipList native detail replay

Fresh VC++ 1.52c / Win98 validation in `C:\DLISTDE3` passes
`DRAW_PLANET_SHIP_LIST_DETAILS_CHECKS=9 FAILURES=0 RESULT=PASS` for resolved
fleet detail dispatch, unavailable-transfer suppression, and ship-mode
suppression. Compile/link pass, the process exits 0, and the final guest
window inventory contains no Program Error dialog. This closes 0 named Step-5
units: localized text/gauges, complete transfer behavior, exact
caller/resource integration, original differential coverage, and promotion
remain open. The authoritative burn-down remains **92/152 evidence units
closed, 60 open, 19 promotion units, 79 practical open work items**. Evidence
is preserved in `analysis/compat/draw-planet-ship-list-details-e3-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-details-e3-native/`, and
`runs/vmctl-vc152dlist-details-e3/`.

## Latest checkpoint — DrawPlanetStats E4 and promotion

Fresh `C:\DPSWPE4` VC++ 1.52c validation passes both the 22-check
`DrawPlanetStats` source geometry/row probe and the 8-check real
`PlanetWndProc(WM_PAINT)` integration replay. Compile/link pass and the final
window inventory contains no Program Error dialog. The resumed original frame
was captured after dismissing the Production Queue dialog; `Oop Be Gone`
retained its mineral/status, population/resources, scanner/defense,
production/fleet, and environment panels.

DrawPlanetStats E4 is closed and its Step-6 ledger promotion is synchronized.
The authoritative burn-down is now **92/152 evidence units closed, 60 open,
19 promotion units, 79 practical open work items**; TODO5 now reports **827
reviewed and 19 partial** procedures. Evidence is preserved in
`analysis/compat/draw-planet-stats-gdi-original-e4.{md,json}`,
`compat/vc152/planet-stats-wndproc-e4/`, and
`runs/vmctl-vc152dpswpe4/` plus
`runs/vmctl-vc152dps-original-e4.bmp`.

## Latest checkpoint — FAttack raw stateful damage dispatch

Fresh `C:\FAPNE14` VC++ 1.52c validation now exercises an optional raw
damage callback after the integrated raw beam scaling path. The callback
routes the selected 39-point event through `FDamageTok`; the host and native
probes pass 4 checks covering 6-point shield absorption, 33-point residual
hull damage, two-ship casualty mutation, and one FAttack hit/battle-record
update. Compile/link pass and the final window inventory contains no Program
Error dialog.

This is supporting evidence only and closes 0 named Step-5 units. Complete
raw part-dependent scaling, capacity/jam formulas, complex torpedo selection,
original differential, full replay, and Step-6 promotion remain open. The
authoritative burn-down remains **91/152 evidence units closed, 61 open, 20
promotion units, 81 practical open work items**. Evidence is preserved in
`analysis/compat/f-attack-raw-damage-e14.{md,json}`,
`compat/vc152/f-attack-complex-native-e14/`, and
`runs/vmctl-vc152fapne14/`.

## Latest checkpoint — FAttack raw beam dispatch scaling

Fresh `C:\FAPNE13` VC++ 1.52c validation now exercises the first
post-selection raw complex-beam damage path. The typed source applies
attacker capacity, target beam defense, and truncated distance/part-DP
attenuation before the damage callback; the host and native probes pass 3
checks, compile/link pass, and the post-run window inventory contains no
Program Error dialog.

This is supporting evidence only and closes 0 named Step-5 units. Remaining
part-dependent scaling, capacity/jam branches, shield/hull follow-up dispatch,
complex torpedo selection, original differential, full replay, and Step-6
promotion remain open. The authoritative burn-down remains **91/152 evidence
units closed, 61 open, 20 promotion units, 81 practical open work items**.
Evidence is preserved in `analysis/compat/f-attack-beam-dispatch-e13.{md,json}`,
`compat/vc152/f-attack-complex-native-e13/`, and
`runs/vmctl-vc152fapne13/`.

## Latest checkpoint — FAttack complex target selection integration

Fresh `C:\FAPNE12` VC++ 1.52c validation now exercises the raw complex
target-score ranking inside the readable `stars_f_attack` candidate loop.
With `raw_scaling` enabled, the higher-value eligible target is selected while
the callback receives ordinary beam damage and the battle-record hit fields
remain correct. The host and native selection probes pass 3 checks;
compile/link pass and the post-run window inventory contains no Program Error
dialog.

This is supporting evidence only and closes 0 named Step-5 units. Complete
part-dependent scaling, capacity/jam behavior, distance and shield/hull
dispatch, original differential, full replay, and Step-6 promotion remain
open. The authoritative burn-down remains **91/152 evidence units closed, 61
open, 20 promotion units, 81 practical open work items**. Evidence is
preserved in `analysis/compat/f-attack-complex-selection-e12.{md,json}`,
`compat/vc152/f-attack-complex-native-e12/`, and
`runs/vmctl-vc152fapne12/`.

## Latest checkpoint — FAttack packed `_dv` target-score modifier

Fresh `C:\FAPNE11` VC++ 1.52c validation now compiles and links the typed
`_tok.dv` target-score extension. The strict host probe and native report both
pass 9 checks, including the low-seven-bit `pctSh`/upper-nine-bit `pctDp`
modifier and the raw truncating hull-reduction sequence. The native report is
`F_ATTACK_COMPLEX_SCORE_CHECKS=9 FAILURES=0 RESULT=PASS`, compile/link pass,
and the post-run window inventory contains no Program Error dialog.

This is supporting evidence only and closes 0 named Step-5 units. Complete raw
complex-beam target selection and dispatch, weapon-part scaling/capacity/jam
branches, original differential, full replay, and promotion remain open. The
authoritative burn-down remains **91/152 evidence units closed, 61 open, 20
promotion units, 81 practical open work items**. Evidence is preserved in
`analysis/compat/f-attack-defense-modifier-e11.{md,json}`,
`compat/vc152/f-attack-complex-native-e11/`, and
`runs/vmctl-vc152fapne11/`.

## Latest checkpoint — FAttack complex target-score source slice

Fresh `C:\FAPNE10` VC++ 1.52c validation now compiles and links the first
typed source boundary for the raw `FAttack` complex-beam target-score branch.
The host and native probes both pass 8 checks, including resource/ironium
valuation, saturation, beam-defense reduction, shield-only scoring, zero
shield rejection, minimum-hull handling, and null safety. The native report
records `MIN_HULL_ACTUAL=18181 EXPECTED=18181`; compile/link pass and the
post-run window inventory contains no Program Error dialog. The initial
rerun used the stale `C:\RUNVC.BAT` wrapper pointed at `C:\VC152D`; a
package-specific CRLF wrapper corrected that and produced the authoritative
run in `runs/vmctl-vc152fapne10/`.

This is supporting evidence only and closes 0 named Step-5 units. The typed
helper is a source boundary, not the complete raw implementation: remaining
complex-beam capacity/jam/dispatch behavior, broader original differentials,
full battle replay, and Step-6 promotion remain open. The authoritative
burn-down remains **91/152 evidence units closed, 61 open, 20 promotion units,
81 practical open work items**.

Evidence is preserved in
`analysis/compat/f-attack-complex-score-e10.{md,json}`,
`compat/vc152/f-attack-complex-native-e10/`, and
`runs/vmctl-vc152fapne10/`.

## Latest checkpoint — FAttack native lookup and typed-scaling support

Fresh `C:\FAPNE9` VC++ 1.52c validation compiled and linked the current
production `lookup_part.c` and `f_attack.c` sources. The native table-family,
packed `_hs` resolver, and complete scaling reports pass with zero failures:
`LOOKUP_PART_TABLE_CHECKS=34`, `F_ATTACK_PACKED_PART_CHECKS=13`, and
`F_ATTACK_CHECKS=34`. Compile/link logs are warning-free; each executable
exited 0 and the window inventory contained no Program Error dialog. This
closes the native lookup/resolver subcheck and supplies native support for the
current typed scaling matrix, but it does not close `FAttack` E1: the raw
complex-beam hull/resource/shield/jam/capacity formulas remain to be modeled
and compared. E2 broader original damage differentials, E3 full battle replay,
and Step-6 promotion also remain. Evidence is in
`analysis/compat/f-attack-packed-native-e9.{md,json}` and
`runs/vmctl-vc152fapne9/`.

## Latest checkpoint — final native combat-chain replay

The production `FDoCoolBattle`/`FAttack` chain was rerun in `C:\CBATP` after
correcting the only compiler warning found in the first attempt: the
section-index comparison in `FAttack` now explicitly uses an unsigned 16-bit
value. The final2 build is warning-free, compiles and links under VC++ 1.52c,
reports `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS`,
exits 0, and has no Program Error dialog. This remains supporting native
evidence and closes 0 named Step-5 units because packed native part binding,
complete damage scaling, original differential, full replay, and promotion
are still open. Evidence: `runs/vmctl-vc152cbatp/COOLATSTA-final2.TXT`,
`COOLACL-final2.LOG`, `COOLATLINK-final2.LOG`, and `CBATRES-final2.TXT`.

## Objective

Continue replacing byte-replay probes with readable, genuine C source
reconstructions. Preserve the original executable's behavior, validate against
the original executable where practical, and keep `TODO5.md` current. The
native compiler environment is Microsoft Visual C++ 1.52c in the Win98 `StarDev`
86Box guest; do not substitute DOSBox.

## TODO5 state

The earlier handoff reported 787 formally reviewed procedures, 52 documented
partial procedures, and 7 open procedures. Phase 0 repaired the stale review
keys and regenerated `TODO5.md`; its current authoritative displayed totals
are:

- 846 named procedures and 890,384 matched bytes
- 833 formally reviewed
- 13 documented partial
- 0 open
- 812,172 / 890,384 named procedure bytes credited (91.22%)

The headline totals do not move when a partial procedure receives another
verified slice. A procedure must be promoted from partial to formally reviewed
before the reviewed and credited-byte totals change.

## Current TODO6 Step-5 workload

The current Phase 2 snapshot is **37 procedures promoted at 6/6 and 14 still
at 5/6**. All 51 have Steps 1–4 complete and have reached the Step-5 evidence
gate, but that gate is a work queue rather than one final test. The detailed
scorecard in
[`TODO6.md`](../../TODO6.md) records the per-procedure evidence state:

- **152 named Step-5 evidence units** are defined across the 51 procedures;
  **103 are closed and 49 remain**.
- **14 promotion units** remain after those evidence packets close.
- The current practical burn-down is therefore **63 open work items**: 49
  behavior/integration units plus 14 final ledger-promotion units.

The latest checkpoint promotes `DrawProductionDlg` to `6/6`. Its E3 geometry
and GDI/caller packet passed the strict-C89 and fresh VC++ 1.52c/Win98 9-check
probes, with empty redirected output and no Program Error dialog; the existing
original Ship & Starbase Designer observation supplies the caller boundary.
The final review-ledger, coverage, TODO5, and reconstruction-note promotion is
recorded in `tracking/reviews.json`, `analysis/coverage.json`, and `TODO5.md`.
Evidence is preserved in `analysis/compat/draw-production-dlg-geometry-e3.md`
and `runs/vmctl-vc152dpdre3/`. At that checkpoint TODO6 was **82/152
evidence units closed, 70 open, 21 promotions remaining, 91 practical open
items**; the current counters below include the subsequent ProductionDlg E2
closure.

The current resumed-VM checkpoint closes ProductionDlg E2 for the direct
resource-ID/callback-order contract. The raw body has only the `0x00d3`
copied-popup lookup and the `0x04c0` compressed queue-menu lookup; the fresh
`C:\PDGRS4` VC++ 1.52c/Win98 run passes the corrected 8-check report with empty
redirected output and no Program Error dialog. The live original right-click
menu shows `<Default>` and `<Customize>`, confirming the editor-entry boundary;
the `0x00d3` localized spelling remains intentionally unasserted. The original
second-row selection then opened `Customize Production Templates`, and Escape
returned to ProductionDlg without a Program Error dialog. ProductionDlg is now
promoted at **3/3** Step-5 evidence and **6/6**, with no remaining evidence or
promotion work.
Evidence is preserved in
`analysis/compat/production-dlg-resource-e2-complete.{md,json}` and
`runs/vmctl-vc152pdgrs3/` plus `runs/vmctl-vc152pdgrs4/`. Evidence is also in
`analysis/compat/production-dlg-original-e3.{md,json}`. TODO6 was **85/152
evidence units closed, 67 open, 20 promotions remaining, 87 practical open
items** at this checkpoint; the current counters below include the subsequent
DrawPlanetStats and EstimateItemProdSched milestones.

The next checkpoint closes DrawPlanetStats E3 service resolution. The strict
host 12-check packet and fresh `C:\DPSSE3` VC++ 1.52c/Win98 packet both pass;
the native report is `DRAW_PLANET_STATS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS`,
redirected output is empty, and no Program Error dialog appears. The production
bridge now forwards the selected player and typed packed service values through
all seven value rows. DrawPlanetStats is now **3/4** Step-5 units closed; only
detailed GDI/original differential and promotion remain.

The following checkpoint closes one `EstimateItemProdSched` Step-5 unit. The
resource/queue service packet was replayed in fresh guest directory `C:\EIRQ`.
The corrected VC++ 1.52c build records `COMPILE=PASS` and `LINK=PASS`, and
`EIRQR.TXT` records
`ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=8 FAILURES=0 RESULT=PASS`.
The first link failure was limited to the probe's `_printf` dependency; the
probe-local summary was changed to `puts` and the rerun completed with no
Program Error dialog. `EstimateItemProdSched` is now **1/3** at its Step-5
acceptance-unit level, with the E3 resource/queue replay closed; E1
turn-schedule/production-record differential and E2 complete
production-record/resource mutation remain. The authoritative burn-down is
now **86/152 evidence units closed, 66 open, 20 promotions remaining, 86
practical open items**. Evidence is preserved in
`analysis/compat/estimate-item-prod-sched-resource-queue-e3.{md,json}` and
`runs/vmctl-vc152eirq2/`.

The next native checkpoint replays the broader `EstimateItemProdSched` E1
schedule differential in fresh `C:\EIPD`. VC++ 1.52c records `COMPILE=PASS`
and `LINK=PASS`, and `EIPDR.TXT` records
`ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=21 FAILURES=0 RESULT=PASS`.
The post-run window list contains no Program Error dialog. This closes the
native replay sub-check for phase-1/phase-2 completion, the 100-turn fallback,
special-item and research-exempt paths, mineral refresh, copied-planet
isolation, and population timing. It does not yet close the authoritative E1
unit because complete packed production-record/resource mutation and original
per-turn queue/resource replay remain; the Step-5 score therefore remains
**1/3**, and the authoritative burn-down remains **86/152 evidence units
closed, 66 open, 20 promotions remaining, 86 practical open items**. Evidence
is preserved in `analysis/compat/estimate-item-prod-sched-differential-e2.{md,json}`
and `runs/vmctl-vc152eipd1/`.

The following native checkpoint closes `PtDisplayPlanetPopInfo` E2. The
localization/fixed-point packet was replayed in fresh `C:\PPFE2`; VC++ 1.52c
records `COMPILE=PASS` and `LINK=PASS`, and `PPFE2.TXT` records
`PT_DISPLAY_PLANET_POP_INFO_FORMAT_CHECKS=27 FAILURES=0 RESULT=PASS`. The
post-run window list contains no Program Error dialog. This closes the E2
localized/fixed-point formatting unit; E1 complete packed planet/player lookup
and E3 detailed stream/wrap GDI plus original comparison remain. The current
authoritative burn-down is **87/152 evidence units closed, 65 open, 20
promotions remaining, 85 practical open items**. Evidence is preserved in
`analysis/compat/pt-display-planet-pop-info-format-e2.{md,json}` and
`runs/vmctl-vc152ppfe2/`.

The following native checkpoint closes `PtDisplayPlanetStateInfo` E2. The
localization/fixed-point packet was replayed in fresh `C:\STFE2`; VC++ 1.52c
records `COMPILE=PASS` and `LINK=PASS`, and `STFE2.TXT` records
`PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS`. The
post-run window list contains no Program Error dialog. This closes the E2
localized/fixed-point formatting unit; E1 complete packed player/planet lookup
and E3 detailed stream/wrap GDI plus original comparison remain. The current
authoritative burn-down is **88/152 evidence units closed, 64 open, 20
promotions remaining, 84 practical open items**. Evidence is preserved in
`analysis/compat/pt-display-planet-state-info-format-e2.{md,json}` and
`runs/vmctl-vc152stfe2/`.

The following native checkpoint closes `PtDisplayPlanetPopInfo` E1. A fresh
production-source packed-planet bridge packet ran in `C:\PPPE4`; the host C89
probe and native report both record
`PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS`, with
`COMPILE=PASS`, `LINK=PASS`, and no Program Error dialog. Validation corrected
two probe defects exposed by the native ABI: a valid 56-byte packed record
bypasses `lookup_planet`, while a 55-byte record uses lookup fallback; the
final self-count assertion was removed so the report represents 21 semantic
checks. This closes E1 complete packed planet/player lookup. With E2
formatting already closed, `PtDisplayPlanetPopInfo` is now **2/3** at Step 5;
E3 detailed stream/wrap GDI/original comparison and promotion remain. The
current authoritative burn-down is **89/152 evidence units closed, 63 open,
20 promotions remaining, 83 practical open items**. Evidence is preserved in
`analysis/compat/pt-display-planet-pop-info-packed-production-e4.{md,json}` and
`runs/vmctl-vc152pppe4/`.

The following native checkpoint closes `PtDisplayPlanetStateInfo` E1. A fresh
production-source packed-record bridge packet ran in `C:\STPE4`; the host and
native reports both record
`PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS`, with
`COMPILE=PASS`, `LINK=PASS`, and no Program Error dialog. The packet includes
the 56-byte `_planet` plus 22-byte `_popupdata` precedence and a short-record
typed-fallback preservation assertion. This closes E1 complete packed
player/planet lookup. E2 formatting is closed separately, so
`PtDisplayPlanetStateInfo` is now **2/3** at Step 5; E3 detailed stream/wrap
GDI/original comparison and promotion remain. The current authoritative
burn-down is **90/152 evidence units closed, 62 open, 20 promotions remaining,
82 practical open items**. Evidence is preserved in
`analysis/compat/pt-display-planet-state-info-packed-production-e4.{md,json}`
and `runs/vmctl-vc152stpe4/`.

The following native checkpoint closes `ReportDlg` E1 packed-view loading. A
fresh production-source packet ran in `C:\RPTVEP`; VC++ 1.52c records
`COMPILE=PASS` and `LINK=PASS`, and `RPTVE-final.TXT` records
`REPORT_DLG_VIEW_CHECKS=13 FAILURES=0 RESULT=PASS`. The process exited 0 and
the post-run window list contained no Program Error dialog. ReportDlg is now
**1/3** at its named Step-5 evidence level, represented as `2 + P` remaining:
populated report unions/rows plus live USER/GDI/menu payloads, native/original
caller integration, and final promotion remain. The current authoritative
burn-down is **91/152 evidence units closed, 61 open, 20 promotions remaining,
81 practical open items**. Evidence is preserved in
`analysis/compat/report-dlg-view-e1.{md,json}`,
`compat/vc152/report-dlg-view-e1-production/`, and
`runs/vmctl-vc152rptvep/`.

The next native checkpoint replays the DumpFleets coordinator boundary through
the production source in fresh `C:\DFBP`. VC++ 1.52c records `COMPILE=PASS`
and `LINK=PASS`, and `DFBR.TXT` records
`DUMP_FLEETS_BOUNDARY_CHECKS=20 FAILURES=0 RESULT=PASS`. The process exited 0
and the post-run window list contained no Program Error dialog. This closes
the native visibility/coordinator boundary replay but **0** named Step-5 units:
localized scanner/task/minefield/battle-plan serialization, complete packed
service integration, original output differential, and promotion remain. The
authoritative burn-down remains **91/152 evidence units closed, 61 open, 20
promotions remaining, 81 practical open items**. Evidence is preserved in
`analysis/compat/dump-fleets-boundary-e3.{md,json}`,
`compat/vc152/dump-fleets-boundary-production-e3/`, and
`runs/vmctl-vc152dfbp/`.

The following native checkpoint replays the SatisfyOrders/DoOrders turn
sequencing chain in fresh `C:\SYTURN`. VC++ 1.52c records `COMPILE=PASS` and
`LINK=PASS`, and `SYTRES.TXT` records
`SATISFY_ORDERS_TURN_CHECKS=8 FAILURES=0 RESULT=PASS` for both pre- and
post-movement sequencing. The process exited 0 and the post-run window list
contained no Program Error dialog. This closes the native turn-sequencing
replay subcheck but **0** named Step-5 units: packed battle-plan/resource
accounting, special-order integration, technology and ship-copy paths,
salvage/message layouts, original behavior, full-turn differential, and
promotion remain. The authoritative burn-down remains **91/152 evidence units
closed, 61 open, 20 promotions remaining, 81 practical open items**. Evidence
is preserved in `analysis/compat/satisfy-orders-turn-e3.{md,json}`,
`compat/vc152/satisfy-orders-turn-e3/`, and `runs/vmctl-vc152syturn/`.

The following native checkpoint replays the DumpPlanets coordinator boundary
through the production source in fresh `C:\DPBP`. VC++ 1.52c records
`COMPILE=PASS` and `LINK=PASS`, and `DPBR.TXT` records
`DUMP_PLANETS_BOUNDARY_CHECKS=24 FAILURES=0 RESULT=PASS`. The process exited 0
and the post-run window list contained no Program Error dialog. This closes
the native visibility/coordinator boundary replay but **0** named Step-5 units:
localized derived-field serialization, complete packed service integration,
original output differential, and promotion remain. The authoritative
burn-down remains **91/152 evidence units closed, 61 open, 20 promotions
remaining, 81 practical open items**. Evidence is preserved in
`analysis/compat/dump-planets-boundary-e3.{md,json}`,
`compat/vc152/dump-planets-boundary-production-e3/`, and
`runs/vmctl-vc152dpbp/`.

The following native checkpoint replays the ExecuteReportClick popup caller
through production `ExecuteReportClick`, `Popup`, and `DrawPopup` sources in
fresh `C:\POPINTP`. VC++ 1.52c records `COMPILE=PASS` and `LINK=PASS`, and
`POPINT-final.TXT` records
`EXECUTE_REPORT_CLICK_POPUP_INTEGRATION_CHECKS=26 FAILURES=0 RESULT=PASS`.
The process exited 0 and the post-run window list contained no Program Error
dialog. This closes the native popup-caller replay subcheck but **0** named
Step-5 units: exact original `GlobalPD` owner/part unions, localized content,
live USER/GDI behavior, complete report-layout integration, original
comparison, and promotion remain. The authoritative burn-down remains
**91/152 evidence units closed, 61 open, 20 promotions remaining, 81
practical open items**. Evidence is preserved in
`analysis/compat/execute-report-click-popup-integration-e3.{md,json}`,
`compat/vc152/execute-report-click-popup-production-e3/`, and
`runs/vmctl-vc152popintp/`.

The next ReportDlg paint-view packet in fresh `C:\RPTPVP` compiled and linked
the current production source and reached the typed callback, writing
`REPORT_DLG_PAINT_VIEW_CHECKS=6 FAILURES=0 RESULT=PASS`. Its ordinary WinMain
return path then produced a Win98 illegal-operation dialog and exit 130; the
dialog screenshot is preserved in `runs/vmctl-vc152rptpvpe2/hung.png`. A
controlled rerun using explicit `ExitProcess` exited 0 with the same report and
no Program Error dialog. This is native supporting evidence plus a
reproducible teardown/ABI defect, not a named Step-5 closure. The authoritative
burn-down remains **91/152 evidence units closed, 61 open, 20 promotions
remaining, 81 practical open items**. Evidence is preserved in
`analysis/compat/report-dlg-paint-view-e2.{md,json}`,
`compat/vc152/report-dlg-paint-view-production-e2/`, and
`runs/vmctl-vc152rptpvpe2/`.

The following combat checkpoint replays the production FDoCoolBattle→FAttack
chain in fresh `C:\CBATP`. VC++ 1.52c records `COMPILE=PASS` and `LINK=PASS`,
and `CBATRES.TXT` records
`F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS`. The
process exited 0 and the post-run window list contained no Program Error
dialog. This closes the native battle-chain replay subcheck but **0** named
Step-5 units: packed token/global mapping, exact part lookup and damage
scaling, complete battle records, full replay, original differential, and
promotion remain. The authoritative burn-down remains **91/152 evidence units
closed, 61 open, 20 promotions remaining, 81 practical open items**. Evidence
is preserved in
`analysis/compat/f-do-cool-battle-attack-integration-e3.{md,json}`,
`compat/vc152/f-do-cool-battle-attack-production-e3/`, and
`runs/vmctl-vc152cbatp/`.

The following native checkpoint closes `ProductionDlg` E1. Fresh `C:\PDGP`
validation compiled and linked the raw `vrgZipProd` adapter and returned
`PRODUCTION_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS`; redirected output was
empty and the Win98 window list contained no Program Error dialog. The packet
checks the 40-byte `_zipprodq` fields, five-record storage cap, twelve packed
item words, sparse valid slots, and the original four-entry visible/menu cap.
ProductionDlg is now `1/3` on Step-5 evidence with `2 + P` remaining. Evidence
is in `analysis/compat/production-dlg-packed-e1.{md,json}` and
`runs/vmctl-vc152pdgp/`.

The latest host-only checkpoint expands the FDoCoolBattle Step-5 gate. A
strict-C89 `F_DO_COOL_BATTLE_PACKED_TOKEN_CHECKS=11` probe now round-trips two
29-byte packed `_tok` records through the real coordinator: load after
initialization, stores after movement/attack/reporting, preservation of every
raw byte on the survivor, and clearing of the defeated token's count and
active bit. This closes **0** authoritative Step-5 units. FDoCoolBattle's
remaining Step-5 concerns are now itemized as exact native `_tok`/`vrgtok`
binding and compaction/production callers, exact movement geometry and
speed/initiative behavior, complete damage/shield/casualty/salvage/bombing
records, fresh VC++/Win98 no-error execution, and original full-battle replay;
the complete 29-byte host field map is now checked.
Evidence is in
`analysis/compat/f-do-cool-battle-packed-token-e6.{md,json}` and
`tests/f_do_cool_battle_packed_token_probe.c`. The authoritative counters stay
**76/152 closed, 76 open, 22 promotion units, 98 practical open work items**.

The paired FAttack combat checkpoint now also consumes the exact raw four-byte
`_hs` slot (`grhst` word plus item/count bytes) inside the readable source. Its
strict-C89 probe passes `F_ATTACK_PACKED_PART_CHECKS=13`, deliberately rejecting
contradictory typed section fields in favor of the packed slot. It closes **0**
authoritative units; native `FLookupPart` table/service binding, full
battle-part scaling, broader original damage differentials, and replay remain
open. Evidence is in
`analysis/compat/f-attack-packed-slot-e6.{md,json}`.

The subsequent FAttack/FLookupPart checkpoint exposes the fixed table contract
through the host lookup seam. For packed group `0x0010`, item `7`, the source
reports segment `0x10e0`, base `0x2450`, stride `0x003c`, item offset `0x25f4`,
and component identity `0x00100007`; the combined packed probe passes 13 checks
and the lookup source probe passes 8. This closes **0** authoritative units.
Native segment/address binding, resource-relative families, complete
battle-part scaling, broader damage differentials, and replay remain open.
Evidence is in `analysis/compat/f-attack-part-table-e7.{md,json}`.

The newest FAttack/FLookupPart host checkpoint expands that address contract to
all eleven fixed table families. `LOOKUP_PART_TABLE_CHECKS=34 FAILURES=0
RESULT=PASS` covers segment `0x10e0`, family base, stride, item offset, and
component identity for hull, engine, scanner, weapon, shield, bomb, mine, mech,
fuel, battle, and component entries. A resource-relative `0x0100` case is also
checked and deliberately leaves table metadata zero. This closes **0** named
Step-5 units: native table/service binding, resource-relative families,
complete battle-part scaling, broader damage differentials, and replay remain
open. Evidence is in
`analysis/compat/f-attack-part-table-families-e8.{md,json}` and
`tests/lookup_part_table_probe.c`.

The current host-only order milestone is `SatisfyOrders` special dispatch.
`SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=0 RESULT=PASS` covers battle-plan
pass 1, mine capability/race-stat gates and repeat counts, same-owner
starbase auto-route/no-path messaging, merge rejection/callback gates, and
player-transfer source/recipient message pairs. It closes **0** authoritative
Step-5 units; native packed battle-plan/resource accounting, technology
discovery, fleet/ship-design copying, salvage creation, complete message
layouts, full-turn replay, and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-special-e4.{md,json}`.

The packed SatisfyOrders bridge has now moved into the production
reconstruction. `stars_satisfy_orders_packed` owns the recovered 124-byte
`_fleet`/18-byte `_order` size checks, identity/kind/target/validity decode,
typed transfer dispatch, and turn-flag/order-validity write-back. The existing
integration probe was changed to call that bridge directly and still passes
`SATISFY_ORDERS_INTEGRATION_CHECKS=3 FAILURES=0 RESULT=PASS`. This is a source
boundary improvement and closes **0** authoritative Step-5 units; native
battle-plan/resource accounting, special-order replay, and original behavior
remain open.

The corresponding `DumpPlanets` packed bridge is now also in production
source. `stars_dump_planets_packed` owns the recovered 56-byte `_planet`
identity, flags, turn, environment, mineral, population/production, scanner,
starbase, wormhole, and mine-count decode, then dispatches the existing typed
report coordinator. The packed probe calls the production bridge and still
passes `DUMP_PLANETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`. This closes **0**
authoritative Step-5 units: the historical native packet used the equivalent
test adapter, and a fresh native replay of the production bridge awaits the
guest-agent reconnect. Localized serialization, complete service callbacks,
original report differential, and promotion remain open.

The corresponding `DumpFleets` packed bridge is now also in production
source. `stars_dump_fleets_packed` owns the recovered 124-byte `_fleet`
identity, owner, flags, planet/coordinate, ship-count, cargo, weight, warp,
and task decode, while the typed boundary carries the live route, scanner,
and mine-service sidecars. The packed probe calls the production bridge and
still passes `DUMP_FLEETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`. This closes
**0** authoritative Step-5 units: the historical native packet used the
equivalent test adapter, and a fresh native replay of the production bridge
awaits the guest-agent reconnect. Localized serialization, complete service
behavior, original fleet-report differential, and promotion remain open.

The `ReportDlg` 54-byte `_rpt` view decoder is now also in production source
as `stars_report_dlg_packed`. The focused view probe calls the bridge directly
and passes `REPORT_DLG_VIEW_CHECKS=13 FAILURES=0 RESULT=PASS`; existing source,
packed/integration, paint-view, and draw-chain probes remain green. This closes
**0** authoritative Step-5 units: the historical native view packet used the
equivalent test loader, and a fresh native replay of the production bridge
awaits the guest-agent reconnect. Populated report unions/rows, live
USER/GDI/menu payloads, full caller integration, original behavior, and
promotion remain open.

The `DrawPlanetStats` 56-byte packed planet decoder is now also in production
source as `stars_draw_planet_stats_packed`. The packed probe calls the bridge
directly and passes `DRAW_PLANET_STATS_PACKED_CHECKS=15 FAILURES=0 RESULT=PASS`;
the source, lookup, service, formatting, and paint-chain probes remain green.
This closes **0** authoritative Step-5 units: the historical native packet
used the equivalent test loader, and a fresh native replay awaits the
guest-agent reconnect. Complete lookup, localized/fixed-point formatting,
exact service behavior, detailed GDI, original differential, and promotion
remain open.

The `ExecuteReportClick` raw lookup seam is now in production source. Its
optional packed fields decode the recovered 124-byte `_fleet`, 56-byte
`_planet`, and 14-byte `_btldata` records directly, while typed callbacks
remain fallback-compatible. The packed probe exercises the production helpers
and passes `EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`.
This closes **0** authoritative Step-5 units: the historical native packet
used the equivalent adapter, and a fresh native production-path replay awaits
the guest-agent reconnect. Exact GlobalPD owner/part unions, live USER/GDI,
complete caller integration, original differential, and promotion remain
open.

The `FDoCoolBattle` packed token seam is now in production source through
`StarsCoolBattleInput.packed_tokens`. It decodes and writes the recovered
29-byte `_tok` records directly; the host probe calls that path and passes
`F_DO_COOL_BATTLE_PACKED_TOKEN_CHECKS=11 FAILURES=0 RESULT=PASS`, while the
58-check source and 18-check FAttack-chain probes remain green. This closes
**0** authoritative Step-5 units: the historical native packet used the
equivalent adapter, and a fresh native production-token replay awaits the
guest-agent reconnect. Exact global binding/compaction, movement and damage
fidelity, full battle replay, and promotion remain open.

The next report-side source checkpoint moves the `DrawPopup` selector decode
into production source. `StarsDrawPopupInput.packed_popupdata` now owns the
22-byte `_popupdata` selector at offset 0 when a sufficiently large view is
provided, while short views fall back to the typed `popup_kind`. The strict
C89 production-path probe passes
`DRAW_POPUP_PACKED_CHECKS=37 FAILURES=0 RESULT=PASS` over raw-selector
precedence, every dispatcher kind, the kind-14 slot-hull alias, GDI-state
restoration, and short-record fallback. This closes 0 authoritative units:
fresh native replay through the production bridge, complete `GlobalPD` union
payloads, localization, hull filtering/formatting, detailed GDI rendering,
caller differential, and promotion remain open. Evidence is in
`analysis/compat/draw-popup-packed-production-e4.{md,json}`; the guest agent
still needs to reconnect before a new native packet can be claimed.

The next planet-display checkpoint moves the `PtDisplayPlanetPopInfo` packed
lookup into production source. `StarsPlanetPopInfoInput.packed_planet_record`
now decodes the recovered 56-byte `_planet` identity, owner, signed
population-change byte, and mineral-rate word; service-derived player,
habitability, maximum-population, desirability, and foreign-environment values
remain explicit sidecars. A short record falls back to `lookup_planet`. The
strict C89 production-path probe passes
`PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS` over raw
identity precedence, owned/unowned/foreign/negative-desirability branches,
the signed population-change field, and short-record fallback. This closes 0
authoritative units: fresh native replay, complete lookup,
localization/fixed-point formatting, detailed stream/wrap GDI behavior,
original differential, and promotion remain open. Evidence is in
`analysis/compat/pt-display-planet-pop-info-packed-production-e4.{md,json}`;
the guest agent still needs to reconnect before a new native packet can be
claimed.

The paired state-display checkpoint now moves the
`PtDisplayPlanetStateInfo` raw field decode into production source.
`StarsPlanetStateInfoInput` accepts the complete 56-byte `_planet` and
22-byte `_popupdata` views, taking identity, environment bytes, and
planet/player limit/value fields from those records while retaining derived
environment and desirability callbacks. Incomplete records leave typed state
data unchanged. The strict C89 production-path probe passes
`PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS` over
raw-field precedence, known/unknown limit branches, service calls,
wrapping/layout, and short-record fallback. This closes 0 authoritative
units: fresh native replay, complete lookup, localization/fixed-point
formatting, detailed stream/wrap GDI behavior, original differential, and
promotion remain open. Evidence is in
`analysis/compat/pt-display-planet-state-info-packed-production-e4.{md,json}`;
the guest agent still needs to reconnect before a new native packet can be
claimed.

The next fleet/planet-display checkpoint moves the `DrawPlanetShipList` raw
selection and transfer fields into production source. Complete 226-byte
selection views now supply the object word at offset 152, and complete
128-byte transfer views supply the transfer ID, kind, and fleet owner at
offsets 0, 2, and 6; typed state and callback lookup remain the fallback for
incomplete views. The strict C89 production-path probe passes
`DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`, while the
source probe remains green at 19 checks. This closes 0 authoritative units:
fresh native replay, exact USER/GDI controls, localized text/gauges, complete
detail rendering, original differential, and promotion remain open. Evidence
is in `analysis/compat/draw-planet-ship-list-packed-production-e4.{md,json}`;
the guest agent still needs to reconnect before a new native packet can be
claimed.

The newest focused milestone is the unsigned packed-capacity guard in the
saved-queue path. Fresh `C:\PDGQ5` host/native validation passes
`PRODUCTION_DLG_QUEUE_CHECKS=10 FAILURES=0 RESULT=PASS` after exercising an
oversized `0xffff` capacity against the recovered four-record table. Compile,
link, process exit, empty redirected output, and no Program Error dialog pass.
This closes only that capacity sub-boundary; the original queue differential,
packed production globals, complete resources, and promotion remain open.
Evidence is in `analysis/compat/production-dlg-queue-e5.{md,json}` and
`runs/vmctl-vc152pdgq5/`.

The earlier focused `ReportDlg` view milestone had only a host production
bridge and a pending native packet; that checkpoint is superseded by the
fresh production-source replay recorded above. The historical `C:\RPTVE1`
packet remains retained, while the authoritative native evidence is now in
`C:\RPTVEP` and `runs/vmctl-vc152rptvep/`.

The next host-only milestone covers the paired planet-information display
procedures. `tests/planet_info_branch_probe.c` passes
`PLANET_INFO_BRANCH_CHECKS=9 FAILURES=0 RESULT=PASS` across the owned,
unowned, foreign, zero-mineral, negative-desirability, maximum-population,
growth-warning, unknown/known-limit, environment, signed-difference, wrap,
and returned-layout branches of `PtDisplayPlanetPopInfo` and
`PtDisplayPlanetStateInfo`. This strengthens their branch integration but
closes no authoritative Step-5 unit: packed lookup, localized fixed-point
formatting, detailed GDI comparison, native replay, original differential,
and promotion remain open. Evidence is in
`analysis/compat/planet-info-branch-e3.{md,json}`; the Win98 Close Program
dialog is still preventing a reliable guest-agent handshake.

The next host-only milestone connects `SatisfyOrders` to the reconstructed
`DoOrders` turn engine. `tests/satisfy_orders_turn_probe.c` passes
`SATISFY_ORDERS_TURN_CHECKS=8 FAILURES=0 RESULT=PASS` for both pre- and
post-movement sequencing, including battle-id creation, battle/thing callback
order, passes 1/2 and 3/4, planet flag/turn mutation, fleet cargo movement,
and order retirement. This closes no authoritative Step-5 unit; packed
battle-plan/resource accounting, technology discovery, fleet/ship-design
copying, salvage creation, complete message layouts, native/original replay,
and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-turn-e3.{md,json}`.
The matching VC++ 1.52c packet is prepared at
`compat/vc152/satisfy-orders-turn-e3/` for `C:\SYTURN`; native build and
runtime evidence remain pending the guest-agent handshake.

The newest `DrawPlanetShipList` host checkpoint exposes the fleet-versus-thing
detail branch through a typed `draw_details` callback. The strict-C89 probe
passes `DRAW_PLANET_SHIP_LIST_DETAILS_CHECKS=9 FAILURES=0 RESULT=PASS` for
resolved fleet detail, unavailable-transfer suppression, and ship-mode
suppression; the existing 19-check source and 18-check packed probes remain
green. This closes **0** authoritative Step-5 units. Native detail rendering,
exact USER/GDI controls, localized text/gauges, original differential, and
promotion remain open. Evidence is in
`analysis/compat/draw-planet-ship-list-details-e3.{md,json}`.

The following `DrawPlanShipBitmap` host checkpoint isolates the previously
described cleanup branches. `tests/draw_plan_ship_bitmap_failure_probe.c`
passes `DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=6 FAILURES=0 RESULT=PASS` for
failed-tile cleanup, suppression of all downstream rendering on failure, hide
flag clearing, and the no-buttons path that still renders a fleet bitmap.
This closes **0** authoritative Step-5 units; native GDI/resource replay,
localized-name comparison, original differential coverage, and promotion
remain open. Evidence is in
`analysis/compat/draw-plan-ship-bitmap-failure-e4.{md,json}`. The resumed
86Box still presents the Win98 Close Program dialog and did not announce
`STARSAGT`, so no native packet was run or guest process was reset.

The next `EstimateItemProdSched` host differential milestone is complete. The
strict-C89 probe passes
`ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=21 FAILURES=0 RESULT=PASS` over
phase-1/phase-2 completion, the 100-turn fallback, final and non-final special
items, partial-item propagation, raw/research-adjusted and research-exempt
resource queries, mineral refresh, copied-planet isolation, and population
timing. A fresh native packet is prepared at
`compat/vc152/estimate-item-prod-sched-diff/` for `C:\EIPD`; native execution
is pending guest-agent reconnection, so the authoritative burn-down remains
**77 evidence units + 22 promotion units = 99 open work items**.

The next reports-cluster host milestone is now complete for `DumpPlanets`. The
strict-C89 boundary probe passes
`DUMP_PLANETS_BOUNDARY_CHECKS=24 FAILURES=0 RESULT=PASS` over active-game and
viewer gates, open/header/row failure cleanup, normal/detailed header counts,
turn-age and owner visibility, scanned service fields, planet-class clearing,
and normal production/resource preservation. A fresh native packet is
prepared at `compat/vc152/dump-planets-boundary/` for `C:\DPB`; native
execution and localized serialization/original-output evidence remain
pending, so the authoritative burn-down remains unchanged.

The paired `DumpFleets` host boundary milestone is also complete. The strict
C89 probe passes
`DUMP_FLEETS_BOUNDARY_CHECKS=20 FAILURES=0 RESULT=PASS` over active-game and
viewer gates, open/header/row failure cleanup, normal/detailed headers, all
ship-class buckets plus the other-class total, all cargo slots, detail
suppression, destination-requires-detail behavior, and detailed scanner/task/
mine/terraform fields. A fresh native packet is prepared at
`compat/vc152/dump-fleets-boundary/` for `C:\DFB`; native execution and
localized serialization/original-output evidence remain pending, so the
authoritative burn-down remains unchanged.

The newest DrawProductionDlg milestone closes its E1 packed-production-global
unit. Fresh `C:\DPD4` host/native validation passes
`DRAW_PRODUCTION_DLG_CHECKS=17 FAILURES=0 RESULT=PASS` after exercising exact
source/planet packed words, recorded-count bounds, planet-list selection
normalization, and the decoded-cost fallback. Compile, link, process exit,
empty redirected output, and no Program Error dialog pass. DrawProductionDlg
remains at `1/3` evidence units with exact resource/localization comparison,
full GDI/caller integration, and promotion still open. Evidence is in
`analysis/compat/draw-production-dlg-packed-e1.{md,json}` and
`runs/vmctl-vc152dpd4/`.

The following `DrawProductionDlg` E2 packet is now natively closed. Its strict
C89 and fresh VC++ 1.52c/Win98 probes pass 10 checks for resource IDs
`0x2fe`, `0x33f`, `0x340`, and `0x0d2`, the DGROUP `rgszMinerals` table at
`0x04cc`, lookup order, mineral labels, and ETA-format dispatch. Compile/link
passed, redirected output was empty, and no Program Error dialog appeared.
Evidence is in `analysis/compat/draw-production-dlg-resource-e2.{md,json}` and
`runs/vmctl-vc152dpdre2/`; E3 full GDI/caller integration and promotion remain
open.

The newest `DrawPlanetStats` formatting packet is now natively closed. The
readable source offers an optional paired
`format_value`/`draw_formatted_value` boundary while retaining the legacy
callback path. A strict C89 probe passes
`DRAW_PLANET_STATS_FORMAT_CHECKS=16 FAILURES=0 RESULT=PASS` for all seven
value rows, fixed-point mineral-rate text, zero rate, and unavailable
scanner/defense cases. Fresh VC++ 1.52c validation in `C:\DPSFE2` reports
`DRAW_PLANET_STATS_FORMAT_CHECKS=16 FAILURES=0 RESULT=PASS`, with no Program
Error dialog. The corrected probe includes the previously omitted `77/0`
defense-string check. This closes E2; the authoritative count is now 78
closed and 74 open evidence units. Evidence is in
`analysis/compat/draw-planet-stats-format-e2.{md,json}` and
`runs/vmctl-vc152dpsfe2/`.

The following native supporting checkpoint validates DrawPopup's detailed GDI
state-order packet. Fresh `C:\DRINT` VC++ 1.52c validation returned
`DRAW_POPUP_STATE_CHECKS=7 FAILURES=0 RESULT=PASS`; the process exited normally
and no Program Error dialog appeared. This does not close a named Step-5 unit:
localized detailed rendering and the caller differential remain open, so the
authoritative counters stay **78/152 closed, 74 open, 22 promotion units, and
96 practical open work items**. Evidence is in
`analysis/compat/draw-popup-state-e3.{md,json}` and
`runs/vmctl-vc152drawpopupstatee3/`.

The following `DrawPlanetStats` lookup packet is also host-complete but
native-pending. The optional `load_data` callback now receives the tile/object
pointer and populates the typed data record before drawing; the strict host
probe passes `DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS` over
the recovered 56-byte planet identity, scanner/defense word, mineral-rate
word, and all seven row consumers. The VC++ 1.52c packet is prepared in
`compat/vc152/draw-planet-stats-lookup-e1/` for `C:\DPSLE1`; it likewise
does not reduce the open count until native execution is captured.

The next `DrawPlanetStats` host checkpoint makes the service boundary explicit.
The optional typed `resolve_data` callback receives the accepted packed planet
view and selected player after tile drawing and before the seven value rows.
`DRAW_PLANET_STATS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS` checks
selected-player forwarding, packed service-word and mineral-rate decoding, and
all seven value-row consumers. This closes **0** authoritative Step-5 units;
native service execution, localized resources, detailed GDI, original
differential, and promotion remain open. Evidence is in
`analysis/compat/draw-planet-stats-services-e3.{md,json}`.

The next display boundary is `PtDisplayPlanetPopInfo`'s formatting seam. The
optional `prepare_segment` callback now receives every selected stream/wrap
segment before font and output dispatch, while the legacy callback path stays
unchanged. The strict host source probe passes
`PT_DISPLAY_PLANET_POP_INFO_CHECKS=25 FAILURES=0 RESULT=PASS`, including the
negative-desirability fixed-point pair `2,5`; the packed 19-check probe also
passes. The VC++ 1.52c packet is prepared in
`compat/vc152/pt-display-planet-pop-info-format-e2/` for `C:\PPFE2`, but
native execution is pending guest-agent reconnection.

The companion `PtDisplayPlanetStateInfo` formatting packet is also
host-complete but native-pending. Its optional `prepare_segment` callback now
receives every selected stream/wrap segment; the strict host probe passes
`PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS`,
including the kind-2 environment value after the service callback. The
existing packed 23-check probe still passes. The VC++ 1.52c packet is
prepared in `compat/vc152/pt-display-planet-state-info-format-e2/` for
`C:\STFE2`, but the open count will not change until native execution is
captured.

TODO6 now documents each procedure's Step-5 units individually, including the
closed count, remaining unit descriptions, evidence needed to close them, and
the separate promotion item. For planning only, 77 remaining evidence units
would be roughly 39, 20, or 10 weeks at sustained rates of 2, 4, or 8
closures per week; including promotion items gives roughly 50, 25, or 13
weeks. These are throughput scenarios, not delivery promises, because shared
harness work and newly discovered boundaries can change the rate.

For each procedure, `E done / E planned` is the number of Step-5 boundaries
closed, while `E open` is the remaining packet count. A row at `5/6` can
therefore be `2/3` with one focused boundary left (for example a remaining
two-unit procedure after one of its units closes), or
`0/4` with a broad UI/turn/replay packet still open (several AI and report
procedures). This is the meaningful progress measure; the `5/6` headline
does not mean that one-sixth of the work remains.

The detailed progress narrative below preserves earlier checkpoint counts for
auditability; the current counters and matrix above are authoritative.

The latest milestone is `ProductionDlg`'s native USER/message-loop
sub-boundary. The fresh `C:\PDGU5` source/native run creates an actual Win98
window and child controls, then routes queued initialization, command,
left-click, two right-click, paint, and erase messages through the reconstructed
dispatcher. It reports
`PRODUCTION_DLG_USER_CHECKS=12 FAILURES=0 RESULT=PASS`, including sparse queue
selection, accepted editor mutation, two queue menus, and 74 USER-loop
dispatches; compile, link, process exit, empty redirected output, and no
Program Error dialog pass. This closes only native USER dispatch: ProductionDlg
remains **0/3** at Step 5 with packed production globals, complete resource
comparison, original queue differential, and final promotion remaining.
Evidence is in `analysis/compat/production-dlg-user-e5.{md,json}` and
`runs/vmctl-vc152pdgu5/`.

The preceding milestone is `ProductionDlg`'s accepted-editor outcome inside the
E3 queue boundary. The fresh `C:\PDGQ4` source/native run preserves accepted
queue mutations and restores the pre-dialog queue table on cancel, while also
covering sparse original indices and the localized editor entry. Strict host
C89 and VC++ 1.52c/Win98 runs report
`PRODUCTION_DLG_QUEUE_CHECKS=9 FAILURES=0 RESULT=PASS`; compile, link, process
exit, empty redirected output, and no Program Error dialog pass. This closes
only the accepted/canceled mutation sub-boundary: ProductionDlg remains **0/3**
at Step 5 with packed production globals, complete resource comparison,
original queue differential, and final promotion
remaining. Evidence is in
`analysis/compat/production-dlg-queue-e4.{md,json}` and
`runs/vmctl-vc152pdgq4/`.

The preceding milestone is `ProductionDlg`'s saved-queue menu/cancel-rollback
sub-boundary inside E3. The fresh `C:\PDGQ3` source/native run filters valid
entries from the four queue slots, preserves sparse original indices, passes
the localized editor entry after the queue names, and restores the queue table
when the editor reports cancel. Strict host C89 and VC++ 1.52c/Win98 runs
report `PRODUCTION_DLG_QUEUE_CHECKS=7 FAILURES=0 RESULT=PASS`; compile, link,
process exit, empty redirected output, and no Program Error dialog pass. This
closes only that E3 sub-boundary; live USER/message-loop behavior, original
queue differential, packed globals, complete resources, and final promotion
remain. Evidence is in
`analysis/compat/production-dlg-queue-e3.{md,json}` and
`runs/vmctl-vc152pdgq3/`.

The preceding milestone is `ProductionDlg`'s exact resource-ID/callback-order
sub-boundary inside E2. The fresh `C:\PDGRS2` source/native run preserves the
original `0x00d3` copied-popup-string and `0x04c0` compressed-string lookup
boundaries, verifies callback ordering, and suppresses both paths outside their
corresponding diamond controls. Strict host C89 and VC++ 1.52c/Win98 runs
report `PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=0 RESULT=PASS`; compile,
link, process exit, empty redirected output, and no Program Error dialog pass.
This closes only the lookup-ID/order sub-boundary: the complete localized
resource-table/string comparison remains open, so ProductionDlg remains **0/3**
at Step 5 with packed production globals, full resource comparison,
queue-control integration, and final promotion remaining. Evidence is in
`analysis/compat/production-dlg-resource-e2.{md,json}` and
`runs/vmctl-vc152pdgrs2/`.

The preceding milestone is `ProductionDlg`'s exact-child-geometry sub-boundary
inside E2. The fresh `C:\PDGE2` source/native run records the complete WM_INITDIALOG layout
trace for full-width and compact-width modes, navigation enablement, child
positions, dimensions, flags, and initialization ordering. Strict host C89
and VC++ 1.52c/Win98 runs report
`PRODUCTION_DLG_LAYOUT_CHECKS=23 FAILURES=0 RESULT=PASS`; compile, link,
process exit, empty redirected output, and no Program Error dialog pass. The
full E2 geometry/resources unit remains open because localized resource
comparison is not yet covered; ProductionDlg remains **0/3** at Step 5, with
packed production globals, popup resources, queue-control integration, and
final promotion remaining.
Evidence is in `analysis/compat/production-dlg-layout-e2.{md,json}` and
`runs/vmctl-vc152pdge2/`.

The earlier closure is `DrawMineSurvey`'s E1 detailed GDI layout and Step-6
promotion. The fresh `C:\DMSIE1` packet emits typed outer-panel PATBLT
records for the 13-command fleet frame, 13-command ordinary-thing frame, and
7-command compact-thing frame, preserving exact offsets, dimensions, raster
operations, and brush roles. Strict host C89 and fresh VC++ 1.52c/Win98 runs
report `DRAW_MINE_SURVEY_LAYOUT_CHECKS=13 FAILURES=0 RESULT=PASS`; compile,
link, process exit, and no Program Error dialog pass. DrawMineSurvey is now
**6/6 and promoted**. The authoritative burn-down is **77 evidence units +
22 promotion units = 99 open work items**, with **29 procedures at 6/6 and
22 at 5/6**. Evidence is in
`analysis/compat/draw-mine-survey-layout-e1.{md,json}` and
`runs/vmctl-vc152dmslay/`.

The preceding closure is `MineClick`'s E3 complete mine-window caller
integration. The typed dispatcher is wired to the real
`PopupMineralScanChoices` selector; fresh `C:\MCLIE3` host/native validation
passes `MINE_CLICK_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`, including
planet/fleet/thing markers, selected-fleet decoding, redraw/main-selection
ordering, fleet-cargo dispatch, double-click suppression, and all remaining
popup hit families. MineClick is promoted at **6/6**; evidence is recorded in
`analysis/compat/mine-click-integration-e3.{md,json}` and
`runs/vmctl-vc152mclint3/`.

The preceding closure is `MineClick`'s E1 packed GlobalPD-facing payload
integration. The typed source accepts the recovered 56-byte `_planet`,
124-byte `_fleet`, and 18-byte `_thing` records and decodes their identity,
owner, detail, environment bounds, fleet ship count, and object point/kind
fields into the popup callback. Strict host C89 and fresh VC++ 1.52c/Win98
`C:\MCLE2` runs report `MINE_CLICK_CHECKS=30 FAILURES=0 RESULT=PASS`;
compile, link, process exit, and no Program Error dialog pass. MineClick was
then **1/3** at Step 5; E2 and E3 remained at that checkpoint. Evidence is in
`analysis/compat/mine-click-packed-e1.{md,json}` and
`runs/vmctl-vc152mcle2/`.

The preceding closure is `DrawMineSurvey`'s E3 localized/caller integration.
Fresh `C:\DMSIE3` host/native validation passed
`DRAW_MINE_SURVEY_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`, covering the
localized `0x04bd` title resource, real `MineWndProc` paint-to-survey dispatch,
adjusted survey geometry, planet lookup/details, state restoration, and paint
ordering. Evidence is in
`analysis/compat/draw-mine-survey-integration-e3.{md,json}` and
`runs/vmctl-vc152dmsint3/`.

The preceding closure is `DrawMineSurvey`'s E2 packed-record integration. The
typed source now accepts the recovered 124-byte `_fleet`, 56-byte `_planet`,
and 18-byte `_thing` records and decodes the ship counts, planet display
fields, and thing identity/point/wormhole range before invoking the render
callbacks. Strict host C89 and fresh VC++ 1.52c/Win98 `C:\DMSE2` runs report
`DRAW_MINE_SURVEY_CHECKS=12 FAILURES=0 RESULT=PASS`; compile, link, process
exit, and no Program Error dialog pass. DrawMineSurvey is now **1/3** at
Step 5; E1 detailed GDI layout and E3 localization/complete caller
integration remain. Evidence is in
`analysis/compat/draw-mine-survey-packed-e2.{md,json}` and
`runs/vmctl-vc152dmse2/`. The authoritative burn-down is **83 evidence
units + 24 promotion units = 107 open work items**, with **27 procedures at
6/6 and 24 at 5/6**.

The preceding closure is `TransferStuff`'s E3 packed lookup/write-back and real
Win16 dialog integration. A fresh `C:\TRCNE3` host/native packet reports
`TRANSFER_STUFF_E3_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link, process
exit, and no Program Error dialog pass. The packet closes the corrected
`grobj=4` split/jettison sentinel, initial packed lookup, post-dialog
fleet/planet/thing update lookup, task-message dialog sequence, lookup
rejection, and refresh/invalidation ordering. TransferStuff is now **6/6**
and promoted; evidence is preserved in
`analysis/compat/transfer-stuff-e3.{md,json}` and
`runs/vmctl-vc152trcne3/`. The authoritative burn-down is **84 evidence
units + 24 promotion units = 108 open work items**, with **27 procedures at
6/6 and 24 at 5/6**.

The preceding closure is `TransferStuff`'s E2 exact cargo-mutation integration.
The transfer orchestration now drives the reconstructed cargo-balance service
at the raw balance boundary. The strict host and fresh Win98 `C:\TRCNE2` probe
report `TRANSFER_STUFF_CARGO_E2_CHECKS=12 FAILURES=0 RESULT=PASS`; compile,
link, process exit, and the post-run window inventory passed with no Program
Error. The packet checks callback order, five-resource conservation,
donor-to-receiver fuel movement, packed per-design status mutation, and the
cancel path with no balance call. `TransferStuff` is now **1/3** named
evidence units complete; E3 live transfer/lookup/refresh integration and P
promotion remain. The authoritative burn-down is **88 evidence units + 26
promotion units = 114 open work items**.

The preceding closure is `PlanetWndProc`'s E3 message/focus/drawing integration,
followed by Step-6 promotion. The typed source now preserves the raw
paint/erase, minimum-size, owner-draw, measure, cursor, focus, and
command/message boundaries. The host and fresh Win98 `C:\PWTE3` probe report
`PLANET_WND_PROC_INTEGRATION_E3_CHECKS=10 FAILURES=0 RESULT=PASS`; compile,
link, process exit, and the post-run window inventory passed with no Program
Error. Together with E1 and E2, the complete packet and original planet-window
evidence are synchronized in the review ledger, coverage, TODO5, and
reconstruction notes. PlanetWndProc is now **6/6**. At that checkpoint the
burn-down was **89 evidence units + 26 promotion units = 115 open work items**.

The preceding closure is `PlanetWndProc`'s E2 packed-transfer evidence. The
host and fresh Win98 `C:\PWTE2` probe report
`PLANET_WND_PROC_TRANSFER_E2_CHECKS=12 FAILURES=0 RESULT=PASS`; its packet
closes fleet-before-thing ordinal ordering, planet filtering, packed `_xfer`
headers and payload copies, TransferStuff mode 1, and the fleet-only
`grobj == 2` selection boundary.

The preceding closure is `PlanetWndProc`'s E1 exact child-layout evidence.

The preceding closure is `FillBuildDD`'s E3 complete population/control
integration and Step-6 promotion. The corrected host and fresh Win98 `C:\FBDE`
probe report `FILL_BUILD_DD_E3_CHECKS=14 FAILURES=0 RESULT=PASS`; compile,
link, process exit, and the post-run window inventory passed with no Program
Error. The matrix covers normal, empty, hidden, queued, built, special-part,
enemy-design, starbase-limit, selector, edit/delete, count-clamp, resize,
and callback-order boundaries. Evidence is recorded in
`analysis/compat/fill-build-dd-e3.md` and `runs/vmctl-vc152fbde2/`.
FillBuildDD is now promoted at `6/6`; the practical burn-down is **92
evidence units + 27 promotion units = 119 open work items**.

The preceding closure is `FillBuildDD`'s E2 localized-label/resource evidence
packet. The host and fresh Win98 `C:\FBDL` probe report
`FILL_BUILD_DD_LABELS_E2_CHECKS=58 FAILURES=0 RESULT=PASS`; compile, link,
process exit, and the post-run window inventory passed with no Program Error.
The exact normal and starbase compressed-string ID tables, both label modes,
reset/add/list-count/end ordering, and three nonzero-mode disable calls are
recorded in `analysis/compat/fill-build-dd-labels-e2.md` and
`runs/vmctl-vc152fbdl/`.

The preceding closure is `FillBuildDD`'s E1 packed-design-record evidence
packet. The host and fresh Win98 `C:\FBDP` probe report
`FILL_BUILD_DD_PACKED_E1_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link,
process exit, and the post-run window inventory passed with no Program Error.
The 147-byte `_shdef` stride, free-bit visibility, name offset, packed
`cExist` edit state, shadow-record conflict, and neighboring-record
preservation are recorded in `analysis/compat/fill-build-dd-packed-e1.md`
and `runs/vmctl-vc152fbdp/`. At that preceding checkpoint, FillBuildDD was
`1/3` evidence and the burn-down was **94 evidence units + 28 promotion units
= 122 open work items**.

The preceding closure is `IDropPart`'s Step-6 promotion after E3 redraw/control
integration evidence.
The host and fresh Win98 `C:\IDPR` probe report
`I_DROP_PART_REDRAW_E3_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link,
process exit, and the post-run window inventory passed with no Program Error.
The raw empty-side and merge service order, rectangle-top replacement, erase
flag, and selected-component/hull arguments are recorded in
`analysis/compat/i-drop-part-redraw-e3.md` and `runs/vmctl-vc152idpr/`.
IDropPart is now promoted at `6/6`: all named evidence and final ledger
synchronization are closed. At that earlier checkpoint, the practical
burn-down was **95 evidence units + 28 promotion units = 123 open work
items**; the current authoritative count is the FillBuildDD-adjusted total
above.

An earlier closure is `IDropPart`'s E1 packed slot hit-table evidence. The host
and fresh Win98 `C:\IDPH` probe report
`I_DROP_PART_HIT_TABLE_E1_CHECKS=12 FAILURES=0 RESULT=PASS`; compile, link,
process exit, and the post-run window inventory passed with no Program Error.
The packed rectangle table and Win16 edge semantics are recorded in
`analysis/compat/i-drop-part-hit-table-e1.md` and
`runs/vmctl-vc152idphit/`. IDropPart remains at 5/6 with two named evidence
units and the final promotion unit open. The practical burn-down is now
**97 evidence units + 29 promotion units = 126 open work items**.

The preceding closure is `DrawNewGame3`'s E4 complete page integration and
Step-6 promotion. The host and fresh Win98 `C:\DNG3I` probe report
`DRAW_NEW_GAME3_INTEGRATION_E4_CHECKS=12 FAILURES=0 RESULT=PASS`; compile,
link, process exit, and the post-run window inventory passed with no Program
Error. The background save/restore, client-rect, DC ownership, read-only
styles, caller HDC, and selective draw-mode boundary are recorded in
`analysis/compat/draw-new-game3-integration-e4.md` and
`runs/vmctl-vc152dng3int/`. DrawNewGame3 is now promoted at 6/6.

The preceding closure is `DrawNewGame3`'s E3 localized-rendering evidence. The
host and fresh Win98 `C:\DNG3L` probe report
`DRAW_NEW_GAME3_LOCALIZATION_E3_CHECKS=34 FAILURES=0 RESULT=PASS`; compile,
link, process exit, and the post-run window inventory passed with no Program
Error. The typed formatter, resource suffix `0x0af4`, all 27 label IDs, and
the complete reference boundary are recorded in
`analysis/compat/draw-new-game3-localization-e3.md` and
`runs/vmctl-vc152dng3loc/`. DrawNewGame3 remains at 5/6 with one named
evidence unit and the final promotion unit open. The practical burn-down is
now **99 evidence units + 30 promotion units = 129 open work items**.

The preceding closure is `DrawNewGame3`'s E2 packed output-rectangle evidence.

The host and fresh Win98 `C:\DNG3R` probe reported
`DRAW_NEW_GAME3_RECTANGLES_E2_CHECKS=95 FAILURES=0 RESULT=PASS`; compile,
link, process exit, and the post-run window inventory passed with no Program
Error. All 18 value rectangles, all 18 paired offsets, row-7/8 carry-over,
and the signed-width spacing boundary are recorded in
`analysis/compat/draw-new-game3-rectangles-e2.md` and
`runs/vmctl-vc152dng3rect/`.

The preceding closure is `DrawNewGame3`'s E1 packed signed-width table evidence.
The host and fresh Win98 `C:\DNG3P` probe report
`DRAW_NEW_GAME3_PACKED_E1_CHECKS=7 FAILURES=0 RESULT=PASS`; compile, link,
process exit, and the post-run window inventory passed with no Program Error.
The raw 18-byte row-major table, zero-width skip, signed percent formatting,
and paired reference boundary are recorded in
`analysis/compat/draw-new-game3-packed-e1.md` and
`runs/vmctl-vc152dng3pack/`. DrawNewGame3 remains at 5/6 with three named
evidence units and the final promotion unit open. The practical burn-down is
now **101 evidence units + 30 promotion units = 131 open work items**.

The preceding closure is `DrawNewGame2`'s E3 complete page GDI integration and
Step-6 promotion. The host and fresh Win98 `C:\DNG2I` probe report
`DRAW_NEW_GAME2_INTEGRATION_E3_CHECKS=16 FAILURES=0 RESULT=PASS`; it checks the
DC ownership, opaque-background/font setup, sixteen-row geometry and draw
order, color reset, read-only/dead path, and caller-provided HDC boundary.
Compile, link, process exit, and the post-run window inventory passed with no
Program Error. `DrawNewGame2` is now promoted at `6/6`.

The preceding closure is `DrawNewGame2`'s E2 localized-formatting evidence. The
host and fresh Win98 `C:\DNG2L` probe report
`DRAW_NEW_GAME2_LOCALIZATION_E2_CHECKS=15 FAILURES=0 RESULT=PASS`; it checks
the empty, built-in race, special/generic, custom-name/file, game-file-mode,
and type-3 AI group/level format paths. Compile, link, process exit, and the
post-run window inventory passed with no Program Error. `DrawNewGame2` is now
`2/3` evidence with only E3 full GDI/resource integration open.

The preceding closure is `DrawNewGame2`'s E1 packed player/type-table evidence.
The host and fresh Win98 `C:\DNG2P` probe report
`DRAW_NEW_GAME2_PACKED_E1_CHECKS=13 FAILURES=0 RESULT=PASS`; it checks the
original one-byte player kind/race table, 13-byte race-file stride, empty-file
fallback, game-file-mode branch, and shadow-record conflict boundary. Compile,
link, process exit, and the post-run window inventory passed with no Program
Error. `DrawNewGame2` remains at `5/6` with E2 localized formatting and E3
full GDI/resource integration open.

The preceding closure is `NewGameDlg2`'s E1 packed race/password evidence and
Step-6 promotion. The host and fresh Win98 `C:\ND2PACK2` probe report
`NEW_GAME_DLG2_PACKED_E1_CHECKS=1238 FAILURES=0 RESULT=PASS`; it checks the
Create/Open/Edit 192-byte player-record and 13-byte race-file commits,
password rejection/acceptance, and neighbor preservation. Compile, link,
process exit, and the post-run window inventory passed with no Program Error.
The original replay, review record, regenerated coverage, TODO5, TODO6, and
reconstruction note are synchronized. `NewGameDlg2` is now promoted at `6/6`.

The preceding closure is `NewGameDlg2`'s E3 transition/caller evidence. The
focused host and fresh Win98 `C:\ND2INT2` probe report
`NEW_GAME_DLG2_INTEGRATION_E3_CHECKS=9 FAILURES=0 RESULT=PASS`; it checks
accepted command results at the `EndDialog` boundary, caller-stage dispatch,
missing-first-player alert gating, read-only acceptance, and player-row race
creation/propagation. Compile, link, process exit, and the post-run window
inventory passed with no Program Error. Together with the original three-page
replay, `NewGameDlg2` is now `2/3` evidence with only packed race/password
storage remaining.

The preceding closure is `NewGameDlg2`'s E2 resource/control evidence. The typed
`0x0187` contract covers the 261x210 second-page dialog, 8-point font, and all
five navigation controls with exact IDs, rectangles, text lengths, and text
checksums. The host and fresh Win98 `C:\ND2RES2` probe report
`NEW_GAME_DLG2_RESOURCE_E2_CHECKS=40 FAILURES=0 RESULT=PASS`; compile, link,
process exit, and the post-run window inventory passed with no Program Error.
`NewGameDlg2` is now `1/3` evidence; packed race/password records and the
complete wizard path remain.

The preceding closure is `SimpleNewGameDlg`'s E3 creation/caller evidence and
Step-6 promotion. The focused host and fresh Win98 `C:\SNGDI` probe report
`SIMPLE_NEW_GAME_DLG_INTEGRATION_E3_CHECKS=12 FAILURES=0 RESULT=PASS`; it
checks accepted density/size/selection state at the `EndDialog(0x430)` caller
boundary, custom-race load/wizard/save and list mutation, focus restoration,
and cancel preservation. Compile, link, process exit, and the post-run window
inventory passed with no Program Error. The original six-page custom-race
Save As/cancel replay, review record, regenerated coverage, TODO5, TODO6, and
reconstruction note are synchronized. `SimpleNewGameDlg` is now promoted at
`6/6`.

The preceding closure is `SimpleNewGameDlg`'s E2 resource/control evidence.
The typed `0x00D1` dialog contract covers all 16 controls, exact rectangles,
text lengths, and resource-text checksums. The host and fresh Win98 `C:\SNGDR`
probe report `SIMPLE_NEW_GAME_DLG_RESOURCE_E2_CHECKS=117 FAILURES=0 RESULT=PASS`;
compile, link, process exit, and the post-run window inventory passed with no
Program Error. This closed the second of three named Step-5 boundaries before
the E3 packet and promotion above.

The preceding closure is `SimpleNewGameDlg`'s E1 packed-player evidence. The host
and fresh Win98 `C:\SNGDP` probe report
`SIMPLE_NEW_GAME_DLG_PACKED_E1_CHECKS=3074 FAILURES=0 RESULT=PASS` across every
byte of the selected 192-byte record and all 15 neighboring slots; compile,
link, process exit, and the post-run window inventory passed with no Program
Error. This packet established the first of three named Step-5 boundaries.

The preceding closure is `NewGameWizard`'s Step-6 promotion. Its packed player
state, all three page resources/control geometry, complete creation transition,
original replay, review record, regenerated coverage, TODO5, TODO6, and
reconstruction note are now synchronized. TODO5 now reports 814 reviewed and
32 partial procedures; `NewGameWizard` is promoted at `6/6`.

The preceding closure is `NewGameWizard`'s E2 resource/control evidence. The
host and fresh Win98 `C:\NWRES` probe report
`NEW_GAME_WIZARD_RESOURCE_E2_CHECKS=366 FAILURES=0 RESULT=PASS` for all 45
controls across resources 0x186–0x188; compile, link, process exit, and the
post-run window inventory passed with no Program Error. This closed the final
numbered Step-5 unit for the wizard; the native package is preserved in
`runs/vmctl-vc152nwres/`.

The earlier closure is `NewGameDlg`'s Step-6 promotion. Its packed game record,
resource/control geometry, shared creation transition, original replay, review
record, regenerated coverage, TODO5, TODO6, and reconstruction note are now
synchronized. TODO5 now reports 813 reviewed and 33 partial procedures;
`NewGameDlg` is promoted at `6/6`.

The preceding closure is the shared creation E3 evidence unit for
`NewGameDlg` and `NewGameWizard`. The host and fresh Win98 `C:\NGINT` probe
report `NEW_GAME_CREATION_E3_CHECKS=11 FAILURES=0 RESULT=PASS`; compile, link,
process exit, and the post-run window inventory passed with no Program Error.
It verifies dialog acceptance, wizard pages 1–3, packed player commit, name
acceptance, bit restoration, world generation, and host dispatch, paired with
the original replay in `runs/vmctl-vc152current/`. At that checkpoint
`NewGameDlg` reached `0 + P` and was then promoted; the subsequent E2 packet
and promotion above complete the wizard as well.

The earlier closure is `NewGameDlg`'s E2 resource/geometry evidence unit. The
typed 0x186 contract matches the original 261x210 dialog, 8-point font, and
all 27 control IDs/rectangles in 30/30 host/native checks; one evidence unit
then remained before the shared creation packet. The native `C:\NGDR` build
and run completed without a Program Error.

The preceding closure is `NewGameDlg`'s E1 packed-game evidence unit. Its
settings offsets 4, 6, 12, 16, and 32 and preservation of unowned record
bytes pass 7/7 in fresh host/native checks; two evidence units were then open
before E2 closed. The native `C:\NGDP` build and run completed without a Program Error.

The earlier closure is `NewGameWizard`'s E1 packed-player evidence unit. Its
flags, salt, dead marker, name offsets, and preservation of unowned record
bytes pass 11/11 in fresh host/native checks; two evidence units remain before
promotion. The native `C:\NWP` build and run completed without a Program Error.

The preceding closure is `DrawPlanetStarbase`'s final promotion. Its E1 packed
lookup, E2 localized resources, and E3 exact service/GDI evidence were already
closed in fresh native runs; the review record, TODO6, coverage, TODO5, and
reconstruction note are now synchronized. `DrawPlanetStarbase` is promoted at
`6/6`.

The preceding closure is `KillUsedWaypoints`' final promotion. Its E1 live-table,
E2 resource, and E3 complete-turn evidence were already closed in fresh native
runs; the review record, TODO6, coverage, TODO5, and reconstruction note are
now synchronized. `KillUsedWaypoints` is promoted at `6/6`.

The preceding closure is `DrawPlanetProduction` E4 and its final promotion. The
retained original Win98 captures show selected `Oop Be Gone`, the empty
production panel, the `Production Queue for Oop Be Gone` dialog with
`-- Queue is Empty --`, and frame-refresh/fleet-selection changes without a
Program Error. Combined with the 14-check native service boundary, all four
evidence units are closed; the review record, TODO6, coverage, and
reconstruction note are synchronized. `DrawPlanetProduction` is now promoted
at `6/6`.

The earlier closure is `KillUsedWaypoints` E3. Its fresh host/native complete-
turn probe composes the real reconstructed turn coordinator, `MoveFleets`,
the packed fleet-table adapter, and `KillUsedWaypoints`; six checks confirm
caller ordering, movement write-back, stale-target retargeting, and PLORD
compaction. The native `C:\KUWTURN` run compiled and linked, exited 0, and
showed no Program Error dialog. That evidence is now followed by the final
promotion recorded above.

The preceding closure is `DrawPlanetStarbase` E3. Its fresh host/native service
probe checks the recovered hull-class title, shield and warp callback
arguments, destination-name lookup, button enablement, and gauge/reference
geometry; both sides report seven checks with zero failures. The native
`C:\DPSBN` run compiled and linked, exited 0, and showed no Program Error
dialog. `DrawPlanetStarbase` is now `0 + P`; only final ledger promotion
remains.

The earlier `KillUsedWaypoints` E1 closure added the packed adapter that models
the original `rglpfl` table, first-null termination, and offset-100 `lpplord`
references; the host and fresh `C:\KUWPACK` Win98 report each pass six packed
table checks with zero failures. That boundary is now supplemented by the
complete-turn E3 evidence above.

## Most recent source work

Three large AI-turn reconstructions received evidence-backed corrections to
their recovered `CheckAiShdefStatus`/`SplitOutShdefs` boundary. Each remains
partial because the later production and fleet-routing body is not yet
complete.

### `DoMacintiAiTurn` — 10,749 bytes

Files:

- `reconstructed/src/do_macinti_ai_turn.c`
- `tests/do_macinti_ai_turn_source_probe.c`
- `tests/do_macinti_ai_turn_typed_source_probe.c`
- `tests/do_macinti_ai_turn_resource_gate_probe.c`
- `tests/do_macinti_ai_turn_task_probe.c`
- `tests/do_macinti_ai_turn_production_probe.c`
- `tests/do_macinti_ai_turn_fleet_action_probe.c`
- `analysis/reconstruction/do-macinti-ai-turn.md`

Corrections from the raw C/assembly:

- design budgets are 50/70/100 for turns below 120, 120–199, and 200+;
- status callbacks expose the selected design and a shared 16-byte mask;
- late turns perform five split calls: the shared mask, entries 0, entries 1,
  entries 14/15, and entries 10/11;
- the probe checks the shared and fixed masks.

Verified host results: `MACINTI_AI_TURN_CHECKS=29`, typed checks 9, resource
gate checks 9, task checks 3, production checks 3, and fleet-action checks 3;
all report zero failures. The resource gate now also covers the recovered
queued-cost-plus-hull-batch subtraction, with exact-zero acceptance and
negative-remainder rejection.

Remaining Macinti work is the detailed planet-production policy, fleet
retargeting/order mutation, colonizer/transport routing, and full AI-table
integration.

### `DoTurinDroneAiTurn` — 8,777 bytes

Files:

- `reconstructed/src/do_turin_drone_ai_turn.c`
- `tests/do_turin_drone_ai_turn_source_probe.c`
- `tests/do_turin_drone_ai_turn_production_source_probe.c`
- `analysis/reconstruction/do-turin-drone-ai-turn.md`

The shared 16-byte status mask is now passed through to the late-turn split.
Host results: `TURIN_DRONE_AI_TURN_CHECKS=35` and production checks 6, both
zero failures.

Remaining Turin work is detailed production policy, fleet-order repair,
colonizer/transport routing, and full AI-table integration.

### `DoRobotoidAiTurn` — 7,201 bytes

Files:

- `reconstructed/src/do_robotoid_ai_turn.c`
- `tests/do_robotoid_ai_turn_source_probe.c`
- `tests/do_robotoid_ai_turn_production_source_probe.c`
- `analysis/reconstruction/do-robotoid-ai-turn.md`

The shared status mask is now preserved. The fourth late-turn split mask is
the raw assembly's entries 11/12/13, not a repeat of entries 0/1.
Host results: `ROBOTOID_AI_TURN_CHECKS=23` and production checks 12, both zero
failures.

## Earlier completed partial-slice corrections

Other recent partial-source work includes `EstFuelUse`, `DrawMineSurvey`, and
`FAttack`, with their source probes and TODO5/ledger entries updated. Their
current status remains partial where packed records, native integration, or
dependent behavior is still unresolved.

The current `NewGameWizard` source also has a fresh native checkpoint. The
typed source and 30-case probe compiled and linked with VC++ 1.52c in fresh
guest directory `C:\NWT`, and the Win98 report was
`NEW_GAME_WIZARD_CHECKS=30 FAILURES=0 RESULT=PASS`. Evidence is in
`analysis/compat/new-game-wizard-native.md` and
`runs/vmctl-vc152nwt/`; packed player storage, localized resources, and full
world-generation integration remain open.

`BrowserDlg` then closed its real USER/control/message-loop unit in fresh
`C:\BRDMSG`. The VC++ 1.52c build and link passed, and the native report was
`BROWSER_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`: actual parent and child
controls, 17 combo entries, paint/erase dispatch, Previous/Next commands,
invalidation, and Done teardown all passed, with no Program Error dialog. A
fresh original caller-path run then opened `Commands > Ship Design...`, entered
Components view, selected `Quick Jump 5` and `Long Hump 6`, changed to Armor
(`Tritanium`/`Crobmnium`), selected `Tritanium`, and changed to Beam Weapons
(`Laser`/`X-Ray Laser`) without an error dialog. BrowserDlg is now promoted to
6/6.

`ZipProdDlg` then closed its real USER/control/resource integration unit in
fresh `C:\ZPDMSG`. The VC++ 1.52c build and link passed, and
`runs/vmctl-vc152zpdmsg/ZPDMSG3.TXT` reports
`ZIP_PROD_DLG_USER_CHECKS=14 FAILURES=0 RESULT=PASS`. The actual message loop
created the parent, queue radios, list, Create/Rename/Delete/Help, and
OK/Cancel controls; dispatched init, paint, erase, control-color, queue
selection, delete, create, rename, help, and accept; and recorded two packed
items, dirty bit `0x20`, two rename callbacks, and clean teardown. No Program
Error dialog appeared. `ZipProdDlg` remains 5/6 with two named evidence units
open—packed global queue/name storage and the broader Add/Remove/Item Up/Item
Down/Clear mutation boundary—plus the separate promotion item. The current
Phase 2 burn-down at that E2 checkpoint was 132 evidence units plus 40
promotion units, or 172 open work items. The subsequent packed-storage
checkpoint below supersedes this snapshot.

The follow-up `ZipProdDlg` packed-storage checkpoint closed E1. CodeView and
the original initialization path identify five 40-byte `_zipprodq` records:
the 13-byte name begins at offset 0, followed by validity at 13,
no-research at 14, item count at 15, and twelve 16-bit packed words at
offset 16. The typed source and `FillZipProdLB` now preserve that layout. Host
and fresh VC++ 1.52c/Win98 probes each pass
`ZIP_PROD_DLG_CHECKS=36 FAILURES=0 RESULT=PASS`; compile/link pass and no
Program Error dialog appears. `ZipProdDlg` is now `1 + P`, with only the
broader production-queue mutation/command and exact resource boundary plus
promotion remaining. The current Phase 2 burn-down is 131 evidence units plus
40 promotion units, or 171 open work items.

The final `ZipProdDlg` checkpoint then closed that remaining boundary. The
original Win98 Planet Summary Report path exercised Add, a second Add with
changed required-mineral/resource totals, Item Down, Item Up, Clear, Add then
Remove, and Cancel back to the report. A fresh `C:\ZPDLAY` USER/listbox
harness reports `FILL_ZIP_PROD_LB_USER_CHECKS=9 FAILURES=0 RESULT=PASS` for
full and empty/invalid queue population, reset/add boundaries, invalidation,
paint, and message-loop delivery. No Program Error dialog appeared. Together
with the 36 packed/layout checks and 14-control USER loop, this closes Step 5;
the review, coverage, TODO6, and reconstruction records now promote
`ZipProdDlg` to 6/6. Current Phase 2 burn-down is **130 evidence units plus 39
promotion units, or 169 open work items**: 39 procedures at 5/6 and 12 at
6/6.

The next production/order checkpoint closed `ZipOrderDlg` E1. Its fresh host
and Win98 packed adapter each pass `ZIP_ORDER_DLG_PACKED_CHECKS=11
FAILURES=0 RESULT=PASS` over four 24-byte `_ziporder` records, five-word
Create copying, names and ampersands, validity cleanup, Rename persistence,
and Cancel closure. E2 localized resources and E3 complete live
Win16/original differential integration remain open. The current Phase 2
burn-down is **129 evidence units plus 39 promotion units, or 168 open work
items**; 39 procedures remain at 5/6 and 12 are at 6/6.

The follow-up native resource checkpoint closed `ZipOrderDlg` E2. The updated
host/native probe checks the original title, caption, empty-slot message, and
five order-action resource IDs/text; fresh Win98 reports
`ZIP_ORDER_DLG_CHECKS=11 FAILURES=0 RESULT=PASS` with compile/link pass,
empty redirected console, and no Program Error dialog. Only E3 complete live
Win16/original differential integration remains. Current Phase 2 burn-down is
**128 evidence units plus 39 promotion units, or 167 open work items**; 39
procedures remain at 5/6 and 12 are at 6/6.

The final `ZipOrderDlg` checkpoint closed E3 in a fresh `C:\ZOD` USER run
extended with an actual Win16 parent window and child controls. VC++ 1.52c
compile/link passed and `runs/vmctl-vc152zodmsg/ZODMSG.TXT` reports
`ZIP_ORDER_DLG_USER_CHECKS=12 FAILURES=0 RESULT=PASS` for paint, radio
selection, Create, Rename, Delete, control-color, Help, and OK teardown. The
original `Rename Fleet` caller/cancel boundary remains preserved in
`runs/vmctl-vc152current/fleet-rename.bmp`; no Program Error dialog appeared.
The reconstruction notes, integration JSON, review ledger, coverage record,
and TODO6 now promote `ZipOrderDlg` to 6/6. Current Phase 2 burn-down is
**127 evidence units plus 38 promotion units, or 165 open work items**: 38
procedures at 5/6 and 13 at 6/6.

The next `KillUsedWaypoints` checkpoint closed its localized message/resource
unit. The original compressed string table resolves message `0x0137` to
`Ultimate Recycling` and `0x004e` to `Annual resources from all planets:  `.
The strict host probe and fresh `C:\KUWRES` VC++ 1.52c run both report
`KILL_USED_WAYPOINTS_RESOURCE_CHECKS=2 FAILURES=0 RESULT=PASS`; compile/link
passed and no Program Error dialog appeared. Live fleet-table enumeration and
complete turn integration remain open. Current Phase 2 burn-down is
**126 evidence units plus 38 promotion units, or 164 open work items**: 38
procedures at 5/6 and 13 at 6/6.

The next `DrawPlanetProduction` checkpoint closed its localized production-label
unit. The original table resolves `0x32e` to `Production`, `0x218` to
`Completion: `, and `0x4e9` to `Route to`. The strict host probe and fresh
`C:\DPRES` VC++ 1.52c run report
`DRAW_PLANET_PRODUCTION_RESOURCE_CHECKS=3 FAILURES=0 RESULT=PASS`; compile/link
passed and no Program Error dialog appeared. Packed production-item lookup,
exact geometry/GDI integration, original differential coverage, and promotion
remain open. Current Phase 2 burn-down is **125 evidence units plus 38
promotion units, or 163 open work items**: 38 procedures at 5/6 and 13 at 6/6.

The following `DrawPlanetProduction` checkpoint closed its packed production
record/list-selection unit. The strict host probe and fresh `C:\DPREC` VC++
1.52c run exercised the linked 4-byte `PLPROD` header, two variable production
records, packed kind/item/count fields, name/schedule/format callbacks, listbox
messages, and direct-item formatting. The report is
`DRAW_PLANET_PRODUCTION_RECORD_CHECKS=16 FAILURES=0 RESULT=PASS`; compile/link
passed after the Win16 stack definition was corrected and no Program Error
dialog appeared. Exact geometry/GDI integration and original differential
coverage remain open. Current Phase 2 burn-down is **124 evidence units plus
38 promotion units, or 162 open work items**: 38 procedures at 5/6 and 13 at
6/6.

The following `DrawPlanetStarbase` checkpoint closed E2, the exact localized
resource boundary. A strict host probe and fresh `C:\SBRES` VC++ 1.52c run
checked the six row labels and the `none`/`Unlimited` special values in the
expected callback order, reporting
`DRAW_PLANET_STARBASE_RESOURCE_CHECKS=7 FAILURES=0 RESULT=PASS`. Compile/link
passed, the process exited 0, and no Program Error dialog appeared. E1 packed
planet/starbase/design-service completion and E3 exact shield/warp services,
full GDI/caller behavior, original differential coverage, and promotion remain
open. Current Phase 2 burn-down is **123 evidence units plus 38 promotion
units, or 161 open work items**: 38 procedures at 5/6 and 13 at 6/6.

The next `DrawPlanetStarbase` checkpoint closed E1, the packed
design/starbase lookup boundary. The readable decoder now follows the
recovered 56-byte planet, 147-byte starbase design, and 143-byte hull records,
and a strict host plus fresh `C:\SBDS` VC++ 1.52c run reports
`DRAW_PLANET_STARBASE_DESIGN_CHECKS=10 FAILURES=0 RESULT=PASS` for design-slot,
hull-ID, cargo/defense, mass-warp, destination, no-starbase, and bounds cases.
Compile/link passed, the process exited 0, and no Program Error dialog
appeared. Only E3 exact shield/warp services, full GDI/caller behavior,
original differential coverage, and promotion remain. Current Phase 2 burn-down
is **122 evidence units plus 38 promotion units, or 160 open work items**: 38
procedures at 5/6 and 13 at 6/6.

## Native VM status

The user restarted the VM because the guest was visually difficult to inspect
and manually started the guest agent. The last successful controller check was:

- agent: `STARSAGT 0.4.2`
- response: `PONG`
- guest windows: Win98 desktop, `Stars! VM Control Agent 0.4.2`, and `Vmctl1`
- correct VM: `StarDev` in 86Box

TODO6 now records a six-step Phase 2 maturity matrix tied directly to the
procedure definition of done. The current distribution is 38 procedures at
5/6 and 13 at final-promotion step 6/6. Its procedure-by-procedure table is
the authoritative progress view; remaining Phase 2 checkboxes stay open until
step 6. The fraction is a gate count rather than a percentage estimate:
5/6 means that steps 1–5 have evidence, but step 5 can still contain several
dependent packed-record, resource, UI, routing, or differential-integration
tasks. The AI cluster now has all six procedures at 5/6:
`DoRobotoidAiTurn`, `DoTurinDroneAiTurn`, `DoAutomitronAiTurn`,
`DoMacintiAiTurn`, `DoCyberAiTurn`, plus `DoRototillAiTurn`; the remaining 32
procedures have reached the broader step-5 acceptance boundary but still need
promotion work.

`DoRototillAiTurn` reached step 5/6 through a fresh packed integration
checkpoint. The host and Win98 reports each pass 12 checks over the recovered
56-byte `_planet` and 124-byte `_fleet` views, including mineral and ship-count
conversion, classification, fleet roles, repair dispatch, and final callback
ordering. The adapter keeps optimal value and desirability as service results,
matching the original call boundary. Queue construction, routing, raw `PLORD`
mutation, complete AI-table integration, and full original-turn differential
evidence remain open.

`DoRobotoidAiTurn` reached step 5/6 through a fresh packed integration
checkpoint. The host and Win98 reports each pass 18 checks over the recovered
56-byte `_planet` and 124-byte `_fleet` views, including status/split masks,
threat, production affordability, fleet roles, and final callback ordering.
Production resources/costs and design selections remain explicit service
results matching the original call boundary. Fleet-order repair, routing, raw
`PLORD` mutation, complete AI-table integration, and full original-turn
differential evidence remain open.

A host-only Robotoid fleet-gate matrix now passes
`ROBOTOID_AI_FLEET_GATE_CHECKS=10 FAILURES=0 RESULT=PASS` across empty,
colonizer, transport, already ordered armed, support/attack-bin,
`warship_count` fallback, and non-owned fleet cases. This makes the inputs to
the order-repair/routing seam explicit without claiming the missing action
logic. It closes no Step-5 unit; native replay, direct `PLORD` mutation,
complete AI-table state, and original-turn differential remain open. Evidence
is in `analysis/compat/do-robotoid-ai-turn-fleet-gates-e3.{md,json}`.

The typed source now dispatches a separate `repair_order` service boundary for
the raw missing-first-order phase before the normal fleet visitor. The strict
host probe passes `ROBOTOID_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS`
for missing-order dispatch, ordered/empty bypasses, policy and potency
forwarding, ordering, and failure propagation. This still closes no Step-5
unit because packed `PLORD` mutation, exact routing, native replay, and
original-turn comparison remain open. Evidence is in
`analysis/compat/do-robotoid-ai-turn-repair-boundary-e3.{md,json}`.

`DoTurinDroneAiTurn` reached step 5/6 through a fresh packed integration
checkpoint. The host and Win98 reports each pass 20 checks over the recovered
56-byte `_planet` and 124-byte `_fleet` views, including design-status masks,
two-pass planet classification, production affordability and queueing,
fleet-role totals, repair policy, and final callback ordering. Production
resources/costs, design selections, and order state remain explicit service
results matching the original call boundary. Detailed production policy,
fleet-order mutation, colonizer/transport routing, complete AI-table
integration, and full original-turn differential evidence remain open.

The typed Turin turn now dispatches a separate `repair_order` service boundary
before the normal fleet visitor. Its strict host probe passes
`TURIN_DRONE_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS` for
missing-order dispatch, ordered/non-owned bypasses, policy and potency
forwarding, ordering, and failure propagation. Packed `PLORD` mutation,
exact routing, native replay, and original-turn comparison remain open.

`DoAutomitronAiTurn` reached step 5/6 through a fresh packed integration
checkpoint. The host and Win98 reports each pass 20 checks over the recovered
56-byte `_planet` and 124-byte `_fleet` views, including merge and status
masks, research/shdef handoff, scanned/mineral production gating, queue
affordability, fleet-role aggregates, and final callback ordering. Planet
service values, production resources/costs, design selections, and detailed
order decisions remain explicit service results matching the original call
boundary. Colonizer/mineral fleet handling, order repair, remaining packed
order integration, and complete original-behavior evidence remain open.

`DoMacintiAiTurn` reached step 5/6 through a fresh packed integration
checkpoint. The host and Win98 reports each pass 31 checks over the recovered
56-byte `_planet` and 124-byte `_fleet` views, including merge/status/split
masks, production plan/queue, fleet-role aggregates, attack dispatch,
transport routing, finalization, and callback ordering. Mineral/service values,
production costs, design choices, and detailed order decisions remain explicit
service results matching the original call boundary. Per-planet table
selection, exact retarget/colonizer/transport predicates, direct packed order
mutation, and complete original-behavior evidence remain open.

`DoCyberAiTurn` reached step 5/6 through a fresh packed integration
checkpoint. The host and Win98 reports each pass 25 checks over the recovered
56-byte `_planet` and 124-byte `_fleet` views, including merge/status/split
masks, research and shdef handoff, low-mineral and hostile-threat gates,
production affordability, fleet-role filtering, and ordered basic-task,
packet, and queue finalization. Habitat/mineral service values, production
costs, and detailed order decisions remain explicit service results matching
the original call boundary. Packet routing, freighter behavior, detailed
fleet-order repair, direct packed `PLORD` mutation, complete AI-table
integration, and full original-behavior evidence remain open.

The typed Cyber turn now dispatches a separate `repair_order` service boundary
before the normal fleet visitor. Its strict host probe passes
`CYBER_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS`, including the five
merge-mask calls, ordered/non-owned bypasses, potency forwarding, ordering,
and failure propagation. Packed `PLORD` mutation, packet/freighter routing,
native replay, and original-turn comparison remain open.

A shared `stars_ai_repair_first_order` helper now applies the recovered
four-byte-header/18-byte-record PLORD repair writes while preserving unrelated
order bytes. The Robotoid repair callback exercises it end-to-end; the strict
host probe passes `AI_PLORD_REPAIR_SOURCE_CHECKS=9 FAILURES=0 RESULT=PASS`.
This is still a host-only single-order checkpoint: multi-order routing,
personality-specific action selection, native replay, and original-turn
comparison remain open. Evidence is in
`analysis/compat/ai-plord-repair-e3.{md,json}`.

The Robotoid source now exposes a separate typed route-plan/apply boundary.
The planner receives the fleet policy and turn, supports the recovered action
family constants, and the applier receives only non-empty plans before the
typed fleet visitor. Named action callbacks are selected before the generic
applier fallback, and the route probes exercise the specialized colonize
callback. The strict host probe passes
`ROBOTOID_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`, including
repair-before-plan ordering, no-op suppression, payload preservation, and
planner/applier failure propagation. This closes no authoritative Step-5
unit; exact route predicates, multi-order packed `PLORD` mutation, complete
AI-table state, native replay, and original-turn differential remain open.
Evidence is in `analysis/compat/do-robotoid-ai-turn-route-plan-e3.{md,json}`.

Turin and Cyber now expose the same typed plan/apply separation at their
fleet seams. The Turin strict host probe passes
`TURIN_DRONE_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`; the
Cyber probe passes
`CYBER_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`. Both cover
repair-before-plan ordering, policy/potency forwarding, no-op suppression,
payload preservation, non-owned filtering, and planner/applier failure
propagation. These are host-only checkpoints and close no authoritative
Step-5 unit; exact personality route predicates, packet/freighter behavior,
multi-order packed `PLORD` mutation, native replay, and original-turn
differential remain open. Evidence is in
`analysis/compat/do-turin-drone-ai-turn-route-plan-e3.{md,json}` and
`analysis/compat/do-cyber-ai-turn-route-plan-e3.{md,json}`.

The shared packed-order layer now also exposes
`stars_ai_move_order_to_plord`, mirroring the reviewed `FMoveAiFleet`
18-byte record copy used by colonize, wormhole, and target actions. Its strict
host probe passes `AI_PLORD_MOVE_ORDER_CHECKS=12 FAILURES=0 RESULT=PASS` for
first-order replacement, append-to-tail, endpoint comparison, all nine order
words, count update, and full-allocation rejection without mutation. The
existing `F_MOVE_AI_FLEET` and `FColonizeAiFleet` probes remain green. This is
still host-only and closes no authoritative Step-5 unit; personality target
searches, complete route decisions, native replay, and original-turn
differential remain open. Evidence is in
`analysis/compat/ai-plord-move-order-e3.{md,json}`.

The reviewed `FColonizeAiFleet` and `FGotoWormholeAiFleet` constructors now
also have a host integration through the shared packed-order adapter. The
strict probe passes `AI_FLEET_ACTION_PLORD_INTEGRATION_CHECKS=12 FAILURES=0
RESULT=PASS`, covering both exact order constructors, ideal-warp nibble
placement, first-order replacement, order-count preservation, selection
forwarding, and movement-success fleet flags. This remains host-only and
closes no authoritative Step-5 unit; personality target searches, exact route
precedence, native replay, and original-turn differential remain open.
Evidence is in
`analysis/compat/ai-fleet-action-plord-integration-e3.{md,json}`.

The reviewed `FMoveToNearestStarbase` constructor now also has a host
integration through the shared PLORD adapter. Its strict probe passes
`AI_STARBASE_ACTION_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS` for
lookup forwarding, replacement mode, target identity, exact `0x1040` flags,
packed record preservation, and no-target rejection. This remains host-only
and closes no authoritative Step-5 unit; personality target searches, route
precedence, native replay, and original-turn differential remain open.
Evidence is in
`analysis/compat/ai-starbase-action-plord-integration-e3.{md,json}`.

The reviewed `IdTargetFreighter` typed target-selection boundary now also has
a host integration through the shared PLORD adapter. Its strict probe passes
`AI_FREIGHTER_ACTION_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS` for
strict score ordering, target and point forwarding, exact `0x0100` flags,
first-order replacement, packed-record preservation, and the cargo-free
rejection path. This remains a host-only typed checkpoint and closes no
authoritative Step-5 unit: raw AI markings, cargo calculations, enemy/unknown
classification, complete eligibility, personality integration, native replay,
and original-turn differential remain open. Evidence is in
`analysis/compat/ai-freighter-action-plord-integration-e3.{md,json}`.

The reviewed `DoCyberFreighter` movement boundary now also has a host
integration through the shared PLORD adapter. Its strict probe passes
`AI_CYBER_FREIGHTER_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS` for
the owned-planet supply branch, target lookup, ideal-warp forwarding, exact
`0x11b0` flags, target/point preservation, first-order replacement, and the
fleet `0x8000` moving flag after success. This remains host-only and closes no
authoritative Step-5 unit; complete Cyber freighter behavior,
packet/freighter interaction, repeated-turn progress semantics, native replay,
and original-turn differential remain open. Evidence is in
`analysis/compat/ai-cyber-freighter-plord-integration-e3.{md,json}`.

`DoCyberFreighter` was then corrected so the drop-off progress field advances
only for carrying/drop-off orders. The strict host probe passes
`DO_CYBER_FREIGHTER_PROGRESS_CHECKS=9 FAILURES=0 RESULT=PASS` for pickup
without drop-off-bit mutation, successful drop-off progress, and
carrying-without-destination rollback with the expected transfer pair and
separate planet-progress increment. This remains host-only; full
mineral-rate/stock predicates, native replay, and original-turn differential
remain open, so promotion counts are unchanged. Evidence is in
`analysis/compat/ai-cyber-freighter-progress-e4.{md,json}`.

The raw owned-planet mineral-rate and stock gate in `DoCyberFreighter` was
then made explicit in the typed view. The strict host probe passes
`DO_CYBER_FREIGHTER_OWNED_GATE_CHECKS=12 FAILURES=0 RESULT=PASS` for negative
rate, low-rate stock `2000`, low-rate stock `2001`, and positive rate with
empty stock, including target kind, movement, and `-1000` or `+1000` transfer
direction. This remains host-only; other raw fleet-cargo predicates, native
replay, and original-turn differential remain open, so promotion counts are
unchanged. Evidence is in
`analysis/compat/ai-cyber-freighter-owned-gate-e5.{md,json}`.

The reviewed `TargetCyberArmada` destination boundary now also has a host
integration through the shared PLORD adapter. Its strict probe passes
`AI_CYBER_ARMADA_PLORD_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS` for
ordinary-planet precedence, enemy-fleet fallback, target kind and identity,
point forwarding, planet marking, exact `0x1140` planet and `0x1240` fleet
flags, first-order replacement, and packed-record preservation. This remains
host-only and closes no authoritative Step-5 unit; complete original-table
state, stale-order and potency branches, native replay of this integration,
and original-turn differential remain open. Evidence is in
`analysis/compat/ai-cyber-armada-plord-integration-e3.{md,json}`.

The `TargetCyberArmada` current-order branch was then corrected against the
raw procedure. The strict host regression probe passes
`TARGET_CYBER_ARMADA_ORDER_RETENTION_CHECKS=12 FAILURES=0 RESULT=PASS` for
valid near-planet continuation, stale-owner rejection, nearby-fleet retention
without lookup, and distant-fleet continuation into ordinary planet
selection. This is host source-fidelity evidence only; fresh native replay
and original-turn comparison remain open, so promotion counts are unchanged.
Evidence is in
`analysis/compat/ai-cyber-armada-order-retention-e4.{md,json}`.

The raw strong-hostile current-planet gate in `TargetCyberArmada` was then
corrected. The strict host probe passes
`TARGET_CYBER_ARMADA_HOSTILE_GATE_CHECKS=4 FAILURES=0 RESULT=PASS` for lookup
without destination enumeration, movement, or task clearing. This remains
host source-fidelity evidence only; fresh native replay, the complete
weak-hostile/random matrix, and original-turn differential remain open, so
promotion counts are unchanged. Evidence is in
`analysis/compat/ai-cyber-armada-hostile-gate-e5.{md,json}`.

The friendly-armada random gate in `TargetCyberArmada` was then corrected to
match the raw short-circuit structure. The strict host probe passes
`TARGET_CYBER_ARMADA_FRIENDLY_RANDOM_CHECKS=9 FAILURES=0 RESULT=PASS` for
random-roll fall-through, early return below the damage ceiling, and the
recovered second roll above the three-times-potency threshold. This remains
host-only; the complete native random matrix, weak-hostile branch matrix, and
original-turn differential remain open, so promotion counts are unchanged.
Evidence is in
`analysis/compat/ai-cyber-armada-friendly-random-e6.{md,json}`.

The weak-hostile random gate in `TargetCyberArmada` was then corrected to
preserve raw short-circuit call order. The strict host probe passes
`TARGET_CYBER_ARMADA_HOSTILE_RANDOM_CHECKS=20 FAILURES=0 RESULT=PASS` for
zero-roll retreat, one-roll rejection, two-roll rejection, and the three-roll
retreat path, including starbase destination, movement, task clear, and
ordinary-destination fall-through behavior. This remains host-only; fresh
native replay and original-turn differential remain open, so promotion counts
are unchanged. Evidence is in
`analysis/compat/ai-cyber-armada-hostile-random-e7.{md,json}`.

During the latest controlled 86Box observation, `StarDev` was running and the
Win98 desktop was visible, but the guest displayed its `Close Program` dialog
over the Stars window. A fresh host listener received no `AGENTINFO` banner
before it was stopped, so no native replay was attempted and the VM was not
reset. Native evidence for the new freighter checkpoints remains open.

The temporary host listener is not left running between sessions. The most
recent session reconnected successfully, rebuilt the corrected GenerateWorld
harness in fresh guest directory `C:\GWT`, and received:

```text
COMPILE=PASS
LINK=PASS
GENERATE_WORLD_CHECKS=31 FAILURES=0 RESULT=PASS
```

The prior “not enough space for arguments” dialog was avoided by using
directory-first CRLF wrappers, a local `WINSTUB.EXE`, and a report path inside
the fresh directory.

The next creation-flow checkpoint is also complete for `DrawNewGame2`. A fresh
`compat/vc152/draw-new-game2/` harness compiled and linked the current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_NEW_GAME2_CHECKS=2 FAILURES=0 RESULT=PASS`. The guest window list showed
no program-error dialog during execution. Evidence is preserved in
`analysis/compat/draw-new-game2-native.md` and `runs/vmctl-vc152dng2/`.
The remaining gap is packed player/type-table, complete localized formatting,
and full game GDI/resource integration.

The following creation-flow checkpoint is complete for `DrawNewGame3`. A fresh
`compat/vc152/draw-new-game3/` harness compiled and linked the current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_NEW_GAME3_CHECKS=2 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/draw-new-game3-native.md` and `runs/vmctl-vc152dng3/`.
The remaining gap is exact race-creation tables, localized formatting, packed
global rectangles, and full game GDI/resource integration.

The next creation-flow checkpoint is complete for `SimpleNewGameDlg`. A fresh
`compat/vc152/simple-new-game-dlg/` harness compiled and linked the current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`SIMPLE_NEW_GAME_DLG_CHECKS=5 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/simple-new-game-dlg-native.md` and
`runs/vmctl-vc152sngd/`. The remaining gap is packed player records,
localized resources, exact frame geometry, and full Win16 control integration.

The following creation-flow checkpoint is complete for `NewGameDlg`. A fresh
`compat/vc152/new-game-dlg/` harness compiled and linked the current source
with VC++ 1.52c in `StarDev`, and the corrected Win98 report was
`NEW_GAME_DLG_CHECKS=22 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/new-game-dlg-native.md` and `runs/vmctl-vc152ngd/`; an
initial no-CRT linker failure from the probe is retained separately as
diagnostic evidence. The remaining gap is packed game/race records, exact
race-button tables, localized resources, and full Win16 control integration.

An earlier creation-flow checkpoint is recorded for `IDropPart`. A fresh
`compat/vc152/i-drop-part/` harness compiled and linked the current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`I_DROP_PART_CHECKS=26 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/i-drop-part-native.md` and `runs/vmctl-vc152idp/`. A later
packed integration checkpoint binds the recovered 123-byte `_hul` record,
`rghs` at offset 58, four-byte `_hs` entries, and `chs` at offset 122; the
host and Win98 reports each pass 24 checks for raw slot round-trip, empty
removal, capacity-limited merging, source decrement, rejection, and
no-modify behavior. Evidence is preserved in
`analysis/compat/i-drop-part-integration.md` and
`runs/vmctl-vc152idpint/`. Exact Win16 hit testing and drag initiation,
build-dialog redraw/control integration, and complete original drop behavior
remain open at that earlier checkpoint. Subsequent E1/E2/E3 packets and
Step-6 ledger synchronization have since promoted the procedure to `6/6`.

An earlier creation-flow checkpoint is recorded for `FillBuildDD`. A fresh
`compat/vc152/fill-build-dd/` harness compiled and linked the current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`FILL_BUILD_DD_CHECKS=8 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/fill-build-dd-native.md`,
`analysis/compat/fill-build-dd-integration.md`, and
`runs/vmctl-vc152fbd/`; the original executable also showed populated
`Armed Probe` Existing Designs and `Small Freighter` Available Hull Types
selector surfaces in `runs/vmctl-vc152current/`, with no Program Error dialog.
One non-fatal signed/unsigned comparison warning remains visible in the
compile log. Subsequent E1 packed-record and E2 localized-label/resource
packets have closed those boundaries; only E3 full population, queue/resize,
caller/control integration, and Step-6 promotion remain.

A live original-executable diagnostic also reached the editable Ship & Starbase
Designer for `IDropPart`: a copied `Armed Probe` showed the component
inventory, populated slots, and selected-component summary, and selecting
`Long Hump 6` changed the inventory detail without a Program Error dialog. The
temporary copy was cancelled. Because the current host controller cannot issue
a press/move/release drag sequence, this confirms only the surrounding dialog
boundary rather than a complete original drop trace. The later packed
integration checkpoint advances `IDropPart` to step 5/6. Evidence is in
`analysis/compat/i-drop-part-live-diagnostic.md` and
`runs/vmctl-vc152idp-live/`.

The first planet/fleet-cluster checkpoint is complete for `PlanetWndProc`. A
fresh `compat/vc152/planet-wnd-proc/` harness compiled and linked the current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`PLANET_WND_PROC_CHECKS=43 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/planet-wnd-proc-native.md` and `runs/vmctl-vc152pwp/`. Exact
child layout, packed transfer lookup, resources, edit subclass, and full
drawing/focus integration remain open.

The next planet-display checkpoint is complete for `DrawPlanetStats`. A fresh
`compat/vc152/draw-planet-stats/` harness compiled and linked the current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_PLANET_STATS_CHECKS=22 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/draw-planet-stats-native.md` and `runs/vmctl-vc152dps/`.
Packed planet/race lookups, localization, fixed-point formatting, and full
GDI integration remain open.

The next planet-display checkpoint is complete for `DrawPlanetStarbase`. A
fresh `compat/vc152/draw-planet-starbase/` harness compiled and linked the
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_PLANET_STARBASE_CHECKS=14 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/draw-planet-starbase-native.md` and
`runs/vmctl-vc152dpsn/`. Packed planet/starbase records, localized resources,
and full GDI integration remain open.

The following `DrawPlanetStarbase` checkpoint closed E2, the exact localized
resource boundary. A strict host probe and fresh `C:\SBRES` VC++ 1.52c run
checked the six row labels and the `none`/`Unlimited` special values in the
expected callback order, reporting
`DRAW_PLANET_STARBASE_RESOURCE_CHECKS=7 FAILURES=0 RESULT=PASS`. Compile/link
passed, the process exited 0, and no Program Error dialog appeared. E1 packed
planet/starbase/design-service completion and E3 exact shield/warp services,
full GDI/caller behavior, original differential coverage, and promotion remain
open. At that point in the handoff chronology, the Phase 2 count was **123
evidence units + 38 promotion units = 161 open work items**, with 38
procedures at 5/6 and 13 at 6/6. Evidence is preserved in
`runs/vmctl-vc152sbr/` and the corresponding
`analysis/compat/draw-planet-starbase-*` notes.

The next `DrawPlanetStarbase` checkpoint closed E1, the packed
design/starbase lookup boundary. The readable decoder follows the recovered
56-byte planet, 147-byte starbase design, and 143-byte hull records; the fresh
host/native `C:\SBDS` run reports
`DRAW_PLANET_STARBASE_DESIGN_CHECKS=10 FAILURES=0 RESULT=PASS` for design-slot,
hull-ID, cargo/defense, mass-warp, destination, no-starbase, and bounds cases.
Compile/link passed, the process exited 0, and no Program Error dialog
appeared. Only E3 exact shield/warp services, full GDI/caller behavior,
original differential coverage, and promotion remain. Evidence is preserved in
`runs/vmctl-vc152sbds2/` and the corresponding reconstruction/compat notes.

The next planet-display checkpoint is complete for `DrawPlanShipBitmap`. A
fresh `compat/vc152/draw-plan-ship-bitmap/` harness compiled and linked the
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_PLAN_SHIP_BITMAP_CHECKS=9 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/draw-plan-ship-bitmap-native.md` and
`runs/vmctl-vc152dpsm/`. Exact Win16 GDI handles, packed selection state,
localized names, and full planet integration remain open.

The next planet-display checkpoint is complete for `DrawPlanetShipList`. A
fresh `compat/vc152/draw-planet-ship-list/` harness compiled and linked the
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_PLANET_SHIP_LIST_CHECKS=19 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/draw-planet-ship-list-native.md` and
`runs/vmctl-vc152dpsl/`. Packed fleet/thing transfer records, exact Win16
control handles, localized text/gauges, and full drawing integration remain
open.

The next fleet-transfer checkpoint is complete for `TransferStuff`. A fresh
`compat/vc152/transfer-stuff/` harness compiled and linked the current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`TRANSFER_STUFF_CHECKS=20 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/transfer-stuff-native.md` and `runs/vmctl-vc152trn/`. Packed
fleet/planet records, exact transfer-dialog behavior, cargo balancing, and
Win16 lookup/refresh integration remain open.

The next fleet-transfer checkpoint is complete for `FTrackXfer`. A fresh
`compat/vc152/f-track-xfer/` harness compiled and linked the current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`F_TRACK_XFER_CHECKS=24 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/f-track-xfer-native.md` and `runs/vmctl-vc152ftx/`. Packed
transfer records, Win16 capture behavior, cargo/supply mutation, and full
integration remain open.

The route-planning checkpoint is complete for `AutoRouteFleet`. A fresh
`compat/vc152/auto-route-fleet/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`AUTO_ROUTE_FLEET_CHECKS=4 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog, and redirected console output was empty.
Evidence is preserved in `analysis/compat/auto-route-fleet-native.md` and
`runs/vmctl-vc152arf/`. Exact packed order/resource offsets, floating helper
constants, original route differential cases, and full fleet integration
remain open.

The packed/integration checkpoint is now also complete for `AutoRouteFleet`.
A fresh `compat/vc152/auto-route-fleet-integration/` adapter compiled and
linked with VC++ 1.52c in `StarDev`; the returned Win98 report was
`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=3 FAILURES=0 RESULT=PASS`, and the
post-run window list showed no Program Error dialog. The adapter binds the
recovered 124-byte `_fleet` view, four-byte `PLORD` header, and 18-byte order
records, then checks target/coordinate/flag writes, stargate selection, fuel
descent, and mineral-cargo exclusion. The original Fleet Waypoints/Fuel &
Cargo surface is preserved as the consumer boundary. Exact resource and
route-helper behavior and complete live fleet-turn comparison remain open.
Evidence is preserved in
`analysis/compat/auto-route-fleet-integration.{md,json}` and
`runs/vmctl-vc152arfint/`.

The mine-survey display checkpoint is complete for `DrawMineSurvey`. A fresh
`compat/vc152/draw-mine-survey/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_MINE_SURVEY_CHECKS=5 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog, and redirected console output was empty.
Evidence is preserved in
`analysis/compat/draw-mine-survey-native.md`,
`analysis/compat/draw-mine-survey-integration.md`, and
`runs/vmctl-vc152dms/`. The original Win98 small-layout selection-summary
child was also observed for `Oop Be Gone` and redrawn for `Armed Probe #1`
without a program-error dialog. Detailed GDI layout, localized formatting,
packed record/resource integration, fleet/planet derived arithmetic, and full
mine-survey integration remain open.

The minefield dispatcher checkpoint is complete for `MineClick`. A fresh
`compat/vc152/mine-click/` adapter compiled and linked the exact current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`MINE_CLICK_CHECKS=9 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/mine-click-native.md`,
`analysis/compat/mine-click-integration.md`, and
`runs/vmctl-vc152mcl/`. The original Win98 selection-summary arrow advanced
from `Oop Be Gone` to `Armed Probe #1` and redrew the dependent panes without
a program-error dialog. Packed `GlobalPD`/planet/fleet/cargo/minefield/part
records, localized popup construction, derived mining/terraform arithmetic,
and full minefield integration remain open.

The mineral-estimation checkpoint is complete for `EstMineralsMined`. A fresh
`compat/vc152/est-minerals-mined/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`EST_MINERALS_MINED_CHECKS=5 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/est-minerals-mined-native.md` and `runs/vmctl-vc152emm/`.
Packed planet/fleet layout, exact fixed-point differential behavior, and
complete mineral-record mutation remain open.

The packed/integration checkpoint is now also complete for
`EstMineralsMined`. A fresh `compat/vc152/est-minerals-mined-integration/`
adapter compiled and linked with VC++ 1.52c in `StarDev`; the returned Win98
report was `EST_MINERALS_MINED_INTEGRATION_CHECKS=4 FAILURES=0 RESULT=PASS`,
and the post-run window list showed no Program Error dialog. The adapter
round-trips the recovered `_planet` player, percentage, concentration, and
`rgwtMin` offsets. The original planet surface showed the Minerals On Hand
rows, but a populated mining-turn differential remains open. Evidence is
preserved in `analysis/compat/est-minerals-mined-integration.{md,json}` and
`runs/vmctl-vc152emmint/`.

The production-dialog dispatcher checkpoint is complete for `ProductionDlg`.
A fresh `compat/vc152/production-dlg/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`PRODUCTION_DLG_CHECKS=9 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/production-dlg-native.md` and `runs/vmctl-vc152pdg/`. Packed
production globals, exact child geometry, localized resources, and complete
popup/command integration remain open.

The production-drawing checkpoint is complete for `DrawProductionDlg`. A fresh
`compat/vc152/draw-production-dlg/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_PRODUCTION_DLG_CHECKS=1 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/draw-production-dlg-native.md` and `runs/vmctl-vc152dpd/`.
Packed production globals, exact formatting/resource tables, localized
strings, and full GDI integration remain open.

The production-schedule checkpoint is complete for `EstimateItemProdSched`. A
fresh `compat/vc152/estimate-item-prod-sched/` adapter compiled and linked the
exact current source with VC++ 1.52c in `StarDev`, and the returned Win98
report was `ESTIMATE_ITEM_PROD_SCHED_CHECKS=4 FAILURES=0 RESULT=PASS`. The
post-run guest window list showed no program-error dialog. Evidence is
preserved in `analysis/compat/estimate-item-prod-sched-native.md` and
`runs/vmctl-vc152eips/`. Exact packed production records,
resource/population mutation, CBuildProdItem integration, and original
differential cases remain open.

The packed/integration checkpoint is now also complete for
`EstimateItemProdSched`. A fresh `compat/vc152/estimate-item-prod-sched-
integration/` adapter compiled and linked with VC++ 1.52c in `StarDev`; the
returned Win98 report was
`ESTIMATE_ITEM_PROD_SCHED_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`, and the
post-run window list showed no Program Error dialog. The adapter binds the
recovered four-byte `PLPROD` header and `_prod` queue records, invokes the
reconstructed `CBuildProdItem` boundary, and round-trips queue quantities and
resource/object/completion state. The original Production Queue surface
showed the available-item, empty-queue, and required-mineral boundary. A
broader populated schedule differential and complete production-record/resource
mutation comparison remain open. Evidence is preserved in
`analysis/compat/estimate-item-prod-sched-integration.{md,json}` and
`runs/vmctl-vc152eipint/`.

The production-queue dialog checkpoint is complete for `ZipProdDlg`. A fresh
`compat/vc152/zip-prod-dlg/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`ZIP_PROD_DLG_CHECKS=21 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/zip-prod-dlg-native.md` and `runs/vmctl-vc152zpd/`. Packed
queue/name records, production bit layout, exact localized resources,
rename-dialog behavior, and full Win16 control integration remain open.

The research-dialog checkpoint is complete for `ResearchDlg`. A fresh
`compat/vc152/research-dlg/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`RESEARCH_DLG_CHECKS=9 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/research-dlg-native.md` and `runs/vmctl-vc152rsd/`. Packed
player storage, localized resources, and full Win16 control integration remain
open.

The next Phase 2 checkpoint expanded `ResearchDlg` packed-player validation in
fresh `C:\RSD2`. The strict host/native matrix now has 10 checks covering the
exact player write offset `0x22`, two-byte length, ordinary packed value
`0x12`, and zero-percent/technology-5/future-technology-6 value `0x65`.
VC++ 1.52c compile/link passed with the existing intentional near-pointer
combo-string warning, the Win98 report was
`RESEARCH_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`, the process exited 0, and no
Program Error dialog appeared. `ResearchDlg` remains at 5/6; localized
resources, exact Win16 control integration, and promotion remain open.
Evidence is in `analysis/compat/research-dlg-native.md`,
`analysis/compat/research-integration.json`, and
`runs/vmctl-vc152rsd2/`.

The final `ResearchDlg` Step-5 evidence unit was closed in isolated `C:\RSDM`.
The native-only `RSDMSG.C` harness created real Win16 parent and child
controls, ran the `GetMessage`/`TranslateMessage`/`DispatchMessage` loop, and
routed initialization, radio selection, and Done through the reconstructed
procedure. VC++ 1.52c compile/link passed; the final report was
`RESEARCH_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`, with the expected commit
and two-byte write at offset `0x22`. The final guest window list showed no
Program Error dialog. Evidence is in
`analysis/compat/research-dlg-native.md`,
`analysis/compat/research-integration.json`, and
`runs/vmctl-vc152rsdmsg/`. The final review-ledger synchronization is now
recorded; `ResearchDlg` is promoted to `6/6`.

The browser-dialog checkpoint is complete for `BrowserDlg`. A fresh
`compat/vc152/browser-dlg/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`BROWSER_DLG_CHECKS=19 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/browser-dlg-native.md` and `runs/vmctl-vc152brd/`. Packed
`vpartBrowser` records, exact component lookup tables, localized string
storage, and full USER control/message integration remain open.

The next Phase 2 checkpoint expanded `BrowserDlg` packed-item validation in
fresh `C:\BRD2`. The strict host/native matrix now has 20 checks covering
preservation of the packed high byte while the low-byte component index moves
`0x1201 -> 0x1200 -> 0x1201`. VC++ 1.52c compile/link passed with the existing
intentional near-pointer combo-string warning, the Win98 report was
`BROWSER_DLG_CHECKS=20 FAILURES=0 RESULT=PASS`, the process exited 0, and no
Program Error dialog appeared. `BrowserDlg` remains at 5/6; exact lookup
tables, localized storage, complete USER control/message integration, and
promotion remain open. Evidence is in
`analysis/compat/browser-dlg-native.md`,
`analysis/compat/browser-dlg-integration.json`, and
`runs/vmctl-vc152brd2/`.

The next `BrowserDlg` checkpoint recovered the exact component-group table from
the original code segment at `CS:1EB5`. A fresh `C:\BRD3` VC++ 1.52c run now
passes `BROWSER_DLG_CHECKS=24 FAILURES=0 RESULT=PASS`, including selection
mapping and empty-combo next/previous wrapping across the sentinel plus sixteen
normal group values while preserving the packed item word. Compile and link
passed, the process exited 0, and no Program Error dialog appeared.
`BrowserDlg` remains at 5/6; localized storage, complete USER control/message
integration, packed `vpartBrowser` payload consumption, original differential
coverage, and promotion remain open. Evidence is in
`analysis/compat/browser-dlg-integration.{md,json}` and
`runs/vmctl-vc152brd3/`.

An additional `BrowserDlg` native checkpoint in fresh `C:\BRD4` verifies the
exact compressed-string resource sequence `0x43F` through `0x44F` (17 entries)
and reports `BROWSER_DLG_CHECKS=25 FAILURES=0 RESULT=PASS`. Compile and link
passed, the process exited 0, and no Program Error dialog appeared.
`BrowserDlg` remains at 5/6; the actual localized payloads, complete USER
control/message integration, packed `vpartBrowser` payload consumption,
original differential coverage, and promotion remain open. Evidence is in
`analysis/compat/browser-dlg-integration.{md,json}` and
`runs/vmctl-vc152brd4/`.

The subsequent `BrowserDlg` packed-record checkpoint in fresh `C:\BRD5`
round-trips the packed `vpartBrowser` group/item header and recovered
`FLookupPart` offsets for group `0x0008` (base `0x04E0`, stride `0x36`, 12
entries) and group `0x0010` (base `0x2450`, stride `0x3C`, 24 entries) through
initialization, navigation, and selection. The native report is
`BROWSER_DLG_CHECKS=31 FAILURES=0 RESULT=PASS`; compile and link passed, the
process exited 0, and no Program Error dialog appeared. `BrowserDlg` remains
at 5/6 pending actual component payloads, localized string contents, complete
USER control/message integration, original differential coverage, and
promotion. Evidence is in
`analysis/compat/browser-dlg-integration.{md,json}` and
`runs/vmctl-vc152brd5/`.

The next `BrowserDlg` packed-component checkpoint in fresh `C:\BRD6`
materializes and consumes the recovered 52-byte `_compart` record fields—ID,
technology bytes, name, mass, resource cost, ore costs, and bitmap ID—after
group 8 and group `0x10` selection. The native report is
`BROWSER_DLG_CHECKS=33 FAILURES=0 RESULT=PASS`; compile and link passed, the
process exited 0, and no Program Error dialog appeared. `BrowserDlg` remains
at 5/6 pending original payload values, localized string contents, complete
USER control/message integration, original differential coverage, and
promotion. Evidence is in
`analysis/compat/browser-dlg-integration.{md,json}` and
`runs/vmctl-vc152brd6/`.

The next `BrowserDlg` payload checkpoint in fresh `C:\BRD7` replaces the
synthetic component bytes with records recovered from `target/stars.exe` at
the original NE segment-2 table offsets. The strict host/native matrix now
passes `BROWSER_DLG_CHECKS=35 FAILURES=0 RESULT=PASS` over actual technology
masks, names, mass/resource costs, ore costs, and bitmap IDs from both lookup
tables. Compile and link passed, the process exited 0, and no Program Error
dialog appeared. `BrowserDlg` remains at 5/6 pending localized string
payloads, complete USER control/message integration, original differential
coverage, and promotion. Evidence is in
`analysis/compat/browser-dlg-integration.{md,json}` and
`runs/vmctl-vc152brd7/`.

The following `BrowserDlg` localized-payload checkpoint in fresh `C:\BRD8`
extends the resource-ID sequence to actual decoded text for all 17 original
compressed-string IDs `0x43F` through `0x44F`. The native report is
`BROWSER_DLG_CHECKS=36 FAILURES=0 RESULT=PASS`; compile and link passed, the
process exited 0, and no Program Error dialog appeared. BrowserDlg remains at
5/6 pending complete USER control/message integration, original differential
coverage, and promotion. Evidence is in
`analysis/compat/browser-dlg-integration.{md,json}` and
`runs/vmctl-vc152brd8/`.

The score/victory checkpoint is complete for `UpdatePlayerScores`. A fresh
`compat/vc152/update-player-scores/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`UPDATE_PLAYER_SCORES_CHECKS=34 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/update-player-scores-native.md` and
`runs/vmctl-vc152ups/`. Packed `_scorex`/`_player` records, fixed-point
helpers, global victory fields, prepended-message serialization, and full
turn/report integration remain open.

The packed/integration checkpoint is now also complete for
`UpdatePlayerScores`. A fresh `compat/vc152/update-player-scores-integration/`
adapter compiled and linked with VC++ 1.52c in `StarDev`; the returned Win98
report was `UPDATE_PLAYER_SCORES_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS`,
and the post-run window list showed no Program Error dialog. The adapter
round-trips the recovered `_scorex` score/rank/validity/achievement fields and
`_player.fDead` bit, and checks prepended-message headers and lengths. The
original `Player Scores` dialog showed its headings and category grid, but the
loaded save had blank values, so a populated-value differential remains open.
Evidence is preserved in
`analysis/compat/update-player-scores-integration.md` and
`runs/vmctl-vc152upsint/`.

The order-service checkpoint is complete for `SatisfyOrders`. A fresh
`compat/vc152/satisfy-orders/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`SATISFY_ORDERS_CHECKS=32 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/satisfy-orders-native.md` and `runs/vmctl-vc152syo/`.
Packed order battle-plan accounting, technology discovery, fleet and
ship-design copying, salvage creation, complete message layouts, and full
turn integration remain open.

The packed/integration checkpoint is now also complete for `SatisfyOrders`.
A fresh `compat/vc152/satisfy-orders-integration/` adapter compiled and linked
with VC++ 1.52c in `StarDev`; the returned Win98 report was
`SATISFY_ORDERS_INTEGRATION_CHECKS=3 FAILURES=0 RESULT=PASS`, and the post-run
window list showed no Program Error dialog. The adapter binds the recovered
124-byte `_fleet` and 18-byte `_order` views, routes a raw transfer order
through the typed resource core, and checks validity/turn-flag write-back and
inactive-fleet gating. The original Fleet Orders surface is preserved as the
consumer boundary. Battle-plan/resource accounting, remaining special-order
branches, complete message layouts, and full turn replay remain open.
Evidence is preserved in
`analysis/compat/satisfy-orders-integration.{md,json}`,
`runs/vmctl-vc152syint/`, and
`runs/vmctl-vc152current/orders-before.bmp`.

The combat-section checkpoint is complete for `FAttack`. A fresh
`compat/vc152/f-attack/` adapter compiled and linked the exact current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`F_ATTACK_CHECKS=33 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/f-attack-native.md` and `runs/vmctl-vc152fat/`.
Packed hull/part lookup, exact battle-part scaling, damage-engine integration,
and full battle-record integration remain open.

The packed/integration checkpoint is now also complete for `FAttack`. A fresh
`compat/vc152/f-attack-integration/` adapter compiled and linked with VC++
1.52c in `StarDev`; the returned Win98 report was
`F_ATTACK_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`, and the post-run window
list showed no Program Error dialog. The adapter binds the recovered 29-byte
`_tok`, 123-byte `_hul`, and six-byte battle record, then routes an attack
through `FDamageTok` to check raw shield/count writes, casualty handling, and
hit/first-target fields. The original battle-report surface is preserved as
the consumer boundary. Exact packed part lookup, complete battle-part
scaling, broader damage differentials, and full battle replay remain open.
Evidence is preserved in
`analysis/compat/f-attack-integration.md` and
`runs/vmctl-vc152faint/`.

The battle-lifecycle checkpoint is complete for `FDoCoolBattle`. A fresh
`compat/vc152/f-do-cool-battle/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`F_DO_COOL_BATTLE_CHECKS=53 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/f-do-cool-battle-native.md` and `runs/vmctl-vc152fcb/`.
Packed token/global mapping, exact damage behavior, full battle-log
integration, and remaining record cases remain open.

The packed-log/original checkpoint is now also recorded for
`FDoCoolBattle`. The existing fresh `C:\CB` harness checks the raw 14-byte
battle header, six-byte movement/attack records, append-at-end staging,
per-battle serialized length, and the `0xffff` overflow marker; its native
report remains `F_DO_COOL_BATTLE_CHECKS=53 FAILURES=0 RESULT=PASS` with no
Program Error dialog. The original battle-report surface is preserved as the
consumer boundary. Packed token/global mapping, global log compaction, exact
movement/damage, remaining records, and full battle replay remain open.
Evidence is preserved in
`analysis/compat/f-do-cool-battle-integration.{md,json}`,
`runs/vmctl-vc152fcb/`, and
`runs/vmctl-vc152current/report-battles.bmp`.

The Robotoid AI checkpoint is complete for `DoRobotoidAiTurn`. A fresh
`compat/vc152/do-robotoid-ai-turn/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`ROBOTOID_AI_TURN_CHECKS=23 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/do-robotoid-ai-turn-native.md` and `runs/vmctl-vc152rot/`.
Fleet-order repair, colonizer/transport routing, and full AI-table integration
remain open.

The Turin Drone AI checkpoint is complete for `DoTurinDroneAiTurn`. A fresh
`compat/vc152/do-turin-drone-ai-turn/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`TURIN_DRONE_AI_TURN_CHECKS=35 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/do-turin-drone-ai-turn-native.md` and
`runs/vmctl-vc152tur/`.
Detailed production policy, fleet-order repair, colonizer/transport routing,
and full AI-table integration remain open.

The Automitron AI checkpoint is complete for `DoAutomitronAiTurn`. A fresh
`compat/vc152/do-automitron-ai-turn/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`AUTOMITRON_AI_TURN_CHECKS=31 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/do-automitron-ai-turn-native.md` and
`runs/vmctl-vc152auto/`. Colonizer/mineral fleet handling, fleet-order repair,
packed integration, and original-behavior evidence remain open.

The Rototill AI checkpoint is complete for `DoRototillAiTurn`. A fresh
`compat/vc152/do-rototill-ai-turn/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`ROTOTILL_AI_TURN_CHECKS=15 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/do-rototill-ai-turn-native.md` and
`runs/vmctl-vc152rotill/`. Queue construction, detailed routing, packed
integration, and original-behavior evidence remain open.

The Cyber AI checkpoint is complete for `DoCyberAiTurn`. A fresh
`compat/vc152/do-cyber-ai-turn/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`CYBER_AI_TURN_CHECKS=32 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog. Evidence is preserved in
`analysis/compat/do-cyber-ai-turn-native.md` and
`runs/vmctl-vc152cyber/`. Packet routing, freighter behavior, fleet-order
repair, packed integration, and original-behavior evidence remained open at
that native-checkpoint milestone; the later packed integration checkpoint is
recorded above in `C:\CYBINT` and advances the procedure to step 5/6.

The ReportDlg checkpoint is complete for `ReportDlg`. A fresh
`compat/vc152/report-dlg/` adapter compiled and linked the exact current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`REPORT_DLG_CHECKS=26 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/report-dlg-native.md` and `runs/vmctl-vc152report/`. Packed
report records, menu resources, USER/GDI integration, and caller integration
remain open.

The ExecuteReportClick checkpoint is complete for `ExecuteReportClick`. A
fresh `compat/vc152/execute-report-click/` adapter compiled and linked the
exact current source with VC++ 1.52c in `StarDev`, and the returned Win98
report was `EXECUTE_REPORT_CLICK_CHECKS=21 FAILURES=0 RESULT=PASS`. The
post-run guest window list showed no program-error dialog. Evidence is
preserved in `analysis/compat/execute-report-click-native.md` and
`runs/vmctl-vc152execute/`. Packed report rows, the GlobalPD union, USER/GDI
integration, and caller integration remain open.

The DumpPlanets checkpoint is complete for `DumpPlanets`. A fresh
`compat/vc152/dump-planets/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DUMP_PLANETS_CHECKS=16 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/dump-planets-native.md` and `runs/vmctl-vc152dumpplanets/`.
Localized formatting, packed integration, and original-behavior evidence
remain open.

The DumpPlanets packed-record checkpoint is now complete. A strict host and
fresh native `C:\DPINT` adapter each passed
`DUMP_PLANETS_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS` over the recovered
56-byte `_planet` fields and detailed report-row dispatch; no Program Error
dialog appeared. The procedure remains at 5/6 pending localized formatting,
full starbase/wormhole/production service coverage, original differential
behavior, and promotion.

The DumpFleets checkpoint is complete for `DumpFleets`. A fresh
`compat/vc152/dump-fleets/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DUMP_FLEETS_CHECKS=10 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/dump-fleets-native.md` and `runs/vmctl-vc152dumpfleets/`;
localized formatting, packed integration, and original-behavior evidence
remain open.

The DrawPopup checkpoint is complete for `DrawPopup`. A fresh
`compat/vc152/draw-popup/` adapter compiled and linked the exact current source
with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_POPUP_CHECKS=51 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/draw-popup-native.md` and `runs/vmctl-vc152drawpopup/`; packed
records, localization, detailed GDI integration, and original-behavior
evidence remain open.

The Popup checkpoint is complete for `Popup`. A fresh
`compat/vc152/popup/` adapter compiled and linked the exact current source with
VC++ 1.52c in `StarDev`, and the returned Win98 report was
`POPUP_CHECKS=31 FAILURES=0 RESULT=PASS`. The post-run guest window list showed
no program-error dialog. Evidence is preserved in
`analysis/compat/popup-native.md` and `runs/vmctl-vc152popup/`; localized
measurement, packed records, detailed content layout, and original-behavior
evidence remain open.

The original/UI step-5 checkpoint is now also recorded for `DrawPopup` and
`Popup`. The original Win98 executable's mineral, population, report, popup
base, starting popup, planet popup, and restored-frame surfaces are preserved
without a Program Error dialog. Combined with the existing 51-case
`DrawPopup` and 31-case `Popup` native probes, this establishes their Win16
consumer boundaries. Packed `GlobalPD`/fleet/planet/hull records, localized
measurement and strings, hull filtering, formatting, and detailed content/GDI
rendering remain open before promotion. Evidence is preserved in
`analysis/compat/draw-popup-integration.{md,json}`,
`analysis/compat/popup-integration.{md,json}`, and
`runs/vmctl-vc152current/`.

The PtDisplayPlanetStateInfo checkpoint is complete for
`PtDisplayPlanetStateInfo`. A fresh
`compat/vc152/pt-display-planet-state-info/` adapter compiled and linked the
exact current source with VC++ 1.52c in `StarDev`, and the returned Win98
report was `PT_DISPLAY_PLANET_STATE_INFO_CHECKS=34 FAILURES=0 RESULT=PASS`.
The post-run guest window list showed no program-error dialog. Evidence is
preserved in `analysis/compat/pt-display-planet-state-info-native.md`,
`analysis/compat/pt-display-planet-state-info-integration.md`, and
`runs/vmctl-vc152stateinfo/`. The original Win98 small-layout
selection-summary pane showed the environment and mineral state rows without
a program-error dialog. Packed lookup, formatting, detailed GDI integration,
and promotion remain open.

The `PtDisplayPlanetPopInfo` checkpoint is complete. A fresh
`compat/vc152/pt-display-planet-pop-info/` adapter compiled and linked the
exact current source with VC++ 1.52c in `StarDev`, and the returned Win98
report was `PT_DISPLAY_PLANET_POP_INFO_CHECKS=25 FAILURES=0 RESULT=PASS`.
The post-run guest window list showed no program-error dialog. Evidence is
preserved in `analysis/compat/pt-display-planet-pop-info-native.md`,
`analysis/compat/pt-display-planet-pop-info-integration.md`, and
`runs/vmctl-vc152popinfo/`. The original Win98 small-layout selection-summary
pane showed `Value: 100%`, `Population: 25,000`, and current-report status
without a program-error dialog. Packed lookup, formatting, detailed GDI
integration, and promotion remain open.

`ReportDlg` now has a packed-record integration checkpoint. A fresh
`compat/vc152/report-dlg-integration/` harness compiled and linked the current
source with VC++ 1.52c in `StarDev`; the host and Win98 reports each passed 20
checks over the recovered 54-byte `_rpt` record, raw-to-typed round trips,
initialization layout, scrollbar handles, horizontal field-first mutation,
vertical row-first mutation, and invalidation/update callbacks. The post-run
window list showed no Program Error dialog. Evidence is preserved in
`analysis/compat/report-dlg-integration.{md,json}` and
`runs/vmctl-vc152rptint/`. Packed report unions and row contents, exact live
USER/GDI handles, menu resources, and full caller integration remain open.

`PopupMenu` is now promoted at step 6/6. The original packed/Win16 integration
run remains in `C:\PMUINT`/`C:\PMUINT2`; the final fresh `C:\PMUINT3` E2
harness passes `POPUP_MENU_E2_CHECKS=30 FAILURES=0 RESULT=PASS`. It uses the
real owner `WM_COMMAND` queue without a synthetic `PostMessage`, validates the
exact recovered `PeekMessage` arguments, checks packed fleet/thing/planet/
deep caller records, and selects command `15000` with a real first-item mouse
click. The source correction preserves the raw direct submenu-header argument
rather than routing it through a kind-0 resolver. Compile and link pass, the
post-run guest window list has no Program Error dialog, and evidence is
preserved in `analysis/compat/popup-menu-integration.{md,json}` and
`runs/vmctl-vc152pmuint3/`. The host listener has been stopped after the run;
the guest agent remains running for the next checkpoint.

All 51 Phase 2 procedures now have the native checkpoint. The remaining work
is step-5 original/integration evidence—packed records, localized resources,
Win16 USER/GDI/menu behavior, and original-executable comparisons—followed by
step-6 ledger synchronization and promotion. `GenerateWorld` is the first
procedure advanced to step 5/6: the original Win98 executable was driven
through New Game, reached the generated game frame, and produced the expected
`.xy`, `.M1`, `.M2`, `.M3`, and `.HST` artifact family. Evidence is preserved
in `analysis/compat/generate-world-integration.md`,
`analysis/compat/generate-world-integration.json`, and
`runs/vmctl-vc152current/`; full packed game-state and broader resource/UI
integration remain open.

The following fleet-fuel checkpoint is already complete for `EstFuelUse`. A fresh
`compat/vc152/est-fuel-use/` harness compiled and linked the current source
with VC++ 1.52c in `StarDev`, including the widened floating-point path, and
the returned Win98 report was
`EST_FUEL_USE_CHECKS=6 FAILURES=0 RESULT=PASS`. The post-run guest window list
showed no program-error dialog. Evidence is preserved in
`analysis/compat/est-fuel-use-native.md` and `runs/vmctl-vc152efu/`. Exact
packed design/component integration and full native differential coverage
remain open.

The packed/integration checkpoint is now also complete for `EstFuelUse`. A
fresh `compat/vc152/est-fuel-use-integration/` adapter compiled and linked with
VC++ 1.52c in `StarDev`; the returned Win98 report was
`EST_FUEL_USE_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`, and the post-run
window list showed no Program Error dialog. The adapter binds the recovered
fleet, ship-design, hull-definition, and engine-record offsets and checks raw
ship counts, mineral weights, component engine selection, the Ram Scoop flag,
race discount, and the fixed-point fuel result. The original Fleet Summary
Report and Fuel & Cargo surface showed per-fleet fuel values. Evidence is
preserved in `analysis/compat/est-fuel-use-integration.{md,json}` and
`runs/vmctl-vc152efuint/`; broader packed-table and route-distance differential
coverage remains open.

The waypoint-fuel checkpoint is also complete for `LFuelUseToWaypoint`. A
fresh `compat/vc152/fuel-use-to-waypoint/` harness compiled and linked the
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`FUEL_USE_TO_WAYPOINT_CHECKS=4 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog, and redirected console output was
empty. Evidence is preserved in
`analysis/compat/fuel-use-to-waypoint-native.md` and
`runs/vmctl-vc152fut/`. Exact packed hull/planet records, original fuel cases,
and full ship integration remain open.

The packed/integration checkpoint is now also complete for
`LFuelUseToWaypoint`. A fresh
`compat/vc152/fuel-use-to-waypoint-integration/` adapter compiled and linked
with VC++ 1.52c in `StarDev`; the returned Win98 report was
`FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`, and the
post-run window list showed no Program Error dialog. The adapter binds the
recovered fleet, 18-byte order, planet status, hull cargo-capacity, and
ship-design hull fields and checks warp/task extraction, distance and
multi-turn dispatch, the special-planet reset boundary, and raw-field
preservation. The original Fleet Summary Report and Fuel & Cargo surface
showed fleet fuel and cargo values. Evidence is preserved in
`analysis/compat/fuel-use-to-waypoint-integration.{md,json}` and
`runs/vmctl-vc152futint/`; broader route-sequence differential coverage
remains open.

The stored-order dialog checkpoint is complete for `ZipOrderDlg`. A fresh
`compat/vc152/zip-order-dlg/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`ZIP_ORDER_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog, and redirected console output was empty.
Evidence is preserved in `analysis/compat/zip-order-dlg-native.md` and
`runs/vmctl-vc152zod/`. Packed order storage, localized resources, original
rename-dialog behavior, and full Win16 integration remain open.

The stargate-jump checkpoint is complete for `FStargateJump`. A fresh
`compat/vc152/f-stargate-jump/` adapter compiled and linked the exact current
source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`F_STARGATE_JUMP_CHECKS=4 FAILURES=0 RESULT=PASS`. The post-run guest window
list showed no program-error dialog, and redirected console output was empty.
Evidence is preserved in `analysis/compat/f-stargate-jump-native.md` and
`runs/vmctl-vc152sgj/`. Packed original fleet layout, full game service
tables, original damage differential cases, and complete cargo integration
remain open.

The packed/integration checkpoint is now also complete for `FStargateJump`. A
fresh `compat/vc152/f-stargate-jump-integration/` adapter compiled and linked
with VC++ 1.52c in `StarDev`; the returned Win98 report was
`F_STARGATE_JUMP_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`, and the
post-run window list showed no Program Error dialog. The adapter binds the
recovered fleet ID/player/flags/planet, cargo-count and cargo-state words, and
ship-design mass/weight fields, then checks the abort message envelope and a
surviving-cargo jump with destination flags, state mutation, and transfer.
The original Fleet Summary Report / Fuel & Cargo surface showed the fleet
identity and cargo state. Evidence is preserved in
`analysis/compat/f-stargate-jump-integration.{md,json}` and
`runs/vmctl-vc152sgjint/`; full damage-service differential coverage remains
open.

The waypoint-cleanup checkpoint is complete for `KillUsedWaypoints`. A fresh
`compat/vc152/kill-used-waypoints/` adapter compiled and linked the exact
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`KILL_USED_WAYPOINTS_CHECKS=6 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog, and redirected console output was
empty. Evidence is preserved in
`analysis/compat/kill-used-waypoints-native.md` and
`runs/vmctl-vc152kuw/`. Exact packed PLORD offsets, localized
message/resource integration, original fleet-table cases, and full fleet
integration remain open.

The packed/integration checkpoint is now also complete for
`KillUsedWaypoints`. A fresh `compat/vc152/kill-used-waypoints-integration/`
adapter compiled and linked with VC++ 1.52c in `StarDev`; the returned Win98
report was `KILL_USED_WAYPOINTS_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`,
and the post-run window list showed no Program Error dialog. The adapter binds
the recovered `_fleet` fields and four-byte PLORD header plus 18-byte order
records, then checks the deleted-fleet message envelope, flagged status word,
stale-fleet retargeting, and waypoint deletion/compaction. The original Fleet
Waypoints surface was visible in the captured game window. Evidence is
preserved in `analysis/compat/kill-used-waypoints-integration.{md,json}` and
`runs/vmctl-vc152kuwint/`; localized resources, live fleet-table enumeration,
and full turn integration remain open.

The next planet-display checkpoint is complete for `DrawPlanetProduction`. A
fresh `compat/vc152/draw-planet-production/` harness compiled and linked the
current source with VC++ 1.52c in `StarDev`, and the returned Win98 report was
`DRAW_PLANET_PRODUCTION_CHECKS=9 FAILURES=0 RESULT=PASS`. The post-run guest
window list showed no program-error dialog. Evidence is preserved in
`analysis/compat/draw-planet-production-native.md` and
`runs/vmctl-vc152dpro/`. Packed planet/production records, localized ETA
formatting, exact Win16 geometry, and full GDI integration remain open.

On a later session, start the listener in an interactive
terminal:

```text
python3 ftp-share/VMCTL1/HOSTCTL.PY listen --port 2122
```

Then run `info`, `ping`, and `windows`. If the guest agent is not connected,
the user should run inside the `StarDev` guest:

```text
C:
CD \VMCTL1
STARTAGT
```

Do not hard-reset the VM while files are being written. Preserve native logs
and use a new run directory for every build.

At handoff time, no native VC1.52 build of these three AI-turn sources had
been completed. The first follow-up should use a small Win16 harness that
transfers one AI-turn source and a file-writing probe into a fresh guest
directory, then records compile/link and execution results.

The first such follow-up is now complete for `DoMacintiAiTurn`. The fresh
`compat/vc152/macinti/` harness compiled and linked the current orchestration
slice with VC++ 1.52c in `StarDev`, and the returned Win16 executable reported
`MACINTI_NATIVE_CHECKS=29 FAILURES=0 RESULT=PASS`. Evidence is in
`analysis/compat/macinti-native.md` and `runs/vmctl-vc152macinti/`. A separate
fresh-directory typed follow-up also passed 43 checks covering planet/fleet
policy and state counters; its artifacts are in
`runs/vmctl-vc152macinti-typed/`. A further fresh typed run in `C:\MACINTI3`
passed 45 checks, including the queued-cost-plus-hull-batch production gate;
its artifacts are in `runs/vmctl-vc152macinti-typed-batch/`. Native validation
is now complete for the recovered slice. A further fresh typed run passed 52
checks, including the fleet plan/apply action boundary; its artifacts are in
`runs/vmctl-vc152macinti-fleet-actions/`. `DoMacintiAiTurn` remains partial
because the raw table-owned production choices, detailed fleet routing, and
full AI-table integration are not complete. A further fresh typed run passed
52 checks with named action-specific bindings; its artifacts are in
`runs/vmctl-vc152macinti-action-bindings/`. A further fresh typed run passed
52 checks for the ordered transport-routing, fleet-finalization, basic-task,
and production-queue phases; its artifacts are in
`runs/vmctl-vc152macinti-final-phases/`. The next useful step is replacing the
explicit plan callbacks with recovered table decisions and attaching the
remaining action callbacks to the reconstructed routing procedures. A further
fresh typed run passed 52 checks after adding optional raw-record and coordinate
views to the typed planet/fleet records; its artifacts are in
`runs/vmctl-vc152macinti-raw-views/`.

The latest Phase 2 checkpoint completed `ExecuteReportClick` packed-record
validation. The host adapter passed 24 checks, and corrected fresh VC++ 1.52c
runs in `C:\EXINT3` and `C:\EXLAY3` passed 24 and 16 native checks with no
Program Error dialog. The initial two attempts exposed an omitted linker
definitions-file line and are retained as diagnostic history. The listener
was stopped while leaving the guest agent available. The procedure stays at
5/6; exact `GlobalPD` owner/part cases, live USER/GDI behavior, and complete
caller integration remain open.

The next Phase 2 checkpoint completed `DumpFleets` packed-record validation.
The strict host adapter and corrected fresh VC++ 1.52c run in `C:\DFINT` each
passed `DUMP_FLEETS_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS` over the recovered
124-byte `_fleet` layout, ship-class mapping, detailed fields, destination
visibility, and write-failure path. Compile and link passed, the process exited
0, and no Program Error dialog appeared. Evidence is preserved in
`analysis/compat/dump-fleets-packed-integration.{md,json}` and
`runs/vmctl-vc152dfint/`. The listener was stopped while leaving the guest
agent available. `DumpFleets` remains at 5/6 pending localized formatting,
exact scanner/minefield/battle-plan/task services, complete original
differential output, and final promotion.

The next Phase 2 checkpoint completed `Popup` packed-record validation. A
strict host adapter and a fresh VC++ 1.52c run in `C:\PUINT` each passed
`POPUP_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS` over the recovered 22-byte
`_popupdata` selector, planet, and slot fields. Compile and link passed, the
process exited 0, and no Program Error dialog appeared. Earlier probe-only
failures are retained in `runs/vmctl-vc152puint/` as diagnostic history; they
corrected the expected screen-origin clamp and the probe's self-count. The
listener was stopped while leaving the guest agent available. `Popup` remains
at 5/6 pending localized measurement, full packed fleet/planet/hull payload
consumption, detailed content layout, and final caller review. Evidence is in
`analysis/compat/popup-packed-integration.{md,json}`.

The next Phase 2 checkpoint completed `DrawPopup` packed-record validation. A
strict host adapter and a fresh VC++ 1.52c run in `C:\DRINT` each passed
`DRAW_POPUP_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS` over the recovered
`_popupdata`, `_fleet`, and `_planet` views. Compile and link passed, the
process exited 0, and no Program Error dialog appeared. Earlier probe-only
reports are retained in `runs/vmctl-vc152drint/` as diagnostic history because
the probe initially asserted 26 instead of 25 checks. The listener was stopped
while leaving the guest agent available. `DrawPopup` remains at 5/6 pending
complete `GlobalPD` union coverage, localization, hull filtering, formatting,
detailed GDI text rendering, and final caller review. Evidence is in
`analysis/compat/draw-popup-packed-integration.{md,json}`.

The next Phase 2 checkpoint completed `PtDisplayPlanetPopInfo` packed-record
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\PPINT` each
passed `PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=19 FAILURES=0 RESULT=PASS`
over the recovered 56-byte `_planet` identity, owner, mineral-rate, and
population-change fields. The owned, unowned, foreign, and negative-
desirability branches passed through the existing lookup/service boundary.
Compile and link passed, the process exited 0, and no Program Error dialog
appeared. An earlier probe-only failure is retained in
`runs/vmctl-vc152ppint/PPINTDEBUG.TXT` as diagnostic history because its
negative-branch label indices were incorrect. The listener was stopped while
leaving the guest agent available. `PtDisplayPlanetPopInfo` remains at 5/6
pending complete packed planet/player lookup, localization, fixed-point
formatting, detailed stream/wrap GDI behavior, original differential coverage,
and final review. Evidence is in
`analysis/compat/pt-display-planet-pop-info-packed-integration.{md,json}`.

The next Phase 2 checkpoint completed `PtDisplayPlanetStateInfo` packed-record
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\STINT` each
passed `PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=23 FAILURES=0 RESULT=PASS`
over the recovered 56-byte `_planet` and 22-byte `_popupdata` views. The
combined identity/limit bridge, known and unknown rows, environment and
desirability service calls, improvement warning, wrapping, and returned
geometry passed. Compile and link passed, the process exited 0, and no Program
Error dialog appeared. The listener was stopped while leaving the guest agent
available. `PtDisplayPlanetStateInfo` remains at 5/6 pending complete packed
player/planet lookup, localization, fixed-point formatting, detailed
stream/wrap GDI behavior, original differential coverage, and final review.
Evidence is in
`analysis/compat/pt-display-planet-state-info-packed-integration.{md,json}`.

The next Phase 2 checkpoint completed `DrawPlanetStats` packed-record
validation. An expanded strict host adapter and a fresh VC++ 1.52c run in
`C:\DPSINT2` each passed
`DRAW_PLANET_STATS_PACKED_CHECKS=15 FAILURES=0 RESULT=PASS` over the
recovered 56-byte `_planet` status, scanner/defense, and mineral-rate fields.
The decoded data reached all seven value rows, including hidden-tile
suppression, references, separators, alignment, a zero mineral-rate boundary,
and maximum scanner/defense bitfields. Compile and link passed, the process
exited 0, and no Program Error dialog appeared. The listener was stopped while
leaving the guest agent available. `DrawPlanetStats` remains at 5/6 pending
complete packed planet/race lookup, localized and fixed-point formatting,
exact scanner/defense services, detailed GDI operations, original differential
coverage, and final review. Evidence is in
`analysis/compat/draw-planet-stats-packed-integration.{md,json}`.

The next Phase 2 checkpoint completed `DrawPlanetStarbase` packed-record
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\DSBINT`
each passed `DRAW_PLANET_STARBASE_PACKED_CHECKS=17 FAILURES=0 RESULT=PASS`
over the recovered 56-byte `_planet` identity, `fStarbase` bit, packed
starbase word, populated detail/gauge paths, no-warp cleanup, and
empty-starbase sentinels. Compile and link passed, the process exited 0, and
no Program Error dialog appeared. The listener was stopped while leaving the
guest agent available. `DrawPlanetStarbase` remains at 5/6 pending localized
resources, exact design/shield/warp services, detailed GDI integration,
original differential coverage, and final review. Evidence is in
`analysis/compat/draw-planet-starbase-packed-integration.{md,json}` and
`runs/vmctl-vc152dsbint/`.

The next Phase 2 checkpoint completed `DrawPlanetProduction` packed-record
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\DPINT`
each passed `DRAW_PLANET_PRODUCTION_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`
over the recovered 56-byte `_planet` identity, `lpplprod` pointer, 4-byte
`PLPROD` header, ETA/name rows, queue action enablement, and empty cleanup
path. Compile and link passed, the process exited 0, and no Program Error
dialog appeared. The listener was stopped while leaving the guest agent
available. `DrawPlanetProduction` remains at 5/6 pending localized ETA
formatting, exact Win16 geometry, full GDI/control integration, original
differential coverage, and final review. Evidence is in
`analysis/compat/draw-planet-production-packed-integration.{md,json}` and
`runs/vmctl-vc152dpint/`.

The next Phase 2 checkpoint completed `DrawPlanShipBitmap` packed-selection
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\DPSBINT`
each passed `DRAW_PLAN_SHIP_BITMAP_PACKED_CHECKS=17 FAILURES=0 RESULT=PASS`
over the recovered 226-byte `_sel` record, fleet and planet subrecords,
fleet/planet dispatch, planet DIB source coordinates, and button
cleanup/grid paths. Compile and link passed, the process exited 0, and no
Program Error dialog appeared. The listener was stopped while leaving the
guest agent available. `DrawPlanShipBitmap` remains at 5/6 pending exact GDI
handles, complete selection integration, localized names, original
differential coverage, and final review. Evidence is in
`analysis/compat/draw-plan-ship-bitmap-packed-integration.{md,json}` and
`runs/vmctl-vc152dpsbint/`.

The next Phase 2 checkpoint completed `DrawPlanetShipList` packed-transfer
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\DLIST`
each passed `DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`
over the recovered 226-byte `_sel` planet view, 128-byte `_xfer` identity,
kind, and owner fields, fleet and thing-mode gating, and no-object visibility.
Compile and link passed, the process exited 0, and no Program Error dialog
appeared. The listener was stopped while leaving the guest agent available.
`DrawPlanetShipList` remains at 5/6 pending complete transfer behavior, exact
Win16 controls and geometry, localized text/gauges, detailed drawing,
original differential coverage, and final review. Evidence is in
`analysis/compat/draw-planet-ship-list-packed-integration.{md,json}` and
`runs/vmctl-vc152dlistint/`.

The next Phase 2 checkpoint completed `TransferStuff` packed-record
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\TRINT`
each passed `TRANSFER_STUFF_PACKED_CHECKS=14 FAILURES=0 RESULT=PASS` over the
recovered 128-byte `_xfer` object identity/kind/fleet-owner fields, fleet
cargo presence, split/cancel cleanup, mode-0 jettison, valid-hull
enumeration, balance, scanner invalidation, refresh, and mode reset. Compile
and link passed, the process exited 0, and no Program Error dialog appeared.
The listener was stopped while leaving the guest agent available.
`TransferStuff` remains at 5/6 pending complete packed mutation, exact dialog
behavior, cargo-balance differentials, Win16 lookup/refresh integration,
original differential coverage, and final review. Evidence is in
`analysis/compat/transfer-stuff-packed-integration.{md,json}` and
`runs/vmctl-vc152trint/`.

The next Phase 2 checkpoint completed `FTrackXfer` packed-record and
cargo-mutation validation. A strict host adapter and a fresh VC++ 1.52c run in
`C:\FTXINT` each passed
`F_TRACK_XFER_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS` over the recovered
128-byte `_xfer` identity/type, fleet-owner and cargo fields, and overlapping
`_thing` data. The adapter exercised owner gating, capture/release,
non-zero-origin drag proportion, signed supply, bounded dialog-mode transfer,
and mutation of both packed cargo records. Compile and link passed, the
process exited 0, and no Program Error dialog appeared. The listener was
stopped while leaving the guest agent available. `FTrackXfer` remains at 5/6
pending exact original `XferSupply`/`ChgCargo` semantics, complete
fleet/planet/thing lookup and Win16 capture integration, original differential
coverage, and final review. Evidence is in
`analysis/compat/f-track-xfer-packed-integration.{md,json}` and
`runs/vmctl-vc152ftxint/`.

The next Phase 2 checkpoint expanded `EstFuelUse` packed-table validation. A
strict host adapter and a fresh VC++ 1.52c run in `C:\EFUINT2` each passed
`EST_FUEL_USE_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`. The matrix covers
engine ordering, race discount, Ram Scoop, fixed-point cost, the no-cargo range
sentinel, warp 10, invalid-warp flag clearing, order-derived distance,
large-intermediate arithmetic, absent-engine fallback, range conversion, and
raw table sizes/mapping. Compile and link passed, the process exited 0, and no
Program Error dialog appeared. The listener was stopped while leaving the
guest agent available. `EstFuelUse` remains at 5/6 pending complete
original-versus-reconstructed packed-table and route-distance differential
coverage and final review. Evidence is in
`analysis/compat/est-fuel-use-integration.{md,json}` and
`runs/vmctl-vc152efuint2/`.

The next Phase 2 checkpoint expanded `LFuelUseToWaypoint` route-sequence
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\FUTINT2`
each passed
`FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=11 FAILURES=0 RESULT=PASS`. The
matrix covers multi-segment accumulation, invalid warp, owner-mismatch versus
owned-planet reset, ram-scoop adjustment, varying route distances, and
preservation of three packed 18-byte order records plus fleet, planet, hull,
and design fields. Compile and link passed, the process exited 0, and no
Program Error dialog appeared. The listener was stopped while leaving the
guest agent available. `LFuelUseToWaypoint` remains at 5/6 pending complete
original route-sequence differential coverage, full ship integration, and
final review. Evidence is in
`analysis/compat/fuel-use-to-waypoint-integration.{md,json}` and
`runs/vmctl-vc152futint2/`.

The next Phase 2 checkpoint completed `ZipOrderDlg` packed-record validation.
A strict host adapter and a fresh VC++ 1.52c run in `C:\ZODINT` each passed
`ZIP_ORDER_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS` over four recovered
24-byte `_ziporder` records, including five order words, names, and validity
bytes. The adapter exercised initialization, selection, Create copying,
name/ampersand handling, Delete cleanup, Rename persistence, and Cancel
closure. Compile and link passed, the process exited 0, and no Program Error
dialog appeared. The listener was stopped while leaving the guest agent
available. `ZipOrderDlg` remains at 5/6 pending localized resources, exact
original dialog behavior, complete Win16 integration, original differential
coverage, and final review. Evidence is in
`analysis/compat/zip-order-dlg-packed-integration.{md,json}` and
`runs/vmctl-vc152zodint/`.

The next Phase 2 checkpoint expanded `FStargateJump` packed integration in a
fresh `C:\SGJINT2` directory. A strict host adapter and a fresh VC++ 1.52c
run each passed
`F_STARGATE_JUMP_INTEGRATION_CHECKS=7 FAILURES=0 RESULT=PASS` over damage
failure, inactive and total-loss empty-fleet paths, surviving-cargo state and
loss-message write-back, the race-stat loss exemption, and multi-slot cargo
with independent design weights. Compile and link passed, the process exited
0, and no Program Error dialog appeared. The listener was stopped while
leaving the guest agent and VM available. `FStargateJump` remains at 5/6;
full original damage-service differential coverage and complete game-service
cargo integration remain open. Evidence is in
`analysis/compat/f-stargate-jump-integration.{md,json}` and
`runs/vmctl-vc152sgjint2/`.

The next Phase 2 checkpoint expanded `KillUsedWaypoints` packed integration in
a fresh `C:\KUWINT2` directory. A strict host adapter and a fresh VC++ 1.52c
run each passed
`KILL_USED_WAYPOINTS_INTEGRATION_CHECKS=9 FAILURES=0 RESULT=PASS` over
deleted-fleet and empty-waypoint messages, missing-target invalidation,
no-move preservation, both recycle decisions, location-triggered compaction
with planet retargeting, and two-fleet enumeration. Compile and link passed,
the process exited 0, and no Program Error dialog appeared. The listener was
stopped while leaving the guest agent and VM available. `KillUsedWaypoints`
remains at 5/6; localized resources, live fleet-table behavior, and complete
turn integration remain open. Evidence is in
`analysis/compat/kill-used-waypoints-integration.{md,json}` and
`runs/vmctl-vc152kuwint2/`.

The next Phase 2 checkpoint expanded `AutoRouteFleet` packed integration in a
fresh `C:\ARFINT2` directory. A strict host adapter and a fresh VC++ 1.52c
run each passed
`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS` over
source-owner mismatch, nonzero gate damage, missing-target early return,
zero-ideal-warp fallback, packed cargo-hull capacity, stargate warp selection,
fuel descent, mineral exclusion, and raw order round-trip. Compile and link
passed, the process exited 0, and no Program Error dialog appeared. The
listener was stopped while leaving the guest agent and VM available.
`AutoRouteFleet` remains at 5/6; exact resource/route-helper behavior and
complete live fleet-turn comparison remain open. Evidence is in
`analysis/compat/auto-route-fleet-integration.{md,json}` and
`runs/vmctl-vc152arfint2/`.

The preceding Phase 2 checkpoint expanded `DrawMineSurvey` in a fresh `C:\DMS2`
directory. The repaired package includes the exact current `DMS.C` source and
a directory-first wrapper; a strict host adapter and fresh VC++ 1.52c run
passed `DRAW_MINE_SURVEY_CHECKS=9 FAILURES=0 RESULT=PASS` over shallow and
unknown-owner planet summaries, non-wormhole thing decoding, invalid
thing-selection suppression, the null-operations guard, and the existing
dispatch/state/summary cases. Compile and link passed, redirected console
output was empty, the process exited 0, and no Program Error dialog appeared.
The listener was stopped while leaving the guest agent and VM available.
At that checkpoint `DrawMineSurvey` remained at 5/6; the later E2 raw-record
adapter is recorded above. Evidence for the earlier checkpoint is in
`analysis/compat/draw-mine-survey-native.md` and
`runs/vmctl-vc152dms2/`.

The next Phase 2 checkpoint repaired and reran `MineClick` in fresh `C:\MCL2`.
The exact current `MCL.C` source, directory-first wrapper, and retained Win32
stub now compile and link under VC1.52c; the native 9-case matrix reports
`MINE_CLICK_CHECKS=9 FAILURES=0 RESULT=PASS`, redirected console output is
empty, the process exits 0, and no Program Error dialog appears. The listener
was stopped while leaving the guest agent and VM available. `MineClick`
remains at 5/6; packed GlobalPD/minefield records, localized popup
construction, derived mining arithmetic, and complete minefield integration
remain open. Evidence is in
`analysis/compat/mine-click-native.md` and
`runs/vmctl-vc152mcl2/`.

The maintained `MineClick` probe was then synchronized into fresh `C:\MCL3`.
Host and native now exercise the same 25-case source matrix; VC1.52c
compile/link passed, the Win98 report was
`MINE_CLICK_CHECKS=25 FAILURES=0 RESULT=PASS`, redirected console output was
empty, the process exited 0, and no Program Error dialog appeared. The
listener was stopped while leaving the guest agent and VM available.
`MineClick` remains at 5/6; packed GlobalPD/minefield records, localized
popup construction, derived mining arithmetic, and complete minefield
integration remain open. Evidence is in
`analysis/compat/mine-click-native.md` and
`runs/vmctl-vc152mcl3/`.

The next Phase 2 checkpoint expanded `EstMineralsMined` packed integration in
fresh `C:\EMMINT2`. A strict host adapter and a fresh VC++ 1.52c run each
passed
`EST_MINERALS_MINED_INTEGRATION_CHECKS=9 FAILURES=0 RESULT=PASS` over invalid
mining context, operating-mine lookup and efficiency, both random-rounding
outcomes, concentration-floor handling, special-race fleet recursion and
consumption, raw `rgwtMin` write-back, and the null-operations guard. Compile
and link passed after restoring the missing Win32 stub, the process exited 0,
redirected console output was empty, and no Program Error dialog appeared. The
listener was stopped while leaving the guest agent and VM available.
`EstMineralsMined` remains at 5/6; broader original mining-turn differential
and complete fleet/planet mineral mutation remain open. Evidence is in
`analysis/compat/est-minerals-mined-integration.{md,json}` and
`runs/vmctl-vc152emmint2/`.

The next Phase 2 checkpoint strengthened `DrawProductionDlg` in fresh
`C:\DPD3`. The source now preserves decimal mineral costs through the
recovered formatter instead of reducing them to one digit, routes mineral
label colors through the recovered service boundary, and restores normal text
color before numeric output. Strict host C89 and VC++ 1.52c each pass; the
Win98 report is
`DRAW_PRODUCTION_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`, redirected output is
empty, and no Program Error dialog appeared. The listener was stopped while
leaving the guest agent and VM available. `DrawProductionDlg` remains at 5/6;
packed production globals, complete resource-table comparison, localized
strings, and full GDI integration remain open. Evidence is in
`analysis/compat/draw-production-dlg-native.md`,
`analysis/reconstruction/draw-production-dlg.md`, and
`runs/vmctl-vc152dpd3/`.

The next Phase 2 checkpoint strengthened `ProductionDlg` in fresh `C:\PDG2`.
Valid saved-queue selections now forward through the typed popup-result
boundary, while the final editor entry forwards to the queue-dialog boundary.
Strict host C89 and VC++ 1.52c each pass; the Win98 report is
`PRODUCTION_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`, redirected output is empty,
and no Program Error dialog appeared. The listener was stopped while leaving
the guest agent and VM available. `ProductionDlg` remains at 5/6; packed
production globals, exact child geometry, localized popup resources, complete
queue-control integration, and promotion remain open. Evidence is in
`analysis/compat/production-dlg-native.md`,
`analysis/reconstruction/production-dlg.md`, and
`runs/vmctl-vc152pdg2/`.

The next Phase 2 checkpoint expanded `ZipProdDlg` packed-queue validation in
fresh `C:\ZPD2`. The strict host/native matrix now has 29 checks covering
negative and out-of-range production records, the maximum packed kind/code
word (`0xffc6`), and the original twelve-entry cap. VC++ 1.52c compile/link
passed, the Win98 report was
`ZIP_PROD_DLG_CHECKS=29 FAILURES=0 RESULT=PASS`, the process exited 0, and no
Program Error dialog appeared. `ZipProdDlg` remains at 5/6; packed global
queue/name storage, exact localized resources, rename-dialog behavior, and
complete Win16 control integration remain open. Evidence is in
`analysis/compat/zip-prod-dlg-native.md`,
`analysis/compat/zip-prod-dlg-integration.json`, and
`runs/vmctl-vc152zpd2/`.

The next Phase 2 closure completed `EstMineralsMined` in fresh `C:\EMMINT4`.
The final strict host/native matrix passes 13 checks: two ordinary-race
populated mining turns at efficiencies 5/10 and 7/10, plus a special-race
planet with two contributing fleets. It compares all three outputs,
fixed-point weights, concentration/percentage mutation, and consumed fleet
orders against the original instruction path. The source was corrected for
the original pre-efficiency depletion product and percentage-cap reset after
full concentration consumption. VC++ 1.52c reports `COMPILE=PASS` and
`LINK=PASS`; `EMIR-MATRIX.TXT` reports
`EST_MINERALS_MINED_INTEGRATION_CHECKS=13 FAILURES=0 RESULT=PASS`; the final
window list contains no Program Error dialog. The E1 unit is closed and
`EstMineralsMined` is promoted to 6/6 in `tracking/reviews.json` and TODO6.
Evidence is in `analysis/compat/est-minerals-mined-integration.{md,json}`,
`analysis/reconstruction/est-minerals-mined.md`, and
`runs/vmctl-vc152emmint4/`.

The unchanged `5/6` labels are intentional gate accounting, not a claim that
only one small task remains per procedure. The active step-5 text in TODO6 is
the per-procedure work breakdown. A new probe or native checkpoint can close
one concern inside that text while leaving the procedure at 5/6; it reaches
6/6 only after the complete listed acceptance boundary is evidenced and the
ledger is promoted. `EstMineralsMined` is now the counterexample: its
13-check matrix closed its final E1 concern and the procedure moved to 6/6.
`DrawProductionDlg` with 10 and `ProductionDlg` with 11 still have multiple
integration concerns open.

The next Phase 2 closure completed `EstFuelUse` in fresh `C:\EFUINT3`. The
strict host and VC++ 1.52c/Win98 adapters each pass
`EST_FUEL_USE_INTEGRATION_CHECKS=29 FAILURES=0 RESULT=PASS`. The matrix
replays the independent original fuel loop across all sixteen packed design
and hull slots, all warp rows and engine IDs, both race-discount and
range-only modes, and route distances 1, 37, 1000, and 50000. Compile and
link pass, and the final guest window list contains no Program Error dialog.
This closes the remaining original-instruction packed-table/route-distance
unit; `EstFuelUse` is promoted to 6/6 in the review ledger and TODO6. Evidence
is in `analysis/compat/est-fuel-use-integration.{md,json}` and
`runs/vmctl-vc152efuint3/`. This is procedure-level differential evidence,
not whole-game turn-generator acceptance.

The next Phase 2 milestone closed `FStargateJump` E1 in fresh `C:\SGJINT4`.
The strict host and VC++ 1.52c/Win98 harnesses each pass
`F_STARGATE_JUMP_INTEGRATION_CHECKS=2310 FAILURES=0 RESULT=PASS`. The added
matrix independently replays the original `MdCalcStargateDamage` behavior
across source/destination armor records, distance limits, and defense/weight
boundaries. Compile and link pass, and the final guest window list contains
no Program Error dialog. The procedure remains at 5/6 with only E2—the real
game-service cargo-transfer comparison—open. Evidence is in
`analysis/compat/f-stargate-jump-integration.{md,json}`,
`analysis/reconstruction/f-stargate-jump.md`, and
`runs/vmctl-vc152sgjint4/`.

The following Phase 2 closure completed `FStargateJump` E2 in fresh
`C:\SGJINT5`. The raw assembly boundary was corrected: the surviving source
fleet is passed to `FleetTransferCargoBalance`, while a zeroed lost-fleet
record carries the lost ship counts and `0x0407` flags; the temporary record is
not copied over the live fleet. The typed model now includes the five packed
resource dwords at offset 76, and the integration adapter invokes the actual
reconstructed balance service. Strict host C89 and VC++ 1.52c/Win98 each pass
`F_STARGATE_JUMP_INTEGRATION_CHECKS=2311 FAILURES=0 RESULT=PASS`, compile and
link pass, and the final guest window list contains no Program Error dialog.
`FStargateJump` is promoted to 6/6. Evidence is in
`analysis/compat/f-stargate-jump-integration.{md,json}`,
`analysis/reconstruction/f-stargate-jump.md`, and
`runs/vmctl-vc152sgjint5/`.

TODO6 now includes an execution-level Step-5 description for every Phase 2
procedure. At that documentation snapshot, each row identified the concrete
work still required, the expected host/native/original evidence package, and
the condition that closes Step 5; the snapshot counts were 148 open evidence
units plus 48 final promotion units. The relative load labels distinguish one
focused boundary (`S`), two (`M`), three dependent boundaries (`L`), and four
broad UI/turn/AI boundaries (`XL`). This does not change procedure status; the
current burn-down is recorded below, and the XL rows still require shared
harness work rather than one-test leftovers merely because their headline
status is `5/6`.

The next Phase 2 milestone closed `LFuelUseToWaypoint` E1 in fresh
`C:\FUTINT3`. The strict host and VC++ 1.52c/Win98 adapters each pass
`FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=23 FAILURES=0 RESULT=PASS`; the added
matrix independently transcribes the original segment loop across twelve route,
warp, cargo-hull, ram-scoop, and planet-reset cases. Compile and link pass and
the final guest window list contains no Program Error dialog. The procedure
remains at 5/6 with only E2—complete ship/waypoint integration across the
remaining route and hull cases—open. The current burn-down at that checkpoint
was 146 evidence units plus 47 promotion units, or 193 open work items.

The following Phase 2 closure completed `LFuelUseToWaypoint` E2 in fresh
`C:\FUTINT4`. The final strict host and VC++ 1.52c/Win98 adapters each pass
`FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=27 FAILURES=0 RESULT=PASS`; the matrix
loads all sixteen packed fleet/design slots, both cargo-hull IDs, the
planet-selected hull definition, and the full two-segment order boundary.
Compile and link pass and the final guest window list contains no Program Error
dialog. `LFuelUseToWaypoint` is promoted to 6/6. The current burn-down is 145
evidence units plus 46 promotion units, or 191 open work items.

The next Phase 2 milestone closed `AutoRouteFleet` E1 in fresh `C:\ARFINT4`.
The strict host and VC++ 1.52c/Win98 adapters each pass
`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS`; the matrix
independently transcribes the raw route loop across twelve helper, gate-damage,
mineral-slot, geometry, fuel-descent, and resource-capacity variations.
Compile and link pass and the final guest window list contains no Program Error
dialog. The procedure remains at 5/6 with only E2—exact resource/helper
behavior and complete live fleet-turn comparison—open. The current burn-down is
144 evidence units plus 46 promotion units, or 190 open work items.

The following milestone closed `AutoRouteFleet` E2 and promoted it in fresh
`C:\ARFINT5`. The strict host and VC++ 1.52c/Win98 adapters each pass
`AUTO_ROUTE_FLEET_E2_CHECKS=86 FAILURES=0 RESULT=PASS`. The probe maps packed
design/hull/engine/cargo/mineral state into the real `EstFuelUse` service,
invokes the real `StargateDamage` callback, and consumes the generated order
through the real `MoveFleets` pass; four resource/turn cases agree with an
independent oracle. Compile and link pass, and the final Win98 window list and
screenshot contain no Program Error dialog. `AutoRouteFleet` is now 6/6. The
current Phase 2 burn-down is 143 evidence units plus 45 promotion units, or
188 open work items, with 45 procedures at 5/6 and 6 promoted at 6/6.

The following milestone closed `UpdatePlayerScores` E1 and E2 and promoted it
in fresh `C:\UPSINT2`. The strict host and VC++ 1.52c/Win98 adapters each pass
`UPDATE_PLAYER_SCORES_E2_CHECKS=36 FAILURES=0 RESULT=PASS`. Six cases compare
typed scores, packed `_scorex`/`_player` fields, report checksum, message
count, dirty invalidation, and complete turn/report replay against an
independent oracle. Compile and link pass, and the final guest window list
and screenshot contain no Program Error dialog. `UpdatePlayerScores` is now
6/6. The current Phase 2 burn-down is 141 evidence units plus 44 promotion
units, or 185 open work items, with 44 procedures at 5/6 and 7 promoted at
6/6.

The following milestone closed `Popup` E1 and E2 and promoted it in fresh
`C:\PUINT2`. The strict host and VC++ 1.52c/Win98 adapters each pass
`POPUP_E2_CHECKS=148 FAILURES=0 RESULT=PASS`. The matrix covers all measured
and fixed popup kinds, localized labels, packed popup/fleet/planet/hull
payload checksums, independent geometry, slot updates, screen clamping,
font/message/capture setup, and no Program Error dialog. Compile and link
pass, and the final screenshot is preserved. `Popup` is now 6/6. The current
Phase 2 burn-down is 139 evidence units plus 43 promotion units, or 182 open
work items, with 43 procedures at 5/6 and 8 promoted at 6/6.

The following milestone closed `PopupMenu` E1 and E2 and promoted it in fresh
`C:\PMUINT3`. The strict host and VC++ 1.52c/Win98 adapters each pass
`POPUP_MENU_E2_CHECKS=30 FAILURES=0 RESULT=PASS`. The native E2 path uses the
real owner `WM_COMMAND` queue without a synthetic `PostMessage`, validates the
exact recovered `PeekMessage` arguments, and checks packed fleet, thing,
planet, and deep-space caller records. A real first-item mouse selection
returns `COMMAND=15000` with `MASK=0`; compile and link pass and the final
window state contains no Program Error dialog. `PopupMenu` is now 6/6. The
current Phase 2 burn-down is 137 evidence units plus 42 promotion units, or
179 open work items, with 42 procedures at 5/6 and 9 promoted at 6/6.

The latest milestone closed and promoted `GenerateWorld` E2 in fresh
`C:\GWINT2`. The strict host and VC++ 1.52c/Win98 adapters each pass
`GENERATE_WORLD_E2_CHECKS=13 FAILURES=0 RESULT=PASS`. The state-backed
pipeline covers layout, planet initialization, starts/homeworld, four startup
ships, five battle plans, file records, player status, and local finalization;
compile/link/process pass and the final window inventory contains no Program
Error dialog. The original Win98 generated-game observation and preserved
`.xy`, `.M1`, `.M2`, `.M3`, and `.HST` artifacts provide the independent
comparison. `GenerateWorld` is now 6/6 and promoted. At that checkpoint, the
Phase 2 burn-down was 135 evidence units plus 41 promotion units, or 176 open
work items, with 41 procedures at 5/6 and 10 promoted at 6/6.

The next milestone closed and promoted the `BrowserDlg` real
USER/control/message-loop unit and direct original caller-path differential.
The fresh `C:\BRDMSG` native report was
`BROWSER_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`; the original run covered
Commands → Ship Design → Components, selection, group changes, detail updates,
and no-error completion. BrowserDlg is now 6/6. The current Phase 2 burn-down
is 133 evidence units plus 40 promotion units, or 173 open work items, with
40 procedures at 5/6 and 11 promoted at 6/6.

The preceding milestone closed `GenerateWorld` E1 in fresh `C:\GWINT`. The strict
host and VC++ 1.52c/Win98 adapters each pass
`GENERATE_WORLD_E1_CHECKS=16 FAILURES=0 RESULT=PASS`. The new typed 64-byte
`_game` view checks the recovered map, density, player, planet-limit,
start-distance, dirty, world-generation, and turn offsets, while the separate
`StarsSeeds` state is initialized through `Randomize2`, consumed by `Random`,
and fed into the real layout phase. Compile and link pass, the process exits 0,
and the final window inventory contains no Program Error dialog.
That E1 milestone was the packed-state/seed/RNG boundary; the E2 milestone
above closed the remaining state-backed pipeline and promotion boundary.

The latest Phase 2 milestone before TransferStuff closed and promoted `FTrackXfer` E3 in fresh
`C:\FTXE3G`. Strict host C89 and VC++ 1.52c/Win98 each pass
`F_TRACK_XFER_E3_CHECKS=8 FAILURES=0 RESULT=PASS`; compile and link pass, the
process exits 0, and no Program Error dialog appears. The packet closes the
raw packed lookup/store boundary, real Win16 task-message/capture behavior,
direct `mdXferDlg` active/opposite-record mutation separate from
`XferSupply`, owner rejection, and refresh/redraw callbacks. The source,
review ledger, TODO5, coverage, and handoff are synchronized, so `FTrackXfer`
is now **6/6** and promoted. Evidence is in
`analysis/compat/f-track-xfer-e3.{md,json}` and
`runs/vmctl-vc152ftxe3/`. That preceding checkpoint had **85 evidence units +
25 promotion units = 110 open work items**, with **26 procedures at 6/6 and
25 at 5/6**.

## Verification commands

The recent host checks use strict C89, for example:

```text
cc -std=c89 -pedantic -Wall -Wextra -I reconstructed/include \
  reconstructed/src/do_turin_drone_ai_turn.c \
  tests/do_turin_drone_ai_turn_source_probe.c \
  -o /tmp/turin_source
```

Run the corresponding Macinti, Turin, and Robotoid probes after any header
change. Also run `jq empty tracking/reviews.json` and `git diff --check`.

## Worktree caution

The worktree contains many existing reconstruction and ledger changes. Treat
them as user work: do not reset, checkout, or clean unrelated files.

## Latest host-only ReportDlg milestone

The ReportDlg paint boundary now exposes an optional typed
`StarsReportDlgDrawView` callback. The strict C89 probe passes
`REPORT_DLG_PAINT_VIEW_CHECKS=6 FAILURES=0 RESULT=PASS`, confirming that the
populated report kind, row window, field count, visible mask, cached column
width, and paint rectangle are visible at the renderer boundary. The legacy
opaque draw callback remains the fallback. This is not a native closure:
replay, populated row-union payloads, exact live USER/GDI/menu behavior,
original caller comparison, and promotion remain open. The current Phase 2
burn-down remains **77 open evidence units plus 22 promotion units = 99 open
work items**, with **29 procedures at 6/6 and 22 at 5/6**. The matching native
packet is prepared at `compat/vc152/report-dlg-paint-view-e2/` for guest
directory `C:\RPTVE2`, but build/replay remain pending guest-agent reconnect.

The following host milestone expands `EstimateItemProdSched` E3 through the
recovered production services. The strict C89 probe passes
`ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=8 FAILURES=0 RESULT=PASS` and
the matching native packet is prepared at
`compat/vc152/estimate-item-prod-sched-resource-queue/` for `C:\EIRQ`.
Native build/replay and original turn comparison remain pending; the current
burn-down is therefore still **77 open evidence units plus 22 promotion units
= 99 open work items**, with **29 procedures at 6/6 and 22 at 5/6**.

The next `ReportDlg` host milestone now exercises the reconstructed renderer
chain rather than only the typed paint payload. A strict C89 probe passes
`REPORT_DLG_DRAW_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS` after populated
`WM_PAINT` dispatch reaches `DrawReport` and `DrawReportItem`; it checks header
measurement, populated planet lookup, text/field drawing, and selection
update. This is host-only evidence and closes no authoritative Step-5 unit:
native replay, packed row payloads, live USER/GDI/menu behavior, original
caller comparison, and promotion remain open. Evidence is in
`analysis/compat/report-dlg-draw-integration-e3.{md,json}`; the guest agent
still needs to announce before the pending native packets can run. The
authoritative burn-down remains **77 evidence units + 22 promotion units = 99
open work items**, with **29 procedures at 6/6 and 22 at 5/6**.

The following host E3 milestone connects `ExecuteReportClick` popup cells to
the reconstructed `Popup` and `DrawPopup` procedures. The strict C89 probe
passes `EXECUTE_REPORT_CLICK_POPUP_INTEGRATION_CHECKS=26 FAILURES=0 RESULT=PASS`
for fleet-detail, factory-installation, and resource cells, checking semantic
payload preservation, measurement/window creation, paint-kind dispatch,
resource-share arithmetic, factory weight extraction, message/capture setup,
and DC release symmetry. This closes no authoritative unit: the matching
native replay, exact original `GlobalPD` owner/part cases, live USER/GDI
behavior, original comparison, and promotion remain open. Evidence is in
`analysis/compat/execute-report-click-popup-integration-e3.{md,json}`; the
guest agent still needs to reconnect before a VC++ packet can be replayed.

The matching native packet is now prepared at
`compat/vc152/execute-report-click-popup-integration-e3/` for guest directory
`C:\POPINT`. `PROBE.C`, `EXE.C`, `POPUP.C`, and `DRAWPOP.C` pass the strict host
packet-shape compile, and `BUILD.BAT`, `RUNVC.BAT`, and `RUNPROBE.BAT` use
directory-changing CRLF wrappers. A listener retry while the resumed 86Box
window was visible still produced no `AGENTINFO STARSAGT 0.4.2`; the stale
Windows Close Program dialog was safely cancelled without ending `Stars!` or
`Vmctl1`.

The next host E3 milestone connects the actual reconstructed `FDoCoolBattle`
orchestrator to the actual reconstructed `FAttack` service. The strict C89
probe passes
`F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS` for a
deterministic two-token battle, including descending-initiative dispatch,
movement and attack events, damage-driven token deactivation, survivor
termination, battle summary, and cleanup. This closes no authoritative unit:
native replay, packed token/global mapping, exact part/damage behavior,
original battle differential, and promotion remain open. Evidence is in
`analysis/compat/f-do-cool-battle-attack-integration-e3.{md,json}`; the
matching packet is prepared at
`compat/vc152/f-do-cool-battle-attack-integration-e3/` for `C:\CBAT`.

A bounded native reconnect attempt was made with the resumed `StarDev` 86Box
window. The host listener remained silent with no `AGENTINFO STARSAGT 0.4.2`
banner, so it was stopped cleanly after the guest's visible stale Close Program
dialog was handled without ending `Stars!` or `Vmctl1`. The combat packet
therefore remains native-pending; this attempt closes no Step-5 unit.

The next host E3 milestone routes `PlanetWndProc(WM_PAINT)` through the actual
reconstructed `DrawPlanetStats` child renderer. The strict C89 probe passes
`PLANET_STATS_WNDPROC_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`, covering
the `BPE` paint lifecycle, packed stats load, seven value rows, references,
separators, and selected mineral/scanner/defense values. This is host-only and
closes no authoritative unit; native replay and original comparison remain
open. Evidence is in
`analysis/compat/planet-stats-wndproc-integration-e3.{md,json}`.

A further bounded listener retry after the host integration work again
received no `AGENTINFO STARSAGT 0.4.2` banner and was stopped cleanly. Native
packets remain ready, but the guest-agent endpoint still needs to reconnect or
be restarted inside Win98 before native replay can proceed.

The next host AI milestone adds a Turin edge-policy matrix. The strict C89
probe passes `TURIN_DRONE_AI_TURN_EDGE_CHECKS=7 FAILURES=0 RESULT=PASS` for
potency thresholds/caps, merge short-circuiting, all eight design-status
requests, five-hull affordability acceptance/rejection, and null input. It
closes no Step-5 unit; production-table and fleet-order mutation, native
replay, and original-turn comparison remain open. Evidence is in
`analysis/compat/do-turin-drone-ai-turn-edge-e3.{md,json}`.

The next host planet-display checkpoint expands `DrawPlanShipBitmap`'s fleet
render seam without discarding the raw call shape. `draw_fleet_ex` now
receives the tile-derived origin and the recovered `1,-1,0,0,-1,0` positional
tail; the strict C89 probe passes
`DRAW_PLAN_SHIP_BITMAP_FLEET_CHECKS=6 FAILURES=0 RESULT=PASS` for normal and
grid origins. The legacy identity callback remains available. This does not
close an authoritative Step-5 unit: native fleet bitmap/GDI behavior,
localized names, complete planet-frame integration, and original differential
remain open. Evidence is in
`analysis/compat/draw-plan-ship-bitmap-fleet-e4.{md,json}`.

The following host checkpoint expands `DrawPlanetShipList`'s combo seam. The
typed source now preserves the raw cached-width comparison and exposes
`SetWindowPos` flags through `set_combo_position_ex`; the strict C89 probe
passes `DRAW_PLANET_SHIP_LIST_COMBO_CHECKS=4 FAILURES=0 RESULT=PASS` for the
112-pixel width, new-width `0x14`, repeated-width `0x15`, and repeated lookup
paths. This remains host-only: native control flags, complete transfer and
drawing behavior, localized text/gauges, original differential, and promotion
remain open. Evidence is in
`analysis/compat/draw-planet-ship-list-combo-e2.{md,json}`.

The next native checkpoint closes `DrawProductionDlg` E2. Fresh `C:\DPDRE2`
VC++ 1.52c validation compiled and linked the resource-table packet, then
returned `DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=10 FAILURES=0 RESULT=PASS`.
The redirected console was empty and the post-run window list contained no
Program Error dialog. This closes one named Step-5 evidence unit, moving the
authoritative count to **76/152 closed and 76 open**, with **22 promotion
units** and **98 practical open work items**. DrawProductionDlg remains at
5/6 because E3 full GDI/caller integration and Step-6 promotion are still
open. Evidence is in
`analysis/compat/draw-production-dlg-resource-e2.{md,json}` and
`runs/vmctl-vc152dpdre2/`.

The current combined state after the DrawPlanetShipList checkpoint is
**77/152 closed, 75 open, 22 promotion units, and 97 practical open work
items**. DrawProductionDlg is at `1 + P`; DrawPlanetShipList is at `3 + P`.

## Latest checkpoint — FDoCoolBattle attack-mask ordering E8

The readable coordinator now matches the recovered loop's order: both tokens
complete movement before attack-mask filtering, and the attack loop rechecks
the filtered survivor mask before each candidate. The focused strict-C89 probe
passes `F_DO_COOL_BATTLE_ATTACK_MASK_ORDER_CHECKS=3 FAILURES=0 RESULT=PASS`;
the existing 58-check source, 18-check battle-chain, 11-check packed-token,
and 24-check packed-field probes remain green.

This closes **0** authoritative Step-5 units. The counters remain **120/152
closed, 32 open, 10 promotion units, and 42 practical open work items**.
Native movement/initiative replay, exact global binding/compaction, complete
damage and battle-record behavior, full replay, and original differential
remain open. Evidence is in
`analysis/compat/f-do-cool-battle-attack-mask-order-e8.{md,json}` and
`tests/f_do_cool_battle_attack_mask_order_probe.c`.

## Latest checkpoint — FDoCoolBattle packed field bridge E7

With the Win98 guest agent still disconnected, the host-side FDoCoolBattle
bridge received a stricter packed-record probe. The production
`StarsCoolBattleInput.packed_tokens` path now has evidence for every scalar
field in the 29-byte `_tok` record, all movement nibbles, active/log flags, a
32-byte caller stride, trailing sentinels, speed-nibble mutation, and
defeated-token count/active mutation. The strict C89 probe passes
`F_DO_COOL_BATTLE_PACKED_FIELD_CHECKS=24 FAILURES=0 RESULT=PASS`.

This closes **0** authoritative Step-5 units and does not change the current
authoritative counters: **120/152 named Step-5 evidence units closed, 32
open, 10 promotion units, and 42 practical open work items**. Native
production-token replay still awaits `STARSAGT` reconnection; exact native
`_tok`/`vrgtok` binding and compaction, movement/damage fidelity, full battle
replay, and original differential remain open. Evidence is in
`analysis/compat/f-do-cool-battle-packed-fields-e7.{md,json}` and
`tests/f_do_cool_battle_packed_field_probe.c`.

## Latest checkpoint — DoCyberAiTurn freighter-service bridge E5

The readable Cyber turn now exposes an optional typed `run_freighter_ex`
callback during the owned-fleet pass. It receives the converted fleet, its
derived role policy, and armada potency after first-order repair and before
fleet-action planning. The strict host C89 packed integration probe passes
`CYBER_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS`, including packed
transport count, fleet identity, potency forwarding, and callback placement.
Fresh VC++ 1.52c / Win98 replay in `C:\\CYBPE5` compiled and linked, returned
the same 31-check report, exited normally, and showed no Program Error dialog.
Direct `DoCyberFreighter` composition, packed `PLORD` mutation, AI-table
integration, original differential, and promotion remain open. The
authoritative counters remain **118/152 closed, 34 open, 10
promotion units, and 44 practical open work items**.

## Latest checkpoint — DoCyberAiTurn direct DoCyberFreighter composition E6

The complete readable `DoCyberAiTurn` caller now invokes the real typed
`stars_do_cyber_freighter` implementation through `run_freighter_ex`. The
composition replay checks owned-planet transfer direction, target selection,
exact `0x11b0` movement, packed first-order replacement, and callback order
before the typed fleet visitor. Host and fresh VC++ 1.52c / Win98 `C:\\CYBFE6`
reports both pass
`CYBER_AI_TURN_FREIGHTER_COMPOSITION_CHECKS=18 FAILURES=0 RESULT=PASS`, with
normal exit and no Program Error dialog. This is supporting evidence and
closes no named Step-5 unit; complete packet/freighter predicates, raw
fleet-cargo behavior, AI-table integration, original differential, and
promotion remain open. Counters remain **118/152 closed, 34 open, 10 promotion
units, and 44 practical open work items**. Evidence is in
`analysis/compat/do-cyber-ai-turn-freighter-composition-e6.{md,json}` and
`runs/vmctl-vc152cybfe6/`.

## Latest checkpoint — DoCyberAiTurn packet/freighter composition E7

The complete readable AI turn now composes both real
`stars_do_cyber_freighter` and `stars_do_cyber_packets`. Host and fresh VC++
1.52c / Win98 `C:\\CYBFE7` reports both pass
`CYBER_AI_TURN_PACKET_FREIGHTER_COMPOSITION_CHECKS=20 FAILURES=0 RESULT=PASS`,
covering freighter-before-basic-task ordering, packet queueing after basic
tasks, packet-strength writeback from 501 to 389, exact `0x11b0` packed
movement, and finalization order. This is supporting evidence and closes no
named Step-5 unit; complete raw cargo predicates, packet/freighter edge
branches, AI-table integration, original differential, and promotion remain
open. Counters remain **118/152 closed, 34 open, 10 promotion units, and 44
practical open work items**. Evidence is in
`analysis/compat/do-cyber-ai-turn-packet-freighter-composition-e7.{md,json}`
and `runs/vmctl-vc152cybfe7/`.

## Latest checkpoint — SatisfyOrders raw player-transfer message E2 replay

The production `SatisfyOrders` source now exposes the original three-payload
`FSendPlrMsg2` player-transfer ABI through `raw_message2`. The typed fleet view
accepts a caller-resolved `WFromLpfl` word, and the order view accepts the new
target fleet identity/word produced by the allocation service. The strict-C89
host probe and fresh Win98 `C:\SYOE12` replay both report
`SATISFY_ORDERS_RAW_TRANSFER_CHECKS=21 FAILURES=0 RESULT=PASS`; compile/link
passed, the process exited 0, and no Program Error dialog remained in the final
window inventory.

This is supporting E2 evidence only. Ordinary eight-payload `FSendPlrMsg`
layouts, the remaining special message families, the original transfer
differential, E3 full-turn/original comparison, and promotion remain open.
Evidence is in `analysis/compat/satisfy-orders-raw-transfer-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-transfer-e2-native/`, and
`runs/vmctl-vc152syoe12/`.

## Latest checkpoint — SatisfyOrders raw auto-route message E2 replay

The production source now exposes the original eight-payload `FSendPlrMsg`
auto-route ABI through `raw_message`. The host and fresh Win98 `C:\SYOE13`
replay both report `SATISFY_ORDERS_RAW_ROUTE_CHECKS=20 FAILURES=0 RESULT=PASS`;
compile/link passed, the process exited 0, and no Program Error dialog remained
in the final window inventory. This is supporting E2 evidence only; ordinary
resource-transfer layouts, the remaining message families, the original
transfer differential, E3 full-turn/original comparison, and promotion remain
open. Evidence is in
`analysis/compat/satisfy-orders-raw-route-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-route-e2-native/`, and
`runs/vmctl-vc152syoe13/`.

## Latest checkpoint — SatisfyOrders raw shortage message E2 replay

The production source now binds pass-four resource shortages to the original
`FSendPlrMsg2` messages `0x011f`, `0x0120`, and `0x0123`. The host and fresh
Win98 `C:\SYOE16` replay report
`SATISFY_ORDERS_RAW_SHORTAGE_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/link
passed, the process exited 0, and the final window inventory had no Program
Error dialog. The exact source marker, fleet id, and resource-index payloads
agree with the recovered pass-four branch. This is supporting E2 evidence;
successful transfer and remaining message families, original differential, E3
comparison, and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-shortage-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-shortage-e2-native/`, and
`runs/vmctl-vc152syoe16/`.

## Latest checkpoint — SatisfyOrders raw colonist rejection E2 replay

The production source now binds colonist-drop rejection outcomes to the
original `FSendPlrMsg2` messages `0x0055`, `0x0056`, and `0x0135`. The host and
fresh Win98 `C:\SYOE17` replay report
`SATISFY_ORDERS_RAW_COLONIST_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/link
passed, the process exited 0, and the final window inventory had no Program
Error dialog. The exact source marker, fleet id, and target planet id payloads
agree with the recovered gates. This is supporting E2 evidence; successful
transfer and technology layouts, original differential, E3 comparison, and
promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-colonist-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-colonist-e2-native/`, and
`runs/vmctl-vc152syoe17/`.

## Latest checkpoint — SatisfyOrders raw resource-transfer E2 replay

The production source now forwards standard successful resource frames through
`raw_message`: planet mineral/colonist `0x002d`/`0x002e` and same-owner fleet
mineral/colonist `0x002b`/`0x002c`. The host and fresh Win98 `C:\SYOE19` replay
report `SATISFY_ORDERS_RAW_RESOURCE_CHECKS=24 FAILURES=0 RESULT=PASS`;
compile/link passed, the process exited 0, and the final window inventory had
no Program Error dialog. All eight caller-resolved payload words were checked;
the compile log retains one non-fatal C4018 warning. Foreign-fleet, fuel,
permission/failure, original differential, E3 comparison, and promotion remain
open. Evidence is in
`analysis/compat/satisfy-orders-raw-resource-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-resource-e2-native/`, and
`runs/vmctl-vc152syoe19/`.

## Latest checkpoint — SatisfyOrders raw technology frame E2 replay

The production source now forwards the caller-resolved recipient and eight-word
technology frame through `raw_message` after signed result binding. The host
and fresh Win98 `C:\SYOE18` replay report
`SATISFY_ORDERS_RAW_TECHNOLOGY_CHECKS=16 FAILURES=0 RESULT=PASS`; compile/link
passed, the process exited 0, and the final window inventory had no Program
Error dialog. Negative point `0x013c` and positive planet `0x013f` cases checked
the recipient and all eight words. This remains transport evidence, not full
original payload-field derivation; successful transfers, original differential,
E3 comparison, and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-technology-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-technology-e2-native/`, and
`runs/vmctl-vc152syoe18/`.

## Latest correction — SatisfyOrders technology/resource raw-frame de-duplication

The consolidated host replay found one regression after ordinary successful
resource frames were exposed through `raw_message`: a technology order could
emit both the ordinary transfer frame and its dedicated signed-result frame.
The production resolver now suppresses the ordinary raw resource frame when
`technology_flag` is set while retaining the typed notification path. All
eight affected host probes pass again (resource 24, technology 16, colonist
12, shortage 12, merge 12, mine 12, route 20, player transfer 21, and foreign
colonist 12: 141/141 checks). Fresh `C:\SYOE18` VC++ 1.52c/Win98 rebuild/replay reports
`SATISFY_ORDERS_RAW_TECHNOLOGY_CHECKS=16 FAILURES=0 RESULT=PASS`, compile/link
pass, exit 0, and no Program Error dialog. This is a correction/supporting
transport milestone and closes no named Step-5 unit. The authoritative TODO6
counters remain **105/152 closed, 47 open, 13 promotion units, and 60
practical open work items**. Evidence is in
`analysis/compat/satisfy-orders-raw-technology-e2-native.{md,json}` and
`runs/vmctl-vc152syoe18-r2/`.

## Latest checkpoint — SatisfyOrders foreign-fleet colonist rejection E2 replay

The production resolver now models the original foreign-fleet colonist
permission branch. A colonist unload targeting a fleet owned by another player
emits `FSendPlrMsg2(0x0155, fleet.id | 0x8000, fleet.id, source planet id)`,
does not invoke cargo transfer or the ordinary resource raw callback, and
cancels the order. The strict-C89 host probe and fresh VC++ 1.52c/Win98
`C:\SYOE20` replay report
`SATISFY_ORDERS_RAW_FOREIGN_COLONIST_CHECKS=12 FAILURES=0 RESULT=PASS`;
compile/link pass, exit 0, and no Program Error dialog remains. The combined
nine-probe raw-order host sweep passes 141/141 checks. This is supporting E2
evidence and closes no named Step-5 unit; fuel/optimal-fuel, remaining
permission/failure families, original differential, E3 comparison, and
promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-foreign-colonist-e2-native.{md,json}` and
`runs/vmctl-vc152syoe20/`.

Correction: the recovered enum is `idmAllowedTransferColonistsAnotherPlayer =
0x0155`; the earlier `0x0119` transcription was wrong (`0x0119` is
`idmHasStolen`). The production constant and packet were corrected and fresh
Win98 evidence in `runs/vmctl-vc152syoe20-r2/` again passes 12/12 with
compile/link pass, exit 0, and no Program Error dialog. The old run is kept as
superseded evidence.

## Latest checkpoint — SatisfyOrders fuel and optimal-fuel E2 replay

The production source now exposes a caller-resolved raw fuel-frame builder for
signed optimal-fuel adjustments (`0x002b`/`0x002d`) and the recovered special
messages `0x0126`, `0x003d`, and `0x003c`. The strict-C89 host probe and fresh
VC++ 1.52c/Win98 `C:\SYOE21` replay report
`SATISFY_ORDERS_RAW_FUEL_CHECKS=40 FAILURES=0 RESULT=PASS`; compile/link pass,
exit 0, and no Program Error dialog remains. This is supporting E2 ABI
evidence; upstream route-engine fuel calculation/pass gating, remaining
permission/failure families, original differential, E3 comparison, and
promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-fuel-e2-native.{md,json}` and
`runs/vmctl-vc152syoe21/`.

## Latest checkpoint — SatisfyOrders raw merge message E2 replay

The production source now binds merge outcomes to the original
`FSendPlrMsg2` messages `0x00f7`, `0x00f6`, and `0x00f5`. The host and fresh
Win98 `C:\SYOE15` replay report
`SATISFY_ORDERS_RAW_MERGE_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/link
passed, the process exited 0, and the final window inventory had no Program
Error dialog. The exact destination/source identity, `WFromLpfl`, and zero
payload layouts agree with the recovered decompilation and `turn3.c`. This is
supporting E2 evidence; ordinary resource-transfer and remaining message
families, original differential, E3 comparison, and promotion remain open.
Evidence is in `analysis/compat/satisfy-orders-raw-merge-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-merge-e2-native/`, and
`runs/vmctl-vc152syoe15/`.

## Latest checkpoint — SatisfyOrders raw mining message E2 replay

The production source now binds mining failures to the original
`FSendPlrMsg2` messages `0x0077`, `0x0075`, and `0x0076`. The host and fresh
Win98 `C:\SYOE14` replay report
`SATISFY_ORDERS_RAW_MINE_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/link
passed, the process exited 0, and the final window inventory had no Program
Error dialog. This remains supporting E2 evidence; resource-transfer and
remaining message-family layouts, original differential, E3 comparison, and
promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-mine-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-mine-e2-native/`, and
`runs/vmctl-vc152syoe14/`.

## Latest checkpoint — ProductionDlg packed-global E1

Fresh `C:\PDGP` VC++ 1.52c validation compiled and linked the
ProductionDlg packed-global adapter and returned
`PRODUCTION_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS`. The process exited
normally, redirected output was empty, and the post-run Win98 window list
contained no Program Error dialog. The packet checks the recovered 40-byte
`_zipprodq` fields, five-record storage cap, twelve packed item words, sparse
valid slots, and the original four-entry visible/menu cap.

This closes ProductionDlg E1. The authoritative current state is now
**82/152 closed evidence units, 70 open, 21 promotion units, 30/51 promoted
procedures, and 91 practical open work items**. ProductionDlg is `1/3` on
Step-5 evidence and `2 + P`; complete localized resources, original queue
differential, and final promotion remain. Evidence is in
`analysis/compat/production-dlg-packed-e1.{md,json}` and
`runs/vmctl-vc152pdgp/`.

## Latest checkpoint — DrawProductionDlg E3 and Step-6 promotion

Fresh `C:\DPDRE2` VC++ 1.52c validation compiled and linked the
DrawProductionDlg geometry/GDI packet and returned
`DRAW_PRODUCTION_DLG_GEOMETRY_CHECKS=9 FAILURES=0 RESULT=PASS`. The process
exited normally, redirected output was empty, and the post-run Win98 window
list contained no Program Error dialog. Combined with the original Ship &
Starbase Designer observation, this closes the named E3 GDI/caller boundary.

The final Step-6 synchronization is now recorded: `tracking/reviews.json`,
`analysis/coverage.json`, `TODO5.md`, TODO6's current scorecard, and the
reconstruction notes all promote `DrawProductionDlg` to `6/6`. The current
authoritative state is **81/152 closed evidence units, 71 open, 21 promotion
units, 30/51 promoted procedures, and 92 practical open work items**. Evidence
is in `analysis/compat/draw-production-dlg-geometry-e3.{md,json}` and
`runs/vmctl-vc152dpdre3/`.

The current combined state now also includes native supporting DrawPopup E3
state-order evidence: `DRAW_POPUP_STATE_CHECKS=7 FAILURES=0 RESULT=PASS` in
fresh `C:\DRINT`, with no Program Error dialog. This closes no named unit
because localized detailed rendering and caller differential remain open. The
authoritative counters remain **78/152 closed, 74 open, 22 promotion units,
and 96 practical open work items**; DrawPlanetStats is at `3 + P`, while
DrawProductionDlg is at `1 + P` and DrawPlanetShipList at `3 + P`.

## Latest checkpoint — DrawPlanShipBitmap GDI E1

Fresh `C:\DPSBINT` VC++ 1.52c validation compiled and linked the
DrawPlanShipBitmap GDI packet and returned
`DRAW_PLAN_SHIP_BITMAP_GDI_CHECKS=8 FAILURES=0 RESULT=PASS`. The process
exited normally and the post-run Win98 window list contained no Program Error
dialog. This closes E1 GDI lifecycle/geometry. The authoritative counters are
now **79/152 closed, 73 open, 22 promotion units, and 95 practical open work
items**. DrawPlanShipBitmap is now `3 + P`: E2 packed selection, E3 localized
names, E4 complete planet integration, and promotion remain. Evidence is in
`analysis/compat/draw-plan-ship-bitmap-gdi-e1.{md,json}` and
`runs/vmctl-vc152dpsb-gdi-e1/`.

## Latest checkpoint — DrawPlanetStats packed-lookup E1

Fresh `C:\DPSLE1` VC++ 1.52c validation compiled and linked the packed lookup
packet and returned `DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS`.
The process exited normally and the post-run Win98 window list contained no
Program Error dialog. This closes E1 packed lookup, moving the authoritative
counters to **80/152 closed, 72 open, 22 promotion units, and 94 practical
open work items**. DrawPlanetStats is now `2 + P`; E3 service/resource
behavior, E4 detailed GDI/original comparison, and promotion remain. Evidence
is in `analysis/compat/draw-planet-stats-lookup-e1.{md,json}` and
`runs/vmctl-vc152dpsle1/`.

## Latest checkpoint — DrawPlanetStats formatting E2

Fresh `C:\DPSFE2` VC++ 1.52c validation compiled and linked the formatting
packet, then returned `DRAW_PLANET_STATS_FORMAT_CHECKS=16 FAILURES=0 RESULT=PASS`.
The host probe reports the same result, the process exited normally, and the
post-run Win98 window list contained no Program Error dialog. A probe
accounting omission was corrected during replay by adding the unavailable
defense-string assertion for `77/0`; the 16-check count is now complete. This
closes E2. The authoritative counters are now **78/152 closed, 74 open, 22
promotion units, and 96 practical open work items**. DrawPlanetStats remains
at `5/6` with `3 + P`: E1 lookup, E3 service/resource behavior, E4 detailed
GDI/original comparison, and promotion remain. Evidence is in
`analysis/compat/draw-planet-stats-format-e2.{md,json}` and
`runs/vmctl-vc152dpsfe2/`.

The following native checkpoint closes `DrawPlanetShipList` E2 control layout.
Fresh `C:\DLIST` VC++ 1.52c validation compiled and linked the packet and
returned `DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=7 FAILURES=0 RESULT=PASS`.
The process exited normally and the post-run window list contained no Program
Error dialog. The authoritative counters are now **77/152 closed, 75 open,
22 promotion units, and 97 practical open work items**. DrawPlanetShipList
remains at `5/6` with `3 + P`: E1 packed records, E3 text/gauges/detail
rendering, and E4 complete transfer/drawing integration remain. Evidence is
in `analysis/compat/draw-planet-ship-list-controls-e2.{md,json}` and
`runs/vmctl-vc152dlist-controls-e2/`.

The next combat source-fidelity checkpoint corrected `FAttack`'s initiative
underflow. Raw `FAttack` stores `tok.initBase + part.dp` in an unsigned
16-bit temporary and caps only values above `0x3f`; the typed source had also
clamped negative sums to zero. The focused strict-C89 probe now passes
`F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS` for
`init_base=0`, `init_delta=-1`, and `input.init=63`, proving the raw
`0xffff -> 63` wrap-before-cap boundary. The packed token/hull adapter still
passes `F_ATTACK_INTEGRATION_CHECKS=2`, and the actual host battle chain still
passes `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18`.

This corrects source fidelity but closes no named Phase-2 Step-5 unit. The
current authoritative counters remain **75/152** closed evidence units,
**77** open evidence units, **22** promotion units, and **29/51** promoted
procedures. Exact packed `FLookupPart` binding, complete battle-part scaling,
broader original damage differentials, and native chain replay remain open.
Evidence is in `analysis/compat/f-attack-init-wrap-e4.{md,json}`. Native
replay remains pending the disconnected Win98 guest-agent endpoint.

The following combat checkpoint exposes the raw packed-part boundary inside
`FAttack`. The new optional resolver receives the `_hs` group/item/count fields
for non-empty `0x10`/`0x20` slots before initiative matching, populates a local
resolved section, and leaves the source section unchanged. The strict host
probe passes `F_ATTACK_PACKED_PART_CHECKS=8 FAILURES=0 RESULT=PASS` for lookup
and dispatch, the reconstructed `stars_lookup_part` gate and
`0x00100007` component identity, out-of-range-item rejection, and zero-count
suppression.

This remains host-only and closes no named Step-5 unit: the complete
native packed `FLookupPart` table/service binding, native packed replay,
complete battle-part scaling, original damage differential, and promotion
remain open. The counters remain **75/152** closed evidence units, **77** open
evidence units, **22** promotion units, and **29/51** promoted procedures.
Evidence is in `analysis/compat/f-attack-packed-part-e5.{md,json}`.

## Latest checkpoint — DrawPlanetShipList control-layout E2

Fresh `C:\DLIST` VC++ 1.52c validation compiled and linked the
DrawPlanetShipList control-layout packet and returned
`DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=7 FAILURES=0 RESULT=PASS`. The process
exited normally and the post-run Win98 window list contained no Program Error
dialog. This closes one named Step-5 evidence unit: E2 exact control handles,
button permutation, visibility order, combo geometry, lookup/reset, detail
fill, and enablement. The authoritative counters are now **77/152 closed,
75 open, 22 promotion units, and 97 practical open work items**.
DrawPlanetShipList remains at `5/6` with `3 + P`: E1 packed records, E3
text/gauges/detail rendering, and E4 complete transfer/drawing integration
remain before promotion. Evidence is in
`analysis/compat/draw-planet-ship-list-controls-e2.{md,json}` and
`runs/vmctl-vc152dlist-controls-e2/`.

## Latest checkpoint — DrawProductionDlg resource E2

Fresh `C:\DPDRE2` VC++ 1.52c validation compiled and linked the
DrawProductionDlg resource-table packet and returned
`DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=10 FAILURES=0 RESULT=PASS`. The process
exited normally, redirected console output was empty, and the post-run Win98
window list contained no Program Error dialog. This closes one named Step-5
evidence unit. The authoritative counters are now **76/152 closed, 76 open,
22 promotion units, and 98 practical open work items**. DrawProductionDlg
remains at `5/6` with `1 + P`: E3 full GDI/caller integration remains, then
the final ledger promotion. Evidence is in
`analysis/compat/draw-production-dlg-resource-e2.{md,json}` and
`runs/vmctl-vc152dpdre2/`.

The current combined state after the DrawPlanetShipList checkpoint is
**77/152 closed, 75 open, 22 promotion units, and 97 practical open work
items**. DrawProductionDlg is at `1 + P`; DrawPlanetShipList is at `3 + P`.
