# AI-turn Step-6 burndown

Snapshot: 2026-09-27. **82 small acceptance units remain; 9 are complete.**
They sit beneath the [20 open evidence gates and 6 promotion audits](ai-turn-step6-ledger.md).
The 26 parent items remain authoritative in [EVIDENCE.md](../../EVIDENCE.md#ai-personalities)
and [TODO7.md](../../TODO7.md#ai-personalities--20). This file splits only
their *remaining* work; previously verified supporting slices are recorded
in those files and are not counted again.

| Procedure | Evidence units open | Promotion units open | Total |
|---|---:|---:|---:|
| Robotoid | 11 | 2 | 13 |
| Turin Drone | 16 | 2 | 18 |
| Automitron | 12 | 2 | 14 |
| Rototill | 8 | 2 | 10 |
| Macinti | 15 | 2 | 17 |
| Cyber | 8 | 2 | 10 |
| **All six** | **70** | **12** | **82** |

Mark a unit complete only with a link to reproducible source, retained
original evidence, and the host or VC1.52c result that proves its stated
scope. A single artifact may prove several units, but each claim needs its
own check. Closing every unit under a gate triggers a separate review of that
gate's full [EVIDENCE.md](../../EVIDENCE.md#ai-personalities) acceptance rule;
the checkboxes alone do not promote it. Promotion units start after all of
that procedure's evidence gates close. Native results require build status,
logs, reports, and a unique return directory. Original Stars! runs follow
the stop-after-abnormal-exit rule in AGENTS.md.

## Robotoid — 13 open

- [ ] **RB-E2.a** — Retain original colonizer candidate, rejection predicate, and chosen target evidence.
- [ ] **RB-E2.b** — Retain original transport candidate, rejection predicate, and chosen target evidence.
- [ ] **RB-E2.c** — Retain original attack candidate, rejection predicate, and chosen target evidence.
- [ ] **RB-E2.d** — Bind those searches to live tables and compare their route/order writes with original packed output.
- [ ] **RB-E3.a** — Exercise a multi-order raw `PLORD` input, retaining all order bytes before and after.
- [ ] **RB-E3.b** — Bind dependent AI-table branches and compare their raw before/after bytes.
- [ ] **RB-E3.c** — Reproduce the same packed mutation through a VC1.52c caller and compare with original evidence.
- [ ] **RB-E4.a** — Assemble one deterministic original/host/native starting state with live AI, player, design, planet, and fleet tables.
- [ ] **RB-E4.b** — Run the complete native personality turn with build logs, callback trace, and returned state.
- [ ] **RB-E4.c** — Compare every resulting fleet, order, and resource field with the same-input original turn.
- [ ] **RB-E4.d** — Compare callback order and AI-table changes; resolve every mismatch or record an explicit gate failure.
- [ ] **RB-P.a** — Audit RB-E2–E4 and update the procedure's review scope and reconstruction notes.
- [ ] **RB-P.b** — Regenerate/check coverage and TODO5; synchronize TODO6/TODO7/EVIDENCE and verify 6/6.

## Turin Drone — 18 open

- [ ] **TU-E1.a** — Bind live production, AI, player, and design tables in the native caller.
- [ ] **TU-E1.b** — Bind the live strength table and verify its role/policy selections against original behavior.
- [ ] **TU-E1.c** — Reproduce remaining growth and resource-service effects from retained original inputs.
- [ ] **TU-E1.d** — Compare queue additions, finalization order, and packed production side effects.
- [ ] **TU-E1.e** — Compare the complete production-policy outcome with original records and a VC1.52c result.
- [ ] **TU-E2.a** — Verify target searches for every Turin fleet action family against original decisions.
- [ ] **TU-E2.b** — Verify the branch predicates and fallback sequence for those actions.
- [ ] **TU-E2.c** — Bind final route and order writes to live fleet/planet tables.
- [ ] **TU-E2.d** — Compare each action's packed order result with original behavior and native execution.
- [ ] **TU-E3.a** — Retain multi-order raw `PLORD` before/after bytes for each dependent branch.
- [ ] **TU-E3.b** — Retain dependent AI-table before/after bytes and branch inputs.
- [ ] **TU-E3.c** — Compare those raw mutations across host, VC1.52c, and original evidence.
- [ ] **TU-E4.a** — Assemble representative deterministic complete-turn inputs shared by native and original runs.
- [ ] **TU-E4.b** — Run the complete native turn and retain build logs, callback/RNG trace, and output state.
- [ ] **TU-E4.c** — Compare complete orders, production, resources, and AI tables with the original turn.
- [ ] **TU-E4.d** — Reconcile exact RNG/callback order and all remaining field-level differences.
- [ ] **TU-P.a** — Audit TU-E1–E4 and update the procedure's review scope and reconstruction notes.
- [ ] **TU-P.b** — Regenerate/check coverage and TODO5; synchronize TODO6/TODO7/EVIDENCE and verify 6/6.

## Automitron — 14 open

- [ ] **AU-E1.a** — Complete remaining colonizer action branches in original decision order.
- [ ] **AU-E1.b** — Complete remaining mineral and planet action branches using live services.
- [ ] **AU-E1.c** — Compare action, queue, and AI-table effects in a VC1.52c caller.
- [ ] **AU-E1.d** — Compare the resulting planet/resource/queue outcomes with retained original behavior.
- [ ] **AU-E3.a** — Verify live target searches and predicates for remaining route families.
- [ ] **AU-E3.b** — Compare each route family's final packed order writes with original evidence.
- [ ] **AU-E3.c** — Retain multi-order `PLORD` and AI-table before/after bytes for dependent branches.
- [ ] **AU-E3.d** — Reproduce those route and table mutations in VC1.52c and compare with original state.
- [ ] **AU-E4.a** — Assemble a deterministic same-input complete native/original AI-turn pair.
- [ ] **AU-E4.b** — Run and retain the complete native turn, build status, callbacks, and packed outputs.
- [ ] **AU-E4.c** — Compare all fleet and order state with the original turn.
- [ ] **AU-E4.d** — Compare resource, planet, AI-table, and message state; resolve all mismatches.
- [ ] **AU-P.a** — Audit AU-E1/E3/E4 and update the procedure's review scope and reconstruction notes.
- [ ] **AU-P.b** — Regenerate/check coverage and TODO5; synchronize TODO6/TODO7/EVIDENCE and verify 6/6.

## Rototill — 10 open

The [selector-3 coordinator](do-rototill-coordinator-e4-r1.md) already
matches complete X2/H2 and all 147 turn-1 plaintext output records. The
production source `WriteDataFile` and record writer now emit complete M1,
M2, and HST files byte for byte against the original. The source turn driver,
generation lifecycle, and exact original backups are also checked. Native
same-input execution and differential still require evidence.

- [x] **RT-E1.a** — Bind live production/resource inputs and exact service order in one complete selector-3 source turn: [queue boundary](do-rototill-queue-boundary-r1.md), [integrated coordinator](do-rototill-coordinator-e4-r1.md).
- [x] **RT-E1.b** — Run the integrated queue/resource path through VC1.52c with live resource and cost services, build logs, and a returned report: [r4 native return](do-rototill-production-turn-e1-native-r4.md). Parent RT-E1 still needs the complete native caller and same-input differential in RT-E4.d/e.
- [x] **RT-E1.c** — Compare complete production, resource, mineral, and queue effects with the original turn: [integrated coordinator](do-rototill-coordinator-e4-r1.md), [retained original oracle](do-rototill-turn-r1-oracle.json).
- [x] **RT-E2.a** — Verify the saved `Random(8)` and three output BOF draws with source `WriteBof` and original byte comparison: [output-tail evidence](do-rototill-rng-tail-r1.md).
- [ ] **RT-E2.b** — Verify original enemy, wormhole, and fallback route outcomes beyond the retained scout success path.
- [ ] **RT-E2.c** — Bind live allocator/global identity and selected fleet/planet pointers for those routes.
- [ ] **RT-E2.d** — Compare native/original route decisions and packed writes across the remaining cases.
- [x] **RT-E2.e** — Audit the retained selector-3 later main RNG call sequence and conditional skips against original source and output: [38-position source trace and 28 direct-caller dispositions](do-rototill-main-rng-audit-r1.md), [eight indirect-site resolution](do-rototill-rng-indirect-resolution-r1.json), and [original X2 execution guard cut](do-rototill-rng-execution-guards-r1.json). Other original route scenarios remain in RT-E2.b/c/d.
- [x] **RT-E3.a** — Bind and verify the colonizer AI-byte-15 marker write in the owned-planet positive-target branch: [source and native evidence](do-rototill-multi-order-e3.md#colonizer-ai-byte-15-marker-continuation-2026-09-27). Other colonizer entry states and live service identity remain in RT-E3.b/c.
- [ ] **RT-E3.b** — Bind and verify other second-sweep route and table branches.
- [ ] **RT-E3.c** — Resolve absent-target/live-service identity and selected-pointer semantics.
- [ ] **RT-E3.d** — Compare original internal multi-order `PLORD` and AI-table before/after bytes with VC1.52c; turn files alone are insufficient.
- [x] **RT-E4.a** — Bind the source-driven turn services, lifecycle, design/player, visibility, and backup state together against the retained decoded input: [selector-3 coordinator evidence](do-rototill-coordinator-e4-r1.md).
- [x] **RT-E4.b** — Source-serialize complete HST and compare every record and encrypted file byte with original: [selector-3 file evidence](do-rototill-coordinator-e4-r1.md).
- [x] **RT-E4.c** — Source-serialize complete M1/M2 and compare every record and encrypted file byte with original: [selector-3 file evidence](do-rototill-coordinator-e4-r1.md).
- [ ] **RT-E4.d** — Run the complete selector-3 coordinator natively in VC1.52c; retain build status, logs, report, and state. The [first compile packet](do-rototill-full-turn-native-r1.md) failed on an oversized test trace. The [segmented stage-9 packet](do-rototill-segmented-native-r1.md) now compiles in `/AL`, but its first full link has 185 unresolved symbols. Native link and execution remain open.
- [ ] **RT-E4.e** — Reconcile the same-input native/original complete-turn queue, route, order, planet, resource, and callback differential.
- [ ] **RT-P.a** — Audit RT-E1–E4 and update the procedure's review scope and reconstruction notes.
- [ ] **RT-P.b** — Regenerate/check coverage and TODO5; synchronize TODO6/TODO7/EVIDENCE and verify 6/6.

## Macinti — 17 open

- [ ] **MA-E1.a** — Capture the original live `vrglpplAi` pointer and planet/fleet-table lifecycle.
- [ ] **MA-E1.b** — Bind live planet and AI-table inputs, retaining selected records and rejected candidates.
- [ ] **MA-E1.c** — Compare original selection/production predicates with source decisions.
- [ ] **MA-E1.d** — Compare resulting planet, queue, AI-table, and fleet writes in native and original cases.
- [ ] **MA-E2.a** — Verify original retarget predicates, candidates, and target choice.
- [ ] **MA-E2.b** — Verify original colonizer predicates, candidates, and target choice.
- [ ] **MA-E2.c** — Verify original transport predicates, candidates, and target choice.
- [ ] **MA-E2.d** — Bind and compare each action family's route and order sequence against original records.
- [ ] **MA-E3.a** — Retain representative direct multi-order raw `PLORD` before/after bytes.
- [ ] **MA-E3.b** — Retain dependent AI-table before/after bytes and branch inputs.
- [ ] **MA-E3.c** — Reproduce and compare both mutations in VC1.52c against original evidence.
- [ ] **MA-E4.a** — Bind remaining personality branches to live tables and original predicates.
- [ ] **MA-E4.b** — Assemble and run a deterministic same-input native/original complete turn.
- [ ] **MA-E4.c** — Compare complete orders and fleets, resolving every field mismatch.
- [ ] **MA-E4.d** — Compare complete resources, planet/AI tables, messages, and callback order.
- [ ] **MA-P.a** — Audit MA-E1–E4 and update the procedure's review scope and reconstruction notes.
- [ ] **MA-P.b** — Regenerate/check coverage and TODO5; synchronize TODO6/TODO7/EVIDENCE and verify 6/6.

## Cyber — 10 open

- [ ] **CY-E3.a** — Bind remaining original raw Cyber branch predicates beyond the verified second-order marker.
- [ ] **CY-E3.b** — Retain complete multi-order `PLORD` before/after bytes for those branches.
- [ ] **CY-E3.c** — Retain dependent AI-table before/after bytes and selected targets.
- [ ] **CY-E3.d** — Compare those raw mutations across source, VC1.52c, and original evidence.
- [ ] **CY-E4.a** — Assemble a deterministic same-input complete native/original personality turn.
- [ ] **CY-E4.b** — Run and retain native build status, callback trace, and complete turn state.
- [ ] **CY-E4.c** — Compare AI tables, fleet, production, orders, and resources with original output.
- [ ] **CY-E4.d** — Compare callback order and remaining branches; resolve every mismatch.
- [ ] **CY-P.a** — Audit CY-E3/E4 and update the procedure's review scope and reconstruction notes.
- [ ] **CY-P.b** — Regenerate/check coverage and TODO5; synchronize TODO6/TODO7/EVIDENCE and verify 6/6.

## Updating the burndown

Check a line only when its evidence link is recorded beside that line. Update
the count table and [concise ledger](ai-turn-step6-ledger.md) together. A
parent gate closes only after its units and the full EVIDENCE acceptance rule
pass; a promotion closes only after both P units and the 6/6 audit pass.
Recount and detect a stale table with:

```text
python3 tools/ai_turn_burndown.py --check
```
