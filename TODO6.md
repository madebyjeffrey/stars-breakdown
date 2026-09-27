# TODO6 — finish the remaining source to review

This checklist turns the current handoff into an execution plan for promoting
the remaining partial and open game procedures to reviewed source. It extends
[TODO5.md](TODO5.md); it does not replace the project-wide completion criteria
in [TODO.md](TODO.md).

The live item-by-item Phase 2 count and newest-first completion timestamps
are maintained in [TODO7.md](TODO7.md). Historical checkpoint paragraphs
below retain the counts that were true when they were written; use the
Current state and Step-5 burn-down rows for the present count.

## Current state

The handoff had reported:

- 846 named procedures and 890,384 matched procedure bytes.
- 787 formally reviewed procedures.
- 52 documented partial procedures.
- 7 open procedures.
- 839 procedures touched and 714,873 credited bytes.

Phase 0 has repaired the checked-in ledger. The authoritative regenerated
TODO5 state is now 838 reviewed procedures, 8 partial procedures, and 0 open
procedures. The apparent open entries were already reconstructed and
validated in `tracking/reviews.json`; their stale one-byte range keys had
prevented the coverage ledger from recognizing them.

These TODO5 and TODO6 numbers are intentionally different measures. TODO5's
current regenerated source-level count is **838 reviewed, 8 partial, and 0
open**. TODO6's Phase 2
gate is stricter: a source can be marked reviewed in TODO5 while its
packed-record, original-behavior, native-integration, or final promotion
concern is still open. The meaningful Phase 2 count is therefore
`9 at 5/6, 42 at 6/6`, not the TODO5 partial count alone.

## Definition of done for a procedure

Promote a procedure only when all of the following are true:

- [ ] Boundaries, debug signature, near/far ABI, segment assumptions, locals,
      embedded tables, calls, data references, and indirect dispatch are
      reconciled against the original code.
- [ ] The source is readable, buildable C and preserves 16-bit widths,
      signedness, far-pointer representation, carry/wrap behavior, aliasing,
      floating-point behavior, RNG order, side effects, and error paths.
- [ ] Host strict-C89 probes cover normal, boundary, and failure cases and
      check complete relevant state and callback/service-call order.
- [ ] The relevant VC++ 1.52c / Win98 harness compiles, links, runs, and
      preserves its reports and logs in a unique `runs/` directory.
- [ ] Behavior is compared with the original executable or with documented
      original-instruction evidence at the appropriate boundary.
- [ ] `tracking/reviews.json`, `analysis/coverage.json`, TODO5, and the
      reconstruction notes record the exact completed scope and evidence.

Byte matching is supporting evidence only. It does not replace semantic source
review, integration, or native validation.

## Six-step Phase 2 progress scale

The six steps below correspond to the definition of done above. A procedure's
main Phase 2 checkbox stays unchecked until step 6; the checked items in the
progress log are completed milestones, not procedure completion claims.

1. **Boundary recovered** — original boundaries, ABI, locals, tables, calls,
   data references, and indirect dispatch are reconciled.
2. **Readable source** — the typed 16-bit source is buildable and preserves
   the identified width, signedness, wrap, aliasing, RNG, and error behavior.
3. **Host probe** — strict-C89 normal, boundary, and failure cases cover the
   relevant state and callback/service-call order.
4. **Native checkpoint** — a fresh VC++ 1.52c / Win98 harness compiles, links,
   runs, and preserves reports and logs.
5. **Original/integration evidence** — packed records, resources, Win16 UI or
   state integration, and documented original-executable behavior agree at the
   procedure's acceptance boundary.
6. **Promotion** — the complete evidence is synchronized in the ledgers and
   reconstruction notes, and the procedure is promoted out of the partial set.

Current Phase 2 distribution: **0 procedures at 4/6, 0 at 3/6, 9 at 5/6,
and 42 at 6/6**. All 51 have steps 1–4 complete; the 9 rows at 5/6 are
working through their Step-5 evidence packets, while the 42 rows at 6/6
have completed promotion. Further reductions require Step-5 integration
and original-behavior work.

For an unambiguous numerical reading, the current promotion counters are:

- **51/51** Phase 2 procedures have entered step 5 and have at least one
  integration/original-behavior evidence item.
- **42/51** have closed every step-5 acceptance concern.
- **42/51** have completed step 6 and been promoted out of the partial set.

- **124/152** named step-5 evidence units are now closed; **28 remain**.
- **9** step-6 promotion units remain, so the practical open-work count is
  **37**.

### Latest checkpoint — SatisfyOrders E2 exact-input native closure

- [x] Recovered task 5's transient `gd.grBits2` bit `0x0008` around
      `UpdateShdefCost`; corrected `GetTruePartCost` from the incorrect
      `0x0800` guard, and set/cleared bit 3 in the packed task-5 pricing path.
      The exact v4 original high-offer SHDEF, player-1 race/technology, planet,
      and original part records pass strict-C89 host and fresh VC1.52c/Win98
      native **74/74** checks. The report verifies costs 1930/494/681/261,
      callback/message order, planet mutation, tech advancement, and fleet
      retirement. [Closure note](analysis/compat/satisfy-orders-task5-v4-current-source-native.md)
      and [native return](runs/vmctl-soe2s/MANIFEST.JSON) retain the evidence.
- [x] SO-E2 now closes the second of three authoritative SatisfyOrders units;
      SO-E3's complete same-input full-turn/RNG comparison remains open, so
      the procedure remains at **5/6**. Current counters: **123/152 evidence
      units closed, 29 open, 10 promotions open, 39 practical open work items**.

### Latest checkpoint — SatisfyOrders E2 service order and E3 input planet

- [x] Corrected packed task-5 friendly-colonist transfer to follow message
      removal and the fleet dead-bit store. The expanded strict-C89 service
      trace passes 30/30; earlier native packets predate this source change.
      Decoded planet 10 from the same original HST input for clean/action
      turns: 469/893/376 minerals, 250 colonists. The scoped production
      four-pass host replay now starts there and ends 478/899/381/250,
      versus original full-turn 485/907/388/287. The 7/8/7/37 remaining
      difference belongs to surrounding turn services. SO-E2/E3 stay open;
      no guest run was made. See [progress](analysis/compat/satisfy-orders-task5-e2-e3-progress.md).

### Previous checkpoint — corrected SatisfyOrders E1 native rerun

- [x] The verified HST/EXE design costs 24 resources and 12/8/7 minerals.
      The source-derived isolated starbase return is 9/6/5 and its owner
      message reports 20. A fresh `C:\SOE1G` VC++ 1.52c packet compiled,
      linked, and passed 19/19 checks, restoring SO-E1 closure. The original
      full-turn stockpile delta of 9/5/5 has an unresolved one-boranium
      difference in SO-E3. E2 resource-branch native 62/62 and E3 scoped
      four-pass native report also passed; those boundaries remain open.
      Current count: **30 evidence + 10 promotions = 40**. See
      [E1 evidence](analysis/compat/satisfy-orders-task5-e1.md) and
      [E2/E3 progress](analysis/compat/satisfy-orders-task5-e2-e3-progress.md).

### Previous correction — verified task-5 design cost

- [x] Recovered original design 0 costs from the HST record and executable
      tables: owner-0 cost 24 resources and 12/8/7 minerals. Isolated starbase
      scrap returns 9/6/5 and the first message reports 20. The earlier
      original full-turn clean/action planet delta remains 9/5/5; the scoped
      four-pass replay pins one additional boranium. The prior SO-E1 native
      packet used inferred design costs, so SO-E1 is reopened pending the
      corrected native run and a resolved accounting comparison. SO-E2 and
      SO-E3 remain open. Current count: **31 evidence + 10 promotions = 41**.
      See the [design-cost oracle](analysis/compat/satisfy-orders-design0-input-oracle.json)
      and [E1 correction](analysis/compat/satisfy-orders-task5-e1.md).

### Previous checkpoint — SatisfyOrders task-5 planet scrap accounting

- [x] Added an opt-in packed task-5 planet-scrap path using the raw fleet,
      order, SHDEF, and planet layouts. It returns minerals and friendly
      colonists, emits the source-derived 0x59/0x5A and 0x140/0x141 raw message
      frames, and retires the fleet/order. The original action run versus the
      original clean run gives planet-10 mineral differences +9/+5/+5. The
      [focused host and Win98 packet](analysis/compat/satisfy-orders-task5-e1.md)
      passes 19/19; build and link pass. The original clean/action planet
      stockpiles and first two serialized message frames match byte for byte;
      SO-E1 is closed. Complete special services and the full differential
      remain as SO-E2/E3: **30 evidence + 10 promotions = 40 counted items**.

### Previous checkpoint — SatisfyOrders action-bearing original run and evidence correction

- [x] Isolated three original-executable runs of a single task-5 order:
      M1-only, HST+M1, and HST-only. The [pinned original oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md)
      shows M1-only preserves six fleets/message 63, while either HST edit
      removes fleet 0 and emits the identical message 90 body. Input backups,
      output hashes, decoded records, and original instruction source are
      checked by a new test. The original task-5 branch is **scrap**, not the
      current production adapter's `battle_plan` behavior. This contradicts
      the SO-E1 packed battle-plan and SO-E2 complete-special-order closure
      claims, so both units are reopened in TODO7. Their earlier host/native
      subchecks remain useful but insufficient. Full host suite **578 tests**,
      153 expected skips, zero failures. SO-E1/E2/E3 and SO-P remain open:
      **31 evidence + 10 promotions = 41 counted items**.

### Previous checkpoint — Macinti raw selected-planet pointer list

- [x] Added an opt-in byte-exact `vrglpplAi` far-pointer list to the
      selected-planet path. It resolves complete offset:segment tokens to
      caller-bound 56-byte planet records, preserves pointer-list order,
      and rejects truncated, unmatched, or ambiguous non-null entries before
      AI-table writes. The original `vrglpplAi` loops stop at the first null
      pointer, so that case now terminates the scan rather than failing.
      The [MA-E1 pointer evidence](analysis/compat/do-macinti-planet-table-e1.md)
      and strict-C89 probe pass **37/37**; full host suite **575 tests**,
      153 expected skips, zero failures; baseline/diff clean. An agent
      reconnect left r25 without its driver and that attempt failed to
      compile; fresh `C:\MAE1N26` was verified as 18 complete files before
      build, then passed compile/link and all eight Win16 probes, including
      **36/36** planet checks. Fresh r27 then passed compile/link and all
      eight Win16 probes with the corrected **37/37** planet checks.
      [Native return](runs/vmctl-mae1n27/MAE1RES.TXT) preserves the
      status, logs, executable and map. A live
      original pointer capture, downstream branches,
      and original-turn comparison remain. **MA-E1 and all 39 counted items
      stay open.**

### Previous checkpoint — live Win98 Macinti native packet and bound attack route

- [x] Restored the STARSAGT connection and ran fresh `C:\MAE1N24` in
      VC++ 1.52c. Compile and link both pass; the Win16 report passes all
      eight probes (**31/19/39/12/10/16/8/14**). The first r23 attempt
      compiled but exposed a probe-only `printf` link error; the r24 driver
      removes that console-only dependency. Build status, logs, executable,
      map, and report are retained in [the native return](runs/vmctl-mae1n24/MAE1RES.TXT).
      The selected-planet loop now has an opt-in bounded attack-production
      bridge with its queue-visible pressure/count updates (**19/19** host
      pressure checks). This is supporting native evidence, not an original
      action-bearing differential. Pointer-list binding and other production
      branches remain; **MA-E1/MA-E2 and all 39 counted items stay open**.

### Previous checkpoint — Macinti mission queue before colonizer/attack route

- [x] Extended the executable 10a0:0f5b–1307 source slice with the
      original selected mission-design queue check before the outer route:
      running mission count below 64 and player `cPlanet / 4`, then
      `Random(3)==0`, mission-count increment before queue, no attack-pressure
      increment. The combined [MA-E1/E2 production-slice note](analysis/compat/do-macinti-attack-pressure-queue-e1.md)
      and strict-C89 probe now cover mission→`Random(100)`→attack order,
      cap rejection, and preflight before side effects (**39/39**). Full
      suite **575 tests**, 153 expected skips, zero failures;
      baseline/diff clean. Fresh `C:\MAE1N22` eight-probe packet staged but
      unrun.
- [ ] Integrate the bounded slice into the main selected-planet pass and
      reconstruct other preceding/later queues, then run native and original
      comparisons. **MA-E1/MA-E2 and all 39 counted items remain open.**

### Previous checkpoint — Macinti bound planet production gates and finish

- [x] Removed synthetic generic typed queue-limit/resource/hull-affordability
      filters from the packed selected-planet path after its original
      starbase/population and live-PLPROD checks. The normal path now invokes
      `FinishProduction(0 or 1)` after the represented plan; an explicit
      early finish is not repeated. The unbound typed compatibility path
      keeps its prior filters. The
      [MA-E1 planet lifecycle](analysis/compat/do-macinti-planet-table-e1.md)
      and [pressure evidence](analysis/compat/do-macinti-attack-pressure-queue-e1.md)
      document this distinction. Focused pressure probe **16/16**, attack
      slice **35/35**, planet table **31/31**; full suite **575 tests**, 153
      expected skips, zero failures; baseline/diff clean. Fresh
      `C:\MAE1N21` eight-probe packet staged but unrun.
- [ ] Integrate the bounded colonizer-to-attack slice and reconstruct the
      surrounding production branches, then obtain native and original
      behavior evidence. **MA-E1/MA-E2 and all 39 counted items remain open.**

### Previous checkpoint — Macinti bounded colonizer-to-attack sequence

- [x] Composed the original 10a0:0f5b–1307 outer route, colonizer queues,
      and inner attack gate in executable source. Packed `rgwtMin[3]` and
      32-bit-wrapped growth select one to three colonizer calls; count rises
      before the first two only. On the later attack choice, pressure rises
      before that queue call. The
      [MA-E1/E2 production-slice note](analysis/compat/do-macinti-attack-pressure-queue-e1.md)
      states the excluded earlier/later production branches. Combined
      strict-C89 probe **35/35**, including call order and partial failure
      state; full suite **575 tests**, 153 expected skips, zero failures;
      baseline/diff clean. Fresh `C:\MAE1N19` eight-probe packet staged but
      unrun.
- [ ] Wire the bounded slice into the main per-planet dispatcher with
      preceding/following queues, then obtain native and original behavior
      evidence. **MA-E1/MA-E2 and all 39 counted items remain open.**

### Previous checkpoint — Macinti outer colonizer/attack route

- [x] Reconstructed original 10a0:0f5b/0ffc route choice before the inner
      attack gate: `FShouldWeBuildColonizers`/count/turn and `Random(100)`
      order, direct late-turn bypass, signed packed `rgwtMin[0..2] < 30`
      early exit, planet-colonizer decision, and turn-4/5 distinction between
      colonizer-only and colonizer-then-attack. The
      [MA-E1/E2 attack production note](analysis/compat/do-macinti-attack-pressure-queue-e1.md)
      records the exact boundary. Combined strict-C89 outer/inner probe
      **28/28**; full suite **575 tests**, 153 expected skips, zero failures;
      baseline/diff clean. Fresh `C:\MAE1N18` eight-probe packet staged but
      unrun; StarDev manager says running, but STARSAGT did not connect to
      the local listener in this attempt.
- [ ] Integrate the outer route, colonizer queue/growth effects and inner
      attack decision into the complete production sequence, then obtain
      native and original comparison. **MA-E1/MA-E2 and all 39 counted items
      remain open.**

### Previous checkpoint — Macinti inner attack-production queue gate

- [x] Isolated original 10a0:1153–1307 after its outer colonizer/random
      branch: selected design/existing-count/pressure limits, `Random(2)`,
      first matching packed owned attack fleet and mine-service value,
      three packed concentration bytes, then optional `Random(10)` fallback.
      The [MA-E1/E2 inner-gate note](analysis/compat/do-macinti-attack-pressure-queue-e1.md)
      documents the caller-supplied outer gate. Strict-C89 probe **16/16**;
      full suite **575 tests**, 153 expected skips, zero failures;
      baseline/diff clean. Fresh DOS packet `C:\MAE1N17` contains all eight
      probes (31/13/16/12/10/16/8/14), staged but unrun.
- [ ] Bind the outer branch and complete production sequence to the new
      decision, then obtain native and original-behavior evidence. **MA-E1/
      MA-E2 and all 39 counted items remain open.**

### Previous checkpoint — Macinti attack-pressure queue increment

- [x] Added an opt-in production-plan marker for the original
      AddItemToQueue(attack design,1,2,1) branch. Starting from the
      packed fleet census, the bound path increments pressure before
      that exact queue callback; unmarked and mismatched plans cannot
      increase it. The [MA-E1/E2 pressure probe](analysis/compat/do-macinti-attack-pressure-queue-e1.md)
      passes **13/13**; prior Macinti probes remain 31/31, 12/12,
      10/10, 16/16, 8/8 and 14/14. Full suite: **574 tests**, 153
      expected skips, zero failures. Baseline/diff clean; fresh
      `C:\MAE1N16` packet staged but unrun.
- [ ] Reconcile the original random/resource/fleet predicate choosing
      this queue branch and verify the caller's opt-in completeness,
      then finish wider routing, native and original comparison.
      **MA-E1/MA-E2 and all 39 counted items stay open.**

### Previous checkpoint — Macinti attack-pressure fleet census

- [x] Captured the original early pressure census from the
      design-available owned packed fleets with ship slot 14/15 present,
      and reject a supplied post-production pressure below that census.
      The [MA-E2 buddy/miner probe](analysis/compat/do-macinti-buddy-miner-e2.md)
      now passes **16/16**; earlier Macinti probes remain 31/31,
      12/12, 10/10, 8/8 and 14/14. Full suite: **573 tests**, 153
      expected skips, zero failures. Baseline/diff clean; fresh
      `C:\MAE1N14` packet staged but unrun.
- [ ] The later attack-production queue increment, other route
      branches, native return and original comparison remain. **MA-E2
      and all 39 counted items stay open.**

### Previous checkpoint — Macinti attack-design buddy/miner followup

- [x] Bound the already-kind-3 attack-design branch to the original
      post-production pressure counter (not game turn), conditional
      14/15 buddy join, packed owned-planet mineral sums, thresholded
      retarget and RNG order. The [MA-E2 buddy/miner probe](analysis/compat/do-macinti-buddy-miner-e2.md)
      passes **12/12**; earlier Macinti probes pass 31/31, 12/12,
      10/10, 8/8 and 14/14. Full suite: **573 tests**, 153 expected
      skips, zero failures. Baseline/diff clean; fresh `C:\MAE1N13`
      packet staged but unrun.
- [ ] Bind the live post-production pressure provenance, remaining
      attack/colonizer routing and complete packed route sequence;
      obtain native return and original comparison. **MA-E2 and all 39
      counted items stay open.**

### Previous checkpoint — Macinti first-sweep transport target repair

- [x] Bound the original FIsAiAttack → FIsAiTransport service order,
      current/second packed-order target choice, planet ownership and
      zero-cargo gate, then selected cord/PLORD truncation, refresh and
      ClearAiCurrentTask(0) through the opt-in live-service boundary.
      The [MA-E2 first-transport probe](analysis/compat/do-macinti-first-transport-e2.md)
      passes **10/10**; earlier Macinti probes pass 31/31, 12/12, 8/8
      and 14/14. Full suite: **572 tests**, 153 expected skips, zero
      failures. Baseline/diff clean; fresh `C:\MAE1N11` packet staged,
      not run.
- [ ] Complete attack/buddy/miner/colonizer routing, full packed route
      sequence, native return and original comparison remain. **MA-E2
      and all 39 counted items stay open.**

### Previous checkpoint — Macinti attack-design kind-3 precedence

- [x] Bound the original one-order attack-design predicate and selected
      PLORD kind-3 write before the later kind-5 branch. The
      [MA-E3 order probe](analysis/compat/do-macinti-attack-order-e3.md)
      now passes **14/14** (seven kind-3 and seven earlier kind-5 checks);
      earlier Macinti probes remain 31/31, 12/12 and 8/8. Full host
      suite: **571 tests**, 153 expected skips, zero failures.
      Baseline/diff clean; fresh `C:\MAE1N10` packet staged but unrun.
- [ ] Already-kind-3 buddy/miner action, remaining first-sweep
      attack/transport/colonizer decisions, multi-order route sequencing,
      native return and original comparison remain. **MA-E3 and all 39
      counted items stay open.**

### Previous checkpoint — Macinti early-turn first-order kind

- [x] Bound the original owned-fleet, turn-0–10 colonizer/ship-slot-2
      predicate and selected PLORD first-kind write to 5, preserving high
      bits, refreshing the selected fleet and advancing to the next fleet.
      The [MA-E3 early-order probe](analysis/compat/do-macinti-early-order-e3.md)
      passes **7/7**; prior Macinti probes pass 31/31, 12/12 and 8/8.
      Full suite: **571 tests**, 153 expected skips, zero failures.
      Baseline/diff clean; fresh `C:\MAE1N9` packet staged, not run
      because STARSAGT did not connect to a new local listener.
- [ ] Earlier first-sweep branch ordering, multi-order route mutation,
      native return and original comparison remain. **MA-E3 and all 39
      counted items stay open.**

### Previous checkpoint — Macinti distant second waypoint

- [x] Bound the original first-sweep task-8 second-waypoint distance
      predicate, including 16-bit subtraction wrap, signed-32-bit threshold,
      foreign-fleet inclusion, selected packed `cord`/`PLORD.iordMac`
      truncation, and refresh. The [MA-E3 supporting probe](analysis/compat/do-macinti-far-waypoint-e3.md)
      passes **8/8**; E1/E2 remain 31/31 and 12/12. Full host suite:
      **570 tests**, 153 expected skips, zero failures. Baseline/diff clean;
      fresh `C:\MAE1N8` native packet staged, not run.
- [ ] Full multi-order PLORD route sequencing, selected-buffer identity in
      Win16, native return and original comparison remain. **MA-E3 and all
      39 counted items stay open.**

### Previous checkpoint — Macinti second transport pass

- [x] Replaced the bound second-pass ship-slot-1 approximation with the
      original owned/`cord < 2`/`FIsAiTransport` service predicate, then
      plan-4 select/refresh before the freighter-target callback. The
      [MA-E2 probe](analysis/compat/do-macinti-packed-fleet-e2.md) passes
      **12/12**, the older integration probe 31/31, and the full host suite
      **569 tests** with 153 expected skips and zero failures. Baseline/diff
      clean; fresh `C:\MAE1N7` packet supersedes r6.
- [ ] First-sweep retarget/colonizer/transport branches, full route/order
      state, Win16 return and original comparison remain. **MA-E2 and all
      39 counted items stay open.**

### Previous checkpoint — Macinti packed fleet/action bridge

- [x] Bound original 124-byte fleet owner/location/`cord` and sixteen
      16-bit ship counts into the Macinti action, transport and
      finalization passes, independent of the optional fleet observer.
      The [MA-E2 supporting probe](analysis/compat/do-macinti-packed-fleet-e2.md)
      passes **8/8**; the earlier integration probe still passes 31/31.
      Fresh combined `C:\MAE1N6` native packet supersedes r5. Full host
      suite: **569 tests**, 153 expected skips, zero failures;
      baseline/diff clean.
- [ ] Exact retarget/colonizer/transport predicates, candidate choice,
      packed route/order sequence, native return and original comparison
      remain. **MA-E2 remains open; 29 evidence + 10 promotions = 39
      counted open items.**

### Previous checkpoint — Macinti observer-independent planet actions

- [x] Removed the bound selected-planet action loop's dependency on
      `visit_planet_typed`; the packed selection, InitProduction, live
      PLPROD bypass, and typed dispatch now run with no observation
      callback. The [MA-E1 probe](analysis/compat/do-macinti-planet-table-e1.md)
      passes **31/31** checks. Fresh `C:\MAE1N5` packet supersedes r4.
      Full host suite: **568 tests**, 153 expected skips, zero failures;
      baseline/diff clean.
- [ ] Native return, actual pointer-list integration, later production
      branches and original-turn comparison remain. **29 evidence + 10
      promotions = 39 counted open items.**

### Previous checkpoint — Macinti live production-queue bypass

- [x] Added the bounded selected-planet `ChangeMainObjSel`/`InitProduction`
      service sequence, original 24-item pre-init skip, post-init PLPROD
      type-2/design-<16 scan, and `FinishProduction(0)` bypass. The
      [MA-E1 evidence](analysis/compat/do-macinti-planet-table-e1.md)
      now passes **29/29** strict-C89 checks including callback order and
      malformed queue rejection. Fresh `C:\MAE1N4` packet supersedes r3.
      Full host suite: **568 tests**, 153 expected skips, zero failures;
      baseline/diff clean.
- [ ] STARSAGT again did not connect to a fresh listener; the VM stayed
      running. Later resource/RNG/add-item branches, native return, and
      original-turn comparison remain. **29 evidence + 10 promotions =
      39 counted open items.**

### Previous checkpoint — Macinti selected-planet action gate

- [x] Bound the original pre-action starbase/mineral/starbase-kind/turn
      predicate in the selected Macinti planet loop. The [MA-E1 evidence](analysis/compat/do-macinti-planet-table-e1.md)
      now passes **23/23** strict-C89 checks, including the 200-unit
      threshold and kind-1 turn-25/26 boundary. Fresh `C:\MAE1N3` Win16
      packet supersedes r2. Full host suite: **568 tests**, 153 expected
      skips, zero failures; baseline/diff clean.
- [ ] Live selection and production lifecycle, the native packet, and
      original action-bearing comparison remain. MA-E1 is open and the
      ledger still has **29 evidence + 10 promotions = 39 items**.

### Previous checkpoint — Macinti fleet-derived AI marker

- [x] Extended the bounded MA-E1 path with the original byte-14 clear and
      owned-fleet second-MOVE-order marker, using packed fleet/PLORD fields
      and the returned 10/11 design status. The [Macinti E1 evidence](analysis/compat/do-macinti-planet-table-e1.md)
      now has **17/17** strict-C89 checks. A fresh `C:\MAE1N2` Win16 packet
      supersedes the unrun r1 packet. Full host suite: **568 tests**,
      153 expected skips, zero failures; baseline/diff clean.
- [ ] Real Win16 return and downstream production/selection/original-turn
      differential remain. **MA-E1 remains open; 29 evidence plus 10
      promotions = 39 open items.**

### Previous checkpoint — Macinti bounded planet-table selection

- [x] Bound the optional Macinti packed planet and 16-byte AI table slice:
      clear/mark selected IDs, compute foreign threat/visited bytes, and
      visit the selected planet order. The [MA-E1 supporting evidence](analysis/compat/do-macinti-planet-table-e1.md)
      has an **8/8** strict-C89 probe and a fresh `C:\MAE1N1` Win16 packet.
      Full host suite: **567 tests**, 153 expected skips, zero failures;
      packet static test and baseline/diff checks pass.
- [ ] The guest packet is unrun while STARSAGT is disconnected. Full
      pointer-list/fleet-marker/production behavior and original-turn
      comparison remain; **MA-E1 and all 39 counted items stay open**.

### Previous checkpoint — StarDev agent reconnection diagnostic

- [x] Confirmed the 86Box StarDev process and manager are running,
      the host protocol self-test passes, and STARSAGT source retries
      roughly every two seconds. A 15-second listener window received
      no guest connection. The [reconnection diagnostic](analysis/compat/win98-agent-reconnect-2026-09-23.md)
      identifies the guest-side STARTAGT/status/log check needed before
      the fresh `C:\RTE1N7` native packet can run. The VM was not reset.
- [ ] No native result or named Phase 2 item closed: **29** evidence and
      **10** promotions (**39** total). Once the guest agent reconnects,
      run the r7 packet and retain compile/link/report artifacts.

### Previous checkpoint — Rototill post-add queue relocation

- [x] Reacquire the bounded live `lpplProdGlob` after each successful
      `AddItemToQueue`, so the second opening-turn add and
      `FinishProduction` observe a relocated allocation. The expanded
      [RT-E1 live-queue probe](analysis/compat/do-rototill-production-e1.md)
      passes **12/12** strict-C89 checks for two-add and one-add pointer
      changes and malformed post-add rejection. [Fresh r7 native packet](analysis/compat/do-rototill-production-e1-native-packet.md)
      is prepared for `C:\RTE1N7` but unrun. Full host suite:
      **566 tests**, 153 expected skips, zero failures; baseline/diff clean.
- [ ] Actual Win16 global identity, VC1.52c return, and original
      production-turn comparison keep RT-E1 open. Phase 2 remains
      **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Rototill post-init live production queue

- [x] Changed the RT-E1 production bypass to read a bounded live
      `lpplProdGlob` allocation after `InitProduction`, rather than a
      possibly stale per-planet snapshot. Queue and finish callbacks also
      receive the active allocation. The new [live-queue probe](analysis/compat/do-rototill-production-e1.md)
      passes **7/7** strict-C89 checks for bypass/open branches, callback
      order, and malformed/failing live binding. [Fresh r6 native packet](analysis/compat/do-rototill-production-e1-native-packet.md)
      bundles it with the prior 20/20 production and 17/17 fleet probes
      for `C:\RTE1N6`; it has not run. Full host suite: **566 tests**,
      153 expected skips, zero failures; baseline/diff clean.
- [ ] Live global-pointer identity, post-add reallocation, VC1.52c return,
      and original production-turn comparison keep RT-E1 open. Phase 2
      stays **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Rototill occupied-destination order repair

- [x] Added the original current-planet occupied-target sequence:
      select the fleet, set packed fleet `cord` and `PLORD.iordMac` to one,
      refresh, then clear AI task zero. Whole-input preflight rejects
      missing mutable records or service callbacks before writes. The
      expanded [RT-E2 probe](analysis/compat/do-rototill-packed-fleet-e2.md)
      passes **17/17** strict-C89 checks for sequence, arguments, and
      failure paths. [Fresh r5 native packet](analysis/compat/do-rototill-production-e1-native-packet.md)
      is prepared for `C:\RTE1N5` and remains unrun. Full host suite:
      **565 tests**, 153 expected skips, zero failures; baseline/diff clean.
- [ ] The selected-record pointer assumption, transport/attack reroutes,
      target search, wider packed `PLORD` mutation, native return, and
      original fleet-turn comparison keep RT-E2/E3 open. Phase 2 remains
      **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Rototill packed unowned-destination marker

- [x] Bound the original owned-fleet slot-7/8 and `cord` branch to the
      live AI-planet byte-1 marker. A current unowned planet ID or the
      second packed `PLORD` order can supply the destination; malformed
      order length and out-of-range IDs are rejected before mutation.
      The expanded [RT-E2 probe](analysis/compat/do-rototill-packed-fleet-e2.md)
      passes **12/12** strict-C89 checks. [Fresh r4 native packet](analysis/compat/do-rototill-production-e1-native-packet.md)
      combines it with RT-E1 production for `C:\RTE1N4` and is unrun.
      Full host suite: **565 tests**, 153 expected skips, zero failures;
      baseline/diff clean.
- [ ] The occupied-destination route reset, transport/attack predicates,
      target search, packed order writes, native return, and original
      fleet-turn comparison keep RT-E2/E3 open. Phase 2 remains **29**
      evidence and **10** promotions (**39** total).

### Previous checkpoint — Rototill packed fleet entry and flag sweep

- [x] Bound original 124-byte fleet identity/owner/planet/cord and all
      sixteen signed 16-bit ship counts, then clear `fMark` in place for
      owned fleets before their callbacks. The [RT-E2 packed-fleet probe](analysis/compat/do-rototill-packed-fleet-e2.md)
      passes **5/5** strict-C89 checks, including 300 ships, contradictory
      typed snapshots, foreign nonmutation, and short/mismatched-record
      rejection before writes. The prior typed-fleet probe remains **3/3**.
      [Fresh r3 native packet](analysis/compat/do-rototill-production-e1-native-packet.md)
      combines production and fleet probes for `C:\RTE1N3`; it has not run.
      Full host suite: **565 tests**, 153 expected skips, zero failures;
      baseline/diff clean.
- [ ] Exact route predicates, target selection, packed `PLORD` writes,
      native return, and original fleet-turn comparison keep RT-E2/E3 open.
      Phase 2 stays **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Rototill live packed planet and AI-table source

- [x] Bound original 56-byte planet ID/owner/class/visibility,
      concentrations, and high/low mineral words directly into both
      Rototill passes. Added ID-indexed `vlpbAiPlanet` selection and
      whole-input preflight before AI-table writes. The expanded
      [RT-E1 probe](analysis/compat/do-rototill-production-e1.md) passes
      **20/20** strict-C89 checks, including an out-of-order ID and
      malformed records. [Fresh r2 native packet](analysis/compat/do-rototill-production-e1-native-packet.md)
      is prepared for `C:\RTE1N2` but has not run. Full host suite:
      **564 tests**, 153 expected skips, zero failures; baseline/diff clean.
- [ ] Live value/production services, VC1.52c return, and original
      production-turn comparison keep RT-E1/RT-P open: **29** evidence and
      **10** promotions (**39** total).

### Previous checkpoint — Rototill packed production source

- [x] Corrected the Rototill class-vs-visibility field, then implemented
      the original two-pass unowned census, packed AI-planet markers,
      four-byte PLPROD type-2/design-17 bypass, opening-turn double queue,
      later one-per-turn design-1 queue, and Init/FinishProduction order.
      The [RT-E1 host probe](analysis/compat/do-rototill-production-e1.md)
      passes **15/15**; corrected older source/integration probes pass.
      Full host suite: **564 tests**, 153 expected skips, zero failures;
      baseline/diff checks pass. [Fresh native packet](analysis/compat/do-rototill-production-e1-native-packet.md)
      is ready for C:\RTE1N1 but has not run.
- [ ] Live service/table binding, VC1.52c, and original production-turn
      comparison keep RT-E1/RT-P open: **29** evidence and **10**
      promotions (**39** total).

### Previous checkpoint — Automitron live planet-value service order

- [x] Added original-position PctPlanetOptValue and
      PctPlanetDesirability service bindings for the pre-production census,
      owned bad-planet gate, and foreign AI-table marker. Callback-supplied
      values supersede stale typed snapshots; the
      [AU-E1 planet-values probe](analysis/compat/do-automitron-planet-values-e1.md)
      passes **8/8** strict-C89 checks for value and call/record order.
      Full host suite: **562 tests**, 153 expected skips, zero failures;
      baseline/diff checks pass. [Fresh r12 packet](analysis/compat/do-automitron-e1-native-packet.md)
      is ready for C:\AUE1N12 but has not run.
- [ ] Live game-memory service binding, growth/resource and fleet/mineral
      branches, VC1.52c and original-turn evidence keep AU-E1/AU-P open:
      **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Automitron production lifecycle ordering

- [x] The production turn now runs optional InitProduction before its
      one-time PLPROD scan and FinishProduction with the actual any-item
      changed flag, including FinishProduction(0) on a packed bypass.
      Non-production planets skip both lifecycle calls. The
      [AU-E1 lifecycle probe](analysis/compat/do-automitron-production-lifecycle-e1.md)
      passes **16/16** strict-C89 checks. Full host suite: **561 tests**,
      153 expected skips, zero failures; baseline/diff checks pass.
      [Fresh r11 packet](analysis/compat/do-automitron-e1-native-packet.md)
      is ready for C:\AUE1N11 but has not run.
- [ ] Live original service binding, other mineral/planet action branches,
      VC1.52c and original-turn evidence keep AU-E1/AU-P open:
      **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Automitron packed production-queue bypass

- [x] Bound the original one-time PLPROD bypass scan directly to a bounded
      four-byte-header/four-byte-item raw allocation. Type 2 with design
      index at least 16 skips construction; later queue mutations do not
      retroactively change that decision. The
      [AU-E1 packed queue probe](analysis/compat/do-automitron-prod-queue-gate-e1.md)
      passes **13/13** strict-C89 checks. Full host suite: **560 tests**,
      153 expected skips, zero failures; baseline/diff checks pass.
      [Fresh r10 packet](analysis/compat/do-automitron-e1-native-packet.md)
      is ready for C:\AUE1N10 but has not run.
- [ ] InitProduction/FinishProduction and remaining action/service
      bindings, VC1.52c and original-turn evidence keep AU-E1/AU-P open:
      **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Automitron two resource-limited hull batches

- [x] Added the original IS/attack design-existence gates and two
      five-item queue loops with fresh per-design budget snapshots and
      per-item hull costs. A first-batch resource deficit now retains
      already-queued hulls and skips the attack batch; a first-design
      existence limit still permits the second batch. The
      [AU-E1 hull-batch probe](analysis/compat/do-automitron-hull-batches-e1.md)
      passes **18/18** strict-C89 checks. Full host suite: **559 tests**,
      153 expected skips, zero failures; baseline/diff checks pass.
      [Fresh r9 packet](analysis/compat/do-automitron-e1-native-packet.md)
      is ready for C:\AUE1N9 but has not run.
- [ ] Live original table/service binding, other action branches,
      VC1.52c and original-turn evidence remain. AU-E1/AU-P stay open:
      **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Automitron early production queue and status semantics

- [x] Corrected the four design-status calls to treat their returns as
      original status/count data, including valid zero, then bound the
      technology/AI-pressure support item and initial design-1 colonizer
      queue decisions ahead of the stochastic seed. Scanned, mineral-eligible
      unowned planets may also enter production. The strict-C89
      [AU-E1 early-queue probe](analysis/compat/do-automitron-early-queue-e1.md)
      passes **17/17**. Full host suite: **558 tests**, 153 expected skips,
      zero failures; baseline/diff checks pass. [Fresh r8 packet](analysis/compat/do-automitron-e1-native-packet.md)
      is ready for C:\AUE1N8 but has not run.
- [ ] Live service/table binding, later production branches, VC1.52c and
      original-turn evidence remain. AU-E1/AU-P stay open: **29** evidence
      and **10** promotions (**39** total).

### Previous checkpoint — Automitron first-potent support queue and census

- [x] The original two-pass planet census now completes before any
      production visit, and the first-potent owned fleet at a production
      planet can queue the exact four-ship support item and skip the later
      hull batch. Explicit caller bindings preserve the original queue
      eligibility and potency service rather than guessing them. Strict-C89
      host probes pass **5/5 census** and **15/15 support** checks. Evidence:
      [AU-E1 support/census](analysis/compat/do-automitron-support-queue-e1.md).
      Full host suite: **557 tests**, 153 expected skips, zero failures;
      baseline/diff checks pass. [Fresh r6 packet](analysis/compat/do-automitron-e1-native-packet.md)
      is ready for `C:\AUE1N6` but has not run.
- [ ] The other original production branches, live potency service, native
      and original-turn evidence keep AU-E1/AU-P open. **29** evidence and
      **10** promotions remain (**39** total).

### Previous checkpoint — Automitron live AI-planet table binding

- [x] The production turn now indexes an optional live 16-byte
      `vlpbAiPlanet` table by planet ID, not planet iteration position;
      explicit record overrides remain supported, and all supplied bindings
      are checked before record mutation. [AU-E1 table evidence](analysis/compat/do-automitron-ai-planet-table-e1.md)
      passes 8/8 strict-C89 host checks; the earlier record probe remains
      14/14. Full host suite: **555 tests**, 153 expected skips, zero failures;
      baseline and diff checks pass. [Fresh r5 packet](analysis/compat/do-automitron-e1-native-packet.md)
      is ready for `C:\AUE1N5` but has not run in Win98.
- [ ] Remaining AU-E1 mineral/action branches, native and original-turn
      evidence keep AU-E1/AU-P open; **29** evidence and **10** promotions
      remain (**39** total).

### Previous checkpoint — Automitron low-fuel scout order gate

- [x] Bound the original scout-like low-fuel branch to explicit live
      hull/fuel/transport facts. It writes first-order task 5 and refreshes
      before the planner; a strict-C89 host probe passes 15/15 for the exact
      flag nibble, exclusions, thresholds, and short-record/owner guards.
      Fresh `C:\AUE1N4` native packet supersedes r1–r3. Evidence:
      [AU-E3 scout fuel gate](analysis/compat/do-automitron-scout-fuel-e3.md).
- [ ] Other target searches, route families, VC1.52c run, and original-turn
      differential remain. AU-E3/AU-P stay open; **29** evidence and **10**
      promotions remain (**39** total).

### Previous checkpoint — Automitron direct packed route apply

- [x] The production Automitron plan/apply boundary now uses the existing
      `FMoveAiFleet` packed-order adapter when a caller supplies an exact
      18-byte MOVE/COLONIZE order and no specialized applier handles it.
      Bounded `PLORD` capacity and refresh are explicit; strict-C89 host probe
      passes 10/10 for move, append, failure, and callback precedence. Fresh
      `C:\AUE1N3` packet supersedes r1/r2. Evidence:
      [AU-E3 packed route apply](analysis/compat/do-automitron-packed-route-e3.md).
- [ ] Target searches, other route branches, complete multi-order mutation,
      VC1.52c run, and original-turn comparison remain. AU-E3/AU-P stay open;
      **29** evidence and **10** promotions remain (**39** total).

### Previous checkpoint — Automitron packed fleet-flag sweep and fresh packet

- [x] Bound the original owned `_fleet.overlap_4` bit-15 clear as a separate
      fleet sweep before later visit callbacks. The strict-C89 host probe
      passes 8/8 exact-byte, foreign-fleet, ordering, and short-record checks.
      A fresh `C:\AUE1N2` packet includes this AU-E3 probe plus the two AU-E1
      probes; the earlier packet is superseded. Evidence:
      [AU-E3 fleet flag sweep](analysis/compat/do-automitron-fleet-flags-e3.md),
      [current native packet](analysis/compat/do-automitron-e1-native-packet.md).
- [ ] Exact route predicates, broader multi-order writes, native run, and
      original AI-turn differential remain. AU-E3/AU-P stay open; **29**
      evidence and **10** promotions remain (**39** total).

### Previous checkpoint — Automitron E1 Win16 packet prepared

- [x] Packaged the two AU-E1 host probes with current production source,
      VC++ 1.52c compile/link response, DOS CRLF wrappers, separate build
      logs/status, and a Win16 result writer for fresh `C:\AUE1N1`. Package
      layout/overwrite tests pass. Evidence:
      [AU-E1 native packet](analysis/compat/do-automitron-e1-native-packet.md).
- [ ] The packet has **not** been transferred or run; native and original-turn
      evidence remain pending. AU-E1/AU-P stay open; **29** evidence and **10**
      promotions remain (**39** total).

### Previous checkpoint — Automitron AI-planet record writes recovered

- [x] Bound the original 16-byte `vlpbAiPlanet` visit, bad-owned, and
      foreign-owner/optimal-value writes to optional live records. A strict-C89
      host probe covers five planet states and the short-record guard,
      passing 14/14 checks. Evidence: [AU-E1 AI-planet writes](analysis/compat/do-automitron-ai-planet-e1.md).
- [ ] Live table pointer identity, remaining mineral/planet action graph,
      native run, and original AI-turn differential remain. AU-E1/AU-P stay
      open; **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — Automitron colonizer-seed branch recovered

- [x] Replaced one broad Automitron production callback gap with the original
      colonizer-seed decision order: first matching owned fleet at planet,
      design-flag/queue gates, `Random(3)` then the count-dependent
      `Random(8)`/`Random(2*csh6+1)` sequence, and raw
      `AddItemToQueue(6,3,2,1)` arguments. A strict-C89 host probe passes
      19 checks. Evidence: [AU-E1 colonizer-seed milestone](analysis/compat/do-automitron-colonizer-seed-e1.md).
- [ ] Live table/queue binding, other mineral/planet branches, VC1.52c native
      validation, and original-turn comparison remain. AU-E1 and AU-P stay
      open; **29** evidence and **10** promotions remain (**39** total).

### Previous checkpoint — SatisfyOrders isolated original-run input staged

- [x] Staged an overwrite-guarded five-file package with verified original
      executable, XY/HST/M2 inputs, the one-order action candidate M1, and
      SHA-256 manifest. A focused test confirms exactly one decoded task-5
      order in the staged file and refuses reuse of the package directory.
      Evidence: [SO-E3 original-run bundle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md).
- [ ] This is input preparation only. The original executable has not accepted
      or run the candidate, and no same-input production differential exists.
      SO-E3/SO-P stay open; **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — SatisfyOrders action-bearing original candidate prepared

- [x] Added a bounded game-stream encoder and verified byte-exact round trips
      for the original clean turn-0 input and two-BOF output. Generated a
      separate task-5 candidate M1; plaintext comparison finds exactly one
      changed order record, while the pinned original input remains intact.
      Evidence: [SO-E3 candidate and encoder checks](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md).
- [ ] The candidate has **not** been accepted or run by the original game;
      task-specific union validity and all resulting behavior remain unknown.
      The Win98 agent has not connected to the host listener. SO-E3/SO-P stay
      open, with **29** evidence and **10** promotions (**39** total).

### Previous checkpoint — SatisfyOrders scoped clean-state production replay

- [x] Replayed the six decoded clean-turn fleet/order fields through strict-C89
      production `stars_do_orders` and `stars_satisfy_orders`, covering passes
      1–4. The scoped host comparison confirms unchanged fleet IDs, owners,
      planet IDs, fuel, and active task-0 orders against the pinned original
      turn-1 records; no order retirement or order message occurs. Evidence:
      [SO-E3 original oracle and production comparison](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md).
- [ ] The clean fixture has only task-0 orders and does not test transfer,
      mining, routing, or retirement. A verified action-bearing original turn
      and same-input production state/message differential remain. SO-E3/SO-P
      stay open; **29** evidence and **10** promotion items remain (**39** total).

### Previous checkpoint — SatisfyOrders original planet prefix decoded

- [x] Added a bounded type-13 planet-prefix decoder to the three-run
      original-turn oracle. Planet ID 10, owner 0, flags `0x2B87`,
      concentrations `68/87/68`, and environment `50/50/50` are stable from
      turn 0 to turn 1; mineral-level selector `0x00→0x15` exposes
      `0/0/0→247/241/247`. The variable tail remains opaque because two
      bytes vary across original replays. The focused decoder tests and full
      host suite pass (**544 tests, 153 expected skips, zero failures**).
      Evidence: [SO-E3 original oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md).
- [ ] This is an original-output turn-stage observation, not attribution to
      `SatisfyOrders` alone. Complete message arguments and the same-input
      production full-turn comparison still remain. SO-E3/SO-P stay open;
      **29** evidence and **10** promotion items remain (**39** total).

### Previous checkpoint — SatisfyOrders original fleet/order state decoded

- [x] Decoded the stable original turn's compact type-16 fleet and type-20
      order records using the checked-in `_fleet`/`_order` CodeView layout and
      `WriteFleet`/`WriteOrders` widths. Across all three original replays,
      owner 0's six fleets retain planet 10, point `(1128,1300)`, one ship in
      each respective design, fifth cargo/fuel `50/300/200/450/530/210`,
      and one valid compact order targeting planet 10. Comparing turn 0 to
      turn 1 shows only the saved `fHereAllTurn` flag `0x0907→0x2907` in
      these decoded fleet/order fields. The scoped decoder rejects truncated
      and wrong-variant records; the full host suite passes **544 tests, 153
      expected skips, zero failures**. Evidence:
      [SO-E3 original oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md).
- [ ] Planet ownership/cargo effects, complete message arguments, and the
      same-input production full-turn differential remain. SO-E3/SO-P stay
      open, with **29** evidence and **10** promotion items (**39** total).

### Previous checkpoint — SatisfyOrders original-turn oracle pinned

- [x] Pinned the original `CLEAN.M1` turn-1 message plus six interleaved
      fleet/order record pairs as an exact 13-record plaintext oracle. A new
      test checks the file hashes and decrypted record sequence across the
      isolated `SYORIG1` output and two earlier original batch replays. The
      focused test passes; the full host suite now passes **543 tests, 153
      expected skips, zero failures**. Evidence:
      [SO-E3 original oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md)
      and [machine-readable subset](analysis/compat/satisfy-orders-original-m1-stable-e3.json).
- [ ] This is original-versus-original stability, not a production full-turn
      differential or a logical planet/cargo/ownership/message-argument
      decoder. SO-E3 and SO-P remain open. Phase 2 still has **29** evidence
      and **10** promotion items (**39** total).

### Previous checkpoint — ExecuteReportClick Win16 MulDiv fidelity

- [x] The original resource branch calls Win16 GDI `MulDiv`, whereas the
      reconstruction had used truncating C multiplication/division that can
      overflow under 16-bit `int`. Production source now uses a 32-bit
      intermediate, nearest-integer half rounding, and signed-16 saturation.
      Six report-to-click host cases cover half rounding, large product,
      saturation, exemption, and negative half behavior: strict-C89 chain
      **33/33**. The full host suite passes **542 tests, 153 expected skips,
      zero failures**. Evidence:
      [ER-E3 MulDiv milestone](analysis/compat/execute-report-click-muldiv-e3.md).
- [ ] The fresh `C:\\ERUSER4` packet includes six real-Win16 report-message
      arithmetic checks in addition to the original four USER/GDI cases, but
      has not been transferred or run; `C:\\ERUSER3` is superseded. The
      original-executable resource-exempt result and complete report scroll
      layout also remain. ER-E2/E3 stay open: **29** evidence and **10**
      promotion items remain (**39** total).

### Previous checkpoint — native report-message retry prepared

- [x] The host STARSAGT protocol self-test passes. The already transferred
      `C:\\ERUSER2` wrapper is DOS CRLF and points at the correct guest
      directory; its native outcome is still unknown. Prepared a fresh
      `C:\\ERUSER3` packet from the current expanded `ReportDlg` →
      `ExecuteReportClick` → popup Win16 harness, with separate compile/link
      status and stale-result refusal. Package source matches the current
      harness and its text files are ASCII CRLF. Evidence:
      [ER-E2 native handoff](analysis/compat/execute-report-click-live-user-e2.md).
- [ ] The agent is not connected, so no new native result is claimed. On
      reconnection, collect `ERUSER2` status/logs first and only then decide
      whether to build the fresh `ERUSER3` packet. ER-E2/E3 remain open;
      **29** evidence and **10** promotion items remain (**39** total).

### Previous checkpoint — ER-E3 original Starbase/Defense overlays

- [x] Routed the retained original Starbase `(190,26)` and Defense `(171,26)`
      clicks through production `ReportDlg` → `ExecuteReportClick` and
      compared the resulting full 22-byte `GlobalPD` overlays with the live
      original captures. The Starbase pointer is `19E7:0012`; Defense carries
      the original eight-byte part. Strict-C89 chain **27/27**, full host
      suite **542 tests, 153 expected skips, zero failures**. Evidence:
      [ER-E3 report-chain milestone](analysis/compat/execute-report-click-report-chain-e3.md).
- [ ] These two report field setups are synthetic and do not prove the
      original scroll state; resource-exempt/modal original outcomes and the
      expanded native `C:\\ERUSER2` return also remain. ER-E3 is still open:
      **29** evidence and **10** promotion items remain (**39** total).

### Previous checkpoint — ER-E3 simultaneous Mine/Factory layout

- [x] The retained original Planet Summary Report screenshot shows Mine and
      Fact adjacent in a single horizontally scrolled view. A coherent
      eight-visible-field host replay, with widths inferred from its pixel
      spans, now dispatches the original `(160,26)` and `(205,26)` clicks
      through production `ReportDlg` → `ExecuteReportClick` to columns 6 and
      7 and their installation popups. The focused strict-C89 chain passes
      **21/21**; the full host suite remains **542 tests, 153 expected skips,
      zero failures**. Evidence:
      [ER-E3 report-chain milestone](analysis/compat/execute-report-click-report-chain-e3.md).
- [ ] The inferred widths are not an original in-memory width capture;
      Planet/Resources still use isolated field setups, and the native
      `C:\\ERUSER2` report-message build remains unverified. ER-E3 stays open:
      **29** evidence and **10** promotion items remain (**39** total).

### Previous checkpoint — full host-suite gate restored

- [x] Corrected the `WriteMemRt` host-test fixture to provide backing storage
      through its intentional cursor-31,999 overflow case and assert the
      resulting header, payload, and final cursor. The source implementation
      is unchanged. The focused test passes, and the full host suite now
      completes: **542 tests, 153 expected skips, zero failures**.
- [ ] This fixes a test-only out-of-bounds crash, not a named Step-5 acceptance
      boundary. A fresh host listener has not yet received a Win98 agent
      reconnection, so the expanded `C:\\ERUSER2` native build remains
      unverified. **29** evidence and **10** promotion items remain (**39**
      total).

### Previous checkpoint — validation and guest-connection boundary

- [x] Rechecked the focused production report chain test, the baseline
      coverage inventory, and whitespace validation: all pass. The prior
      fresh Win16 popup lifecycle result remains **54/54**.
- [ ] Two broad host-test attempts aborted in the existing
      `stars_write_mem_rt` native-library call (`SIGBUS` per the macOS crash
      report); neither is a suite pass. The expanded `C:\\ERUSER2` native
      build has no returned status or logs: the Win98 guest remains running,
      but STARSAGT disconnected during its process-status request and has not
      reconnected. Do not infer a build failure or success or restart the VM
      from this observation. **29** evidence and **10** promotion items remain
      (**39** total).

### Previous checkpoint — ExecuteReportClick ER-E3 report hit-test chain

- [x] Connected production `ReportDlg` mouse hit-testing to production
      `ExecuteReportClick` on the host. Original planet, resource, mine, and
      factory click coordinates reach their expected columns/popup kinds;
      resource-exempt, modal suppression/beep, header, out-of-range,
      selection/invalidation, and DC symmetry branches pass strict-C89
      **18/18**.
- [ ] This uses a one-visible-field layout per case and has no new native
      return, so ER-E3 remains open. The expanded real-Win16 report-message
      build is not yet verified because the guest agent stopped answering
      status requests; the existing 54/54 popup lifecycle run remains valid.
      No named unit closes: **29** evidence and **10** promotion items remain
      (**39** total). Evidence:
      [report chain host milestone](analysis/compat/execute-report-click-report-chain-e3.md).

### Earlier checkpoint — ExecuteReportClick ER-E2 real Win16 lifecycle

- [x] Fresh VC1.52c/Win98 `C:\ERUSER1` compiled the current production
      `ExecuteReportClick`, `Popup`, `DrawPopup`, and `PopupWndProc` sources
      into a real USER/GDI harness. Four report clicks created actual popup
      windows matching the original rectangles, class, parent, effective
      style, paint, capture, and teardown; native **54/54** passed with no
      Program Error.
- [ ] The first live harness drives `ExecuteReportClick` directly. ER-E2
      still needs native production `ReportDlg` mouse dispatch/refresh and
      original comparison before closure. No named unit closes: Phase 2
      remains **123/152** evidence units closed, **29** evidence units plus
      **10** promotions open (**39** counted items). Evidence:
      [real USER/GDI lifecycle](analysis/compat/execute-report-click-live-user-e2.md);
      returns: `runs/vmctl-eruser-r1/`.

### Previous checkpoint — ExecuteReportClick ER-E2 original popup geometry

- [x] Captured live Win98 `starspopup` rectangles, parent, class, effective
      style, focus, and mouse capture for original planet, resource, mine,
      factory, and starbase report clicks. These show bottom-right click
      anchoring; the old reconstructed top-left rule was masked by clamping
      in earlier probes.
- [x] Corrected production `Popup` to subtract measured width/height before
      clamping and pass the full original 32-bit CreateWindow style argument.
      Strict-C89 host replays pass 31/31, 22/22, 148/148, and the expanded
      production `ExecuteReportClick -> Popup -> DrawPopup` four-click
      differential passes 86/86. Fresh VC1.52c/Win98 `C:\ERUI2` passes
      86/86; `C:\PUIE4` passes the corrected Popup E2 matrix 148/148.
      Compile/link reports, maps, binaries, original captures, and no-error
      window inventory are retained.
- [ ] **ER-E2 remains open:** the reconstructed callback has not yet
      created and painted a real Win16 popup. Actual USER/GDI lifecycle,
      report selection/refresh, and teardown/capture release still need a
      live native differential. ER-E3 caller/layout acceptance also remains.
      Phase 2 stays **123/152** evidence units closed, **29** evidence units
      and **10** promotions open (**39** counted items). Evidence:
      [live UI geometry milestone](analysis/compat/execute-report-click-live-ui-geometry-e2.md);
      returns: `runs/vmctl-gp-ui-r2/`.

### Previous checkpoint — ExecuteReportClick ER-E1 live original union closure

- [x] Captured eight full 22-byte `GlobalPD` payloads directly from the
      isolated original Win98 game: starbase, planet, resources, defense,
      fleet, mineral, mine, and factory. The starbase far pointer agrees
      with the independently read original ship-definition base/design
      index. Planet and installation clicks reveal persistent union bytes.
- [x] Added optional caller-owned 22-byte union persistence to production
      `ExecuteReportClick`; exact host and fresh VC++ 1.52c/Win98 `C:\GPDI7`
      differential passes **49/49**, with original binary/text returns,
      compile/link reports, map/object/executable, and no Program Error.
      Full host suite: **541 tests, 153 expected skips**.
- [x] **ER-E1 closed** at 2026-09-23 02:40:30 MDT. Phase 2 now has
      **123/152** evidence units closed, **29** evidence units and **10**
      promotions open (**39** counted items). E2 live USER/GDI and E3 full
      report/click/popup caller/layout remain. Evidence:
      [original GlobalPD differential](analysis/compat/execute-report-click-original-globalpd-e1.md);
      returns: `runs/vmctl-gp-live-preflight/`.

### Previous checkpoint — ExecuteReportClick E1 packed pointer tables

- [x] Bound optional packed `vlprgidFleet` → `rglpfl` far-pointer lookup and
      per-player ship-definition far-pointer table in the production
      dispatcher. The starbase pointer uses the recovered planet design
      nibble and 0x93-byte stride with 16-bit offset wrap. Invalid table
      indices suppress the popup rather than consuming unrelated values.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\GPDP4` pass 30/30
      independently counted cases, including fallback and bounds. Build,
      process, and report pass without Program Error. The full host suite
      passes **540 tests, 153 expected skips**.
- [ ] ER-E1 still needs live original fleet/starbase far-pointer and
      defense-part capture/differential across click kinds. This is
      instruction-derived table binding, not a named closure: Phase 2
      remains **122/152** evidence units closed, **30** evidence units plus
      **10** promotions open (**40** practical items). Evidence:
      `analysis/compat/execute-report-click-pointer-tables-e1.{md,json}`;
      returns: `runs/vmctl-gpdp4/`.

### Previous checkpoint — ExecuteReportClick E1 resource union correction

- [x] Reconciled original `GlobalPD` resource-cell writes and the
      `PtDisplayResourceInfo` consumer. The first union word now preserves
      the selected planet ID, the second is the resource/part alias, and the
      third is the research-adjusted value; an unrelated caller word no
      longer overwrites the planet ID.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\GPDE3` pass 20/20 across
      fleet, starbase, defense, and corrected resource popup payloads.
      Build, process, and report pass; no Program Error appears. The full
      host suite passes **540 tests, 153 expected skips**.
- [ ] ER-E1 still needs exact live fleet/starbase far-pointer and defense
      part values across clicks; ER-E2/ER-E3 and promotion remain. No named
      unit closes: Phase 2 remains **122/152** evidence units closed,
      **30** evidence units plus **10** promotions open (**40** practical
      items). Evidence:
      `analysis/compat/execute-report-click-resource-union-e1.{md,json}`;
      returns: `runs/vmctl-gpde3/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 desirability production gate

- [x] Corrected the original owned-planet branch: negative
      `PctPlanetDesirability` now sets the packed AI bad-world flag and skips
      `ChangeMainObjSel`/`InitProduction` and `FinishProduction`, even if
      the typed production slot is open. The result is carried through the
      second pass without calling desirability twice.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURDE1` pass 27/27,
      including a negative-desirability/open-slot regression. Build,
      process, and report pass with no Program Error. The full host suite
      passes **539 tests, 153 expected skips**.
- [ ] Turin E1 still needs live table enumeration, remaining production
      and service effects, and original-turn comparison. No named closure:
      **122/152** evidence units closed, **30** evidence units plus **10**
      promotions open (**40** practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-desirability-e1.{md,json}`;
      returns: `runs/vmctl-turde1/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 packed AI-planet writes

- [x] Bound the recovered 16-byte-per-ID `vlpbAiPlanet` table optionally in
      production source. The second pass now writes universal byte `+9`,
      visible-unowned capped mineral score at `+1`, and negative-owned
      desirability flag at `+2`, with all IDs/extents checked before mutation.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURAI1` probes pass 26/26,
      including truncated table and negative-ID rejection without writes.
      Build, process, and report pass; no Program Error appears. The full
      host suite passes **539 tests, 153 expected skips**.
- [ ] Turin E1 still needs live table enumeration, other AI/production
      writes, growth/resource and queue/finalization service effects, and
      original-turn comparison. This is a supporting subcheck, not a named
      closure: **122/152** evidence units closed, **30** evidence units plus
      **10** promotions open (**40** practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-ai-planet-e1.{md,json}`;
      returns: `runs/vmctl-turai1/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 packed-service bridge

- [x] Added production-source decoders for recovered 56-byte `_planet`,
      124-byte `_fleet`, and variable-length `PLPROD` queue records. The
      queue scanner uses the original category-2/design-below-16 test and
      rejects truncated records. A default support-fleet decision now
      implements original `FPotentRobWarFleet(fleet,2)` strength from slots
      2–7 when no external callback is supplied.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURPK1` probes pass 17/17
      over packed field widths, count 256, queue classification, and an
      end-to-end support queue/finalization from decoded planet/fleet data.
      Compile/link and process exit pass with no Program Error. Full host
      suite: **539 tests, 153 expected skips**, **2,241** layout assertions,
      **846** declarations.
- [ ] Turin E1 still needs live VM AI/design/production table enumeration,
      growth/resource and queue/finalization service effects, remaining
      design inputs, and original-turn comparison. No named unit closes;
      Phase 2 remains **122/152** evidence units closed, **30** evidence
      units plus **10** promotions open (**40** practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-packed-service-e1.{md,json}`;
      returns: `runs/vmctl-turpk1/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 support-fleet production

- [x] Mapped original Turin raw lines 328–352: the first same-owner,
      same-planet fleet accepted by `FPotentRobWarFleet(fleet,2)` is
      decisive. If its slot-13+14 count reaches the support-potency
      threshold, the source queues `(selected,4,2,1)` and skips the ordinary
      five-hull loops before `FinishProduction(changed)`; a weak first
      potent fleet stops the search even if a later fleet is stronger.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURSP1` probes pass 23/23
      across threshold, enumeration order, owner/planet filtering, early
      finalization, unavailable service, and queue rejection. Compile/link
      and process exit pass with no Program Error. Full host suite:
      **538 tests, 153 expected skips**, **2,241** layout assertions,
      **846** declarations.
- [ ] Turin E1 still needs real `FPotentRobWarFleet` binding, live AI/
      design/production tables, preceding growth/resource service effects,
      actual queue/finalization side effects, and original-turn comparison.
      No named unit closes; Phase 2 remains **122/152** evidence units
      closed, **30** evidence units plus **10** promotions open (**40**
      practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-support-e1.{md,json}`;
      returns: `runs/vmctl-tursp1/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 design-1/minelayer production

- [x] Restored the original first pass over all visible unowned planets
      before owned-planet production. The design-1 branch now issues four
      `(1,1,2,1)` requests when the complete viable-world count is nonzero
      and the 32-bit existing count is below two.
- [x] Added a full four-argument queue callback and retained signed 16-bit
      fleet ship counts. The slot-12 minelayer branch now consumes
      `Random(3)`, optional `Random(8)`, then `Random(2*cFr+1)` in the raw
      order, selecting the first qualifying local fleet and issuing
      `(12,3,2,1)` only on the final zero. Strict-C89 and fresh VC++ 1.52c/
      Win98 `C:\TURMI1` pass 32/32; compile/link and process exit pass, with
      no Program Error. Full host suite: **537 tests, 153 expected skips**,
      **2,241** layout assertions, **846** declarations.
- [ ] Turin E1 still needs the remaining original service effects before
      this branch, support-fleet selection and early finalization, live
      packed AI/design/production table binding, actual queue/finalization
      side effects, and original-turn differential. No named unit closes;
      Phase 2 remains **122/152** closed, **30** evidence units plus **10**
      promotions open (**40** practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-minelayer-e1.{md,json}`;
      returns: `runs/vmctl-turmi1/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 group-8..9 technology queue

- [x] Reconciled original `CheckAiShdefStatus`'s saturated existing-count
      return with Turin raw lines 139–140 and 280–294. The production source
      now uses the greater of twice the AI-data scale and one-tenth the
      player's planet count, technology byte 2, the selected 8–9 design,
      and the conditional `Random(4)` before later production loops.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURTQ2` probes pass 21/21
      across direct and random-gated queueing, threshold bypasses, selected-
      design and technology gates, and missing/rejected service callbacks.
      Compile/link and process exit pass; no Program Error window. One full
      host-suite run passes **536 tests, 153 expected skips**, **2,241**
      layout assertions, **846** declarations; two other broad-suite
      attempts exited 138 early, cause unresolved.
- [ ] Turin E1 still needs design-1 four-item queueing, minelayer RNG/fleet
      scan, support-fleet choices, the full four-argument queue request,
      live packed bindings, production-service side effects, and original-
      turn differential. No named unit closes; Phase 2 remains **122/152**
      closed, **30** evidence units plus **10** promotions open (**40**
      practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-tech-queue-e1.{md,json}`;
      returns: `runs/vmctl-turtq2/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 seed/finalization milestone

- [x] Reconciled original Turin production raw lines 230–278 and 490:
      existing ship queue bypasses new items and calls `FinishProduction(0)`;
      turn zero seeds design 0 per item using the 191 threshold and 100/30
      decrements; later turns apply the base-design inactive/hull/count/built
      gate; constructed queues call `FinishProduction(changed)`.
- [x] Added typed queue-presence and base-design inputs, the turn-zero seed,
      and a finalization callback. Strict-C89 and fresh VC++ 1.52c/Win98
      `C:\TURSF1` probes pass 21/21 across queue order, bypass, count/built
      exclusions, accepted-prefix failure, and finish rejection. Compile/link
      and process exit pass; no Program Error window. Full host suite:
      **535 tests, 153 expected skips**, **2,241** layout assertions,
      **846** declarations.
- [ ] E1 still needs technology/design-count and RNG-gated queue choices,
      support-fleet selection, live packed production/AI/design table binding,
      actual production-service side effects, and original-turn differential.
      Read-only next-branch audit: raw lines 280–294 consume `Random(4)`
      conditionally before the slot-12 minelayer branch at 308–327, which
      consumes `Random(3)`, optional `Random(8)`, and `Random(2*cFr+1)`;
      the current queue callback loses the latter branch's `(12,3,2,1)`
      argument tuple. Reconcile the scalar `CheckAiShdefStatus(8,9,...)`
      return and widen typed fleet ship counts before that implementation.
      No named unit closes; Phase 2 remains **122/152** closed, **30** evidence
      units plus **10** promotions open (**40** practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-seed-finish-e1.{md,json}`;
      returns: `runs/vmctl-tursf1/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 four-role design queue milestone

- [x] Mapped the original status outputs and production loops: selected
      design roles **3, 0, 7, 5** correspond to groups 4–5, 6–7, 10–11,
      and 15. The previous source used role 0 for the first loop; it now
      uses role 3 and accepts typed plan limit, design counts, four-channel
      hull costs, and the design-15 gate for all four loops.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURDQ1` matrix pass 31/31
      across loop order, exact caps, design-15 exclusion, partial stop, and
      resource carry. Compile/link and process exit pass; no Program Error.
      Turin packed integration and edge probes pass 20/20 and 7/7. Full host
      suite: **534 tests, 153 expected skips**, **2,241** layout assertions,
      **846** declarations.
- [ ] E1 still needs turn-zero queue seeding, technology/design-count and
      RNG-gated choices, support-fleet checks, `FinishProduction` service
      behavior, live packed-table binding, and original-turn differential.
      No named unit closes; Phase 2 remains **122/152** closed, **30** evidence
      units plus **10** promotions open (**40** practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-design-queue-e1.{md,json}`;
      returns: `runs/vmctl-turdq1/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 per-item production milestone

- [x] Corrected the typed production path against original Turin raw lines
      353–385: queue cost is subtracted across four channels, each affordable
      hull is debited and queued individually, and a later shortage retains
      previously queued items. The former all-or-nothing quantity-five
      callback was not faithful to this loop.
- [x] Strict-C89 production matrix passes 11/11 across five items, three-
      and four-item prefixes, initial resource rejection, and callback
      rejection after two accepted items. Existing packed integration and
      edge probes pass 20/20 and 7/7. Fresh VC++ 1.52c/Win98 `C:\TURPE1`
      compiles, links, exits 0, and reports 11/11 with no Program Error.
      Full host suite: **533 tests, 153 expected skips**, **2,241** layout
      assertions, **846** declarations.
- [ ] Turin E1 remains open: other selected-design loops, population/
      design-count gates, RNG-gated items, and `FinishProduction` interaction
      are not yet represented. No named unit closes; Phase 2 remains
      **122/152** closed, **30** evidence units plus **10** promotions open
      (**40** practical items). Evidence:
      `analysis/compat/do-turin-drone-ai-turn-partial-production-e1.{md,json}`;
      returns: `runs/vmctl-turpe1/`.

### Previous checkpoint — DoTurinDroneAiTurn E1 repair subcheck

- [x] Reconciled original `1088:5811–588A`: the slot-12/`cord == 1` branch
      reads the packed first kind and writes only kind 6 plus two `0x0005`
      words. Turin source now treats packed PLORD as authoritative when
      present and repairs an eligible owned fleet even without a typed visitor.
- [x] Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURRE1` matrices pass 18/18
      over exact three-order preservation, stale typed-kind values, bypasses,
      and callback rejection. Compile/link and process exit pass; no Program
      Error appears. Full suite: **532 tests, 153 expected skips**, **2,241**
      layout assertions, **846** declarations.
- [ ] Turin E1 remains open: the original production phase has additional
      design selection, per-item queue writes, RNG gates, and population/
      resource thresholds beyond the current batch callback. E2 routing, E3
      broader PLORD mutation, E4 original-turn comparison, and promotion also
      remain. No named unit closes; Phase 2 stays **122/152** closed, **30**
      evidence units plus **10** promotions open (**40** practical items).
      Evidence: `analysis/compat/do-turin-drone-ai-turn-multi-order-repair-e1.{md,json}`;
      returns: `runs/vmctl-turre1/`.

### Previous checkpoint — DoAutomitronAiTurn E2 multi-order repair closure

- [x] Original Automitron instructions at `1098:1225–129E` gate on ship slot
      6, `cord == 1`, and empty first-order kind, then write only kind 6 and
      two `0x0005` words. No header count or later order is changed.
- [x] The repair was moved out of the typed-visitor-only branch to match the
      original owned-fleet loop. The expanded strict-C89 matrix passes 22/22,
      including exact three-order PLORD preservation, ineligible bypasses,
      callback order/failure, and typed/legacy/no-visitor execution. Fresh
      VC++ 1.52c/Win98 `C:\AUTRE3` compiles, links, runs, and reports 22/22;
      process exits 0 with no Program Error. The prior 17/17 return is retained
      in `runs/vmctl-autre2/`. Full host
      suite: **531 tests, 153 expected skips**, **2,241** layout assertions,
      **846** declarations.
- [x] This closes Automitron Step-5 **E2** only. It is **1/4 named evidence
      units closed and 5/6 overall**; E1 colonizer/mineral, E3 route/broader
      packed orders, E4 original behavior, and promotion remain. Phase 2 is
      **122/152** closed, **30** evidence units plus **10** promotions open
      (**40** practical items). Evidence:
      `analysis/compat/do-automitron-ai-turn-multi-order-repair-e2.{md,json}`;
      returns: `runs/vmctl-autre3/`.

### Previous checkpoint — DoRobotoidAiTurn E1 multi-order repair closure

- [x] Corrected the shared first-order repair to preserve PLORD header
      counts and every queued order after the first. Original Robotoid
      instructions at `1088:130D–136F` write only the first order's kind and
      two repair words; the analogous Cyber, Automitron, Turin and Macinti
      branches likewise have no `iordMac` write. The old helper incorrectly
      forced the count to one.
- [x] New strict-C89 production-source Robotoid matrix passes 14/14 across
      three-order preservation, exact whole-buffer mutation, route bypass,
      ineligible `cord`, foreign ownership, and callback order. The shared
      helper and existing five-personality repair/route probes also pass.
      Fresh VC++ 1.52c/Win98 `C:\ROBRE1` compiles, links and runs the current
      source; native report passes 14/14, process exits 0, and no Program
      Error window appears. Full host suite: **530 tests, 153 expected skips**,
      **2,241** layout assertions, **846** declarations.
- [x] This closes Robotoid Step-5 **E1**. It is now **1/4 named evidence
      units closed and 5/6 overall**; E2 exact routing, E3 broader PLORD
      mutation, E4 AI-table/original-turn differential, and promotion remain.
      Phase 2 burn-down is **121/152** units closed, **31** evidence units and
      **10** promotions open (**41** practical items). Evidence:
      `analysis/compat/do-robotoid-ai-turn-multi-order-repair-e1.{md,json}`;
      returns: `runs/vmctl-robre1/`.

### Previous checkpoint — SatisfyOrders E3 decoded original-turn stability

- [x] Added `tools/stars_game_stream.py`, a bounded record-framing and
      plaintext diagnostic using the retained reconstructed file-XOR/seed
      routines. It distinguishes BOF salt differences from decrypted game
      records instead of comparing ciphertext or file lengths alone. Its three
      framing/append/error unit tests pass.
- [x] Compared the fresh isolated `C:\SYORIG1` output with both earlier
      original batch replays. All 63 M1 record kinds/lengths agree; only the
      second BOF and one planet record differ in plaintext. The new message
      record (ID 63, no production queue), all six fleet records, and all six
      order records match both prior replays exactly. HST and M2 have other
      unnormalized planet/ship-record differences.
- [ ] This is original-versus-original stability evidence, not a
      production-source differential or broad special-order matrix. The
      SatisfyOrders E3 named unit and promotion remain open; counters stay
      **120/152** evidence units closed, **32** open, **10** promotions open,
      **42** practical items. Evidence:
      `analysis/compat/satisfy-orders-full-turn-e3-original-oracle.{md,json}`.

### Previous checkpoint — SatisfyOrders E3 isolated original batch capture

- [x] Recovered the host controller from a reproducible `LIST` error hang:
      `ERR LIST open` now ends the response loop, and the local protocol test
      covers the failure reply. The guest agent and VM were left running.
- [x] Copied the pinned original executable and verified clean 2400 snapshot
      into isolated Win98 directory `C:\SYORIG1`, then ran one original batch
      turn. The process reported `EXIT 1`, but its HST/M1/M2 outputs and backup
      files were retrieved to `runs/vmctl-syorig1/`. Backup hashes match the
      input snapshot; HST and M1 BOFs advance to turn 1, and record framing,
      sizes, and record-kind sequences match an earlier original batch replay;
      HST/M2 have three variable type-26 record lengths, while M1 lengths
      match.
- [ ] This is an original output *capture*, not yet the E3 source differential:
      decoded fleet/planet/message state and a production-source replay of the
      same clean input remain necessary. SatisfyOrders stays **2/3 evidence
      units and 5/6 overall**; Phase 2 counters remain **120/152** closed,
      **32** evidence units and **10** promotions open (**42** practical items).
      Details: `analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md`.

### Previous checkpoint — Cyber AI multi-order packed PLORD mutation E12

- [x] Expanded the raw `FMoveAiFleet` (`19:3D0A`) packed-order cases to
      multi-record queues: append a third route, replace the last same-endpoint
      route, truncate a non-append first-endpoint replacement, non-append at a
      new endpoint, and helper-boundary full-capacity rejection (the original
      `LpplReAlloc` allocator remains outside this subprobe). Strict C89 passes
      15/15.
- [x] Fresh E12 VC++ 1.52c/Win98 package compiles and links; the native report
      passes 15/15, process exits 0, and no Program Error appears. Returns are
      in `runs/vmctl-ai-plord-multi-e12/`; package:
      `compat/vc152/ai-plord-multi-order-e12/`.
- [x] Full host suite after E12 passes: **526 tests**, **153 expected skips**,
      **2,241 layout assertions**, and **846 declarations**.
- [ ] This supports Cyber AI Step-5 E3 but closes **0 named units**. The
      remaining E3 personality predicates and E4 complete AI-table/original-
      turn evidence remain open. Cyber remains 2/4; Phase 2 counters stay
      120/152 evidence units closed, 32 open, 10 promotions open, and 42
      practical open items. Evidence:
      `analysis/compat/ai-plord-multi-order-e12.{md,json}`.

### Previous checkpoint — FAttack complex-beam live hull damage E60

- [x] Extended the raw complex-beam live-target case through both damage
      layers: a 408-point beam absorbed 6 shield damage, killed two ships at
      10 hull DP each, reduced the live ship count to zero, consumed the
      residual beam, and recorded the hit/first target. Strict C89 passes
      15/15; related scaling, packed-part, and torpedo probes pass 86/86,
      24/24, and 17/17.
- [x] Fresh E60 VC++ 1.52c/Win98 compile/link pass; all three guest processes
      exit 0. Native reports pass packed part 24/24, raw torpedo dispatch
      17/17, and raw damage/hull casualty 15/15. The original segment matches
      and no Program Error appears. Returns are retained in
      `runs/vmctl-fapraw-e60/`; package:
      `compat/vc152/f-attack-raw-residual-e60/`.
- [x] Full host suite after E60 passes: **525 tests**, **153 expected skips**,
      **2,241 layout assertions**, and **846 declarations**.
- [ ] This remains support only: **0 named Step-5 units close**. FAttack is
      still 0/3 and 5/6 overall; complete weapon/hull/target service
      acceptance, broader original damage differential, complete battle
      replay, and promotion remain. Counters stay 120/152, 32 evidence units
      open, 10 promotions open, and 42 practical open items. Evidence:
      `analysis/compat/f-attack-complex-beam-hull-e60.{md,json}`.

### Previous checkpoint — FAttack large-intermediate shield-ratio E59

- [x] E59 stresses the exact ratio calculation with `dpMain=408,000`, so the
      numerator products exceed 32 bits. The probe observes pass values
      163,200→163,194 and 163,193→163,187 and confirms the second-pass pool
      scaling; strict C89 reports pass 14/14.
- [x] Fresh E59 VC++ 1.52c/Win98 compile/link pass; all three processes exit
      0, with packed part 24/24, raw torpedo dispatch 17/17, raw damage/large
      ratio 14/14, matching segment bytes, and no Program Error. Returns are
      retained in `runs/vmctl-fapraw-e59/`; package:
      `compat/vc152/f-attack-raw-residual-e59/`.
- [x] Full host suite after E59 passes: **525 tests**, **153 expected skips**,
      **2,241 layout assertions**, and **846 declarations**. The E59 JSON,
      package source parity, Python syntax, and `git diff --check` pass.
- [ ] This remains support only: **0 named Step-5 units close**. FAttack is
      still 0/3 and 5/6 overall; broad original damage differential, complete
      battle replay, and promotion remain. Counters stay 120/152, 32 evidence
      units open, 10 promotions open, 42 practical open items. Evidence:
      `analysis/compat/f-attack-complex-beam-residual-e59.{md,json}`.

### Previous checkpoint — FAttack complex-beam shield-ratio oracle E58

- [x] E58 adds a shield-ratio oracle to the live two-target test using original
      item 21 (`dp=204`): pool 408, first damage 163→157, bounded residual
      392, second damage 156→150. Target order is mode 1 then mode 0. Strict
      C89 raw-damage reports pass 13/13.
- [x] Fresh E58 VC++ 1.52c/Win98 build/link pass; packed part 24/24, raw
      torpedo dispatch 17/17, and raw damage/rescan/shield ratio 13/13. All
      three processes exit 0, the original segment matches, and the guest
      window inventory has no Program Error. Returns are in
      `runs/vmctl-fapraw-e58/`; package is
      `compat/vc152/f-attack-raw-residual-e58/`.
- [ ] This improves supporting evidence only and closes **0 named Step-5
      units**. FAttack remains 0/3 and 5/6 overall; broader original damage
      differentials, full battle replay, and promotion remain. TODO6 counters
      stay 120/152, 32 evidence units open, 10 promotions open, 42 practical
      open items. Evidence:
      `analysis/compat/f-attack-complex-beam-residual-e58.{md,json}`.
- [x] Full host suite after E58 passes: **525 tests**, **153 expected skips**,
      **2,241 layout assertions**, and **846 declarations**; JSON, package
      source parity, Python syntax, and `git diff --check` pass.

### Previous checkpoint — FAttack raw complex-beam residual rescan E55

- [x] Added optional live-target reread and explicit attacker ship-count
      inputs; the raw complex-beam path now seeds `dpMain` from the resolved
      part damage, installed count, and attacker count, rescans mode 1 before
      mode 0, and updates the residual pool from the callback-mutated damage.
- [x] Added the strict-C89 `F_ATTACK_RAW_DAMAGE_CHECKS=12` regression and
      automated runner. A two-target shield-only sequence exhausts the mode-1
      target, verifies live-state rereading, falls back to mode 0, exhausts
      that target, then terminates.
- [x] Fresh E57 VC++ 1.52c/Win98 package compiles/links all three FAttack
      probes; packed-part passes 24/24, raw torpedo dispatch 17/17, and raw
      damage/rescan/mode-0 fallback 12/12. Processes exit 0, the returned
      segment matches the package, and the guest window inventory has no
      Program Error. Logs, reports, maps, objects, executables, screenshot,
      segment, and agent log are retained in `runs/vmctl-fapraw-e57/`; package
      sources are in `compat/vc152/f-attack-raw-residual-e57/`. E56's earlier
      11-case pass and E55's wrapper-link failure remain preserved separately.
- [x] Post-change full host suite passes: **525 tests**, **153 expected
      skips**, **2,241 layout assertions**, and **846 declarations**. JSON,
      generated package text, and `git diff --check` also validate.
- [ ] This is supporting evidence and closes **0 named Step-5 units**; FAttack
      remains 0/3 named units and 5/6 overall. A fuller retarget sequence,
      shield-ratio oracle coverage, broader original damage differentials,
      full battle replay, and Step-6 promotion remain. Counters remain 120/152
      closed, 32 evidence units open, 10 promotions open, and 42 practical
      open items. Evidence:
      `analysis/compat/f-attack-complex-beam-residual-e55.{md,json}`.

### Previous checkpoint — FAttack `dpMain` arithmetic and callback alias audit E53

- [x] Traced original `dpMain` initialization: on weapon kind `0x10` it is
      the low-32-bit product of selected-part `_beam.dp` at offset `0x36`,
      installed component count, and attacker `_tok.csh` at offset `0x13`,
      using retained helper `1118:0c7e`; the alternate branch zeroes it. The
      count comes from the current hull-slot byte; CodeView's temporary name
      `dxRangeCur` is misleading. Raw `_hul *` / `wtFuelMax` is an overlapping
      type alias.
- [x] Traced the bounded update at `10f0:7753–77fe`: the integer path and
      x87 fallback form `dpMain * dp / dpT`, capped at `dpMain - 1`. CodeView
      maps `dp` to BP `-102` decimal and `dpT` to BP `-88` decimal. `dpT`
      snapshots mutable `dp` immediately before shield-only `FDamageTok`, so
      the ratio is post-shield beam remainder over its pre-shield value. It is
      distinct from `cTorpsLeft` (BP `-58` decimal).
- [x] Confirmed the callback alias/order: hull `FDamageTok` receives
      `&cTorpsLeft`, can reduce it by casualties with a zero floor, then
      `FAttack` separately subtracts selected split terms on return. Evidence:
      `analysis/compat/f-attack-dpmain-loop-arithmetic-e53.{md,json}`.
- [ ] Independent source state and an original-backed callback-plus-caller
      mutation-order case remain open. No source behavior changed and this
      closes **0 named Step-5 units**; `FAttack` stays 0/3 and 5/6 overall.
      Counters remain 120/152 closed, 32 evidence units open, 10 promotions
      open, and 42 practical open items. Next represent `dpMain` and the
      shield-ratio state in readable source, then capture that ratio and the
      callback-plus-caller residual mutations against the original executable.

### Previous checkpoint — FAttack torpedo loop condition raw branch audit E52

- [x] Matched the two tested machine dwords to original CodeView locals:
      `dpMain` (BP -36 decimal) and `cTorpsLeft` (BP -58 decimal), both
      4-byte `long`s. The exact loop condition is
      `dpMain > 0 || cTorpsLeft > 0`. `cTorpsLeft` is the remaining-torpedo
      quantity; `dpMain`'s complete update semantics remain open.
- [x] Inspected the retained original instruction listing, not just the
      decompiler pseudocode. `10f0:7c84–7cb2` repeats the group-0x20 pass at
      `10f0:6e04` while either signed 32-bit pair `[BP-0x24]:[BP-0x22]` or
      `[BP-0x3a]:[BP-0x38]` is positive. The second pair is initialized on the
      non-`0x10` weapon-kind path and decremented by the selected split terms
      at `10f0:7c65–7c68`. The first pair is computed on the `0x10` path,
      zeroed on the alternate path, and later updated in target selection; its
      complete update semantics still need call/lifetime tracing. The Ghidra raw-C
      variable names at lines 764–769 do not match the machine stack slots.
- [x] Updated the E20 FAttack evidence map with this storage-level result and
      the exact current-source discrepancy (`remaining_packets > 0` plus
      `made_progress`). Two focused host regressions pass; no source behavior
      changed. Evidence: `analysis/compat/f-attack-outer-loop-assembly-e52.{md,json}`.
- [ ] At E52, `dpMain` was not modeled as independent source state and its
      update semantics were incomplete. E53 has since traced its arithmetic
      and callback alias; source modeling and original mutation-order evidence
      remain open.
      This closes **0 named Step-5 units**;
      `FAttack` stays 0/3 named Step-5 units and 5/6 overall. Counters remain
      120/152 evidence units closed, 32 open, 10 promotion units, and 42
      practical open work items. Next trace the first dword and both quantities
      through `FDamageTok` mutations before changing the C loop.

### Previous checkpoint — FAttack packed raw-part resolver native integration E51

- [x] Fresh package `compat/vc152/f-attack-packed-record-native-e51/` compiles
      and links in Win98 with VC++ 1.52c. The packed FAttack executable uses
      `stars_lookup_part_record` with the original unrelocated NE segment-2
      image and passes `F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0 RESULT=PASS`;
      the companion original-record torpedo/residual probe passes
      `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=0 RESULT=PASS`. Both
      processes exit 0. Guest window inventory has no Program Error dialog.
      The returned segment matches the package byte-for-byte. Reports, logs,
      EXEs, maps, objects, screenshot, segment, and agent log are preserved in
      `runs/vmctl-fapne51/`. The two targeted host tests and
      `git diff --check` pass. Full-suite evidence remains E47's 524 tests,
      153 expected skips, 2,241 layout assertions, and 846 declarations.
- [ ] Supporting native integration evidence only; closes **0 named Step-5
      units**. Static copied segment bytes and scripted services do not prove a
      live Stars! CS mapping, complete race/hidden/technology service behavior,
      original-executable damage differential, whole-battle replay, or
      promotion. FAttack stays 0/3 named Step-5 units and 5/6 overall. TODO6
      counters remain 120/152 evidence units closed, 32 open, 10 promotion
      units, and 42 practical open work items. Full report:
      `analysis/compat/f-attack-packed-record-native-e51.{md,json}`.

### Previous checkpoint — FLookupPart original-segment record binding E47

- [x] Added `stars_lookup_part_record`, which validates the successful lookup's
      current-CS marker, literal family base/stride, computed record offset,
      and complete record extent before returning a borrowed record pointer
      into a caller-supplied segment image.
- [x] Host probe derives the unrelocated NE segment-2 bytes directly from the
      input `target/stars.exe` (SHA-256
      `ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59`). It
      checks all 16 family addresses and all 24 beam plus 12 torpedo records'
      `dRangeMax`, `dp`, and `init` words, along with truncated-span and forged
      address metadata rejection: `LOOKUP_PART_MEMORY_CHECKS=129 FAILURES=0
      RESULT=PASS`. Targeted E47 and packed-FAttack tests pass. Full suite via
      `PYTHONMALLOC=malloc python3 -m unittest discover -s tests -q`:
      524 tests passed, 153 expected skips, 2,241 layout assertions, and 846
      declarations; baseline and `git diff --check` pass.
- [x] Fresh VC++ 1.52c packet E50 compiles/links and passes the same 129 checks
      in Win98 (`C:\\FAPTA50`); returned `SEG2.BIN` matches the package
      byte-for-byte and no Program Error appears. E49's response-file omission
      caused a retained linker failure; E50 fixes the explicit map-file slot.
      Evidence: `compat/vc152/f-lookup-part-memory-native-e50/` and
      `runs/vmctl-faptae50/`.
- [x] E51 now exercises the validated borrowed record through packed FAttack
      in the host and native probes (24/24 packed checks; 17/17 torpedo record
      and residual checks). This closes none of the named Step-5 units. The
      copied static segment is still not a live Win98 CS mapping; complete
      race/hidden/technology services, original damage differential, replay,
      and Step-6 promotion remain.

### Previous checkpoint — FLookupPart current-CS table pointers E46

- [x] Reconciled the raw assembly for all sixteen valid group branches. Each
      computes its literal family base/stride, then executes `MOV DX,CS` before
      storing the far pointer. `0x10e0` is Ghidra's synthetic NE29 selector,
      not DGROUP or the table selector; same-offset CodeView global labels are
      overlays. `StarsLookupPartInput` now marks current-CS addressing for all
      sixteen families and reports their original CS-relative bases and item
      addresses. The original bases/strides are checked against the exact
      assembly immediates.
- [x] The strict-C89 probe checks all 16 valid groups plus the unsupported
      group for current-CS mode, zero literal segment, base, selected address,
      stride, and component identity. It passes
      `LOOKUP_PART_TABLE_CHECKS=66 FAILURES=0 RESULT=PASS`; the race/visibility
      probe passes `LOOKUP_PART_CHECKS=8`. Updated the packed-FAttack host
      regression to assert current-CS mode and a zero literal segment instead
      of the superseded `0x10e0` interpretation; its targeted test passes.
- [x] Fresh VC++ 1.52c/Win98 packet `C:\\FAPTA48` compiles and links, exits 0,
      returns `LOOKUP_PART_TABLE_CHECKS=66 FAILURES=0 RESULT=PASS`, and shows
      no Program Error. The 523-test suite passes with 153 expected skips,
      2,241 layout assertions, and 846 declarations; baseline and diff checks
      pass. Evidence: `analysis/compat/f-attack-current-cs-tables-e46.{md,json}`
      and `runs/vmctl-faptae48/`.
- [ ] Closes **0 named Step-5 units**. Runtime-anchor identities and address
      expressions now resolve to exact original CS-relative offsets, but a
      live table-memory service, complete race/hidden/technology services,
      FAttack damage acceptance, original differential, full replay, and
      promotion remain open.

### Previous checkpoint — FLookupPart absolute-offset model E45 (superseded)

- [x] Used CodeView overlays to assign DGROUP symbol addresses and selector
      `0x10e0` to the part tables. The numeric CS-relative bases/strides were
      right, including ZIP's `0x36` byte stride, but the selector was not: raw
      assembly sets CS for every branch, while `0x10e0` identifies Ghidra NE29.
      E46 supersedes the address-space interpretation. E45's passing native
      packet only proved consistency with its model; it is not original-table
      selector evidence. Retained in
      `analysis/compat/f-attack-live-table-anchors-e45.{md,json}` and
      `runs/vmctl-faptae47/`.

### Previous checkpoint — FLookupPart symbolic live-anchor schema E44 (superseded)

E44 also modeled ZIP's word-pointer expression as a `0x1b`-byte stride; the
machine instructions use `0x36`. E45 corrected that stride but introduced the
selector/overlay error described above. Both historical reports are retained
but not counted as original-fidelity evidence.

### Previous checkpoint — FAttack raw DGROUP hull far-pointer tables E43

- [x] Added `stars_lpshdef_from_tok_globals` using the original DGROUP globals:
      normal `rglpshdef` at `0025:00FE`, alternate `rglpshdefSB` at
      `0025:014C`, sixteen four-byte offset/segment entries per table, and
      `0x93`-byte hull stride. `stars_attack_bind_cool_battle_global_hull`
      composes this with `_hul.rghs`/`chs` decoding and the live attack callback.
- [x] Strict-C89 and fresh Win98/VC++ 1.52c integration pass 44/44. Test
      addresses prove alternate `1234:2000 + 2*0x93 = 1234:2126` and normal
      `2345:1000 + 5*0x93 = 2345:12DF`; both flow through the callback, with
      damage checked for the populated alternate hull. Build/link pass,
      process exit 0, and no Program Error. Full host suite passes 523 tests
      (153 expected skips), 2,241 layout assertions, and 846 declarations;
      baseline/diff checks pass. Evidence:
      `analysis/compat/f-attack-dgroup-hull-far-tables-e43.{md,json}` and
      `runs/vmctl-fcbate43/`.
- [ ] Closes **0 named Step-5 units**. The selected raw global far pointer is
      now exercised, but resolving it against the actual native hull-table
      memory remains an explicit service. Complete part services, damage
      acceptance, original differential, full replay, and promotion remain.

### Previous checkpoint — FAttack design-to-hull normal/alternate table branches E42

- [x] Extended E41's selected-hull integration to both original
      `LpshdefFromTok` branches: player 1/design `0x12` resolves the alternate
      table, index 2, and its raw `_hs` slot reaches lookup/damage; player
      3/design 5 resolves the normal table, index 5, including the zero-slot
      path. `_hul.rghs[16]` and `chs` remain read at byte offsets 58 and 122.
- [x] Host and fresh Win98/VC++ 1.52c reports pass 44/44. Compile/link pass,
      process exits 0, and the guest has no Program Error. Full suite passes
      523 tests (153 expected skips), 2,241 layout assertions, and 846
      declarations; baseline/diff checks pass. Evidence:
      `analysis/compat/f-attack-design-hull-table-branches-e42.{md,json}` and
      `runs/vmctl-fcbate42/`.
- [ ] Closes **0 named Step-5 units**. Exact native table globals/far pointers,
      complete native part services and damage acceptance, original
      differential, full battle replay, and promotion remain open.

### Previous checkpoint — FAttack token design-to-hull slot resolution E41

- [x] Added `stars_attack_bind_cool_battle_design_hull`: it builds original
      `_tok` player/design bytes from the battle token and calls the shared
      `stars_lpshdef_from_tok` selection logic. The resolved 123-byte `_hul`
      supplies `rghs[16]` at byte 58 and active `chs` at byte 122; the adapter
      then composes E40's raw-slot/context binder. Counts beyond 16 are rejected
      instead of reading beyond the recovered array.
- [x] End-to-end callback test uses player 1/design `0x12` and verifies the
      alternate design table, index 2, selector `0x2468`, raw slot group
      `0x10`/item 7/count 2 overriding conflicting typed metadata, lookup,
      damage, and battle cleanup. Strict-C89/native report passes 38/38.
- [x] Fresh `C:\\FCBATE41` VC++ 1.52c build/link and Win98 run pass; process
      exit 0 and no Program Error. Full suite passes 523 tests (153 expected
      skips), 2,241 layout assertions, and 846 declarations; baseline and diff
      checks pass. Evidence: `analysis/compat/f-attack-design-hull-resolution-e41.{md,json}`
      and `runs/vmctl-fcbate41/`.
- [ ] This closes **0 named Step-5 units**. The reconstructed design-to-hull
      selection is now exercised, but its callback still supplies the original
      per-player hull-table service; exact native `_tok`/`vrgtok` globals/far
      pointers, full `FLookupPart` services, beam/torpedo acceptance, original
      differential, complete battle replay, and promotion remain.

### Previous checkpoint — FAttack battle callback raw hull-slot binding E40

- [x] Composed battle-token context binding and raw `_hs` decoding as
      `stars_attack_bind_cool_battle_hull`. In the real `FDoCoolBattle` to
      `FAttack` callback probe, raw group `0x10`/item 7/count 2 conflicts with
      typed group `0x20`/item 4/count 1; the resolver receives the raw values,
      damage deactivates the target, and battle reporting/cleanup still pass.
- [x] Strict-C89 host and fresh Win98/VC++ 1.52c report pass 33/33;
      compile/link pass, process exits 0, no Program Error. Full suite passes
      523 tests (153 expected skips), 2,241 layout assertions, and 846
      declarations; baseline and diff checks pass. Evidence:
      `analysis/compat/f-attack-cool-battle-hull-e40.{md,json}` and
      `runs/vmctl-fcbate40/`.
- [ ] Supporting integration evidence only; closes **0** named Step-5 units.
      The callback still owns actual design-to-hull selection and native
      `_tok`/`vrgtok` global binding. Full `FLookupPart` table/service/race/
      visibility behavior, beam/torpedo service acceptance, original
      differential, full battle replay, and promotion remain.

### Previous checkpoint — FAttack raw `_hs` slot decoding bridge E39

- [x] Added `stars_attack_bind_raw_hull_slots`, decoding each four-byte
      little-endian `_hs` record (group word, item byte, count byte) into the
      FAttack packed-part view. It validates raw stride and destination
      capacity, and avoids aliasing/type-punning game storage.
- [x] Reworked the packed-part probe to start from raw bytes and deliberately
      conflict with duplicated typed section metadata. Raw slots drive the
      resolver and damage dispatch; unsupported item, zero count, short stride,
      and insufficient destination cases are checked. Host and fresh Win98
      report: 16/16; compile/link pass, process exit 0, no Program Error.
- [x] Native packet `compat/vc152/f-attack-complex-native-e39/` and all return
      evidence are retained in `runs/vmctl-vc152fapne39/`. Reports also pass:
      beam scale 86/86, raw damage 9/9, torpedo target 5/5, and raw torpedo
      dispatch 17/17.
- [x] Full verbose suite passes 523 tests, 153 expected skips, 2,241 layout
      assertions, and 846 declarations. Baseline and diff checks pass.
- [ ] Supporting evidence only; closes **0** Step-5 units. FAttack remains
      0/3 at Step 5 and 5/6 overall. This adapter is not yet connected to the
      actual battle callback's packed hull-slot source. Complete original
      FLookupPart table/service and race/visibility binding, attacker/target/
      design-to-hull resolution, beam/torpedo damage-service acceptance, E2
      original differentials, E3 full battle replay, and promotion remain.
      Evidence: `analysis/compat/f-attack-raw-hs-slot-bridge-e39.{md,json}`.

### Previous checkpoint — FAttack context from packed battle token E38

- [x] Added `stars_attack_bind_cool_battle_token` as the shared FDoCoolBattle
      callback seam. It maps the selected token index, player, raw object kind
      (`grobj`), battle coordinate, current initiative, and surviving-player
      mask into FAttack's caller input without taking ownership of weapon,
      target, or damage services.
- [x] Upgraded the integration probe to enter through the production 29-byte
      packed-token load/store bridge and call the adapter. It verifies decoded
      object kind and coordinates reach FAttack and remain intact through
      battle mutation/write-back. Strict-C89 host and fresh Win98/VC++ 1.52c
      report: 29/29; compile/link pass, process exit 0, no Program Error
      dialog. The first packet's missing `COMMON.H` compile failure is
      retained separately from the corrected passing run.
- [x] Full verbose suite passes 523 tests, 153 expected skips, 2,241 layout
      assertions, and 846 declarations. Baseline and diff checks pass.
- [ ] Supporting evidence only; closes **0** Step-5 units. FAttack remains
      0/3 at Step 5 and 5/6 overall; complete packed weapon/hull/target and
      damage-service acceptance, E2 differentials, E3 full battle replay, and
      promotion remain. FDoCoolBattle's exact global/caller binding and full
      original battle comparison also remain. Evidence:
      `analysis/compat/f-attack-cool-battle-context-e38.{md,json}` and
      `runs/vmctl-vc152fcbate4-success/`.

### Previous checkpoint — FAttack original torpedo initiative/range E37

- [x] Bound all twelve original `_torp.init` (offset 56) and
      `_torp.dRangeMax` (offset 52) values from `rgtorp` at NE segment
      2:`0x2180` into the successful packed group-0x20 FAttack branch. The
      selected record now replaces resolver-provided initiative/range fields;
      torpedo `dp` remains table-backed from E29.
- [x] The strict-C89 matrix checks all twelve `dp`/`init`/range entries,
      invalid item 12, and item 9's integration boundaries: `initBase=3` plus
      `init=1` supplies 4; `dRangeMax=5` with `grobj=1` admits distance 6 and
      rejects distance 7. Dispatch result: 17/17. Fresh `C:\\FAPNE37` native
      compile/link passes; beam scale 86/86, packed part 13/13, raw damage
      9/9, torpedo target 5/5, and torpedo dispatch 17/17 all pass. Processes
      exit 0 and no Program Error dialog appears.
- [x] Full verbose suite passes 522 tests, 153 expected skips, 2,241 layout
      assertions, and 846 declarations. Baseline and diff checks pass.
- [ ] Supporting evidence only; closes **0** named Step-5 units. FAttack
      remains 0/3 at Step 5 and 5/6 overall. Complete packed battle-token/
      object-kind wiring and beam/torpedo service acceptance, E2 original
      damage differentials, E3 full battle replay, and Step-6 promotion remain.
      Evidence: `analysis/compat/f-attack-torpedo-fields-e37.{md,json}` and
      `runs/vmctl-vc152fapne37/`.

### Previous checkpoint — FAttack beam table range gate E36

- [x] Recovered all 24 original `_beam.dRangeMax` words (record offset 52)
      from `rgbeam` at NE segment 2:`0x2450`. Packed group-0x10 FAttack now
      sets the raw coordinate-range gate from this value and adds exactly one
      only when the attacker's raw `grobj` is 1. `StarsAttackInput` now exposes
      that caller-owned object kind; the comparison remains inclusive, matching
      raw `DzFromBrcBrc(...) <= grobjBonus + dRangeMax`.
- [x] The strict-C89 matrix checks every range, `dp`, and `init` table word,
      invalid item 24, all previous scaling cases, and three range boundaries:
      planet-kind distance 2 accepted at max 2; distance 3 rejected; fleet-kind
      distance 2 rejected at max 1. Result: 86/86. Fresh `C:\\FAPNE36` native
      build/link and replay pass: beam 86/86, packed part 13/13, raw damage
      9/9, torpedo target 5/5, and torpedo dispatch 11/11. No warnings,
      process errors, or Program Error dialog.
- [x] Full verbose suite passes 521 tests, 153 expected skips, 2,241 layout
      assertions, and 846 declarations. Baseline and diff checks pass.
- [ ] Supporting evidence only; closes **0** named Step-5 units. FAttack
      remains 0/3 at Step 5 and 5/6 overall. The object-kind value still must
      be traced through complete packed battle-token/orchestrator replay, and
      E1 beam/torpedo service acceptance, E2 original damage differentials,
      E3 full battle replay, and Step-6 promotion remain open. Evidence:
      `analysis/compat/f-attack-beam-range-e36.{md,json}` and
      `runs/vmctl-vc152fapne36/`.

### Previous checkpoint — FAttack beam table initiative binding E35

- [x] Raw `_beam` has `dRangeMax` at byte 52, `dp` at byte 54, and `init` at
      byte 56 (`_beam` size 60). E34 had bound `dp`; E35 now also recovers
      every original `init` word from NE segment 2:`0x2450` and makes packed
      group-0x10 FAttack override the resolver's `init_delta` with the part's
      actual initiative. The existing unsigned wrap-and-cap logic then
      computes `initBase + beam.init` before matching the active initiative.
- [x] Expanded the table probe to verify all 24 `dp` and all 24 `init` values,
      plus both invalid-index cases and eight damage/branch scenarios:
      58/58 pass. The strict-C89 packed-part integration now proves item 7's
      original `init=5` plus `initBase=4` dispatches at initiative 9: 13/13
      pass. Raw damage remains 9/9.
- [x] Fresh `C:\\FAPNE35` VC++ 1.52c compile/link and Win98 replay pass.
      Native reports: beam/table 58/58, packed part 13/13, raw damage 9/9,
      torpedo target 5/5, and torpedo dispatch 11/11. All tested processes
      exit 0; final compile logs have no warnings and the window list has no
      Program Error dialog.
- [x] Full verbose repository suite passes: 521 tests, 153 expected skips,
      2,241 layout assertions, and 846 declarations. Baseline and diff checks
      pass.
- [ ] Supporting evidence only; closes **0** named Step-5 units. FAttack
      remains 0/3 at Step 5 and 5/6 overall. Full raw beam/torpedo part-field
      and service acceptance, E2 original damage differentials, E3 full battle
      replay, and Step-6 promotion remain open. Evidence:
      `analysis/compat/f-attack-beam-table-init-e35.{md,json}` and
      `runs/vmctl-vc152fapne35/`.

### Previous checkpoint — FAttack beam table integration E34

- [x] Recovered the original group-0x10 `_beam.dp` table from NE segment 2,
      offset `0x2450`, using the CodeView `_beam` size of 60 bytes and `dp`
      offset 54. Added `stars_lookup_beam_dp` with all 24 executable values
      and made FAttack replace resolver-supplied beam damage with the packed
      part's original table value, paralleling the existing torpedo path.
- [x] Expanded the strict-C89 production beam probe to check all 24 table
      entries plus invalid-item handling and eight scaling/branch cases:
      33/33 pass. The raw stateful damage probe now uses beam items 15 and 21
      to prove the actual table selects damage classes 1 and 2 despite an
      intentionally bogus resolver value; 9/9 pass. Corrected its expected
      post-shield residual from 33 to 34 for item 15's original `dp=169`.
- [x] Fresh `C:\\FAPNE34` VC++ 1.52c build and link pass. Win98 native reports:
      beam/table 33/33, raw damage 9/9, torpedo target 5/5, and raw torpedo
      dispatch 11/11; all tested processes exit 0. Final compile logs are
      warning-free. The initial duplicate-stub compile failure and the first
      raw residual mismatch are retained in the run directory as diagnostics.
- [x] Full repository suite passes: 520 tests, 153 expected skips, 2,241
      layout assertions, and 846 declarations. Baseline and scoped diff checks
      pass.
- [ ] The verbose full-suite run is authoritative and passes; two quiet-mode
      full-suite invocations terminate with signal 10 before printing a test
      summary. `test_common_source -q` passes 221/221 in isolation. Track this
      as a runner-level intermittent until the quiet-mode crash is localized.
- [ ] Supporting evidence only; closes **0** named Step-5 units. FAttack
      remains 0/3 at Step 5 and 5/6 overall. Full E1 raw beam/torpedo
      battle-part and table/service acceptance, E2 broader original damage
      differentials, E3 full battle replay, and Step-6 promotion remain open.
      Evidence: `analysis/compat/f-attack-beam-table-e34.{md,json}` and
      `runs/vmctl-vc152fapne34/`.

### Previous checkpoint — FAttack beam broadcast falloff correction E33

- [x] Raw FAttack distinguishes the group-0x10 broadcast/simple-beam branch
      from complex target selection. The broadcast branch applies attacker
      `pctCap` and target `pctBeamDef`, with a range gate, but does not apply
      the complex branch's `distance * 10 / part.dp` falloff. The readable
      source had shared that falloff between both paths; it now applies only
      to complex target selection.
- [x] Expanded the instruction-order probe with a paired case using identical
      inputs: complex selection yields 39 damage after distance falloff, while
      broadcast yields 40 without it. Host eight-case matrix and fresh native
      C:\\FAPNE33 VC++ 1.52c replay pass; compile/link pass, process exits 0,
      and no Program Error dialog appears.
- [x] Full repository suite passes after the source correction: 520 tests,
      153 expected skips, 2,241 layout assertions, and 846 declarations.
- [ ] This corrects readable source but closes no named Step-5 unit. FAttack
      remains 0/3 at Step 5 and 5/6 overall; complete E1 raw beam/torpedo
      battle-part and table/service acceptance, E2 broader original damage
      differentials, E3 full battle replay, and Step-6 promotion remain open.
      Evidence: `analysis/compat/f-attack-beam-broadcast-e33.{md,json}` and
      `runs/vmctl-vc152fapne33/`.

### Latest checkpoint — FAttack raw beam scale instruction-order probe E32

- [x] Added a strict-C89 host probe that exercises the production `FAttack`
      group-0x10 path for nonzero and zero capacity percentage, target beam
      defense, distance falloff, the `dp == 200` damage-class boundary, zero
      effective damage, and falloff beyond 100 percent. All seven cases pass.
- [x] The oracle follows the raw sequence at
      `analysis/decompiler/export/raw/10f0_69ac_FAttack.c:515-561`:
      nonzero attacker `pctCap`, target `pctBeamDef`, then
      `distance * 10 / part.dp` and the remaining percentage. The packed
      record's beam damage class changes from 1 to 2 at `dp == 200`.
- [x] Fresh VC++ 1.52c/Win98 packet `C:\FAPNE32` compiles and links; the
      native seven-case report passes and the process exits 0. The post-run
      windows check finds no Program Error dialog. The harmless duplicate test
      macro warning is retained in `runs/vmctl-vc152fapne32/BEAMCL.LOG`.
- [ ] Supporting evidence only: no Step-5 unit closes. Complete E1
      beam/torpedo acceptance, E2 broader original damage differentials, E3
      full-battle replay, and promotion remain open. Phase 2
      remains **10 procedures at 5/6, 41 at 6/6; 120/152 evidence units
      closed, 32 open, 10 promotion units, 42 practical open work items**.
      Evidence: `analysis/compat/f-attack-beam-scaling-e32.{md,json}`.

### Phase 2 workload table correction

- [x] Audited the compact per-procedure burn-down against the expanded
      workload bands below. The compact ledger has 10 procedures below 6/6,
      not 13: `SatisfyOrders` has 1 open evidence unit; `FAttack`,
      `FDoCoolBattle`, and `ExecuteReportClick` have 3 each; five AI
      personalities have 4 each and `DoCyberAiTurn` has 2. The correct total
      is 32 open evidence units plus 10 promotion units = 42 open work items.
- [x] Corrected the stale workload-band and cluster summaries to match that
      per-procedure ledger; no completion or unit closure is inferred from the
      correction.

### Latest checkpoint — DoRobotoidAiTurn repair eligibility E4 host support

- [x] Replaced the broad repair condition with the raw deterministic gate:
      owned fleet, slot 0 nonzero, turn >= 41, `cord == 1`, empty first-order
      kind. Fleet route planning now runs first and a selected action bypasses
      repair. Strict-C89 repair/route probes pass 7/7 and 18/18; the route
      matrix checks both action-bypass and no-op-then-repair order.
- [x] Robotoid source/phase/production/integration/fleet-gate/typed-fleet
      probes pass 23/6/12/18/10/2. Full suite: 519 tests passed, 153 expected
      skips; 2,241 layout assertions and 846 declarations. Baseline and diff
      checks pass.
- [ ] Host-only evidence; **0** Step-5 units closed. The original
      `Random(5)` nearby-route choice and exact draw consumption remain open,
      as do full AI-table integration, native replay of this revision,
      original-turn differential, and Step-6 promotion. Counters remain
      **120/152 closed, 32 open, 10 promotion units, 42 practical open**.
      Evidence: `analysis/compat/do-robotoid-ai-turn-repair-gate-e4.{md,json}`.

### Latest checkpoint — DoTurinDroneAiTurn exact repair gate E4 host support

- [x] Corrected Turin's broad any-ships/`cord < 2` repair predicate to the raw
      slot matrix at `analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c:675-693`:
      slots 2, 3, 1, 8, 9, 13, 14, 0, 10, and 11 are zero; slot 12 is nonzero;
      `cord == 1`; first-order kind is empty. The strict-C89 probe passes 6/6
      checks with negative cases for other slots, cord, order kind, and owner.
- [x] Turin source, packed integration, route-plan, and edge probes pass at
      35/20/14/7 checks. Full suite: 519 tests passed, 153 expected skips;
      generated headers remain at 2,241 layout assertions and 846 declarations.
      Baseline and diff checks pass.
- [ ] Host support only; this closes **0** named Step-5 units. Native replay,
      complete packed order mutation, AI-table integration, original-turn
      differential, and Step-6 promotion remain open. Counters remain
      **120/152 closed, 32 open, 10 promotion units, 42 practical open**.
      Evidence: `analysis/compat/do-turin-drone-ai-turn-repair-gate-e4.{md,json}`.

### Prior checkpoint — AI PLORD first-order kind writes E11 host support

- [x] Added the shared `stars_ai_set_first_order_kind` helper for the raw
      Robotoid-family branches that perform `(word & 0xfff0) | 5/6`. The
      strict-C89 probe reports `AI_PLORD_FIRST_KIND_SOURCE_CHECKS=12
      FAILURES=0 RESULT=PASS`; the existing nine-check repair regression also
      remains green. The helper preserves the existing queue count, the high
      order-word nibble, and every unrelated byte, while rejecting invalid
      kinds and null input.
- [x] The surrounding Robotoid/PLORD probes pass, and the full repository
      regression reports `Ran 519 tests`, `OK (skipped=153)`; generated
      headers remain at 2,241 layout assertions and 846 function declarations.
      `baseline.py --check` and `git diff --check` pass.
- [ ] This is supporting host evidence only. It closes **0** named Step-5
      units; native replay, complete personality route selection, multi-order
      packed mutation, original-turn differentials, and Step-6 promotion
      remain open. The authoritative counters remain **120/152 closed, 32
      open, 10 promotion units, and 42 practical open work items**. The guest
      agent is still disconnected. Evidence is in
      `analysis/compat/ai-plord-first-kind-e11.{md,json}`.

### Audit resolution — Automitron E3 repair path confirmed in instruction listing

- [x] The decompiler's structured C appears to put the cord-one repair test in
      `else` of `cord < 2`, but the original instruction listing at
      `analysis/decompiler/export/raw/1098_01e0.asm:1235-129e` confirms a
      reachable path: `rgcsh[6]` at fleet offset `0x18` is nonzero, fleet
      `[BX+0x62] == 1`, empty packed first-order low nibble at `[PLORD+0x0a]`,
      kind-6 write, two `0x0005` words, then
      `FLookupFleet`. Treat the structured-C contradiction as a decompiler
      control-flow artifact.
- [x] Tightened the readable policy from any nonempty fleet to the raw
      `rgcsh[6]` gate; the strict-C89 8-check repair probe includes a slot-2-only
      fleet that must not be repaired. Automitron source/production/integration
      (31/12/20), route plan (14), and nine-action matrix (20) probes pass.
      Full suite: 519 tests passed, 153 expected skips; generated headers:
      2,241 layout assertions and 846 declarations. Baseline and diff checks
      pass. This is still host evidence and closes no Step-5 unit.
- [ ] Native replay, full raw route/order coverage, and original-turn
      differential evidence remain open. All Phase 2 counters remain
      **120/152 closed, 32 open, 10 promotion units, 42 practical open**.

### Prior checkpoint — DoAutomitronAiTurn packed repair boundary E3 host support

- [x] Added the Automitron fleet fields for `cord`, first-order kind, and
      optional packed `PLORD` storage. The host seam applies the shared
      kind-6/two-word repair before its typed fleet visitor for cord-one/
      empty-kind test input, and exposes the surrounding repair callback. The
      strict-C89 probe reports
      `AUTOMITRON_AI_REPAIR_BOUNDARY_CHECKS=8 FAILURES=0 RESULT=PASS`.
- [x] Existing Automitron source, production, and packed integration probes
      remain green at 31, 12, and 20 checks respectively; baseline and diff
      checks pass.
- [x] The full repository regression after the public-header/source change
      reports `Ran 519 tests`, `OK (skipped=153)`; generated headers remain at
      2,241 layout assertions and 846 function declarations.
- [ ] This is supporting host evidence only. Instruction-level evidence
      confirms the cord-one/empty-kind raw path, but the probe is not native and
      does not close **0** named Step-5 units; native replay, complete
      Automitron route/order behavior, original-turn differentials, and Step-6
      promotion remain open. The
      authoritative counters remain **120/152 closed, 32 open, 10 promotion
      units, and 42 practical open work items**. Evidence is in
      `analysis/compat/do-automitron-ai-turn-repair-boundary-e3.{md,json}`.

### Latest checkpoint — DoAutomitronAiTurn route plan/apply boundary E4 host support

- [x] Added the typed Automitron fleet plan/apply seam for the recovered
      action families. The strict-C89 probe reports
      `AUTOMITRON_AI_ROUTE_PLAN_CHECKS=14 FAILURES=0 RESULT=PASS`, covering
      owned-fleet filtering, colonize/move payloads, callback ordering, and
      planner/applier failure propagation.
- [ ] This is supporting host evidence only. It closes **0** named Step-5
      units; raw Automitron target predicates, complete packed-order mutation,
      native replay, original-turn differentials, and Step-6 promotion remain
      open. The authoritative counters remain **120/152 closed, 32 open, 10
      promotion units, and 42 practical open work items**. Evidence is in
      `analysis/compat/do-automitron-ai-turn-route-plan-e4.{md,json}`.
- [x] The full repository regression after this public-header/source change
      reports `Ran 519 tests`, `OK (skipped=153)`; generated headers remain at
      2,241 layout assertions and 846 function declarations.

### Latest checkpoint — DoAutomitronAiTurn action dispatch matrix E5 host support

- [x] Extended the Automitron full-caller host matrix across all nine named
      action callbacks. `AUTOMITRON_AI_ACTION_MATRIX_CHECKS=20 FAILURES=0
      RESULT=PASS` verifies every action maps to its intended callback,
      preserves the complete plan payload, filters a foreign fleet, and then
      visits each owned fleet.
- [ ] Supporting host evidence only: **0** named Step-5 units closed. Native
      replay, raw target predicates, packed order writes, the original-turn
      differential, and Step-6 promotion remain open. The counters remain
      **120/152 closed, 32 open, 10 promotion units, and 42 practical open
      work items**. Evidence is in
      `analysis/compat/do-automitron-ai-turn-action-matrix-e5.{md,json}` and
      `tests/do_automitron_ai_turn_action_matrix_probe.c`.

### Latest checkpoint — Phase 2 host regression after combat/order corrections

- [x] Recompiled and reran the focused strict-C89 boundaries. `FAttack`
      reports `F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS`, and the complete
      four-pass `SatisfyOrders`/`DoOrders` integration reports
      `SATISFY_ORDERS_FULL_TURN_E3_CHECKS=18 FAILURES=0 RESULT=PASS`.
- [x] The full project suite reports `Ran 519 tests`, `OK (skipped=153)`;
      generated headers remain at 2,241 layout assertions and 846 function
      declarations. `baseline.py --check` and `git diff --check` pass.
- [ ] This is regression evidence only. The VMCTL1 listener still has no
      guest connection/banner, so no native closure or Step-6 promotion is
      claimed. The authoritative counters remain **120/152 closed, 32 open,
      10 promotion units, and 42 practical open work items**. Evidence is in
      `analysis/compat/phase2-host-regression-20260922.{md,json}`.

### Latest checkpoint — DoMacintiAiTurn policy boundary E2 host support

- [x] Added a focused strict-C89 matrix for the readable Macinti policy
      seam. `MACINTI_AI_POLICY_BOUNDARY_CHECKS=11 FAILURES=0 RESULT=PASS`
      covers owned/unowned/hostile planet classification, mineral-score
      saturation and viability, queue and batch affordability, the capped
      packet-threat calculation plus starbase increment, owned repair /
      transport / colonizer fleet roles, and foreign-fleet exclusion.
- [ ] This is supporting evidence only. Native replay, exact original
      retarget/colonizer/transport predicates, broader packed-order mutation,
      the original-turn differential, and Step-6 promotion remain open. The
      authoritative counters remain **120/152 closed, 32 open, 10 promotion
      units, and 42 practical open work items**. Evidence is in
      `analysis/compat/do-macinti-ai-turn-policy-boundary-e2.{md,json}` and
      `tests/do_macinti_ai_turn_policy_boundary_probe.c`.

### Latest checkpoint — DoCyberAiTurn repair and packed order write E9

- [x] The complete readable Cyber caller now combines shared first-order
      repair, typed route planning/action dispatch, and packed `PLORD` writing
      in one matrix. An owned colonizer fleet takes a same-endpoint
      replacement, an owned armed fleet appends a movement order, and a
      foreign fleet is bypassed. The strict host C89 probe reports
      `CYBER_AI_TURN_ORDER_WRITE_CHECKS=24 FAILURES=0 RESULT=PASS`.
- [x] Fresh VC++ 1.52c / Win98 validation in `C:\CYBOWE9` compiled and linked,
      returned the same 24-check report, exited 0, and showed no Program Error
      dialog. Planner/applier failure propagation and callback ordering are
      included. This closes **1** additional named Step-5 unit: detailed
      repair/order writes. Evidence is in
      `analysis/compat/do-cyber-ai-turn-order-write-e9.{md,json}` and
      `runs/vmctl-vc152cybowe9/`.
- [ ] Direct raw personality branch coverage beyond these typed action
      families, complete Cyber AI-table integration, original-turn
      differential, and Step-6 promotion remain open. Counters are now
      **120/152 closed, 32 open, 10 promotion units, and 42 practical open
      work items**.

### Latest checkpoint — DoCyberAiTurn action dispatch matrix E10

- [x] The complete readable Cyber caller now dispatches all nine named action
      families—clear task, move, colonize, wormhole, starbase, attack,
      armada, transport, and freighter targeting—and routes seeded movement
      records through the shared packed writer. The strict host C89 probe and
      fresh VC++ 1.52c / Win98 replay in `C:\CYBAME10` both report
      `CYBER_AI_TURN_ACTION_MATRIX_CHECKS=34 FAILURES=0 RESULT=PASS`, with
      compile/link PASS, exit 0, and no Program Error dialog.
- [ ] This is supporting evidence and closes **0** additional named Step-5
      units. Raw Cyber personality predicates, complete AI-table integration,
      original-turn differential, and Step-6 promotion remain open; counters
      remain **120/152 closed, 32 open, 10 promotion units, and 42 practical
      open work items**. Evidence is in
      `analysis/compat/do-cyber-ai-turn-action-matrix-e10.{md,json}` and
      `runs/vmctl-vc152cybame10/`.

### Latest checkpoint — FDoCoolBattle packed field bridge E7

- [x] With the guest agent still disconnected, the production
      `StarsCoolBattleInput.packed_tokens` path received a stricter host probe.
      `F_DO_COOL_BATTLE_PACKED_FIELD_CHECKS=24 FAILURES=0 RESULT=PASS` checks
      every scalar in the 29-byte `_tok` record, all movement nibbles,
      active/log flags, a 32-byte caller stride, trailing sentinels, typed
      speed-nibble mutation, and defeated-token count/active mutation.
- [ ] This closes **0** authoritative Step-5 units. Native `_tok`/`vrgtok`
      binding and compaction, exact movement/damage behavior, fresh full-battle
      replay, original differential, and Step-6 promotion remain open. The
      authoritative counters remain **120/152 closed, 32 open, 10 promotion
      units, and 42 practical open work items**. Evidence is in
      `analysis/compat/f-do-cool-battle-packed-fields-e7.{md,json}` and
      `tests/f_do_cool_battle_packed_field_probe.c`.

### Latest checkpoint — FDoCoolBattle attack-mask ordering E8

- [x] The readable coordinator now matches the recovered loop: both tokens
      complete movement before attack-mask filtering, and the attack loop
      rechecks the filtered survivor mask before each candidate. The focused
      host probe passes
      `F_DO_COOL_BATTLE_ATTACK_MASK_ORDER_CHECKS=3 FAILURES=0 RESULT=PASS`;
      the existing 58-check source, 18-check battle-chain, 11-check
      packed-token, and 24-check packed-field probes remain green.
- [ ] This closes **0** authoritative Step-5 units. Native movement/initiative
      replay, exact global binding/compaction, complete damage and battle-record
      behavior, full replay, original differential, and Step-6 promotion remain
      open. Counters remain **120/152 closed, 32 open, 10 promotion units, and
      42 practical open work items**. Evidence is in
      `analysis/compat/f-do-cool-battle-attack-mask-order-e8.{md,json}` and
      `tests/f_do_cool_battle_attack_mask_order_probe.c`.

### Latest checkpoint — DoCyberAiTurn packet/freighter edge matrix E8

- [x] The complete readable `DoCyberAiTurn` caller now composes the real
      packet and freighter services across five full-caller scenarios:
      high-stock owned-planet drop-off, low-stock owned-planet pickup, enemy
      ordinary-planet starbase fallback, open-space short circuit, and
      packet-target rejection. The strict host C89 probe reports
      `CYBER_AI_TURN_PACKET_FREIGHTER_EDGE_CHECKS=45 FAILURES=0 RESULT=PASS`.
- [x] Fresh VC++ 1.52c / Win98 validation in `C:\CYBFE8` compiled and linked,
      returned the same 45-check report, exited 0, and showed no Program Error
      dialog. Combined with E5's native freighter gate/progress matrix and
      E7's native accepted packet/freighter composition, this closes **1**
      named Step-5 unit: packet/freighter behavior. Evidence is in
      `analysis/compat/do-cyber-ai-turn-packet-freighter-edge-e8.{md,json}`
      and `runs/vmctl-vc152cybfe8/`.
- [ ] Detailed order-repair/order-write coverage, complete Cyber AI-table
      integration, original-turn differential, and Step-6 promotion remain
      open. Counters are now **119/152 closed, 33 open, 10 promotion units,
      and 43 practical open work items**.

### Latest checkpoint — DoCyberAiTurn freighter-service bridge E5

- [x] Added the optional typed `run_freighter_ex` boundary to the owned-fleet
      pass. It forwards the converted fleet, derived fleet-role policy, and
      armada potency after first-order repair and before fleet-action planning.
- [x] The strict host C89 packed integration probe now passes
      `CYBER_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS`, including
      packed transport count, fleet identity, potency forwarding, and callback
      placement. Fresh VC++ 1.52c / Win98 replay in `C:\\CYBPE5` compiled and
      linked, returned the same report, exited normally, and showed no Program
      Error dialog.
- [ ] This is supporting host evidence only and closes **0** named Step-5
      units. Direct `DoCyberFreighter` composition, fresh native replay,
      complete packed `PLORD` mutation, AI-table integration, original-turn
      differential, and Step-6 promotion remain open. Counters remain
      **118/152 closed, 34 open, 10 promotion units, and 44 practical open
      work items**. Evidence, including native artifacts in
      `runs/vmctl-vc152cybe5/`, is in
      `analysis/compat/do-cyber-ai-turn-freighter-bridge-e5.{md,json}`.

### Latest checkpoint — DoCyberAiTurn direct DoCyberFreighter composition E6

- [x] The complete readable `DoCyberAiTurn` caller now invokes the real
      `stars_do_cyber_freighter` implementation through `run_freighter_ex`.
      The composition replay covers owned-planet transfer direction, target
      selection, exact `0x11b0` movement, packed first-order replacement, and
      ordering before the typed fleet visitor.
- [x] Host and fresh VC++ 1.52c / Win98 `C:\\CYBFE6` validation both report
      `CYBER_AI_TURN_FREIGHTER_COMPOSITION_CHECKS=18 FAILURES=0 RESULT=PASS`;
      compile/link and process exit pass and no Program Error dialog appears.
- [ ] This is supporting integration evidence and closes **0** named Step-5
      units. Complete packet/freighter predicates, raw fleet-cargo behavior,
      complete AI-table integration, original-turn differential, and Step-6
      promotion remain open. Counters remain **118/152 closed, 34 open, 10
      promotion units, and 44 practical open work items**. Evidence is in
      `analysis/compat/do-cyber-ai-turn-freighter-composition-e6.{md,json}` and
      `runs/vmctl-vc152cybfe6/`.

### Latest checkpoint — DoCyberAiTurn packet/freighter composition E7

- [x] The complete readable AI turn now composes both real
      `stars_do_cyber_freighter` and `stars_do_cyber_packets`. The replay
      covers freighter-before-basic-task ordering, packet queueing after basic
      tasks, packet-strength writeback from 501 to 389, exact `0x11b0` packed
      movement, and finalization order.
- [x] Host and fresh VC++ 1.52c / Win98 `C:\\CYBFE7` validation both report
      `CYBER_AI_TURN_PACKET_FREIGHTER_COMPOSITION_CHECKS=20 FAILURES=0 RESULT=PASS`;
      compile/link and process exit pass and no Program Error dialog appears.
- [ ] This is supporting evidence and closes **0** named Step-5 units.
      Complete raw cargo predicates, packet/freighter edge branches, complete
      AI-table integration, original-turn differential, and Step-6 promotion
      remain open. Counters remain **118/152 closed, 34 open, 10 promotion
      units, and 44 practical open work items**. Evidence is in
      `analysis/compat/do-cyber-ai-turn-packet-freighter-composition-e7.{md,json}`
      and `runs/vmctl-vc152cybfe7/`.

### Latest checkpoint — DrawPopup original consumer E4 completion and Step-6 promotion

- [x] Closed the final named DrawPopup E4 boundary from the original Win98
      consumer surface. Retained captures cover fleet summary, starbase/base,
      planet mineral/status, planet population/report, Planet Summary Report,
      and restored-frame behavior without a Program Error dialog. This is an
      original consumer-boundary comparison, not a claim of pixel identity.
- [x] Combined that original evidence with the authoritative native E1
      38-check payload, E2 21-check content, E3 25-check geometry/style,
      7-check GDI state-order, and 26-check production caller replays. The
      final native artifacts all compile/link and run successfully with no
      Program Error dialog. Evidence is in
      `analysis/compat/draw-popup-original-e4.{md,json}`,
      `runs/vmctl-vc152current/`, and the E1–E3 run directories recorded in
      the review entry.
- [x] Synchronized `tracking/reviews.json`, `analysis/coverage.json`, TODO5,
      TODO6, and `analysis/reconstruction/draw-popup.md`. `DrawPopup` is now
      **6/6**, with **4/4** named Step-5 units closed. The authoritative
      counters are **118/152 evidence units closed, 34 open, 10 promotion
      units, and 44 practical open work items**.

### Latest checkpoint — SatisfyOrders E3 original-instruction boundary audit

- [x] Added a machine-readable original `DoOrders` orchestrator oracle. It
      records the exact pre-/post-movement callback order, inhabited-planet
      flag update, battle-id formula, post-movement planet-turn clearing, and
      the pre-movement-only `TransferToOthers` gate. The readable source and
      the fresh native four-pass fixture agree with that sequence.
- [x] The existing strict-C89/native matrix remains **18/18 PASS**, including
      the callback/event traces, battle-id creation, order retirement, state
      mutation, message order, and post-movement suppression. Evidence is now
      explicit in
      `analysis/compat/satisfy-orders-full-turn-e3-original-oracle.{md,json}`.
- [ ] This strengthens the original-instruction boundary but is not claimed
      as the original executable's complete full-turn state/message
      differential. SatisfyOrders remains **2/3 named units at 5/6**; its
      runtime differential and Step-6 promotion remain open. Counters remain
      **118/152 closed, 34 open, 10 promotion units, and 44 practical open
      work items**.

### Latest checkpoint — ExecuteReportClick packed mineral-status E6 native support

- [x] Preserved the original packed `_planet.overlap_4` status word at offset
      `0x04` and propagated bit 10 to the mineral popup's raw
      `GlobalPD.iPlanVal` offset `0x06`. The strict-C89 packed matrix remains
      `EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`.
- [x] Replayed the current production header/source/probe in fresh Win98
      directory `C:\EXINT6`. Compile/link passed, the native report matches
      34/34, process exit was 0, and the final guest window list contained no
      Program Error dialog. Evidence is in
      `analysis/compat/execute-report-click-packed-e6-native.{md,json}`,
      `compat/vc152/execute-report-click-packed-e6-native/`, and
      `runs/vmctl-vc152exint6/`.
- [ ] This is supporting packed/native evidence, not closure of a named
      ExecuteReportClick unit. Exact GlobalPD pointer/part ownership, live
      USER/GDI report integration, complete report-layout caller coverage,
      original differential, and promotion remain open. Counters remain
      **118/152 closed, 34 open, 10 promotion units, and 44 practical open
      work items**.

### Latest checkpoint — ExecuteReportClick raw GlobalPD E1 native support

- [x] Extended the production `ExecuteReportClick` popup boundary to retain
      the complete 22-byte overlapping `GlobalPD` / `_popupdata` view. Fleet
      and starbase far-pointer words, the eight-byte defense `_part` union,
      the resource part word, and the five overlapping `rgi` long views are
      now preserved as opaque raw Win16 values; no host pointer is
      dereferenced.
- [x] Added `tests/execute_report_click_globalpd_probe.c`. The strict-C89
      host matrix reports `EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=20
      FAILURES=0 RESULT=PASS`.
- [x] Replayed the corrected current production header/source/probe in fresh
      Win98 directory `C:\GPDE2`. Compile/link passed, the native report
      matches 20/20, process exit was 0, and the final guest window list
      contained no Program Error dialog. Evidence is in
      `analysis/compat/execute-report-click-globalpd-e1-native.{md,json}`,
      `compat/vc152/execute-report-click-globalpd-e1-native-r2/`, and
      `runs/vmctl-vc152gpde2/`. The earlier `GPDE1` run is retained as
      superseded evidence because its starbase oracle predated the raw
      `iPlanVal=1` correction.
- [x] This is supporting E1 boundary evidence, not closure of a named unit;
      exact original pointer/part values and differentials, live USER/GDI
      behavior, complete report-layout caller coverage, and promotion remain
      open. The authoritative counters remain **117/152 evidence units
      closed, 35 open, 10 promotion units, and 45 practical open work items**.

### Latest checkpoint — SatisfyOrders E2 closure

- [x] Consolidated the existing production-source/native evidence into the
      authoritative SatisfyOrders E2 boundary. Technology-table/result
      binding, real research and salvage services, created-player fleet and
      design materialization, special-order gates, packed special dispatch,
      and all recovered `FSendPlrMsg`/`FSendPlrMsg2` message families now have
      independent strict-C89 and fresh VC++ 1.52c/Win98 PASS evidence. The
      corrected raw sweep covers 141/141 affected order checks, and the fuel
      and failure packets add 40 and 44 exact-payload checks.
- [x] E2 is now **2/3 authoritative units closed** and the procedure remains
      at **5/6**. The complete four-pass/original full-turn differential is
      explicitly retained as E3; Step-6 promotion remains open. The counters
      are now **117/152 evidence units closed, 35 open, 10 promotion units,
      and 45 practical open work items**. Evidence is consolidated in
      `analysis/compat/satisfy-orders-e2-closure.{md,json}` and recorded in
      the SatisfyOrders review entry.
- [x] The closure audit caught and corrected a typed-callback regression in
      the foreign-fleet colonist branch. The raw `0x0155` frame remains exact
      when `raw_message2` is installed, while the legacy `send_message`
      fallback is preserved otherwise. Host source/raw probes pass, and the
      fresh `C:\SYOE24` replay reports `12/12 PASS`, compile/link PASS, exit
      0, and no Program Error dialog. The authoritative regression evidence
      is `analysis/compat/satisfy-orders-foreign-colonist-fallback-e2-native.{md,json}`.

### Latest checkpoint — SatisfyOrders E3 full-turn integration support

- [x] Expanded the existing turn-sequencing harness into a four-pass
      pre-/post-movement state and message matrix bound through the real
      `DoOrders`/`SatisfyOrders` production sources. It checks the exact
      sequencing, battle/thing callbacks, planet and fleet mutation, order
      retirement, mine/route/player-transfer dispatch, and generic/player
      message order. The strict-C89 host probe reports
      `SATISFY_ORDERS_FULL_TURN_E3_CHECKS=18 FAILURES=0 RESULT=PASS`.
- [x] The fresh VC++ 1.52c/Win98 replay in `C:\SYFTE4` reports the same
      `18/18 PASS`, with compile/link PASS, process exit 0, and no Program
      Error dialog. The authoritative native artifacts are preserved in
      `analysis/compat/satisfy-orders-full-turn-e3-native.{md,json}`,
      `compat/vc152/satisfy-orders-full-turn-e3-r2/`, and
      `runs/vmctl-vc152syfte4/`.
- [ ] This is a supporting E3 integration subcheck, not closure of the
      named E3 unit: the original executable's complete full-turn state and
      message differential, followed by Step-6 promotion, remain open. Keep
      the authoritative counters at **117/152 evidence units closed, 35 open,
      10 promotion units, and 45 practical open work items**; SatisfyOrders
      remains **2/3 named Step-5 units closed at 5/6**.

### Latest checkpoint — DoCyberFreighter native gate/progress E5 support

- [x] Added a fresh combined strict-C89/native matrix for the corrected
      `DoCyberFreighter` boundary. It covers the owned-planet mineral-rate and
      stock threshold, pickup/drop-off transfer direction, carrying-only
      progress-bit advancement, no-destination rollback, enemy ordinary-planet
      starbase fallback, special-planet carrying, open-space short circuit,
      exact `0x11b0` movement, and packed PLORD first-order replacement.
- [x] Host and fresh VC++ 1.52c/Win98 `C:\CYBFE5` reports both show
      `DO_CYBER_FREIGHTER_E5_CHECKS=26 FAILURES=0 RESULT=PASS`; compile/link
      passed, the process exited 0, and the final window inventory contained
      no Program Error dialog. Evidence is preserved in
      `analysis/compat/ai-cyber-freighter-native-e5.{md,json}`,
      `compat/vc152/do-cyber-freighter-e5-native-r1/`, and
      `runs/vmctl-vc152cybfe5/`.
- [ ] This is native source-boundary support only. It does not close a named
      DoCyberAiTurn unit or alter the authoritative counters: complete raw
      fleet-cargo predicates, packet/freighter interaction, full AI-table
      integration, original-turn differential, and Step-6 promotion remain.

### Latest checkpoint — DrawPopup detailed text geometry/style E3 support

- [x] Added explicit x/y coordinates, left/right/center anchors, normal/bold
      font selection, opaque background mode, and optional measurement-driven
      label offsets to the typed `DrawPopup` text boundary. The strict-C89
      geometry probe passes
      `DRAW_POPUP_GEOMETRY_E3_CHECKS=25 FAILURES=0 RESULT=PASS` across popup
      kinds 1–4, hull rows, separator placement, row spacing, and the
      short-client fallback.
- [x] The final VC++ 1.52c/Win98 packet in `C:\DPE9` reports the same 25-check
      pass with compile/link and process exit success; the final window list
      contained no Program Error dialog. The first wrapper-ending attempt is
      retained as diagnostic evidence because mixed CRLF/LF batch endings
      produced no reports; corrected CRLF wrappers produced the authoritative
      replay.
- [x] Together with the native 7-check state-order probe and the fresh native
      26-check `ExecuteReportClick -> Popup -> DrawPopup` caller replay, this
      closes the named E3 detailed GDI/caller unit. DrawPopup is now **3/4**
      named Step-5 units closed at **5/6**; E4 original completion and Step-6
      promotion remain. Counters are now **116/152 evidence units closed, 36
      open, 10 promotion units, and 46 practical open work items**. Evidence
      is in `analysis/compat/draw-popup-detailed-e3.{md,json}`,
      `analysis/compat/draw-popup-geometry-e3-{host,native}.{md,json}`,
      `compat/vc152/draw-popup-geometry-e3-r2/`, and
      `runs/vmctl-vc152drawpopupgeometrye3r2/`.

### Latest checkpoint — DrawPopup localization/hull formatting E2 native replay

- [x] The readable typed text-segment boundary for popup kinds 1–4 now has a
      fresh VC++ 1.52c/Win98 replay in `C:\DPE7`. The native report is
      `DRAW_POPUP_CONTENT_E2_CHECKS=21 FAILURES=0 RESULT=PASS`; compile/link
      passed, the process exited 0, and the final guest window inventory had
      no Program Error dialog. The first compile-only link failure is retained
      separately; it was a packet-only unresolved host `printf` and was fixed
      under `TARGET_NATIVE` without changing the host oracle.
- [x] This closes DrawPopup E2, changing the row to **2/4** named Step-5
      units while the procedure remains **5/6**. E3 detailed GDI/caller
      differential, E4 original completion, and Step-6 promotion remain.
      Current counters are **115/152 evidence units closed, 37 open, 10
      promotion units, and 47 practical open work items**. Evidence is in
      `analysis/compat/draw-popup-content-e2-native.{md,json}`,
      `compat/vc152/draw-popup-content-e2/`, and
      `runs/vmctl-vc152drawpopupcontente2-r2/`.

### Supporting checkpoint — DrawPopup localization/hull formatting E2 host boundary

- [x] Added the readable typed text-segment boundary for popup kinds 1–4:
      localized string IDs, numeric format kinds, planet/fleet name seams,
      hull visibility and decoration callbacks, count/damage formatting, and
      coordinate labels. Optional raw 124-byte fleet and 147-byte hull
      definition views preserve the recovered packed offsets without
      dereferencing popup far-pointer words.
- [x] The strict-C89 host probe passes
      `DRAW_POPUP_CONTENT_E2_CHECKS=21 FAILURES=0 RESULT=PASS`; the 221-test
      common-source suite, baseline check, and `git diff --check` also pass.
- [x] The host boundary is now backed by the native replay recorded above; the
      host probe remains `DRAW_POPUP_CONTENT_E2_CHECKS=21 FAILURES=0 RESULT=PASS`.

### Latest checkpoint — DrawPopup complete packed `GlobalPD` payload E1

- [x] Expanded the production `DrawPopup` callback payload to materialize the
      complete recovered 22-byte `_popupdata` union: raw bytes, all five
      `rgi` long values, planet/player and production-counter overlays,
      near/far pointer words, damage/display flags, the eight-byte `_part`
      view, and token-summary fields. Pointer words remain raw and are not
      dereferenced by the dispatcher.
- [x] The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\DPE6`
      both report `DRAW_POPUP_PACKED_CHECKS=38 FAILURES=0 RESULT=PASS`.
      Compile/link and process exit passed, and the final guest window list
      contained no Program Error dialog. The first diagnostic replay is
      preserved separately; it exposed a test-oracle mismatch corrected before
      the final pass.
- [x] This closes DrawPopup E1, changing the row to **1/4** named Step-5
      units while the procedure remains **5/6**. E2 localization/hull
      filtering/formatting, E3 detailed GDI/caller differential, E4 original
      completion, and Step-6 promotion remain. Current counters are **114/152
      evidence units closed, 38 open, 10 promotion units, and 48 practical
      open work items**. Evidence is in
      `analysis/compat/draw-popup-globalpd-e1.{md,json}`,
      `compat/vc152/draw-popup-packed-e6-payload/`, and
      `runs/vmctl-vc152drawpopupe6/`.

### Latest checkpoint — DumpPlanets Step-6 promotion

- [x] Closed the final original-behavior E4 boundary from direct Win98
      observation: `Reports > Dump to Text File > Planet Information` wrote
      the 334-byte `GAME.pla`, displayed its success message, and completed
      without a Program Error. The artifact hash and screenshots are
      preserved in `analysis/compat/report-dump-integration.json`.
- [x] E1 localized normal serialization, E2 complete header/two-row
      differential, and E3 packed-service resolution were already closed.
      The review ledger, TODO5, TODO6, coverage, and reconstruction notes are
      synchronized; `DumpPlanets` is now **6/6 and promoted**. The current
      counters are **113/152 evidence units closed, 39 open, 10 promotion
      units, and 49 practical open work items**. Evidence is in
      `analysis/compat/dump-planets-original-behavior-e4.{md,json}`,
      `runs/vmctl-vc152current/`, and `runs/vmctl-vc152dpsor2/`.

### Latest checkpoint — DumpPlanets complete original-output E2

- [x] Added an independent full-stream differential over the production
      formatter path. The fresh strict-C89 host and VC++ 1.52c/Win98 packet in
      `C:\DPSOR2` both report
      `DUMP_PLANETS_SERIALIZATION_CHECKS=14 FAILURES=0 RESULT=PASS`, including
      the `GAME.pla` header, both rows, tabs, and CRLF terminators byte-for-byte.
- [x] Compile/link and process exit passed, and the final guest window
      inventory contained no Program Error dialog. Together with E1 localized
      normal serialization and E3 packed-service integration, `DumpPlanets`
      now has **3/4** named Step-5 units closed and remains at **5/6** only for
      the remaining original behavior boundary and final promotion. The
      current counters are **112/152 evidence units closed, 40 open, 11
      promotion units, and 51 practical open work items**. Evidence is in
      `analysis/compat/dump-planets-original-output-e2.{md,json}`,
      `compat/vc152/dump-planets-original-output-e2/`, and
      `runs/vmctl-vc152dpsor2/`.

### Latest checkpoint — DumpPlanets localized normal serialization E1

- [x] Added the production twenty-column normal-report formatter boundary to
      `stars_dump_planets`, preserving the raw callback path and making field
      order/row termination coordinator-owned while localized text remains a
      report-host service.
- [x] Strict-C89 host and fresh VC++ 1.52c/Win98 validation in `C:\DPSER1`
      both report `DUMP_PLANETS_SERIALIZATION_CHECKS=12 FAILURES=0 RESULT=PASS`.
      The matrix covers two-row raw/formatted field order, localized
      planet/owner/starbase/queue/value/defence text, the preserved `GAME.pla`
      tab/CRLF oracle, and invalid callback rejection. Compile/link and
      process exit passed; the final guest window inventory had no Program
      Error dialog.
- [x] This closes one named `DumpPlanets` Step-5 unit. Its packed-service E3
      unit was already closed; detailed/service-column serialization, complete
      original export differential, and Step-6 promotion remain. `DumpPlanets`
      is still **5/6**, now **2/4** named Step-5 units closed, and the current
      counters are **111/152 evidence units closed, 41 open, 11 promotion
      units, and 52 practical open work items**. Evidence is in
      `analysis/compat/dump-planets-serialization-e1.{md,json}`,
      `compat/vc152/dump-planets-serialization-e1/`, and
      `runs/vmctl-vc152dpser1/`.

### Latest checkpoint — NewGameWizard stale-status reconciliation

- [x] Reconciled the authoritative `tracking/reviews.json` status for
      `NewGameWizard` from stale `partial` to `manually_reviewed`. Its existing
      host/native source, packed-player, 45-control resource, complete-creation
      replay, and original Win98 evidence already recorded
      `phase2_step5_complete; step6_promotion_recorded`.
- [x] Regenerated coverage and TODO5. The authoritative counters are now
      **110/152 evidence units closed, 42 open, 11 promotion units, and 53
      practical open work items**; NewGameWizard is consistently recorded as
      **6/6** across TODO5, TODO6, the reconstruction note, and the coverage
      ledger.

### Latest checkpoint — DumpFleets Step-6 promotion

- [x] Promoted `DumpFleets` after closing all four named Step-5 evidence
      units: localized scanner/task serialization, serialized field order,
      packed service resolution, and complete original `GAME.fle` output
      differential.
- [x] Synchronized `tracking/reviews.json`, TODO5, TODO6, the reconstruction
      note, compatibility evidence, and the regenerated coverage ledgers. The
      authoritative counters are now **110/152 evidence units closed, 42 open,
      12 promotion units, and 54 practical open work items**. `DumpFleets` is
      now **6/6**.

### Latest checkpoint — DumpFleets complete original-output E4

- [x] Expanded the formatted production serialization probe from the first-row
      oracle to all six fleet rows in the preserved original export. The
      complete header/data/CRLF stream now compares byte-for-byte with
      `runs/vmctl-vc152current/GAME.fle`.
- [x] Strict-C89 host and fresh VC++ 1.52c/Win98 validation in `C:\DFSER4`
      both report `DUMP_FLEETS_SERIALIZATION_CHECKS=26 FAILURES=0 RESULT=PASS`.
      Compile, link, and process exit passed; the final guest window inventory
      contained no Program Error dialog.
- [x] This closes the named complete original-output E4 unit. E1 localized
      scanner/task serialization, E2 serialized field order, and E3 packed
      service integration are also closed, so DumpFleets is now **4/4** on
      named Step-5 evidence. It remains at **5/6** only because Step-6 ledger
      promotion is still pending. The authoritative counters are now
      **110/152 closed, 42 open, 13 promotion units, and 55 practical open
      work items**. Evidence is in
      `analysis/compat/dump-fleets-serialization-e4.{md,json}`,
      `compat/vc152/dump-fleets-serialization-e4/`, and
      `runs/vmctl-vc152dfser4/`.

### Latest checkpoint — DumpFleets localized formatting/original-row E1

- [x] Extended the production `stars_dump_fleets` serialization boundary with
      formatted text callbacks while preserving the raw field-order path. The
      host matrix now checks localized fleet/planet/destination/battle-plan
      text, numeric formatting, the preserved original `GAME.fle` first-row
      text oracle, and detailed task/scanner fields.
- [x] Strict-C89 host and fresh VC++ 1.52c/Win98 validation in `C:\DFSER4`
      both report `DUMP_FLEETS_SERIALIZATION_CHECKS=23 FAILURES=0 RESULT=PASS`.
      Compile, link, and process exit passed; the final guest window inventory
      contained no Program Error dialog.
- [x] This closes the named localized scanner/task serialization E1 unit.
      E2 serialized field order and E3 packed service integration were already
      closed; E4 complete original export differential and Step-6 promotion
      remain. `DumpFleets` therefore remains at **5/6**, with **3/4** named
      Step-5 evidence units closed and **1 + P** remaining. The authoritative
      counters are now **109/152 closed, 43 open, 13 promotion units, and 56
      practical open work items**. Evidence is in
      `analysis/compat/dump-fleets-serialization-e4.{md,json}`,
      `compat/vc152/dump-fleets-serialization-e4/`, and
      `runs/vmctl-vc152dfser4/`.

### Latest checkpoint — DumpFleets serialized field-order E2

- [x] Added the production `write_text_field`/`write_text_end` boundary.
      `stars_dump_fleets` now owns the recovered 12-field normal sequence and
      29-field detailed sequence while localization and numeric formatting
      remain callback-owned.
- [x] Strict-C89 host and fresh Win98 validation in `C:\DFSER4` both report
      `DUMP_FLEETS_SERIALIZATION_CHECKS=14 FAILURES=0 RESULT=PASS`. Compile,
      link, and process exit passed; the final guest window inventory had no
      Program Error dialog.
- [x] This closes the named serialized field-order E2 unit for `DumpFleets`.
      Localized text formatting, original export comparison, and promotion
      remain open. The authoritative counters are now **108/152 closed, 44
      open, 13 promotion units, and 57 practical open work items**. Evidence
      is in `analysis/compat/dump-fleets-serialization-e4.{md,json}`,
      `compat/vc152/dump-fleets-serialization-e4/`, and
      `runs/vmctl-vc152dfser4/`.

### Latest checkpoint — DumpPlanets and DumpFleets packed service-resolution E3

- [x] Added the production `resolve_services` boundary to both packed report
      bridges. `stars_dump_planets_packed` invokes it after the 56-byte
      `_planet` decode; `stars_dump_fleets_packed` invokes it after the
      124-byte `_fleet` decode, before their ordinary report gates.
- [x] Strict-C89 host and fresh Win98 runs both pass:
      `DUMP_PLANETS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS` in `C:\DPSE3`
      and `DUMP_FLEETS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS` in
      `C:\DFSE3`. Both compile/link and exit cleanly, with no Program Error
      dialog in the final window inventory.
- [x] This closes one named packed-service integration unit for each report
      procedure. Localized names/formatting, complete serialized output,
      original row-output differentials, and promotion remain open. The
      authoritative counters are now **107/152 closed, 45 open, 13 promotion
      units, and 58 practical open work items**. Evidence is in
      `analysis/compat/dump-planets-services-e3.{md,json}`,
      `analysis/compat/dump-fleets-services-e3.{md,json}`,
      `compat/vc152/dump-planets-services-e3/`,
      `compat/vc152/dump-fleets-services-e3/`,
      `runs/vmctl-vc152dpse3/`, and `runs/vmctl-vc152dfse3/`.

### Latest checkpoint — ExecuteReportClick production packed bridge E5 native replay

- [x] Fresh Win98 directory `C:\EXINT5` compiled and linked the current
      production `execute_report_click.c`, packed probe, and `core.h`. The
      native report is `EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0
      RESULT=PASS`; the process exited 0 and the final guest window list had no
      Program Error dialog.
- [x] The replay covers production `_fleet`/`_planet`/`_btldata` decoding,
      fleet/planet/battle selection, popup unions for fleet, installation,
      mineral, resource, starbase, planet, and defense cells, the
      resource-exempt bit, and production/transfer modal guards.
- [x] This closes the native production-lookup subcheck, not a named
      ExecuteReportClick Step-5 unit. Exact original `GlobalPD` owner/part
      cases, live USER/GDI handles, complete report caller/layout integration,
      original behavior, and promotion remain open. The authoritative counters
      remain **105/152 closed, 47 open, 13 promotion units, and 60 practical
      open work items**. Evidence is in
      `analysis/compat/execute-report-click-packed-e5-native.{md,json}`,
      `compat/vc152/execute-report-click-packed-e5-native/`, and
      `runs/vmctl-vc152exint5/`.

### Latest checkpoint — DrawPopup packed-selector E5 native replay

- [x] The current production `DrawPopup` source and 22-byte `_popupdata`
      selector probe were replayed in fresh Win98 directory `C:\DPE5R`.
      VC++ 1.52c compile/link passed, the process exited 0, and the native
      report is `DRAW_POPUP_PACKED_CHECKS=37 FAILURES=0 RESULT=PASS`; the final
      guest window list contained no Program Error dialog.
- [x] The matrix covers selector precedence, all dispatcher kinds, kind-14
      slot/hull aliasing, client/slot geometry, GDI-state callback order, and
      short-record fallback through the production bridge. The first packet
      attempt is preserved in `runs/vmctl-vc152dpe5/` as a compile-only
      `core.h` transfer failure; the corrected replay is authoritative.
- [x] This closes the native replay subcheck, not a named DrawPopup Step-5
      unit. Complete `GlobalPD` union payloads, localization, hull filtering/
      formatting, detailed GDI rendering, original caller differential, and
      promotion remain open. The authoritative counters remain **105/152
      closed, 47 open, 13 promotion units, and 60 practical open work items**.
      Evidence is in `analysis/compat/draw-popup-packed-e5-native.{md,json}`,
      `compat/vc152/draw-popup-packed-e5r-native/`, and
      `runs/vmctl-vc152dpe5r/`.

Thus the headline `5/6` value is currently an **active-gate label**, not a
completed-step count. The useful numbers that can increase before a promotion
are the evidence milestones and the number of remaining concerns in each
matrix row; the `5/6` distribution changes only when a row's entire step-5
boundary is closed and its step-6 ledger promotion is recorded.

The fraction is a gate count, not a percentage estimate. `5/6` means that
steps 1–5 have evidence; it does not mean that only one-sixth of the remaining
engineering effort is left. For UI, packed-record, AI, and turn procedures,
step 5 can still contain several dependent integration tasks. Step 6 is the
final synchronization and promotion gate, so no procedure is complete while
its matrix row is below `6/6`.

### Latest checkpoint — SatisfyOrders raw transfer-failure E2 replay

- [x] Added the caller-resolved `stars_satisfy_raw_failure_message` boundary
      for the recovered set-amount `0x0121`, set-colonists `0x0122`, deep-space
      colonist `0x0165`, and futile mineral-packet `0x011e` frames. The first
      two preserve all eight `FSendPlrMsg` payload words; the latter two
      preserve all three `FSendPlrMsg2` payload words.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE22` replay both report
      `SATISFY_ORDERS_RAW_FAILURE_CHECKS=44 FAILURES=0 RESULT=PASS`; compile/
      link passed, the process exited 0, and no Program Error dialog was in the
      final guest window list.
- [x] This closes a supporting transfer-failure ABI boundary, not the named
      SatisfyOrders E2 unit. The caller's packed action-union conditions,
      original full-turn transfer differential, E3 replay, and promotion remain
      open. The authoritative counters remain **105/152 closed, 47 open, 13
      promotion units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-failure-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-failure-e2-native/`, and
      `runs/vmctl-vc152syoe22/`.

### Latest checkpoint — SatisfyOrders raw resource-transfer E2 replay

- [x] Added the production `raw_message` mapping for standard successful
      `FSendPlrMsg` resource frames: planet mineral `0x002d`, planet colonist
      `0x002e`, same-owner fleet mineral `0x002b`, and same-owner fleet
      colonist `0x002c`. Caller-resolved target word/x fields preserve all
      eight payload words, including the zero tail.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE19` replay both report
      `SATISFY_ORDERS_RAW_RESOURCE_CHECKS=24 FAILURES=0 RESULT=PASS`.
      Compile/link passed, the process exited 0, and the final guest window
      inventory had no Program Error dialog. The native compile log records
      one non-fatal C4018 warning from the combined legacy translation unit.
- [x] The standard payload mapping agrees with the recovered successful
      load/unload calls. This closes a supporting ordinary-transfer message
      boundary, not the named E2 unit. Foreign-fleet colonist, fuel/optimal-
      fuel, permission/failure families, the complete original transfer
      differential, E3 full-turn/original comparison, and promotion remain
      open. The authoritative counters remain **105/152 closed, 47 open, 13
      promotion units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-resource-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-resource-e2-native/`, and
      `runs/vmctl-vc152syoe19/`.

### Latest checkpoint — SatisfyOrders raw technology frame E2 replay

- [x] Added the production handoff from signed technology-result binding to
      the exact `raw_message` ABI. The caller supplies the raw recipient and
      eight-word frame through `StarsSatisfyTechnologyOutcome`; the resolver
      forwards it whenever the result selects a nonzero technology message.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE18` replay both report
      `SATISFY_ORDERS_RAW_TECHNOLOGY_CHECKS=16 FAILURES=0 RESULT=PASS`.
      Compile/link passed, the process exited 0, and the final guest window
      inventory had no Program Error dialog. Signed negative point (`0x013c`)
      and positive planet (`0x013f`) results, recipient, all eight payloads,
      result id, amount, and order completion were checked.
- [x] This closes the raw technology-frame transport boundary while retaining
      the distinction between caller-resolved payload forwarding and complete
      original field derivation. Successful ordinary-transfer layouts, the
      complete original transfer differential, E3 full-turn/original
      comparison, and promotion remain open. The authoritative counters
      remain **105/152 closed, 47 open, 13 promotion units, and 60 practical
      open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-technology-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-technology-e2-native/`, and
      `runs/vmctl-vc152syoe18/`.

### Latest correction — SatisfyOrders technology/resource raw-frame de-duplication

- [x] The consolidated host replay exposed one regression after the ordinary
      resource-frame callback was added: technology orders emitted both the
      ordinary transfer frame and their dedicated signed-result frame.
- [x] Guarded the ordinary raw resource callback with `technology_flag`, then
      reran all eight affected raw-order probes: resource 24, technology 16,
      colonist 12, shortage 12, merge 12, mine 12, route 20, and player
      transfer 21 checks; **141/141 passed** across the nine raw-order probes.
- [x] Rebuilt and replayed the corrected source in fresh `C:\SYOE18`; the
      technology report is `16/16 PASS`, compile/link passed, the process
      exited 0, and the guest had no Program Error dialog. This corrects the
      transport boundary but closes no named Step-5 unit. The authoritative
      counters remain **105/152 closed, 47 open, 13 promotion units, and 60
      practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-technology-e2-native.{md,json}` and
      `runs/vmctl-vc152syoe18-r2/`.

### Latest checkpoint — SatisfyOrders foreign-fleet colonist rejection E2 replay

- [x] Added the recovered foreign-fleet colonist permission branch: target
      fleets owned by another player emit `FSendPlrMsg2(0x0155, fleet-marker,
      fleet-id, source-current-planet-id)`, perform no transfer, and cancel
      the order.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE20` replay both report
      `SATISFY_ORDERS_RAW_FOREIGN_COLONIST_CHECKS=12 FAILURES=0 RESULT=PASS`;
      compile/link passed, the process exited 0, and no Program Error dialog
      was present. The combined raw-order host sweep now passes **141/141**
      checks across nine probes.
- [x] Corrected an enum transcription discovered during source audit:
      `idmAllowedTransferColonistsAnotherPlayer` is `0x0155` (`0x0119` is
      `idmHasStolen`). Fresh host and Win98 reruns pass 12/12 with the corrected
      code; the old packet remains preserved as superseded evidence.
- [x] This closes a supporting permission-message boundary, not the named
      Step-5 unit. Fuel/optimal-fuel, remaining permission/failure families,
      original transfer differential, E3 comparison, and promotion remain
      open. The authoritative counters remain **105/152 closed, 47 open, 13
      promotion units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-foreign-colonist-e2-native.{md,json}`
      and `runs/vmctl-vc152syoe20-r2/`.

### Latest checkpoint — SatisfyOrders fuel and optimal-fuel E2 replay

- [x] Added the caller-resolved fuel-frame builder for signed optimal-fuel
      adjustments (`0x002b`/`0x002d`) and recovered special messages:
      failed-load `0x0126`, capacity `0x003d`, and insufficient-fuel `0x003c`.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE21` replay both report
      `SATISFY_ORDERS_RAW_FUEL_CHECKS=40 FAILURES=0 RESULT=PASS`; compile/link
      passed, the process exited 0, and the guest had no Program Error dialog.
- [x] This closes a supporting raw fuel ABI boundary, not the named Step-5
      unit. Upstream route-engine fuel calculation/pass gating, remaining
      permission/failure families, original differential, E3 comparison, and
      promotion remain open. The authoritative counters remain **105/152
      closed, 47 open, 13 promotion units, and 60 practical open work items**.
      Evidence is in
      `analysis/compat/satisfy-orders-raw-fuel-e2-native.{md,json}` and
      `runs/vmctl-vc152syoe21/`.

### Latest checkpoint — SatisfyOrders raw colonist rejection E2 replay

- [x] Added the production `raw_message2` mapping for colonist-drop rejection:
      unowned/uninhabited `0x0055`, race-specific `0x0056`, and
      planet-blocked `0x0135`. Each exact three-payload message preserves the
      source fleet marker, raw fleet id, and target planet id.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE17` replay both report
      `SATISFY_ORDERS_RAW_COLONIST_CHECKS=12 FAILURES=0 RESULT=PASS`.
      Compile/link passed, the process exited 0, and the final guest window
      inventory had no Program Error dialog.
- [x] The mapping agrees with the recovered original colonist-drop gates. This
      closes a supporting raw colonist rejection message-layout boundary, not
      the named E2 unit. Successful transfer layouts, the technology message
      family, the complete original transfer differential, E3 full-turn/
      original comparison, and promotion remain open. The authoritative
      counters remain **105/152 closed, 47 open, 13 promotion units, and 60
      practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-colonist-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-colonist-e2-native/`, and
      `runs/vmctl-vc152syoe17/`.

### Latest checkpoint — SatisfyOrders raw shortage message E2 replay

- [x] Added the production `raw_message2` mapping for pass-four resource
      shortages: planet `0x011f`, fleet `0x0120`, and point `0x0123`. Each
      exact three-payload message preserves the source fleet marker, raw fleet
      id, and resource index.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE16` replay both report
      `SATISFY_ORDERS_RAW_SHORTAGE_CHECKS=12 FAILURES=0 RESULT=PASS`.
      Compile/link passed, the process exited 0, and the final guest window
      inventory had no Program Error dialog.
- [x] The mapping agrees with the recovered original pass-four branch. This
      closes a supporting raw shortage message-layout boundary, not the named
      E2 unit. Successful resource-transfer layouts, colonist/technology
      message families, the complete original transfer differential, E3
      full-turn/original comparison, and promotion remain open. The
      authoritative counters remain **105/152 closed, 47 open, 13 promotion
      units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-shortage-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-shortage-e2-native/`, and
      `runs/vmctl-vc152syoe16/`.

### Latest checkpoint — SatisfyOrders raw merge message E2 replay

- [x] Added the production `raw_message2` mapping for merge outcomes from the
      original `FSendPlrMsg2` call: same-owner success `0x00f7`, enemy
      destination `0x00f6`, and absent/inactive destination `0x00f5`. The
      success payload preserves the destination marker, source `WFromLpfl`,
      and destination id; the rejection payloads preserve source identity/id
      and zero.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE15` replay both report
      `SATISFY_ORDERS_RAW_MERGE_CHECKS=12 FAILURES=0 RESULT=PASS`. Compile/link
      passed, the process exited 0, and the final guest window inventory had
      no Program Error dialog.
- [x] The payload mapping agrees with both the raw decompiler and the reviewed
      `turn3.c` reference. This closes a supporting raw merge message-layout
      boundary, not the named E2 unit. Ordinary resource-transfer layouts,
      shortage/colonist/technology message families, the complete original
      transfer differential, E3 full-turn/original comparison, and promotion
      remain open. The authoritative counters remain **105/152 closed, 47
      open, 13 promotion units, and 60 practical open work items**. Evidence
      is in `analysis/compat/satisfy-orders-raw-merge-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-merge-e2-native/`, and
      `runs/vmctl-vc152syoe15/`.

### Latest checkpoint — SatisfyOrders raw mining message E2 replay

- [x] Added the production `raw_message2` mapping for mining failures:
      no-planet `0x0077`, empty-source `0x0075`, and hostile-planet `0x0076`.
      The exact fleet marker, fleet id, optional planet id, and message-free
      success path are preserved.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE14` replay both report
      `SATISFY_ORDERS_RAW_MINE_CHECKS=12 FAILURES=0 RESULT=PASS`. Compile/link
      passed, the process exited 0, and the final guest window inventory had
      no Program Error dialog.
- [x] This closes a supporting raw mining message-layout boundary, not the
      named E2 unit. Ordinary resource-transfer layouts, shortage/merge/
      colonist/technology message families, the complete original transfer
      differential, E3 full-turn/original comparison, and promotion remain
      open. The authoritative counters remain **105/152 closed, 47 open, 13
      promotion units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-mine-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-mine-e2-native/`, and
      `runs/vmctl-vc152syoe14/`.

### Latest checkpoint — SatisfyOrders raw auto-route message E2 replay

- [x] Added the production `raw_message` boundary for the original
      eight-payload `FSendPlrMsg` auto-route messages. The typed fleet view
      carries raw `idPlanet` and the order view preserves the starbase word
      used by the original `(lStarbase & 0x3ff) - 1` payload calculation.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE13` replay both report
      `SATISFY_ORDERS_RAW_ROUTE_CHECKS=20 FAILURES=0 RESULT=PASS`. The matrix
      checks both message variants, all eight payloads, identity markers, and
      trailing zero fields. Compile/link passed, the process exited 0, and the
      final guest window inventory had no Program Error dialog.
- [x] This closes a supporting raw auto-route message-layout boundary, not the
      named E2 unit. Ordinary resource-transfer layouts, other message
      families, the complete original transfer differential, E3 full-turn/
      original comparison, and promotion remain open. The authoritative
      counters remain **105/152 closed, 47 open, 13 promotion units, and 60
      practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-route-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-route-e2-native/`, and
      `runs/vmctl-vc152syoe13/`.

### Latest checkpoint — SatisfyOrders raw player-transfer message E2 replay

- [x] Added the production `raw_message2` boundary for the original
      three-payload `FSendPlrMsg2` player-transfer messages. The typed view now
      carries caller-resolved `WFromLpfl` words and the created target fleet's
      raw identity, avoiding guesses from the reduced record model.
- [x] The strict-C89 host probe and fresh Win98 `C:\SYOE12` replay both report
      `SATISFY_ORDERS_RAW_TRANSFER_CHECKS=21 FAILURES=0 RESULT=PASS`. The matrix
      checks all three payloads for created source/recipient, no-design source/
      recipient, no-ship, blocked, and invalid-target outcomes. Compile/link
      passed, the process exited 0, and the final guest window inventory had
      no Program Error dialog.
- [x] This closes a supporting raw player-transfer message-layout boundary,
      not the named E2 unit. Ordinary eight-payload `FSendPlrMsg` layouts,
      other message families, the complete original transfer differential,
      E3 full-turn/original comparison, and promotion remain open. The
      authoritative counters remain **105/152 closed, 47 open, 13 promotion
      units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-raw-transfer-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-raw-transfer-e2-native/`, and
      `runs/vmctl-vc152syoe12/`.

### Latest checkpoint — SatisfyOrders technology-table/result binding E2 native replay

- [x] Bound the resource-resolution technology branch to the production
      `stars_itech_learn_a_tech` result through
      `stars_bind_satisfy_technology`. Point destinations use message codes
      `0x013c`/`0x013d`; planet destinations use `0x013e`/`0x013f`; signed
      negative and positive results map to trader-part and battle-technology
      IDs respectively.
- [x] Host and fresh Win98 `C:\SYOE11` validation report
      `SATISFY_ORDERS_TECHNOLOGY_BINDING_E2_CHECKS=12 FAILURES=0 RESULT=PASS`.
      Compile/link passed, the process exited 0, and the final guest window
      inventory contained no Program Error dialog. The final packet uses `/O`.
      The earlier dialog was traced to an omitted `SYO11.DEF` line in the
      response file; adding it restored the required Win16 stack/heap/stub
      directives.
- [x] This closes the signed technology-result/message-code boundary as
      supporting E2 evidence but not the named E2 unit. Complete raw message
      layouts, original transfer differential, and E3 full-turn/original
      differential remain. Evidence is in
      `analysis/compat/satisfy-orders-technology-binding-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-technology-binding-e2-native/`, and
      `runs/vmctl-vc152syoe11/`. The authoritative counters remain
      **105/152 closed, 47 open, 13 promotion units, and 60 practical open
      work items**.

### Latest checkpoint — SatisfyOrders player-design materialization E2 native replay

- [x] Added and wired the production player-specific design-materialization
      boundary in the created-player-transfer branch. It reuses equivalent
      active destination designs, allocates the first hidden slot for an
      unmatched design, copies the complete 0x93-byte record, rewrites the
      destination identity flags, clears the two accumulated design totals,
      and transfers per-design fleet count/status and both 32-bit totals.
- [x] Host and fresh Win98 `C:\SYOE10` validation report
      `SATISFY_ORDERS_DESIGN_MATERIALIZATION_E2_CHECKS=15 FAILURES=0 RESULT=PASS EXIT=0`.
      Compile/link pass, and the final guest window inventory contains no
      Program Error dialog. The packet uses production `common.c` and
      `satisfy_orders.c`; `/O` is a packet-only setting needed to stay below
      the VC++ 1.52c code-segment limit.
- [x] This closes the player-specific design-record materialization/copying
      boundary as supporting E2 evidence but not the named E2 unit. Original
      technology-table/result binding, complete raw message layouts, original
      transfer differential, and E3 full-turn/original differential remain.
      The authoritative counters remain **105/152 closed, 47 open, 13
      promotion units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-design-materialization-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-design-materialization-e2-native/`, and
      `runs/vmctl-vc152syoe10/`.

### Latest checkpoint — SatisfyOrders fleet/design copy E2 native replay

- [x] Added a distinct created-player-transfer fleet/design service boundary.
      The callback invokes the production `stars_find_similar_design` helper
      for matching and no-match selection, then invokes `stars_duplicate_fleet`
      from `common.c` and verifies the complete fixed 124-byte fleet record,
      destination allocation, source preservation, and the variable 18-byte
      order-record tail.
- [x] Host and fresh Win98 `C:\SYOE9` validation report
      `SATISFY_ORDERS_FLEET_COPY_E2_CHECKS=10 FAILURES=0 RESULT=PASS EXIT=0`.
      Compile/link pass, and the final guest window inventory contains no
      Program Error dialog. The native packet uses small stubs only for
      unrelated `common.c` helpers because the full `utilgen.c` exceeds the
      VC++ 1.52c code-segment limit; the tested fleet-copy function is the
      production implementation.
- [x] This closes the design-selection helper and a concrete fleet-state-copy
      boundary but not the named E2 unit. Player-specific ship-design record
      materialization/copying, original technology-table/result binding,
      complete raw message layouts, original transfer differential, and E3
      full-turn/original differential remain. The
      authoritative counters remain **105/152 closed, 47 open, 13 promotion
      units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-fleet-copy-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-fleet-copy-e2-native/`, and
      `runs/vmctl-vc152syoe9/`.

### Latest checkpoint — SatisfyOrders concrete production-service E2 replay

- [x] Bound the SatisfyOrders technology and point-drop callbacks to the
      existing production implementations. The typed path invokes
      `stars_itech_learn_a_tech` and checks research/event/result state; the
      point-drop path invokes `stars_drop_salvage` and checks the actual
      two-object mineral split, values, and coordinates. The packed path
      repeats the technology service through raw 124-byte `_fleet` and
      18-byte `_order` records.
- [x] Host and fresh Win98 `C:\SYOE8` validation report
      `SATISFY_ORDERS_PRODUCTION_E2_CHECKS=18 TYPED=12 PACKED=6 FAILURES=0
      RESULT=PASS`; the native wrapper records `EXIT=0`. Compile/link pass,
      and the final guest window inventory contains no Program Error dialog.
- [x] This is stronger supporting E2 evidence, not closure of the named E2
      unit. Original technology-table/result binding, actual fleet/ship-design
      copying, complete raw message argument layouts, original
      destruction-to-salvage caller comparison, and E3 full-turn/original
      differential remain. The authoritative counters remain **105/152
      closed, 47 open, 13 promotion units, and 60 practical open work items**.
      Evidence is in
      `analysis/compat/satisfy-orders-production-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-production-e2-native/`, and
      `runs/vmctl-vc152syoe8/`.

### Latest checkpoint — SatisfyOrders advanced E2 native boundary replay

- [x] Added explicit production-source service boundaries for technology
      discovery, created-transfer ship-copy completion, point-drop salvage
      handoff, and richer ordinary/player-transfer message-layout tracing.
      The strict-C89 host and fresh Win98 `C:\SYOE7` packet both report
      `SATISFY_ORDERS_ADVANCED_E2_CHECKS=20 FAILURES=0 RESULT=PASS`.
- [x] The matrix covers typed and packed dispatch, transfer totals,
      technology amount, salvage mineral vectors, created-transfer recipient
      and message-code pairs, no-design suppression, and packed technology-flag
      propagation. Compile/link and process exit pass, and the final guest
      window inventory contains no Program Error dialog.
- [x] This is a supporting E2 milestone, not closure of the named E2 unit:
      original technology-table/results, actual fleet/ship-design record copy,
      concrete salvage object creation/placement, complete raw message
      argument layouts, and E3 full-turn/original differential remain. The
      authoritative counters remain **105/152 closed, 47 open, 13 promotion
      units, and 60 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-advanced-e2-native.{md,json}`,
      `compat/vc152/satisfy-orders-advanced-e2-native/`, and
      `runs/vmctl-vc152syoe7/`.

### Latest checkpoint — SatisfyOrders packed resource E1 native replay

- [x] Fresh host and Win98 production-source validation in `C:\SYOE6` both
      report `SATISFY_ORDERS_PACKED_RESOURCE_E1_CHECKS=21 FAILURES=0 RESULT=PASS`.
      The raw 124-byte `_fleet`/18-byte `_order` matrix closes E1 packed
      battle-plan/resource accounting: capacity and source limits, target-kind
      restrictions, exact messages, stat-percent mode, write-back, and
      battle-plan dispatch all pass; compile/link and process exit pass, with
      no Program Error dialog.
- [x] Updated the reconstruction note, review ledger, coverage, and TODO6
      progress. SatisfyOrders is now **1/3 named Step-5 units closed** and
      remains at **5/6**. Current counters are **105/152 evidence units
      closed, 47 open, 13 promotion units, and 60 practical open work items**.
      E2 technology/ship-copy/salvage/message-layout work and E3 full-turn/
      original differential remain. Evidence is in
      `analysis/compat/satisfy-orders-packed-resource-e1-native.{md,json}`,
      `compat/vc152/satisfy-orders-packed-resource-e1-native/`, and
      `runs/vmctl-vc152syoe6/`.

### Latest checkpoint — DrawPlanShipBitmap planet-window E4 native replay and promotion

- [x] Replayed the current production `PlanetWndProc` WM_PAINT path directly
      into `DrawPlanShipBitmap` in fresh guest directory `C:\DPSBE4`. The
      12-check matrix verifies begin-paint/planet-paint/end-paint ordering,
      packed fleet and planet selection forwarding, exact localized tile names,
      fleet dispatch, ten planet-frame pattern operations, DIB source
      coordinates, and no-buttons positioning suppression.
- [x] Host and native reports are both
      `DRAW_PLAN_SHIP_BITMAP_PLANET_WINDOW_E4_CHECKS=12 FAILURES=0 RESULT=PASS`;
      compile/link passed, the process exited 0, and the final guest window
      list contained no Program Error dialog. Combined with the original
      Win98 planet/fleet frame and map-selection refresh evidence, this closes
      DrawPlanShipBitmap E4.
- [x] Synchronized the E4 evidence packet, reconstruction note, review
      record, coverage ledger, TODO5/TODO6, and current handoff. DrawPlanShipBitmap
      is now **6/6 and promoted**. Current counters are **38/51 procedures at
      6/6, 13 at 5/6, 104/152 evidence units closed, 48 open, 13 promotion
      units, and 61 practical open work items**. Evidence is in
      `analysis/compat/draw-plan-ship-bitmap-planet-window-e4-native.{md,json}`,
      `compat/vc152/draw-plan-ship-bitmap-planet-window-e4-native/`, and
      `runs/vmctl-vc152dpsbe4/`.

### Latest checkpoint — DrawPlanShipBitmap packed-selection E2 native replay

- [x] Added a production-source 226-byte `_sel` bridge that decodes the
      original `grobj`/`id` words at offsets 4/8 and rejects undersized records
      before dispatch.
- [x] The strict-C89 and fresh Win98 replay in `C:\DPSBE2` both report
      `DRAW_PLAN_SHIP_BITMAP_PACKED_E2_CHECKS=11 FAILURES=0 RESULT=PASS`;
      compile/link passed, the process exited 0, and the final guest window
      list contained no Program Error dialog. This closes DrawPlanShipBitmap
      E2.
- [x] DrawPlanShipBitmap remains at **5/6**, with E3 localized-name behavior
      and E4 complete planet-window/original differential integration open.
      Current counters are **102/152 evidence units closed, 50 open, 14
      promotion units, and 64 practical open work items**. Evidence is in
      `analysis/compat/draw-plan-ship-bitmap-packed-e2-native.{md,json}`,
      `compat/vc152/draw-plan-ship-bitmap-packed-e2-native/`, and
      `runs/vmctl-vc152dpsbe2/`.

### Latest checkpoint — DrawPlanShipBitmap localized-name E3 native replay

- [x] Replayed the current production source through the packed selection
      boundary in fresh guest directory `C:\DPSBE3`. The 11-check matrix
      verifies exact fleet and planet localized strings, correct name-service
      selection and IDs, callback-before-tile ordering, and packed kind/ID
      forwarding.
- [x] Host and native reports are both
      `DRAW_PLAN_SHIP_BITMAP_LOCALIZED_E3_CHECKS=11 FAILURES=0 RESULT=PASS`;
      compile/link passed, the process exited 0, and the final guest window
      list contained no Program Error dialog. This closes DrawPlanShipBitmap
      E3.
- [x] DrawPlanShipBitmap remains at **5/6**, with only E4 complete
      planet-window/original differential integration and final promotion open.
      Current counters are **103/152 evidence units closed, 49 open, 14
      promotion units, and 63 practical open work items**. Evidence is in
      `analysis/compat/draw-plan-ship-bitmap-localized-e3-native.{md,json}`,
      `compat/vc152/draw-plan-ship-bitmap-localized-e3-native/`, and
      `runs/vmctl-vc152dpsbe3/`.

### Latest checkpoint — ReportDlg Step-6 promotion

- [x] With E1 packed-view loading, E2 populated rows/live USER-GDI-menu
      payloads, and E3 native/original caller integration all closed, the
      ReportDlg review record is now synchronized as **manually reviewed**.
- [x] Updated the coverage ledger, TODO5, TODO6, and reconstruction handoff;
      ReportDlg is now **6/6 and promoted**. The current counters are
      **37/51 procedures at 6/6, 14 at 5/6, 101/152 evidence units closed,
      51 evidence units open, 14 promotion units, and 65 practical open work
      items** at that earlier checkpoint.

### Latest checkpoint — ReportDlg populated E2 native replay

- [x] Replayed the current production `ReportDlg` plus `DrawReport` and
      `DrawReportItem` sources in fresh VC++ 1.52c/Win98 directory `C:\RPE2`.
      The 43-check matrix covers populated planet, fleet, scan-fleet, and
      battle rows, packed-view loading, scrollbar/DC/paint callbacks, header
      and text/GDI payloads, row lookup, and frame-menu cleanup.
- [x] The native report is
      `REPORT_DLG_POPULATED_E2_CHECKS=43 FAILURES=0 RESULT=PASS`; compile,
      link, and process exit passed, and the final guest window list had no
      Program Error dialog. This closes the named ReportDlg E2 unit.
- [x] ReportDlg now has **3/3 named Step-5 units closed** but remains at the
      `5/6` procedure gate because final Step-6 ledger promotion is separate.
      The authoritative burn-down is now **101/152 evidence units closed,
      51 open, 15 promotions, 66 practical open work items**. Evidence is in
      `analysis/compat/report-dlg-populated-e2-native.{md,json}`,
      `compat/vc152/report-dlg-populated-e2-native/`, and
      `runs/vmctl-vc152rptpe2/`.

### Latest checkpoint — ReportDlg populated caller-chain E3 native replay

- [x] Replayed the populated `New Eden` report row through the current
      production `ReportDlg` → `DrawReport` → `DrawReportItem` chain in fresh
      VC++ 1.52c/Win98 directory `C:\RPTDINT`. The native report is
      `REPORT_DLG_DRAW_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS`; compile,
      link, and process exit all passed, and the final guest window list had
      no Program Error dialog.
- [x] Combined with the original Reports-menu → Planet Summary Report →
      `Planet Name` header-popup observation, this closes ReportDlg E3
      native/original caller integration. ReportDlg is now **2/3 named
      Step-5 units closed** while remaining at the `5/6` procedure gate.
- [x] E2 exact populated report unions and live USER/GDI/menu payloads, plus
      final Step-6 promotion, remain open. The authoritative burn-down is now
      **100/152 evidence units closed, 52 open, 15 promotions, 67 practical
      open work items**. Evidence is preserved in
      `analysis/compat/report-dlg-draw-integration-e3-native.{md,json}`,
      `compat/vc152/report-dlg-draw-integration-e3-native/`, and
      `runs/vmctl-vc152rptdint/`.

### Latest checkpoint — EstimateItemProdSched E1 original per-turn differential and promotion

- [x] Completed the original-instruction-derived 8-check per-turn trace for
      queue order, phase-5 turn break/resumption, first/last completion turns,
      mineral/resource callback counts, population advance, special-item
      rejection, and the 100-turn cutoff.
- [x] Fresh `C:\EIPOD` VC++ 1.52c/Win98 validation reports
      `COMPILE=PASS`, `LINK=PASS`, and
      `ESTIMATE_ITEM_PROD_SCHED_ORIGINAL_DIFFERENTIAL_CHECKS=8 FAILURES=0 RESULT=PASS`;
      the process exited 0 and the final guest window list contained no
      Program Error dialog.
- [x] Closed E1 and synchronized Step-6 promotion. `EstimateItemProdSched`
      is now `6/6`; its E2 packed mutation and E3 resource/queue units were
      already closed. The authoritative burn-down is now **99/152 evidence
      units closed, 53 open, 15 promotions, 68 practical open work items**.
      Evidence is in
      `analysis/compat/estimate-item-prod-sched-original-differential-e1-native.{md,json}`,
      `compat/vc152/estimate-item-prod-sched-original-differential-native/`,
      and `runs/vmctl-vc152eipod/`.

### Latest checkpoint — SatisfyOrders packed resource E1 native replay

- [x] Added a production-source raw-record matrix for the first remaining
      SatisfyOrders Step-5 boundary. Fresh host and Win98 validation in
      `C:\SYOE6` cover the 124-byte `_fleet` and 18-byte `_order` views,
      source/capacity limiting, target-kind resource restrictions, exact
      transfer and shortage messages, stat-percent mode, active/turn-flag
      write-back, and pass-1 battle-plan dispatch.
- [x] Host and native reports are both
      `SATISFY_ORDERS_PACKED_RESOURCE_E1_CHECKS=21 FAILURES=0 RESULT=PASS`;
      compile/link passed, the process exited 0, and the final guest window
      list contained no Program Error dialog. This closes SatisfyOrders E1
      packed battle-plan/resource accounting. Current counters are **105/152
      evidence units closed, 47 open, 13 promotion units, and 60 practical
      open work items**. E2 technology/ship-copy/salvage/message-layout work
      and E3 full-turn/original differential remain. Evidence is in
      `analysis/compat/satisfy-orders-packed-resource-e1-native.{md,json}`,
      `compat/vc152/satisfy-orders-packed-resource-e1-native/`, and
      `runs/vmctl-vc152syoe6/`.

### Latest checkpoint — SatisfyOrders special-order dispatch native replay

- [x] Replayed the existing 16-case strict-C89 special-order matrix through
      the production `SatisfyOrders` source in fresh guest directory
      `C:\SYOE4`. The native report is
      `SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=0 RESULT=PASS`; compile and
      link passed, the process exited 0, and the final guest window list
      contained no Program Error dialog.
- [x] Upgraded the battle-plan pass-1, mine gates/repeats, same-owner
      starbase auto-route, merge ownership/callback, and player-transfer
      message-pair matrix from host-only to native evidence. This closes **0**
      named Step-5 units because packed battle-plan/resource accounting,
      technology discovery, fleet/ship-design copying, salvage, complete
      message layouts, and the full-turn/original differential remain open.
      The authoritative burn-down remains **99/152 evidence units closed, 53
      open, 15 promotions, 68 practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-special-e4-native.{md,json}`,
      `compat/vc152/satisfy-orders-special-native-e4/`, and
      `runs/vmctl-vc152syoe4/`.

### Latest checkpoint — SatisfyOrders packed special-order bridge native replay

- [x] Extended the production packed bridge with the recovered source-coordinate
      and auto-route-message fields, then replayed raw 124-byte `_fleet` and
      18-byte `_order` records in fresh guest directory `C:\SYOP5`.
- [x] The native report is
      `SATISFY_ORDERS_PACKED_SPECIAL_CHECKS=10 FAILURES=0 RESULT=PASS`;
      compile and link passed, the process exited 0, and the final guest
      window list contained no Program Error dialog. The packet covers packed
      battle-plan, mine, merge, pass-4 auto-route, and player-transfer
      creation/message paths.
- [x] This is stronger packed/native integration evidence but closes **0**
      named Step-5 units. The remaining SatisfyOrders boundary still includes
      packed battle-plan/resource accounting, technology discovery,
      fleet/ship-design copying, salvage creation, complete message layouts,
      and the full-turn/original differential. The authoritative burn-down
      remains **99/152 evidence units closed, 53 open, 15 promotions, 68
      practical open work items**. Evidence is in
      `analysis/compat/satisfy-orders-packed-special-e5-native.{md,json}`,
      `compat/vc152/satisfy-orders-packed-special-e5/`, and
      `runs/vmctl-vc152syop5/`.

### Latest checkpoint — DrawPlanetStats E4 and promotion

- [x] Captured the resumed original `Oop Be Gone` planet frame after
      dismissing the live Production Queue dialog. The mineral/status,
      population/resources, scanner/defense, production/fleet, and
      environment panels remained visible with no Program Error dialog.
- [x] Fresh `C:\DPSWPE4` VC++ 1.52c validation passed the 22-check
      `DrawPlanetStats` source geometry/row probe and the 8-check
      `PlanetWndProc(WM_PAINT)` integration replay; compile/link passed and
      the final window list contained no Program Error dialog.
- [x] Closed DrawPlanetStats E4 and synchronized final Step-6 promotion in
      `tracking/reviews.json`, `analysis/coverage.json`, `TODO5.md`, and the
      reconstruction note. DrawPlanetStats is now `6/6`.
- [x] The authoritative burn-down is now **92/152 evidence units closed,
      60 open, 19 promotions, 79 practical open work items**. Evidence is in
      `analysis/compat/draw-planet-stats-gdi-original-e4.{md,json}`,
      `compat/vc152/planet-stats-wndproc-e4/`, and
      `runs/vmctl-vc152dpswpe4/` plus
      `runs/vmctl-vc152dps-original-e4.bmp`.

### Latest checkpoint — DrawPlanShipBitmap native boundary replays

- [x] Replayed the fleet-forwarding boundary in fresh guest directory
      `C:\DPSBFLE4`. The native report is
      `DRAW_PLAN_SHIP_BITMAP_FLEET_CHECKS=6 FAILURES=0 RESULT=PASS`; compile
      and link passed, the process exited 0, and the final guest window list
      contained no Program Error dialog.
- [x] Replayed the tile-failure/hidden-button cleanup boundary in fresh guest
      directory `C:\DPSBFAI4`. The native report is
      `DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=6 FAILURES=0 RESULT=PASS`; compile
      and link passed, the process exited 0, and the final guest window list
      contained no Program Error dialog.
- [x] Upgraded both focused boundaries from host-only to host/native evidence.
      This closes **0** named Step-5 units: exact Win16 resource handles,
      complete packed-selection ownership, localized names, planet-window
      ownership/redraw, original differential coverage, and promotion remain
      open. The authoritative burn-down remains **92/152 evidence units
      closed, 60 open, 19 promotions, 79 practical open work items**.
      Evidence is in
      `analysis/compat/draw-plan-ship-bitmap-native-boundaries-e4.{md,json}`,
      `compat/vc152/draw-plan-ship-bitmap-{fleet-e4,failure-e4}/`, and
      `runs/vmctl-vc152dpsbfleet-e4/` plus
      `runs/vmctl-vc152dpsbfailure-e4/`.

### Latest checkpoint — DrawPlanetShipList native detail replay

- [x] Replayed the focused detail-dispatch probe in fresh guest directory
      `C:\DLISTDE3`. The native report is
      `DRAW_PLANET_SHIP_LIST_DETAILS_CHECKS=9 FAILURES=0 RESULT=PASS`; compile
      and link passed, the process exited 0, and the final guest window list
      contained no Program Error dialog.
- [x] Upgraded the resolved-fleet-detail, unavailable-transfer suppression,
      and ship-mode suppression boundary from host-only to host/native
      evidence. This closes **0** named Step-5 units: localized text/gauges,
      complete transfer behavior, exact caller/resource integration, original
      differential coverage, and promotion remain open. The authoritative
      burn-down remains **92/152 evidence units closed, 60 open, 19
      promotions, 79 practical open work items**. Evidence is in
      `analysis/compat/draw-planet-ship-list-details-e3-native.{md,json}`,
      `compat/vc152/draw-planet-ship-list-details-e3-native/`, and
      `runs/vmctl-vc152dlist-details-e3/`.

### Latest checkpoint — EstimateItemProdSched E2 packed mutation

- [x] Added a three-case strict-C89 packed mutation probe covering complete
      queue consumption, partial quantity write-back, and failed-object
      resource rollback. It preserves packed type/object bits, checks all
      four resource lanes, and verifies completion-message suppression on
      failure.
- [x] Fresh `C:\EIPM2` VC++ 1.52c/Win98 validation reports
      `COMPILE=PASS`, `LINK=PASS`, and
      `ESTIMATE_ITEM_PROD_SCHED_MUTATION_CHECKS=3 FAILURES=0 RESULT=PASS`;
      the process exited 0 and the final guest window list contained no
      Program Error dialog.
- [x] Closed one named Step-5 unit: `EstimateItemProdSched` E2 production-
      record/resource mutation. The procedure remains `5/6` with E1's
      independent original per-turn/complete schedule differential and
      Step-6 promotion open. The authoritative burn-down is now **93/152
      evidence units closed, 59 open, 19 promotions, 78 practical open
      work items**. Evidence is in
      `analysis/compat/estimate-item-prod-sched-mutation-e2.{md,json}`,
      `compat/vc152/estimate-item-prod-sched-mutation-e2/`, and
      `runs/vmctl-vc152eipm2/`.

### Latest checkpoint — Planet information display E3 native replay

- [x] Replayed the shared nine-case `PtDisplayPlanetPopInfo` and
      `PtDisplayPlanetStateInfo` branch harness in fresh `C:\PBR3`. The
      native report is `PLANET_INFO_BRANCH_CHECKS=9 FAILURES=0 RESULT=PASS`;
      compile and link passed, the process exited 0, and the final guest
      window list contained no Program Error dialog.
- [x] The native trace covers complete stream/wrap callback order, branch
      selection, environment/desirability services, signed differences,
      wrapping, and returned layout. The original `Oop Be Gone` Small Screen
      capture supplies the visible comparison: state gravity/temperature/
      radiation/mineral rows and population value/population/report rows.
- [x] Closed two named Step-5 units and completed Step-6 promotion for both
      planet-information procedures. Both rows are now `6/6`; the
      authoritative burn-down is **95/152 evidence units closed, 57 open,
      17 promotions, 74 practical open work items**. TODO5 now reports **829
      reviewed and 17 partial**.
      Evidence is in
      `analysis/compat/planet-info-branch-e3-native.{md,json}`,
      `compat/vc152/planet-info-branch-e3-native/`, and
      `runs/vmctl-vc152pbr3/`.

### Latest checkpoint — DrawPlanetShipList E1 production packed replay

- [x] Replayed the current production `DrawPlanetShipList` source and
      packed-record probe in fresh `C:\DLP4`. The native report is
      `DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`;
      compile and link passed, the process exited 0, and the final guest
      window list contained no Program Error dialog.
- [x] Closed E1, the packed fleet/thing transfer-record unit. The procedure
      is now `2/4` named Step-5 units closed: E2 exact controls/geometry is
      closed; E3 localized text/gauges/detail rendering and E4 complete
      drawing/transfer integration remain, along with promotion. The
      authoritative burn-down is now **96/152 evidence units closed, 56 open,
      17 promotions, 73 practical open work items**. Evidence is in
      `analysis/compat/draw-planet-ship-list-packed-production-e4-native.{md,json}`,
      `compat/vc152/draw-planet-ship-list-packed-production-e4-native/`, and
      `runs/vmctl-vc152dlp4/`.

### Latest checkpoint — DrawPlanetShipList E3 detail-render replay

- [x] Replayed the current production detail-render path in fresh `C:\DLD3`.
      The native report is
      `DRAW_PLANET_SHIP_LIST_DETAIL_RENDER_CHECKS=9 FAILURES=0 RESULT=PASS`;
      compile and link passed, the process exited 0, and the final guest
      window list contained no Program Error dialog.
- [x] The trace verifies fleet/thing detail dispatch, the exact detail
      rectangle, button order and geometry, callback-before-button sequencing,
      unavailable-transfer suppression, and ship-mode suppression. The
      original fleet-frame capture shows composition, fuel/cargo, and
      waypoint controls.
- [x] Closed E3. `DrawPlanetShipList` is now `3/4` named Step-5 units closed;
      only E4 complete drawing/transfer integration, broader original
      differential coverage, and Step-6 promotion remain. The authoritative
      burn-down is now **97/152 evidence units closed, 55 open, 17
      promotions, 72 practical open work items**. Evidence is in
      `analysis/compat/draw-planet-ship-list-detail-render-e3-native.{md,json}`,
      `compat/vc152/draw-planet-ship-list-details-e3-production-native/`, and
      `runs/vmctl-vc152dld3p/`.

### Latest checkpoint — DrawPlanetShipList E4 complete integration and promotion

- [x] Replayed the current production source in fresh `C:\DLP4E`. The host
      and native report is `DRAW_PLANET_SHIP_LIST_E4_CHECKS=12 FAILURES=0
      RESULT=PASS`; VC++ 1.52c compile/link passed, the process exited 0, and
      the final guest window list contained no Program Error dialog.
- [x] The combined trace covers packed selection/transfer decoding,
      tile-change hide/reset cleanup, exact combo geometry and cached-width
      flags, fleet detail rectangle and callback order, `1,2,0` button
      mapping, thing-mode gating, unavailable-transfer suppression, and
      failed-tile cleanup. Together with the original fleet-frame capture,
      this closes E4 complete drawing/transfer/original integration.
- [x] Promoted `DrawPlanetShipList` to `6/6` in the review ledger. The
      authoritative burn-down is now **98/152 evidence units closed, 54 open,
      16 promotions, 70 practical open work items**. Evidence is in
      `analysis/compat/draw-planet-ship-list-e4-native.{md,json}`,
      `compat/vc152/draw-planet-ship-list-e4-production-native/`, and
      `runs/vmctl-vc152dlep4/`.

### Latest checkpoint — FAttack raw stateful damage dispatch E14

- [x] Added an optional raw damage callback to the readable FAttack
      operations boundary. Raw beam dispatch now passes the selected amount
      by reference to the stateful callback, while typed sections retain the
      existing callback path.
- [x] Host strict-C89 and fresh `C:\FAPNE14` VC++ 1.52c/Win98 probes pass
      4 checks: the scaled 39-point event is split by `FDamageTok` into a
      6-point shield record and 33-point hull record, two ships are removed,
      and FAttack records one hit. Compile/link pass and no Program Error
      dialog appears.
- [x] This remains supporting evidence only: **0** named Step-5 units close.
      The authoritative burn-down remains **91/152 evidence units closed, 61
      open, 20 promotions, 81 practical open work items**. Full raw part
      scaling, capacity/jam formulas, complex torpedo selection, original
      differential, replay, and promotion remain open.
- [x] Evidence is in `analysis/compat/f-attack-raw-damage-e14.{md,json}`,
      `compat/vc152/f-attack-complex-native-e14/`, and
      `runs/vmctl-vc152fapne14/`.

### Latest checkpoint — FAttack packed raw-damage integration E15

- [x] Extended the raw stateful-damage probe through the packed `_hs`
      attacker-part slot. The strict C89 host report is
      `F_ATTACK_RAW_DAMAGE_CHECKS=7 FAILURES=0 RESULT=PASS`; it covers exact
      group/item/count forwarding, local resolved-section ownership, raw
      capacity/beam-defense/distance attenuation, `FDamageTok` shield/hull
      ordering, casualty mutation, and the FAttack hit/battle-record fields.
- [x] Revalidated E15 from the current checkout: the focused C89 probe again
      passes 7/7, the repository suite passes 519 tests (153 expected skips),
      generated headers remain at 2,241 layout assertions and 846 function
      declarations, and baseline plus diff checks pass.
- [x] Prepared a fresh packet for `C:\FAPNE15` in
      `compat/vc152/f-attack-complex-native-e15/`.
- [x] Normalized all 40 text files in the 41-file packet to ASCII CRLF line
      endings, removing a `COMMAND.COM`/VC1.52c response-file parsing risk
      before transfer.
- [x] The seven host evidence units are now named explicitly: (1) packed
      `_hs` group forwarding, (2) packed item/count forwarding, (3) resolved
      section ownership, (4) raw capacity/beam-defense/distance scaling, (5)
      shield-record mutation, (6) hull-record/casualty mutation, and (7)
      FAttack hit-count/first-hit battle-record updates.
- [ ] Native replay is pending the Win98 agent reconnection. This is stronger
      supporting evidence but closes **0** named Step-5 units. The authoritative
      counters remain **120/152 closed, 32 open, 10 promotion units, and 42
      practical open work items**; this E15 packet does not change them. The
      broader original differential, full battle replay, and Step-6 promotion
      remain open.
- [x] Evidence is in
      `analysis/compat/f-attack-packed-raw-damage-e15.{md,json}` and the
      prepared packet directory.

### Latest checkpoint — FAttack torpedo candidate ranking E16 host support

- [x] Recovered the deterministic raw torpedo candidate score from
      `analysis/decompiler/export/raw/10f0_69ac_FAttack.c:383-515`: resource
      value per expected shield/hull torpedoes, including adjusted hit
      percentage, `_dv` hull reduction, part-family multiplier, and the raw
      truncation order. `CTorpHit` is now called only for the selected target;
      the prior source consumed random draws for every candidate.
- [x] The new strict-C89 probe passes
      `F_ATTACK_TORPEDO_TARGET_CHECKS=5 FAILURES=0 RESULT=PASS`, proving
      higher-value target selection, exactly three draws for the selected
      target's three torpedoes, damage dispatch, first-hit recording, and the
      zero-effective-hit gate with no random draws or damage dispatch.
      Existing FAttack probes pass at 34/9/3/13/7 checks; the full suite passes
      519 tests with 153 expected skips. Generated headers remain at 2,241
      layout assertions and 846 declarations; baseline and diff checks pass.
- [ ] Host support only; this closes **0** named Step-5 units. The E15 native
      packet contains the previous `f_attack.c` revision and is stale for this
      change. A fresh 42-file `C:\FAPNE16` packet is now prepared, with all
      41 text files in ASCII CRLF; the packaged source also passes a strict-C89
      syntax check against its `CORE.H`. Win98 replay remains pending agent
      reconnection. Raw shield/hull torpedo damage splitting,
      residual-torpedo behavior, original differentials, full battle replay,
      and promotion remain open.
      The Phase 2 counters remain **120/152 closed, 32 open, 10 promotion
      units, and 42 practical open work items**. Evidence is in
      `analysis/compat/f-attack-torpedo-target-e16.{md,json}`.

### Latest checkpoint — FAttack raw torpedo damage-call bridge E17

- [x] Matched the `FDamageTok` callback boundary to the recovered seven-
      parameter signature: mutable beam amount, torpedo damage, `grfWeapon`,
      shields-only gate, and mutable packet count. Raw torpedo shield and hull
      calls now forward their distinct argument forms; typed damage remains
      the fallback.
- [x] Strict-C89 host probes pass the new raw torpedo dispatch matrix (6
      checks), E15 raw damage matrix (7), and E16 torpedo candidate/RNG matrix
      (5). The fresh E17 packet has 43 files (42 ASCII/CRLF text files and the
      DOS stub) and its packaged sources pass strict-C89 syntax checks against
      its reduced `CORE.H`.
- [ ] Guest compile/run is not yet verified. The production part resolver
      still does not derive `grfWeapon` from the selected weapon and target
      state, and the raw shield/hull split plus residual-torpedo loop remain
      incomplete. This supporting checkpoint closes **0** named Step-5 units;
      counters remain **120/152 closed, 32 open, 10 promotion units, and 42
      practical open work items**. Original differentials, full battle replay,
      and promotion remain open. Evidence is in
      `analysis/compat/f-attack-raw-torpedo-dispatch-e17.{md,json}` and
      `compat/vc152/f-attack-complex-native-e17/` for `C:\FAPNE17`.

### Latest checkpoint — FAttack torpedo damage-class selection E18

- [x] Recovered the raw torpedo `grfWeapon` class rule: base class 4, with
      bit 8 added for packed item indices 8–11; shield-only dispatch adds
      `0x80`. `FAttack` now derives these raw callback flags from the selected
      part item instead of relying on a caller-populated generic field.
- [x] The strict-C89 dispatch probe passes 7 checks, including item 8 => hull
      `12`/shield `0x8c` and item 7 => hull `4`/shield `0x84`; E16 target/RNG
      probe passes 5 checks. A fresh 43-file native packet is prepared with
      42 ASCII/CRLF text files, and packaged sources pass strict-C89 syntax
      checks against its reduced header.
- [ ] Guest compile/run remains unverified. Exact shield/hull partitioning,
      residual-packet iteration, beam class derivation from part weight,
      broader original differential, full battle replay, and Step-6 promotion
      remain open. This closes **0** named Step-5 units; counters remain
      **120/152 closed, 32 open, 10 promotion units, and 42 practical open
      work items**. Evidence is in
      `analysis/compat/f-attack-torpedo-damage-class-e18.{md,json}` and
      `compat/vc152/f-attack-complex-native-e18/` for `C:\FAPNE18`.

### Latest checkpoint — FAttack launched torpedo packet budget E19

- [x] Separated launched packet budget from `CTorpHit` successes when calling
      raw `FDamageTok`. Original FAttack initializes `uStack42` from the
      torpedo budget, then partitions that budget and passes the split sum by
      pointer; it is not the hit count. The readable bridge now forwards the
      section's launched budget.
- [x] The strict-C89 dispatch probe passes 7 checks with a two-of-three hit
      case: the hull callback receives packet count 3 and mutates it to 2.
      E18 item-class assertions remain in the same matrix. A fresh 43-file
      packet (42 ASCII/CRLF text files plus DOS stub) passes packaged-source
      strict-C89 syntax validation.
- [ ] The exact `uStack34`/`uStack38` split and same-target residual loop
      (`lVar19`/`lVar16`) are still absent; post-damage token state is not yet
      looped back through the split. Native execution is pending agent
      connection. This closes **0** named Step-5 units; counters remain
      **120/152 closed, 32 open, 10 promotion units, and 42 practical open
      work items**. Evidence is in
      `analysis/compat/f-attack-torpedo-packet-budget-e19.{md,json}` and
      `compat/vc152/f-attack-complex-native-e19/` for `C:\FAPNE19`.

### Latest checkpoint — FAttack torpedo split/residual source map E20

- [x] Traced the raw allocation and call sequence at
      `10f0_69ac_FAttack.c:620-769`: launched budget and successful hits are
      separate; the split scan is bounded by ship count/budget and shield/
      hull capacities; the two terms feed different `/8` and `/2` damage
      calls; and the residual loop tracks both unused budget and another
      counter. `FDamageTok` also mutates token and packet state.
- [ ] No implementation or test result is claimed for this analysis-only
      checkpoint. The current readable loop still attacks once per pass with
      the unchanged budget and has no exact allocation result or mutable
      same-target state. Next: add a typed split result, preserve raw integer
      rounding/threshold branches, then test fallback/scan boundaries and
      residual state before fresh native replay. TODO6 remains **120/152
      closed, 32 open, 10 promotion units, and 42 practical open work items**.
      Evidence map: `analysis/compat/f-attack-torpedo-split-analysis-e20.md`.

### Latest checkpoint — FAttack raw beam damage class E23

- [x] Raw group-0x10 beam dispatch now derives the `FDamageTok` damage class
      from the selected record's byte-54 word: below 200 => class 1; 200 or
      above => class 2. The recovered `_beam` record names byte 54 `dp`, and
      raw FAttack's branch uses the same threshold.
- [x] The strict-C89 raw-damage probe passes **9 checks**, including `dp=50`
      and the threshold boundary `dp=200`. Full suite: **519 tests passed,
      153 expected skips**, 2,241 layout assertions, and 846 declarations;
      `git diff --check` passes.
- [ ] This host milestone closes **0** named Step-5 units. Native guest replay
      remains pending: port 2122 is listening but has no established agent
      connection. The residual torpedo loop/state feedback, runtime group-0x20
      record population, original differential, full battle replay, and
      promotion remain open. Counters remain **120/152 closed, 32 open, 10
      promotion units, and 42 practical open work items**. Evidence:
      `analysis/compat/f-attack-beam-damage-class-e23.md` and
      `analysis/decompiler/export/raw/10f0_69ac_FAttack.c:572-580`.

### Superseded checkpoint — FAttack group-0x20 torpedo record address E22

- [x] Corrected the raw field interpretation: FAttack's decompiled
      `_hul.wtFuelMax` is a type-inference alias for byte 54 of the selected
      group-0x20 torpedo record; the recovered `_torp` layout names byte 54
      `dp`. E22 inferred a DGROUP `0x10e0:0x2180` pointer from the decompiler's
      M6108 label and incorrectly concluded that static bytes were an overlay.
      E29's original instruction and CodeView audit supersedes both claims:
      the branch stores `CS`, and `rgtorp` is in NE segment 2 at `0x2180`.
- [x] The current original-table values and native dispatch are recorded in
      E29/E30 below. E22 remains only as historical evidence of the initial
      address hypothesis. This correction closes **0** named Step-5 units;
      complete beam scaling, broader original differential, full native
      battle replay, and promotion remain. Counters stay **120/152 closed,
      32 open, 10 promotion units, and 42 practical open work items**.
      Evidence: `analysis/compat/f-attack-torpedo-record-address-e22.md` and
      `analysis/compat/f-attack-torpedo-table-native-e29.md`.

### Previous checkpoint — FAttack raw torpedo allocation E21

- [x] Implemented the raw shield/hull allocation result and threshold scan:
      `_dv`-adjusted hull capacity, special-item no-shield power doubling,
      exact truncation/ceiling order, `/8` shield term, `/2` hull term, and
      packet allocation passed to their respective raw callbacks. Packed
      group/item/count are now authoritative in the resolved local section.
- [x] The strict-C89 dispatch probe passes 9 checks for fallback and scan
      allocation, item-7/item-8 class, doubled power, callback argument shape,
      packet mutation, and hit-record behavior. FAttack source/target/raw-damage
      focused regressions pass 34/5/7. A new 43-file VC1.52c packet has 42
      ASCII/CRLF text files and passes packaged-source strict-C89 syntax checks.
- [x] The full suite after E21 source changes passes: 519 tests, 153 expected
      skips, 2,241 generated layout assertions, and 846 function declarations.
- [ ] Native guest replay is pending agent connection. The residual
      `lVar19`/`lVar16` loop, runtime group-0x20 resolver population of
      `raw_torpedo_power`,
      beam class derivation, original differential, full battle replay, and
      promotion remain open. This closes **0** named Step-5 units; counters
      remain **120/152 closed, 32 open, 10 promotion units, and 42 practical
      open work items**. Evidence is in
      `analysis/compat/f-attack-torpedo-allocation-e21.{md,json}` and
      `compat/vc152/f-attack-complex-native-e21/` for `C:\FAPNE21`.

### Latest checkpoint — DoMacintiAiTurn packed first-order repair support

- [x] Extended the readable Macinti fleet view with an optional packed
      `PLORD` allocation. When the recovered fleet policy requires first-order
      repair, the turn now applies the shared raw repair before typed task and
      action planning.
- [x] The strict-C89 host probe reports
      `MACINTI_AI_PLORD_REPAIR_CHECKS=5 FAILURES=0 RESULT=PASS`; it checks the
      repaired count, order-kind nibble, repair words, callback ordering, and
      preservation of unrelated bytes.
- [ ] This is host supporting evidence only. A fresh VC++ 1.52c replay,
      exact personality routing predicates, broader packed-order mutation, and
      the original full-turn differential remain open. The authoritative
      counters remain **120/152 closed, 32 open, 10 promotion units, and 42
      practical open work items**. Evidence is in
      `analysis/compat/do-macinti-ai-turn-integration.{md,json}` and
      `tests/do_macinti_ai_turn_plord_repair_probe.c`.

### Latest checkpoint — DoCyberAiTurn packet-service bridge E4

- [x] Added an optional typed packet callback carrying the prepared mutable
      planet table, planet count, player id, and computed packet pressure. The
      player-only callback remains the compatibility fallback.
- [x] The strict C89 packed integration probe now reports
      `CYBER_AI_TURN_INTEGRATION_CHECKS=30 FAILURES=0 RESULT=PASS`, including
      the packed planet's 501 packet-strength value, write-back to 401, and
      final callback order.
- [ ] Native replay is pending the Win98 agent reconnection. This closes **0**
      named Step-5 units; counters remain **118/152 closed, 34 open, 10
      promotion units, and 44 practical open work items**. Direct packet /
      freighter composition, packed `PLORD` mutation, AI-table integration,
      original-turn differential, and Step-6 promotion remain open.
- [x] Evidence is in
      `analysis/compat/do-cyber-ai-turn-packet-bridge-e4.{md,json}`.

### How to read the unchanged `5/6` rows

The right-hand column in the procedure matrix is the active step-5 work list
for that procedure, not a single final checkbox. Each distinct concern in that
cell is an acceptance subtask: for example, `EstimateItemProdSched` still has
turn scheduling, production-record mutation, and resource/queue integration
units open. `ProductionDlg` is no longer an example of an open row: its live
original menu-to-editor transition and cancel return closed its final E3 unit
and allowed promotion to `6/6`.

This is why the progress log can grow while most procedures remain at
`5/6`: a milestone strengthens or narrows step 5; it changes
the headline number only when every remaining step-5 concern is closed and the
step-6 ledger promotion is performed. The matrix's remaining-gate text is the
authoritative per-procedure work breakdown. The next useful number to report is
therefore the number of remaining concerns in that cell, not the number of
native checks alone.

The current `ProductionDlg` E2 checkpoint is complete for the direct
resource-ID/callback-order contract, not for the entire localized resource
table. The raw source has only two direct lookups: `0x00d3`
(`CchGetString`) and `0x04c0` (`PszGetCompressedString`). A fresh resumed-VM
run under `C:\PDGRS4` passes the corrected native 8-check report with
`COMPILE=PASS`, `LINK=PASS`, empty redirected output, and no Program Error
dialog. The live original right-click menu shows `<Default>` and `<Customize>`;
the latter is the observed `0x04c0` editor entry. The `0x00d3` callback is
validated by ID, ordering, and live hit-boundary evidence without asserting an
unverified localized spelling. `InitializeProductionDlg`'s separate `0x033e`
title resource is already covered by its own reviewed procedure. ProductionDlg
is now promoted at `3/3` authoritative units and Phase 2 `6/6`. The live
original menu-to-editor transition is recorded in
`analysis/compat/production-dlg-original-e3.{md,json}`; the previous E3
boundary is now closed and no ProductionDlg Step-5 or promotion work remains.
Evidence is in
`analysis/compat/production-dlg-resource-e2-complete.{md,json}` and
`analysis/compat/production-dlg-original-e3.{md,json}`, with native evidence in
`runs/vmctl-vc152pdgrs3/` and `runs/vmctl-vc152pdgrs4/`.

### Latest checkpoint — ProductionDlg original E3 and promotion

- [x] Re-focused the original `Production Queue for Oop Be Gone` dialog in
  Win98 and captured the real template menu with `<Default>` and
  `<Customize>`.
- [x] Selected `<Customize>` with a real second mouse event. The original
  dialog opened `Customize Production Templates`; Escape canceled it and
  returned to ProductionDlg with no Program Error dialog.
- [x] Closed ProductionDlg's final E3 original queue-differential unit and
  recorded final Step-6 promotion in `tracking/reviews.json`.
- [x] Updated the authoritative burn-down from **84/152 closed, 68 open, 20
  promotions, 88 practical** to **86/152 closed, 66 open, 20 promotions, 86
  practical open work items**. ProductionDlg is now `6/6`; the remaining
  Phase 2 queue is **20 procedures at 5/6**.

### Latest checkpoint — DrawPlanetStats E3 services

- [x] Re-ran the strict host service-resolution probe: 12/12 checks pass for
  selected-player forwarding, packed service-word/mineral-rate decoding, and
  all seven value-row consumers.
- [x] Added a fresh `C:\DPSSE3` VC++ 1.52c/Win98 bridge packet. Compile and
  link pass; the native report is
  `DRAW_PLANET_STATS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS`, redirected
  output is empty, and no Program Error dialog appears.
- [x] Closed DrawPlanetStats E3. Its score is now `3/4`; only the detailed
  GDI/original differential and final promotion remain.

### Latest checkpoint — EstimateItemProdSched E3 native resource/queue replay

- [x] Replayed the prepared resource/queue packet in fresh guest directory
  `C:\EIRQ`. The corrected probe compiled and linked under VC++ 1.52c after
  replacing a probe-only `_printf` summary with `puts`; the failed first link
  was a harness dependency issue, not a source/procedure result.
- [x] Retrieved `EIRQSTA.TXT` with `COMPILE=PASS` and `LINK=PASS`, plus
  `EIRQR.TXT` with
  `ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=8 FAILURES=0 RESULT=PASS`.
  The post-run window list had no Program Error dialog.
- [x] Closed one named Step-5 unit. `EstimateItemProdSched` remains `5/6`
  with **1/3** authoritative units closed: E3 resource/queue native replay is
  closed; E1 turn-schedule/production-record differential and E2 complete
  production-record/resource mutation remain. The current burn-down is now
  **86/152 closed, 66 open, 20 promotions, 86 practical open work items**.

### Latest checkpoint — EstimateItemProdSched E1 native schedule differential

- [x] Replayed the broader 21-check schedule differential in fresh guest
  directory `C:\EIPD`. VC++ 1.52c recorded `COMPILE=PASS` and `LINK=PASS`,
  and `EIPDR.TXT` recorded
  `ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=21 FAILURES=0 RESULT=PASS`.
- [x] Confirmed the post-run window list contained no Program Error dialog.
- [x] Closed the native replay sub-check for E1, including phase-1/phase-2
  completion, the 100-turn fallback, special-item and research-exempt paths,
  mineral refresh, copied-planet isolation, and population timing. This does
  **not** increment the authoritative `1/3` count yet: complete packed
  production-record/resource mutation and original per-turn queue/resource
  replay remain distinct E1 requirements. The authoritative burn-down remains
  **86/152 closed, 66 open, 20 promotions, 86 practical open work items**.

### Latest checkpoint — PtDisplayPlanetPopInfo E2 native formatting

- [x] Replayed the 27-check localization/fixed-point packet in fresh guest
  directory `C:\PPFE2`. VC++ 1.52c recorded `COMPILE=PASS` and `LINK=PASS`,
  and `PPFE2.TXT` recorded
  `PT_DISPLAY_PLANET_POP_INFO_FORMAT_CHECKS=27 FAILURES=0 RESULT=PASS`.
- [x] Confirmed the post-run window list contained no Program Error dialog.
- [x] Closed the authoritative E2 formatting unit. `PtDisplayPlanetPopInfo`
  remains `5/6` with **1/3** units closed: E2 localized/fixed-point
  formatting is closed; E1 complete packed planet/player lookup and E3
  detailed stream/wrap GDI plus original comparison remain. The current
  burn-down is **87/152 closed, 65 open, 20 promotions, 85 practical open
  work items**.

### Latest checkpoint — PtDisplayPlanetStateInfo E2 native formatting

- [x] Replayed the 35-check localization/fixed-point packet in fresh guest
  directory `C:\STFE2`. VC++ 1.52c recorded `COMPILE=PASS` and `LINK=PASS`,
  and `STFE2.TXT` recorded
  `PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS`.
- [x] Confirmed the post-run window list contained no Program Error dialog.
- [x] Closed the authoritative E2 formatting unit. `PtDisplayPlanetStateInfo`
  remains `5/6` with **1/3** units closed: E2 localized/fixed-point
  formatting is closed; E1 complete packed player/planet lookup and E3
  detailed stream/wrap GDI plus original comparison remain. The current
  burn-down is **88/152 closed, 64 open, 20 promotions, 84 practical open
  work items**.

### Latest checkpoint — PtDisplayPlanetPopInfo E1 native packed bridge

- [x] Built and replayed a fresh production-source packet in guest directory
  `C:\PPPE4`. The host C89 probe and corrected native packet both report
  `PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS`; native
  status is `COMPILE=PASS`, `LINK=PASS`, with no Program Error dialog.
- [x] Corrected the probe contract exposed by the native ABI: a valid 56-byte
  packed record bypasses `lookup_planet`, while a 55-byte record uses lookup
  fallback. Removed the off-by-one self-counting assertion so the report's 21
  checks are semantic checks rather than a failing check-count check.
- [x] Closed E1 complete packed planet/player lookup. `PtDisplayPlanetPopInfo`
  is now `2/3` at Step 5: E1 lookup and E2 formatting are closed; E3 detailed
  stream/wrap GDI plus original comparison and promotion remain. The current
  burn-down is **89/152 closed, 63 open, 20 promotions, 83 practical open
  work items**.

### Latest checkpoint — PtDisplayPlanetStateInfo E1 native packed bridge

- [x] Built and replayed a fresh production-source packet in guest directory
  `C:\STPE4`. The host C89 probe and native packet both report
  `PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS`; the
  native status is `COMPILE=PASS`, `LINK=PASS`, with no Program Error dialog.
- [x] Corrected the copied harness's unsupported `_printf` summary and added
  the short-record typed-fallback preservation assertion; the 25 reported
  checks are now substantive packed-field, branch, callback, layout, and
  fallback checks.
- [x] Closed E1 complete packed player/planet lookup. `PtDisplayPlanetStateInfo`
  is now `2/3` at Step 5: E1 lookup and E2 formatting are closed; E3 detailed
  stream/wrap GDI plus original comparison and promotion remain. The current
  burn-down is **90/152 closed, 62 open, 20 promotions, 82 practical open
  work items**.

Recent examples:

| Procedure | New evidence | Why the gate is still open |
|---|---|---|
| `EstMineralsMined` | 13 native packed/instruction-differential checks | E1 populated mining-turn differential closed; source corrected for pre-efficiency depletion and percentage-cap reset, then promoted |
| `DrawProductionDlg` | 17 native packed-list checks, plus host/native 10-check resource and 9-check geometry probes | Promoted: E1 packed globals, E2 resource/formatting, E3 GDI/caller integration, and Step-6 synchronization are closed |
| `DrawPlanShipBitmap` | 17 packed-selection checks, host/native 8-check GDI lifecycle, host/native 11-check packed E2, host/native 11-check localized E3, and host/native 6-check fleet-forwarding plus 6-check failure/hide probes | E1 GDI lifecycle, E2 production packed selection, and E3 exact localized-name forwarding are closed; only complete planet integration/original differential remains |
| `DrawPlanetShipList` | 18 production packed-transfer checks, host/native 7-check control-layout probe, host/native 9-check detail-render probe, and host/native 12-check E4 integration probe | Promoted: the 226-byte selection and 128-byte transfer decode, exact control layout, detail rendering, complete transfer/drawing integration, and original fleet-frame boundary are synchronized |
| `DrawPopup` | 38 production packed-payload checks, host/native 7-check GDI state-order probe, host/native 21-check content probe, host/native 25-check geometry/style probe, native 26-check caller replay, and original Win98 consumer captures | Promoted: E1 complete `GlobalPD` payloads, E2 localization/hull filtering/formatting, E3 detailed GDI/caller integration, E4 original consumer completion, and Step-6 synchronization are closed |
| `DrawPlanetStats` | host/native 9-check packed lookup, 16-check formatting, 12-check service-resolution, 22-check source geometry, and 8-check parent-paint integration probes | Promoted: E1 packed lookup, E2 seven-row fixed-point/zero-rate/unavailable formatting, E3 service resolution, E4 detailed GDI/original integration, and final ledger synchronization are closed |
| `PtDisplayPlanetPopInfo` | 21-check host/native production packed-planet bridge, 27-check host/native segment-preparation probe, and shared 9-check native branch integration | Promoted: the 56-byte `_planet` decode, E2 localization/fixed-point formatting, detailed stream/wrap GDI seam, original Small Screen comparison, and Step-6 synchronization are closed |
| `PtDisplayPlanetStateInfo` | 25-check host/native production packed-record bridge, 35-check host/native segment-preparation probe, and shared 9-check native branch integration | Promoted: the 56-byte `_planet` plus 22-byte `_popupdata` decode, E2 localization/fixed-point formatting, detailed stream/wrap GDI seam, original Small Screen comparison, and Step-6 synchronization are closed |
| `ProductionDlg` | 11 native popup-result checks, 8 native resource-lookup checks, 9 native accepted-editor checks, 12 native USER checks, 10 native oversized-capacity checks, 11 native packed-global checks, and original Win98 menu/editor/cancel evidence | Promoted: E1 packed globals, E2 direct resource-ID/order contract, E3 original queue-menu to `Customize Production Templates` transition and cancel return, plus final ledger synchronization are closed |

The `DrawProductionDlg` E2 packet is now natively closed: the strict C89 and
fresh VC++ 1.52c/Win98 probes both pass 10 checks for the exact `0x2fe`,
`0x33f`, `0x340`, and `0x0d2` resources plus the DGROUP `rgszMinerals` table
at `0x04cc`. The native process exited cleanly with empty redirected output
and no Program Error dialog. Evidence is recorded in
`analysis/compat/draw-production-dlg-resource-e2.{md,json}` and
`runs/vmctl-vc152dpdre2/`; E3 and final promotion are now also closed below.

The `DrawProductionDlg` E3 geometry boundary is now natively closed. The
strict C89 probe passes 9 checks for the original 20-pixel mineral-label inset,
measured
separator/right-text offsets, DC acquisition/release, both list controls,
selection traversal, ETA dispatch, and diamond geometry. Fresh VC++ 1.52c
validation also passes 9 checks with empty redirected output and no Program
Error dialog. Together with the original designer observation, this closes
E3. The source correction and probe are recorded in
`analysis/compat/draw-production-dlg-geometry-e3.{md,json}` and
`runs/vmctl-vc152dpdre3/`; final Step-6 promotion is recorded in the review
ledger, so DrawProductionDlg is now `6/6` and `0` open.

The `DrawPlanetShipList` control-layout packet is now natively closed. Its 7
checks cover the original `rghwndBtn[1,2,0]` position permutation, logical
show order, combo geometry, transfer lookup, reference reset, detail fill,
and enablement. Compile/link passed and no Program Error dialog appeared.
Evidence is recorded in
`analysis/compat/draw-planet-ship-list-controls-e2.{md,json}` and
`runs/vmctl-vc152dlist-controls-e2/`; the row is now `3 + P` with text/gauge
rendering and complete transfer/drawing integration still open.

The combo-width sub-boundary is now also explicit. The strict C89 probe passes
`DRAW_PLANET_SHIP_LIST_COMBO_CHECKS=4 FAILURES=0 RESULT=PASS` for the computed
112-pixel width, raw `0x14` new-width flags, raw `0x15` repeated-width flags,
and repeated transfer lookup. This remains host-only and closes no
authoritative Step-5 unit; native control flags, complete transfer/drawing
behavior, localized text/gauges, original differential, and promotion remain
open. Evidence is in
`analysis/compat/draw-planet-ship-list-combo-e2.{md,json}`.

The `DrawPopup` detailed-GDI state packet is now natively validated. Its 7
checks cover ordinary and kind-14 slot-hull dispatch, the bold/opaque
transition, adjusted slot geometry, and reverse state restoration; no Program
Error dialog appeared. This is supporting E3 evidence and closes no named
Step-5 unit yet because localized detailed rendering and caller differential
evidence remain. The probe and packet are recorded in
`analysis/compat/draw-popup-state-e3.{md,json}` and
`runs/vmctl-vc152drawpopupstatee3/`; the row remains `3 + P`.

The `DrawPlanetStats` formatting boundary is now natively closed. Its strict
C89 and fresh VC++ 1.52c probes pass 16 checks through the new paired
`format_value`/`draw_formatted_value` callbacks: all seven value rows,
fixed-point mineral-rate text, zero mineral rate, and unavailable
scanner/defense text. Existing legacy and packed probes still pass; no Program
Error dialog appeared. The packet is recorded in
`analysis/compat/draw-planet-stats-format-e2.{md,json}` and
`runs/vmctl-vc152dpsfe2/`; the `DrawPlanetStats` row is now `2 + P` with
complete packed/race lookup and service behavior, detailed GDI/original
comparison, and promotion still open.

The packed lookup seam is now explicit and natively closed. A typed
`load_data` callback populates the
`StarsPlanetStatsData` from the recovered 56-byte planet view before drawing;
the strict C89 probe passes `DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0
RESULT=PASS` for identity, scanner/defense bits, mineral rate, and all seven
row consumers. The packet and evidence are in
`analysis/compat/draw-planet-stats-lookup-e1.{md,json}` and
`compat/vc152/draw-planet-stats-lookup-e1/` and
`runs/vmctl-vc152dpsle1/`; compile/link passed, the native 9-check report
passed, and no Program Error dialog appeared. This closes E1; the remaining
service/GDI/original boundaries are still open.

The `PtDisplayPlanetPopInfo` formatting boundary is now closed. The optional
`prepare_segment` callback sees every stream and wrap segment after branch
selection, including the original fixed-point pair `2,5` for negative
desirability `-25`. The strict C89 and fresh VC++ 1.52c probes each pass all
27 checks; the native run is preserved in
`runs/vmctl-vc152ppfe2/`. This closes E2 and advances the procedure to **1/3**
authoritative units; E1 packed lookup, E3 detailed GDI/original evidence, and
promotion remain.

The `PtDisplayPlanetStateInfo` formatting boundary is now native-complete. The
optional `prepare_segment` callback sees every selected
stream/wrap segment and preserves the environment service's kind-2 value
handoff. The strict C89 source probe passes 35 checks, the packed 23-check
probe still passes, and the fresh VC++ 1.52c packet is recorded in
`analysis/compat/pt-display-planet-state-info-format-e2.{md,json}` and
`compat/vc152/pt-display-planet-state-info-format-e2/`. This closes E2 and
advances the row to `2 + P`; E1 lookup, E3 GDI/original evidence, and promotion
remain.

No recent procedure is being claimed as complete merely because its source
probe or native report passes. A row moves from `5/6` to `6/6` only after its
entire listed acceptance boundary is evidenced, synchronized in the ledgers,
and promoted.

### Step-5 workload view

The ledger below is the practical burn-down view for Phase 2. Read each row as
`N + P`:

- **N** is the number of named Step-5 evidence units still open for that
  procedure. Each unit is a concrete closure boundary, such as a packed-record
  comparison, a resource/localization check, a live USER/GDI slice, or an
  original differential/replay.
- **P** is one final promotion item. It is not a second behavior probe: it is
  the ledger, coverage, TODO5, and reconstruction-note synchronization that
  follows once N reaches zero.
- A row reaches the last gate at **`0 + P`**, then reaches `6/6` only when P is
  also closed.

Therefore **`5/6` is not “one test left.”** It means the procedure has a
readable source reconstruction and is in the evidence/validation gate, but it
may still have several named E-units. For example, after the latest checkpoint
`DrawPlanetStarbase` now reads `0 + P`: all named E1/E2/E3 evidence is closed;
only final ledger promotion remains. The compact
fraction stays `5/6` until every named E-unit and final promotion close; the `N + P` row and
the expanded table are the authoritative workload view.

The current open-work shape is:

| Relative load | Procedures | Open evidence units | Meaning |
|---|---:|---:|---|
| S | 0 | 0 | No single-unit evidence boundary remains |
| M | 0 | 0 | No one-unit procedure remains |
| L | 3 | 8 | Battle and report evidence chains remain; ExecuteReportClick has two E-units left |
| XL | 6 | 20 | Turin, Rototill, and Macinti have four each; Robotoid and Automitron have three each; Cyber has two |
| **Evidence total** | **9** | **28** | **Step-5 behavior/integration work still open** |
| **Promotion** | **9** | **9** | **Step-6 synchronization items after evidence closes** |
| **All open work items** |  | **37** | **The useful project-level burn-down number** |

These bands are deliberately relative rather than calendar estimates: one
large original differential can take longer than several small field checks.
They still make the shape of the work visible. `EstFuelUse` and
`FStargateJump` have closed their remaining evidence and promotion items;
`LFuelUseToWaypoint` has closed both evidence units and its promotion item;
`AutoRouteFleet` has now closed E2 and its promotion item, and
`UpdatePlayerScores` has closed its populated-score and turn/report replay,
`Popup` has closed its content-measurement and packed-payload boundary, and
`PopupMenu` has closed its real owner/message-queue and packed caller-record
boundary, and `GenerateWorld` has closed its packed game-state/seed/RNG and
state-backed pipeline boundary. The order/accounting row now has two open
items after SO-E2 closure;
the other work is concentrated in the L and XL rows. The six AI-turn rows
and the broad UI/report rows are mostly in the four-unit band. A passing native check
reduces the count only when it closes one of the
named units in the row; extra checks that strengthen an already-covered unit
do not change the count.

For planning, the shortest honest sequence is therefore: take the L rows in
dependency order, and reserve the XL rows for shared packed-record, USER/GDI, turn-replay, and
AI infrastructure that can close several concerns together. This is a
work-order recommendation, not a claim that all rows have equal duration.

| Cluster | Procedures | 4/6 | 5/6 | 6/6 |
|---|---:|---:|---:|---:|
| Creation and new-game flow | 9 | 0 | 0 | 9 |
| Planet, fleet, production, orders, and research | 25 | 0 | 1 | 24 |
| Combat | 2 | 0 | 2 | 0 |
| AI personalities | 6 | 0 | 6 | 0 |
| Reports and popups | 9 | 0 | 1 | 8 |
| **Total** | **51** | **0** | **10** | **41** |

### Procedure-by-procedure matrix

| Procedure | Gate state (headline; step 5 active) | Remaining gate before promotion |
|---|---:|---|
| `GenerateWorld` | 6/6 | Promoted: state-backed layout-to-file/finalization pipeline, original Win98 generation observation, and final ledger synchronization are closed |
| `NewGameWizard` | 6/6 | Promoted: packed player storage, all three page resources/control geometry, complete creation replay, and ledger synchronization are closed |
| `SimpleNewGameDlg` | 6/6 | Promoted: packed player record, resource/control geometry, accepted creation/caller boundary, original custom-race replay, and ledger synchronization are closed |
| `NewGameDlg` | 6/6 | Promoted: packed game record, resource/control geometry, complete creation transition, original replay, and ledger synchronization are closed |
| `NewGameDlg2` | 6/6 | Promoted: packed Create/Open/Edit race records, password rejection/acceptance, resource geometry, transition replay, and ledger synchronization are closed |
| `DrawNewGame2` | 6/6 | Promoted: packed player/type tables, localized formatting, complete wizard-page GDI integration, original replay, and ledger synchronization are closed |
| `DrawNewGame3` | 6/6 | Promoted: packed signed-width table, complete rectangles, localized rendering, full page GDI/original comparison, and ledger synchronization are closed |
| `IDropPart` | 6/6 | Promoted: E1 packed slot hit table, E2 drag-state mutation, E3 redraw/control integration, and final ledger synchronization are closed |
| `FillBuildDD` | 6/6 | Promoted: packed design records, localized labels/resources, complete population/control integration, and final ledger synchronization are closed |
| `PlanetWndProc` | 6/6 | Promoted: E1 exact child layout/resources/edit subclass, E2 packed transfer lookup, E3 message/focus/drawing integration, and final ledger synchronization are closed |
| `DrawPlanetStats` | 6/6 | Promoted: E1 packed lookup, E2 fixed-point/zero-rate/unavailable formatting, E3 typed race/resource/scanner/defense service resolution, E4 detailed GDI/original integration, and final ledger synchronization are closed |
| `DrawPlanetStarbase` | 6/6 | Promoted: packed lookup, resources, exact service/GDI boundary, and final ledger synchronization are closed |
| `DrawPlanetProduction` | 6/6 | Promoted: packed records, resources, exact geometry/control service, original differential, and final ledger synchronization are closed |
| `DrawPlanShipBitmap` | 6/6 | Promoted: E1 GDI lifecycle, E2 production-source packed `_sel` selection, E3 exact localized fleet/planet names, E4 PlanetWndProc WM_PAINT/original frame integration, and final ledger synchronization are closed. |
| `DrawPlanetShipList` | 6/6 | Promoted: E1 packed fleet/thing transfer records, E2 exact Win16 controls/geometry, E3 localized text/gauges/detail rendering, E4 complete drawing/transfer integration, original frame evidence, and final ledger synchronization are closed. |
| `TransferStuff` | 6/6 | Promoted: packed records, exact cargo mutation, complete lookup/write-back, Win16 transfer-dialog message path, rejection/refresh behavior, and ledger synchronization are closed |
| `FTrackXfer` | 6/6 | Promoted: E1/E2 packed records and exact `XferSupply`/`ChgCargo` mutation, E3 packed lookup/store, real Win16 task queue/capture, direct dialog mutation, rejection/refresh differentials, and ledger synchronization are closed |
| `EstFuelUse` | 6/6 | Promoted: 29-check original-instruction differential across packed design/hull tables, engine/warp rows, race/range modes, and route distances |
| `LFuelUseToWaypoint` | 6/6 | Promoted: original route differential and full packed ship/waypoint integration are closed |
| `ZipOrderDlg` | 6/6 | Promoted: packed orders, localized resources, real USER/message-loop mutation, and original Rename Fleet caller boundary are closed |
| `FStargateJump` | 6/6 | Promoted: E1 original damage-service differential and E2 real game-service cargo comparison are closed; lost-fleet construction, surviving-source preservation, destination transfer fields, and five-resource redistribution are checked |
| `KillUsedWaypoints` | 6/6 | Promoted: live-table, resource, complete-turn integration, and final ledger synchronization are closed |
| `AutoRouteFleet` | 6/6 | Promoted: E1 raw route differential, E2 real resource/helper integration, complete live fleet-turn comparison, and final ledger synchronization are closed |
| `DrawMineSurvey` | 6/6 | Promoted: E1 detailed GDI layout, E2 packed-record adapter, E3 localized/complete mine-survey caller integration, and final ledger synchronization are closed |
| `MineClick` | 6/6 | Promoted: E1 packed payload adapter, E2 localized/derived popup resolver, E3 complete minefield caller integration, original selection evidence, and final ledger synchronization are closed |
| `EstMineralsMined` | 6/6 | Promoted: 13-check packed/instruction-differential matrix closed the populated turn boundary, including pre-efficiency depletion, percentage-cap reset, and two fleet contributions |
| `ProductionDlg` | 6/6 | Promoted: E1 packed globals, E2 direct resource-ID/order contract, E3 original queue-menu to `Customize Production Templates` transition and cancel return, and final ledger synchronization are closed |
| `DrawProductionDlg` | 6/6 | Promoted: E1 packed globals, E2 resource/formatting, E3 GDI/caller integration, and final ledger synchronization are closed |
| `EstimateItemProdSched` | 6/6 | Promoted: E1 original-instruction-derived per-turn/complete schedule differential, E2 packed production-record/resource mutation, E3 resource/queue service boundary, and final ledger synchronization are closed. |
| `ZipProdDlg` | 6/6 | Promoted: packed 40-byte queue/name layout, real USER control/resource loop, real listbox population, original Add/Remove/Item Up/Item Down/Clear/resource-total sequence, and final ledger synchronization are closed |
| `ResearchDlg` | 6/6 | Promoted: Step-5 evidence and final review-ledger synchronization are complete |
| `BrowserDlg` | 6/6 | Promoted: real USER/control/message-loop validation, direct original Ship & Starbase Designer caller-path differential, component selection/group/detail behavior, and final ledger synchronization are closed |
| `UpdatePlayerScores` | 6/6 | Promoted: six-case populated-score oracle, packed report checksum, message/dirty invalidation, complete turn/report replay, and final ledger synchronization are closed |
| `SatisfyOrders` | 6/6 | Promoted: SO-E1 exact task-5 accounting, SO-E2 complete service/repricing boundary, SO-E3 same-input full-turn and six-file original differential, and final ledger synchronization are closed. |
| `FAttack` | 5/6 | Step 5: still 0/3 named units closed. E31 corrects group-0x20 selector metadata; E32/E33 validate beam scaling and broadcast-vs-complex falloff. E34 binds original group-0x10 `dp`; E35 binds `init`; E36 binds all beam `dRangeMax` values; E37 binds torpedo `init`/`dRangeMax`; E38 wires decoded battle-token context into FAttack (native 29/29); E39 decodes raw four-byte `_hs` slots (native 16/16); E40 composes context+slot adapters in the real battle callback (33/33); E41 drives player/design through `stars_lpshdef_from_tok`, selects alternate design 2 for `0x12`, reads `_hul.rghs`/`chs`, and verifies lookup/damage (38/38); E42 covers normal and alternate callback branches (44/44); E43 reads actual-layout raw DGROUP entries at `0x00FE`/`0x014C`, applies far-pointer segment/offset and `0x93` stride, and verifies selected `1234:2126`/`2345:12DF` addresses in native integration (44/44). E46 reconciles all 16 FLookupPart assembly branches: each stores current CS and the literal per-family base/stride; host and fresh native table probes verify all selected offsets (66/66). E44's ZIP stride and E45's `0x10e0`/DGROUP selector interpretation are both superseded. E46 still does not bind the current-CS table pointer to live memory. E1 needs actual native table-memory binding, complete `FLookupPart` race/hidden/technology services, packed target resolution, and beam/torpedo service acceptance including hull/resource/shield/jam/capacity; E2 broader original damage differentials and E3 full battle replay remain. |
| `FDoCoolBattle` | 5/6 | Step 5: 1/6 subchecks closed (staged log + host/native attack chain); E7 checks all 29-byte scalar/bitfield values, 32-byte stride, sentinels, and targeted write-back. E38 validates packed-token decode through the attack callback (native 29/29); E40 binds callback raw `_hs` bytes through lookup/damage (33/33); E41 derives alternate per-player design selection and reads `_hul.rghs`/`chs` (38/38); E42 tests normal/alternate table branches (44/44); E43 consumes raw DGROUP far-pointer entries and resolves both exact segmented hull addresses (44/44). These close 0 additional units. Actual game DGROUP/hull memory binding; movement geometry/speed/initiative; complete damage/shield/casualty/salvage/bombing/report records; full-battle replay and original differential remain. |
| `DoRobotoidAiTurn` | 5/6 | E1 first-order repair is closed in fresh native multi-order replay; exact colonizer/transport routing, broader raw PLORD mutation, complete AI-table integration, and full original-turn differential remain |
| `DoTurinDroneAiTurn` | 5/6 | E1 repair 18/18, design loops 31/31, seed/base/finalization 21/21, group-8..9 technology 21/21, design-1/minelayer 32/32, support fleet 23/23, and packed planet/fleet/queue with default potency 17/17 pass native; live table/service integration, E2 routing, E3 broader PLORD mutation, E4 original-turn differential, and promotion remain |
| `DoAutomitronAiTurn` | 5/6 | E2 packed first-order repair is closed in fresh native multi-order replay; E1 colonizer/mineral fleet handling, E3 route/broader packed-order integration, E4 original behavior, and promotion remain |
| `DoRototillAiTurn` | 5/6 | RT-E1 packed queue/census/AI-planet/lifecycle source has 20/20 host checks plus a 12/12 post-init/post-add live-PLPROD probe; RT-E2 packed fleet entry, markers, and occupied-target order repair have 17/17 host checks. Live production global/native/original acceptance, broader routing, wider PLORD mutation, complete AI-table integration, and full original-turn differential remain |
| `DoMacintiAiTurn` | 5/6 | Host packed first-order repair is now checked; per-planet table selection, exact retarget/colonizer/transport predicates, broader direct packed order mutation, and full original-behavior evidence remain |
| `DoCyberAiTurn` | 5/6 | Packet/freighter behavior and detailed repair/order writes are closed by the E5/E7/E8/E9 native matrices; E10 now covers all nine named action dispatches and seeded packed writes, while raw personality branch coverage, full AI-table integration, original-turn evidence, and promotion remain |
| `ReportDlg` | 6/6 | Promoted: E1 packed `_rpt` view load, E2 populated report unions/rows plus live USER/GDI/menu payloads, E3 native/original caller integration, and final Step-6 ledger synchronization are closed in fresh production-source replays in `C:\RPTVEP`, `C:\RPE2`, and `C:\RPTDINT`. |
| `ExecuteReportClick` | 5/6 | ER-E1 exact live-original `GlobalPD` union differential is closed: eight captured popup payloads, host/native `C:\GPDI7` 49/49, and persistent 22-byte production storage. E2 live USER/GDI integration and E3 complete report-layout/caller integration remain; Step-6 promotion awaits both. |
| `DumpPlanets` | 6/6 | Promoted: E1 localized normal serialization, E2 complete original normal export, E3 packed-service resolution, E4 original Win98 report-menu/file-write behavior, and final Step-6 ledger synchronization are closed |
| `DumpFleets` | 6/6 | Promoted: E1 localized scanner/task formatting, E2 serialized field-order, E3 packed-service, E4 complete original-output differential, and final Step-6 ledger synchronization are closed |
| `DrawPopup` | 6/6 | Promoted: E1 complete `GlobalPD` union payloads, E2 localization/hull filtering/formatting, E3 detailed GDI/caller integration, E4 original consumer completion, and Step-6 ledger synchronization are closed by the fresh native replays and retained original Win98 consumer captures. |
| `Popup` | 6/6 | Promoted: all measured/fixed popup kinds, localized content measurement, packed fleet/planet/hull payload boundary, detailed layout, and final ledger synchronization are closed |
| `PopupMenu` | 6/6 | Promoted: real owner/message-queue behavior without a harness bridge, exact `PeekMessage` arguments, packed fleet/thing/planet/deep lookup, and final ledger synchronization are closed |
| `PtDisplayPlanetStateInfo` | 6/6 | Promoted: E1 packed lookup, E2 localized/fixed-point formatting, E3 detailed stream/wrap GDI/original comparison, and final ledger synchronization are closed. |
| `PtDisplayPlanetPopInfo` | 6/6 | Promoted: E1 packed lookup, E2 localized/fixed-point formatting, E3 detailed stream/wrap GDI/original comparison, and final ledger synchronization are closed. |

### Step-5 burn-down ledger

The headline `5/6` is now decomposed below. An **evidence unit** is one
independently closable acceptance boundary, such as a packed-record adapter, a
resource/formatting comparison, a live USER/GDI integration slice, or an
original differential/replay slice. The number is a work-item count, not a
time estimate: a single unit can be small or require shared infrastructure.
`+P` means that final ledger synchronization and promotion are still required
after the numbered evidence units reach zero. The unit descriptions below are
the actual closure boundaries; a passing native check only closes the named
unit it exercises.

| Procedure | Open step-5 units | Remaining units, individually |
|---|---:|---|
| `GenerateWorld` | 0 | Closed: E1 packed state/seed/RNG/turn data plus E2 state-backed layout-to-file/finalization pipeline and original Win98 generation comparison are complete; promoted in fresh `C:\GWINT2` |
| `NewGameWizard` | 0 | Promoted: E1 packed player/game state, E2 all three page resources/control geometry, E3 complete creation transition/original replay, and Step-6 synchronization are closed |
| `SimpleNewGameDlg` | 0 | Promoted: E1 packed player record, E2 resources/control geometry, E3 accepted creation/caller boundary, original custom-race replay, and Step-6 synchronization are closed |
| `NewGameDlg` | 0 | Promoted: E1 packed game record, E2 resources/geometry, E3 complete creation transition/original replay, and Step-6 synchronization are closed |
| `NewGameDlg2` | 0 | Promoted: E1 packed race/password records, E2 resource/control geometry, E3 transition/replay, original menu replay, and Step-6 synchronization are closed |
| `DrawNewGame2` | 0 | Promoted: E1 packed player/type tables, E2 localized text/formatting, E3 complete page GDI integration, original replay, and ledger synchronization are closed |
| `DrawNewGame3` | 0 | Promoted: E1 packed signed-width table, E2 rectangles, E3 localization, E4 complete page GDI/original comparison, and Step-6 synchronization are closed |
| `IDropPart` | 0 | Promoted: E1 packed hit table, E2 drag mutation, E3 redraw/control integration, and Step-6 synchronization are closed |
| `FillBuildDD` | 0 | Promoted: E1 packed design records, E2 labels/resources, E3 complete population/control integration, and Step-6 synchronization are closed |
| `PlanetWndProc` | 0 | Promoted: E1 exact child layout, E2 packed transfer lookup, E3 message/focus/drawing integration, original planet-window evidence, and Step-6 synchronization are closed |
| `DrawPlanetStats` | 0 | Promoted: E4 detailed GDI/original integration and final ledger synchronization are closed |
| `DrawPlanetStarbase` | 0 | Promoted: E1 packed lookup, E2 localized resources, E3 exact service/GDI boundary, and Step-6 synchronization are closed |
| `DrawPlanetProduction` | 0 | Promoted: E1 packed records, E2 localized labels, E3 exact geometry/control service, E4 original differential, and Step-6 synchronization are closed |
| `DrawPlanShipBitmap` | 0 | Promoted: E4 PlanetWndProc WM_PAINT/original planet-fleet frame integration and Step-6 synchronization are closed in fresh `C:\DPSBE4` |
| `DrawPlanetShipList` | 0 | Promoted: E1 packed fleet/thing records, E2 exact controls/geometry, E3 detail rendering, E4 complete drawing/transfer integration, original frame evidence, and Step-6 synchronization are closed |
| `TransferStuff` | 0 | Promoted: E1 packed records, E2 exact cargo mutation, E3 complete packed lookup/write-back, Win16 transfer-dialog message path, rejection/refresh behavior, and Step-6 synchronization are closed |
| `FTrackXfer` | 0 | Closed: E1/E2 packed records and exact `XferSupply`/`ChgCargo` mutation, E3 packed lookup/store, real Win16 task queue/capture, direct dialog mutation, rejection/refresh differentials, and Step-6 promotion are synchronized |
| `EstFuelUse` | 0 | Closed: E1 original-instruction differential over packed component/hull and route-distance tables and Step-6 promotion are recorded in the review ledger |
| `LFuelUseToWaypoint` | 0 | Closed: E1 route differential, E2 full packed ship/waypoint integration, and Step-6 promotion are recorded in fresh `C:\FUTINT3`/`C:\FUTINT4` |
| `ZipOrderDlg` | 0 | Closed: packed 24-byte order/name storage, exact localized resources, real USER Create/Rename/Delete/Help/OK loop, original Rename Fleet caller/cancel boundary, and Step-6 promotion are recorded |
| `FStargateJump` | 0 | Closed: E1 original damage-service differential and E2 real game-service cargo comparison are recorded in fresh `C:\SGJINT4`/`C:\SGJINT5`; Step-6 promotion is complete |
| `KillUsedWaypoints` | 0 | Promoted: E1 live-table enumeration, E2 resources, E3 complete-turn integration, and Step-6 synchronization are closed |
| `AutoRouteFleet` | 0 | Closed: E1 raw route differential, E2 real resource/helper integration, complete live fleet-turn comparison, and Step-6 promotion are recorded in fresh `C:\ARFINT4`/`C:\ARFINT5` |
| `DrawMineSurvey` | 0 | Closed: E1 detailed GDI layout, E2 packed records, E3 localization/complete survey integration, and Step-6 promotion are recorded |
| `MineClick` | 0 | Closed: E1 packed payload adapter, E2 localized/derived popup resolver, E3 complete minefield caller integration, original selection evidence, and Step-6 promotion are recorded |
| `EstMineralsMined` | 0 | Closed: E1 populated mining-turn original-instruction differential and Step-6 promotion are recorded in the review ledger |
| `ProductionDlg` | 0 | Closed: E1 packed globals, E2 direct resource-ID/order contract, E3 original queue-menu/editor transition and cancel return, and Step-6 promotion are recorded |
| `DrawProductionDlg` | 0 | Closed: all named Step-5 units and final ledger promotion are recorded |
| `EstimateItemProdSched` | 0 | Promoted: E1 original-instruction-derived per-turn/complete schedule differential, E2 packed production-record/resource mutation, E3 resource/queue integration, and final ledger synchronization are closed |
| `ZipProdDlg` | 0 | Closed: packed 40-byte queue/name layout, real USER control/resource loop, real listbox population, original Add/Remove/Item Up/Item Down/Clear/resource-total sequence, and Step-6 promotion are recorded |
| `ResearchDlg` | 0 | Closed: Step-5 E1 and Step-6 promotion are recorded in the review ledger |
| `BrowserDlg` | 0 | Closed: real USER/control/message integration and direct original Ship & Starbase Designer caller-path differential are recorded; Step-6 promotion is complete |
| `UpdatePlayerScores` | 0 | Closed: six-case populated-score oracle, packed report checksum, message/dirty invalidation, complete turn/report replay, and Step-6 promotion are recorded in fresh `C:\UPSINT2` |
| `SatisfyOrders` | 0 | Promoted: SO-E1, SO-E2, SO-E3 and Step-6 audit are closed for their documented boundaries. |
| `FAttack` | 3 + P | E1 group-0x20 original torpedo `dp`/`init`/`dRangeMax` and group-0x10 beam `dp`/`init`/`dRangeMax` plus the packed token-to-caller context adapter are supported by E29/E34/E35/E36/E37/E38, but complete weapon/hull/target wiring and raw beam/torpedo service acceptance (including hull/resource/shield/jam/capacity) remain; E2 broader original damage differentials; E3 full native battle replay and final differential |
| `FDoCoolBattle` | 3 + P | E1 exact native `_tok`/`vrgtok` binding, global compaction/production callers, and native field map (host 29-byte map and fresh packed-token-to-attack-callback bridge are checked; the 29-check adapter replay is supporting evidence); E2 movement geometry/speed/initiative and complete damage/shield/casualty/salvage/bombing records; E3 fresh native/original full-battle replay and final differential |
| `DoRobotoidAiTurn` | 3 + P | E1 first-order repair closed by original-instruction audit and fresh 14/14 host/native multi-order mutation replay; E2 route plan/apply seam now separates colonizer/transport/attack families, but exact target predicates and order writes remain; E3 broader raw PLORD mutation; E4 complete AI-table/original-turn comparison |
| `DoTurinDroneAiTurn` | 4 + P | E1 repair 18/18, design loops 31/31, seed/base/finalization 21/21, group-8..9 technology 21/21, design-1/minelayer 32/32, support fleet 23/23, packed fields/default potency 17/17, AI-planet writes 26/26, and desirability gate 27/27 on host/native; live tables and actual service effects remain; E2 route plan/apply separates action families but exact searches/writes remain; E3 broader raw PLORD mutation; E4 AI-table/original-turn comparison |
| `DoAutomitronAiTurn` | 3 + P | E2 packed first-order repair closed by original instructions and fresh 22/22 host/native three-order replay, including callback-independent execution; E1 colonizer/mineral handling, E3 route plan/apply and broader packed-order integration, E4 original-behavior evidence remain |
| `DoRototillAiTurn` | 4 + P | E1 queue construction; E2 routing; E3 raw PLORD mutation; E4 complete AI-table/original-turn comparison |
| `DoMacintiAiTurn` | 4 + P | E1 planet-table selection; E2 retarget predicates; E3 direct order mutation; E4 complete original-behavior evidence |
| `DoCyberAiTurn` | 2 + P | E1 packet/freighter behavior closed by the combined E5/E7/E8 native matrices; E2 detailed repair/order writes closed by E9; E3 raw personality predicates and broader original-turn packed PLORD mutation (E10 covers nine action dispatches; E12 covers multi-order packed replacement/append semantics; both are supporting evidence); E4 complete AI-table/original-turn evidence |
| `ReportDlg` | 0 | Promoted: E1/E2/E3 and final Step-6 ledger synchronization are closed |
| `ExecuteReportClick` | 2 + P | E1 exact original owner/part union values and persistence closed by eight live payloads and host/native `C:\GPDI7` 49/49; E2 live USER/GDI layout remains; E3 complete report-layout caller cases remain (the 26-check production `ExecuteReportClick` → `Popup` → `DrawPopup` replay supports, but does not close, E3) |
| `DumpPlanets` | 0 | Promoted: E1 localized normal serialization, E2 complete original normal export, E3 packed-service resolution, E4 original Win98 report-menu/file-write behavior, and final Step-6 ledger synchronization are closed |
| `DumpFleets` | 0 | Promoted: all four named Step-5 evidence units and final Step-6 ledger synchronization are closed |
| `DrawPopup` | 0 | Promoted: E1/E2/E3/E4 and final Step-6 ledger synchronization are closed |
| `Popup` | 0 | Closed: 148-check all-kind content-measurement matrix, packed popup/fleet/planet/hull payload checksums, geometry oracle, Win98 replay, and Step-6 promotion are recorded in fresh `C:\PUINT2` |
| `PopupMenu` | 0 | Closed: E1/E2 real owner/message-queue and packed caller-record lookup are recorded in fresh `C:\PMUINT3`; Step-6 promotion is complete |
| `PtDisplayPlanetStateInfo` | 0 | Promoted: E1 packed lookup, E2 localized fixed-point formatting, E3 detailed stream/wrap GDI/original comparison, and Step-6 synchronization are closed |
| `PtDisplayPlanetPopInfo` | 0 | Promoted: E1 packed lookup, E2 localized fixed-point formatting, E3 detailed stream/wrap GDI/original comparison, and Step-6 synchronization are closed |

At the checkpoint represented by the table above, the ledger had **30 open
evidence units plus 10 promotion units**. The current count is **29 evidence
units plus 10 promotions**, after SO-E2 was closed; use
[TODO7](TODO7.md) for the live burn-down. A completed procedure removes its remaining
evidence units first, then removes one `+P` unit when its review record,
coverage, and reconstruction note are promoted. This gives the project a
meaningful progress counter even when the headline distribution remains
`10 at 5/6` and forty-one promoted `6/6` rows. In particular, a meaningful
progress update should say which `E` unit was closed and show the new total;
it should not merely report that another probe passed.

### Step-5 scorecard: how much of each procedure is actually left

The headline gate is supplemented here with an evidence-unit score. `E done /
E planned` counts the named Step-5 boundaries already closed for that
procedure. `E open` is the number in the burn-down ledger above; `P` is the
separate final promotion item. This is still not a time estimate—an open UI or
turn-replay unit can require more work than several small field checks—but it
does distinguish “one focused boundary remains” from “the whole integration
packet remains.”

The current ledger defines **152 named Step-5 evidence units** across the 51
procedures: **123 are closed, 29 remain**, and **10 promotion units** remain.
Thus the project is not at “one test left per procedure”: 123 of the 152
named evidence boundaries have closed so far, even though every procedure has
reached the Step-5 gate. The scorecard is the quickest way to see that
difference.

| Procedure | E progress | E open | Remaining shape |
|---|---:|---:|---|
| `GenerateWorld` | 2/2 | 0 | Promoted; no Step-5 packet remains |
| `NewGameWizard` | 3/3 | 0 | Promoted; packed player state, all three page resources/control geometry, complete creation replay, and ledger synchronization are closed |
| `SimpleNewGameDlg` | 3/3 | 0 | Promoted; packed player record, resources/geometry, accepted creation/caller boundary, original replay, and ledger synchronization are closed |
| `NewGameDlg` | 3/3 | 0 | Promoted; packed game record, resources/geometry, complete creation replay, and ledger synchronization are closed |
| `NewGameDlg2` | 3/3 | 0 | Promoted; packed race/password records, resource geometry, transition replay, original menu boundary, and ledger synchronization are closed |
| `DrawNewGame2` | 3/3 | 0 | Promoted; packed tables, localized formatting, full page GDI integration, original replay, and ledger synchronization are closed |
| `DrawNewGame3` | 4/4 | 0 | Promoted; E1 signed-width table, E2 rectangles, E3 localization, and E4 page integration/original comparison are closed |
| `IDropPart` | 3/3 | 0 | Promoted; E1 hit table, E2 drag mutation, E3 redraw/control integration, and ledger synchronization are closed |
| `FillBuildDD` | 3/3 | 0 | Promoted; E1 packed design records, E2 labels/resources, E3 complete population/control integration, and ledger synchronization are closed |
| `PlanetWndProc` | 3/3 | 0 | Promoted; E1 exact child layout/resources/edit subclass, E2 packed transfer lookup, E3 message/focus/drawing integration, original planet-window evidence, and ledger synchronization are closed |
| `DrawPlanetStats` | 4/4 | 0 | Promoted; detailed GDI/original differential, native parent-paint replay, and final ledger synchronization are closed |
| `DrawPlanetStarbase` | 3/3 | 0 | Promoted; no Step-5 packet remains |
| `DrawPlanetProduction` | 4/4 | 0 | Promoted; no Step-5 packet remains |
| `DrawPlanShipBitmap` | 4/4 | 0 | Promoted; E4 PlanetWndProc WM_PAINT/original planet-fleet frame integration and ledger synchronization are closed |
| `DrawPlanetShipList` | 4/4 | 0 | Promoted; E1 packed records, E2 exact controls/geometry, E3 detail rendering, E4 complete drawing/transfer integration, and original frame evidence are closed |
| `TransferStuff` | 3/3 | 0 | Promoted; E1 packed records, E2 exact cargo mutation, E3 packed lookup/write-back, real Win16 dialog-message path, rejection/refresh behavior, and Step-6 synchronization are closed |
| `FTrackXfer` | 3/3 | 0 | Promoted: E1 packed records, E2 exact service mutation, E3 full lookup/USER/original differential integration, and Step-6 synchronization are closed |
| `EstFuelUse` | 1/1 | 0 | Promoted; no Step-5 packet remains |
| `LFuelUseToWaypoint` | 2/2 | 0 | Promoted; no Step-5 packet remains |
| `ZipOrderDlg` | 3/3 | 0 | Promoted; no Step-5 packet remains |
| `FStargateJump` | 2/2 | 0 | Promoted; no Step-5 packet remains |
| `KillUsedWaypoints` | 3/3 | 0 | Promoted; no Step-5 packet remains |
| `AutoRouteFleet` | 2/2 | 0 | Promoted; no Step-5 packet remains |
| `DrawMineSurvey` | 3/3 | 0 | Promoted; no Step-5 packet remains |
| `MineClick` | 3/3 | 0 | Promoted; no Step-5 packet remains |
| `EstMineralsMined` | 1/1 | 0 | Promoted; no Step-5 packet remains |
| `ProductionDlg` | 3/3 | 0 | Promoted; E1 packed globals, E2 direct resource-ID/order contract, and E3 original queue-menu/editor transition and cancel return are closed |
| `DrawProductionDlg` | 3/3 | 0 | Promoted; E1 packed globals, E2 formatting/resources/localization, E3 full GDI/caller integration, and Step-6 synchronization are closed |
| `EstimateItemProdSched` | 3/3 | 0 | Promoted after the original-instruction-derived E1 per-turn/record differential; E2 packed production-record mutation and E3 resource/queue integration were already closed |
| `ZipProdDlg` | 3/3 | 0 | Promoted; no Step-5 packet remains |
| `ResearchDlg` | 1/1 | 0 | Promoted; no Step-5 packet remains |
| `BrowserDlg` | 2/2 | 0 | Promoted; no Step-5 packet remains |
| `UpdatePlayerScores` | 2/2 | 0 | Promoted; no Step-5 packet remains |
| `SatisfyOrders` | 3/3 | 0 | Promoted: E3 complete pinned control/action turn matches 274 records and all six encrypted output files; E1/E2 remain closed. |
| `FAttack` | 0/3 | 3 | E1 original group-0x20 torpedo `dp`/`init`/`dRangeMax` and group-0x10 beam `dp`/`init`/`dRangeMax` have native support (E29/E34/E35/E36/E37); E38 maps packed token context into the actual callback; E32/E33 cover scaling and broadcast-vs-complex falloff. Complete weapon/hull/target wiring and raw beam/torpedo service acceptance, including hull/resource/shield/jam/capacity, remain; E2 broader original damage differentials; E3 full battle replay and final differential |
| `FDoCoolBattle` | 0/3 | 3 | E1 exact native `_tok`/`vrgtok` binding, global compaction/production callers, and native field map (host 29-byte map and fresh packed-token callback bridge checked); E2 movement geometry/speed/initiative and complete damage/shield/casualty/salvage/bombing records; E3 native full-battle replay and final differential |
| `DoRobotoidAiTurn` | 1/4 | 3 | E1 repair closed in `C:\ROBRE1`; E2 routing, E3 broader PLORD mutation, E4 full AI replay remain |
| `DoTurinDroneAiTurn` | 0/4 | 4 | E1 repair in `C:\TURRE1`, design loops in `C:\TURDQ1`, seed/base/finalization in `C:\TURSF1`, group-8..9 technology in `C:\TURTQ2`, design-1/minelayer in `C:\TURMI1`, support fleet in `C:\TURSP1`, packed fields/default potency in `C:\TURPK1`, AI-planet writes in `C:\TURAI1`, and desirability gate in `C:\TURDE1` pass; live table/service effects remain; E2 routing, E3 broader PLORD mutation, E4 full AI replay remain |
| `DoAutomitronAiTurn` | 1/4 | 3 | E2 repair closed in `C:\AUTRE3`; E1 colonizer/mineral handling, E3 route/broader packed orders, E4 original behavior remain |
| `DoRototillAiTurn` | 0/4 | 4 | E1 packed production/planet/AI-table host 20/20 plus live post-init/post-add queue host 12/12, but global/native/original queue acceptance open; E2 packed fleet entry/markers and occupied-target repair host 17/17 but broader routing open; E3 wider PLORD/AI table, E4 full AI replay open |
| `DoMacintiAiTurn` | 0/4 | 4 | Planet table; retarget predicates; order mutation; original behavior |
| `DoCyberAiTurn` | 2/4 | 2 | Packet/freighter behavior and detailed repair/order writes are closed by the E5/E7/E8/E9 native matrices; broader packed PLORD/personality branches and full AI replay remain |
| `ReportDlg` | — | Promoted: E1 packed `_rpt` view load, E2 populated row unions/live USER/GDI/menu payloads, E3 native/original caller integration, and Step-6 synchronization are closed |
| `ExecuteReportClick` | 1/3 | 2 | E1 pointer unions closed; USER/GDI and report-layout branches remain |
| `DumpPlanets` | 4/4 | 0 | Promoted; localized normal serialization, complete original normal export, packed service integration, original Win98 report behavior, and Step-6 synchronization are closed |
| `DumpFleets` | 4/4 | 0 | Promoted; localized scanner/task serialization, packed service integration, serialized field order, complete original output, and ledger synchronization are closed |
| `DrawPopup` | 4/4 | 0 | Promoted: E1 complete `GlobalPD` payloads, E2 localization/hull filtering/formatting, E3 detailed GDI/caller integration, E4 original consumer completion, and Step-6 synchronization are closed |
| `Popup` | 2/2 | 0 | Promoted; no Step-5 packet remains |
| `PopupMenu` | 2/2 | 0 | Promoted; no Step-5 packet remains |
| `PtDisplayPlanetStateInfo` | 3/3 | 0 | Promoted; E1 packed lookup, E2 fixed-point/localization, and E3 detailed GDI/original comparison are closed |
| `PtDisplayPlanetPopInfo` | 3/3 | 0 | Promoted; E1 packed lookup, E2 fixed-point/localization, and E3 detailed GDI/original comparison are closed |

### Expanded Step-5 work descriptions

The compact ledger above is the count used for progress accounting. The table
below is the execution-level interpretation of each open unit. It answers three
different questions that the `5/6` label cannot answer by itself:

1. **What still has to be built or compared?** This is the actual remaining
   engineering work, not a restatement of the procedure name.
2. **What evidence should come back?** This is the expected closure package:
   normally a strict host matrix, a fresh VC++ 1.52c/Win98 run, and either an
   original executable/instruction comparison or a documented integration
   boundary.
3. **What closes Step 5?** A procedure moves to `0 + P` only when the entire
   acceptance boundary named here is covered. Additional checks that merely
   repeat an already-covered boundary improve confidence but do not reduce the
   count.

The effort label is deliberately relative: **S** is one focused comparison,
**M** is two, **L** is three dependent comparisons, and **XL** is four broad
comparisons or a shared UI/turn/AI integration slice. These labels are not
calendar estimates. They make it clear why a row with two units may still take
longer than a row with three small field checks, and why shared harness work can
pay down several rows at once.

| Procedure | Load | Work still required at Step 5 | Evidence that closes Step 5 |
|---|:---:|---|---|
| `GenerateWorld` | — | Closed: E1 packed seed/RNG/turn state and E2 state-backed layout-to-file/finalization pipeline agree; original Win98 generation/artifact evidence is recorded. | Host/native 13-check E2 matrix passed in fresh `C:\GWINT2`; compile/link/process passed with no Program Error dialog, and the row is promoted. |
| `NewGameWizard` | — | Closed: E1 packed player/game initialization, E2 all three page resources/control geometry, E3 page transition/creation mutation, original replay, and Step-6 promotion are synchronized. | Host/native 30-, 11-, and 366-check packets, recovered NE layout, original replay, and final ledger promotion. |
| `SimpleNewGameDlg` | — | Closed: E1 complete 192-byte player-record copy, E2 resource/control mapping, E3 accepted creation/caller boundary, original custom-race replay, and Step-6 promotion are synchronized. | Host/native 3074-, 117-, and 12-check packets, original replay, review ledger, and derived ledgers are synchronized. |
| `NewGameDlg` | — | Closed: E1 game-record packing, E2 resource/control geometry, and E3 wizard mutation/creation replay all passed; Step-6 promotion is recorded. | Host/native 7-check packed probe, 30-check resource probe, 11-check creation probe, original replay, and synchronized ledgers. |
| `NewGameDlg2` | — | Closed: E1 packed Create/Open/Edit records and password boundary, E2 resource/control geometry, E3 transition/replay, original menu observation, and Step-6 promotion are synchronized. | Host/native 1,238-, 40-, and 9-check packets, original replay, review ledger, and derived ledgers are synchronized. |
| `DrawNewGame2` | — | Closed: E1 packed player/type tables, E2 localized formatting, E3 complete page GDI integration, original replay, and Step-6 promotion are synchronized. | Host/native 13-, 15-, and 16-check packets, raw instruction/table evidence, original replay, review ledger, and derived ledgers are synchronized. |
| `DrawNewGame3` | — | Closed: E1 packed signed-width table, E2 output rectangles, E3 localized rendering, E4 complete page integration/original comparison, and Step-6 promotion are synchronized. | Host/native E1–E4 packets, original page evidence, review ledger, coverage, TODO5, and reconstruction note. |
| `IDropPart` | — | Closed and promoted: E1 packed hit table, E2 drag-state mutation, E3 raw redraw/control service order, and Step-6 synchronization are complete. | Host/native E1–E3 packets, raw original service-order evidence, live designer boundary, review ledger, coverage, TODO5, and reconstruction note are synchronized; the controller's missing drag gesture remains an explicit limitation. |
| `FillBuildDD` | — | Closed and promoted: E1 packed design records, E2 localized labels/resources, E3 complete population/queue/resize/caller integration, and Step-6 synchronization are recorded. | Host/native E1–E3 packets, raw branch boundary, original designer observation, review ledger, coverage, TODO5, and reconstruction note are synchronized. |
| `PlanetWndProc` | — | Closed and promoted: E1 exact child layout/resources/edit subclass, E2 packed transfer lookup, E3 message/focus/drawing integration, original planet-window evidence, and final ledger synchronization are complete. | Fresh host/native E1–E3 packets, original Win98 observation, review ledger, coverage, TODO5, and reconstruction notes are synchronized. |
| `DrawPlanetStats` | XL | E1 complete planet/race lookup matrix; E2 fixed-point, zero-rate, scanner, and defense-bit boundaries; E3 typed race/resource/scanner/defense service resolution and localized comparison are now natively closed; E4 full GDI/caller/original integration remains. The production `stars_draw_planet_stats_packed` bridge owns the recovered 56-byte planet decode, and the fresh `C:\DPSSE3` packet replays the service seam under VC++ 1.52c. | Native GDI output/geometry and original formatting/branch comparison, followed by final ledger promotion. |
| `DrawPlanetStarbase` | — | Closed: E1 packed lookup, E2 localized resources, E3 exact service/GDI boundary, and Step-6 promotion are recorded. | Fresh `C:\DPSBN` host/native service matrix, review ledger, and reconstruction note are synchronized. |
| `DrawPlanetProduction` | — | Closed: E1 packed production-item records, E2 localized labels, E3 exact Win16 geometry/control integration, E4 original selected/empty/frame-refresh differential, and Step-6 promotion are recorded. | Host/native service report, original Win98 production captures, review ledger, and reconstruction note are synchronized. |
| `DrawPlanShipBitmap` | — | Closed and promoted: E1 GDI lifecycle, E2 packed selection state, E3 localized ship-name lookup, E4 PlanetWndProc WM_PAINT/original planet-fleet frame integration, and final ledger synchronization are complete. | Fresh `C:\DPSBE2`, `C:\DPSBE3`, and `C:\DPSBE4` production-source replays plus the original fleet-frame observation and synchronized review ledgers. |
| `DrawPlanetShipList` | — | Closed and promoted: E1 packed fleet/thing transfer records, E2 exact control handles, E3 localized text/gauges/detail dispatch, E4 complete drawing/transfer integration, and original behavior evidence are synchronized. | Fresh `C:\DLP4E` host/native 12-check integration packet, prior E1–E3 native packets, original fleet-frame evidence, review ledger, coverage, TODO5, and reconstruction note. |
| `TransferStuff` | — | Closed and promoted: E1 packed identity/kind/owner mapping, E2 exact cargo mutation, and E3 packed lookup/write-back, real Win16 transfer-dialog message sequence, rejection, refresh/invalidation, and original boundary are complete. | Host/native `TRANSFER_STUFF_E3_CHECKS=8` passes; VC++ 1.52c compile/link, process exit, no Program Error dialog, and final ledger promotion are recorded. |
| `FTrackXfer` | — | Closed: E1 packed identity/kind/owner/thing fields, E2 exact `XferSupply`/`ChgCargo` direction/clamp/destination/rollback mutation, and E3 full packed lookup/store, real Win16 task queue/capture, direct dialog mutation, rejection, refresh, and original differential boundary are complete. | Host/native `F_TRACK_XFER_PACKED_CHECKS=22`, `F_TRACK_XFER_SERVICE_E12_CHECKS=12`, and `F_TRACK_XFER_E3_CHECKS=8` pass; VC++ 1.52c compile/link, process exit, no Program Error dialog, and final ledger promotion are recorded. |
| `EstFuelUse` | S | Closed: the packed component/hull, engine/warp, race/range, and route-distance differential is complete. | Closed in `C:\EFUINT3`; promotion is recorded, so no Step-5 work remains. |
| `LFuelUseToWaypoint` | S | Closed: E1 route-sequence differential and E2 complete packed ship/waypoint integration are both evidenced. | Fresh host/native 27-check matrix covers all sixteen fleet/design slots, both cargo-hull IDs, the planet-selected hull definition, and the full two-segment order boundary; Step-6 promotion is complete. |
| `ZipOrderDlg` | L | Closed: E1 four packed 24-byte `_ziporder` records and mutation, E2 exact title/caption/empty-slot/action resources, and E3 real USER/message-loop Create/Rename/Delete/Help/OK integration plus original Rename Fleet Cancel boundary all pass. | Host/native packed `ZIP_ORDER_DLG_PACKED_CHECKS=11`, native resource `ZIP_ORDER_DLG_CHECKS=11`, and native USER `ZIP_ORDER_DLG_USER_CHECKS=12` all pass; compile/link passed and no Program Error dialog appeared. |
| `FStargateJump` | S | Closed: the real balance service is wired to the recovered surviving-source/lost-fleet boundary, including destination fields and resource mutation. | Host/native 2,311-check service-integrated matrix passed in `C:\SGJINT5`; E1 and E2 are closed and the row is promoted. |
| `KillUsedWaypoints` | — | Closed: E1 live-table enumeration, E2 resources, E3 complete-turn integration, and Step-6 promotion are recorded. | Fresh `C:\KUWTURN` host/native turn matrix, review ledger, and reconstruction note are synchronized. |
| `AutoRouteFleet` | S | Closed: E1 raw route differential, E2 exact resource/helper behavior, complete live fleet-turn comparison, and Step-6 promotion are complete. | Fresh host/native 86-check matrix maps packed design/hull/engine state into `EstFuelUse`, invokes `StargateDamage`, consumes the generated order through `MoveFleets`, and agrees with an independent oracle. |
| `DrawMineSurvey` | — | Closed and promoted: E1 detailed GDI layout, E2 packed records, E3 localization/complete mine-survey caller integration, and original selection evidence are synchronized. | Host/native `DRAW_MINE_SURVEY_LAYOUT_CHECKS=13 FAILURES=0 RESULT=PASS`; final ledger promotion recorded. |
| `MineClick` | — | Closed and promoted: E1 packed payload adapter, E2 localized/derived popup resolver, E3 complete minefield caller integration, and original selection evidence are synchronized. | Host/native `MINE_CLICK_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`; final ledger promotion recorded. |
| `EstMineralsMined` | S | Closed: populated ordinary and special-race mining turns, depletion, concentration, percentage, and fleet consumption were compared. | Closed in `C:\EMMINT4`; promotion is recorded, so no Step-5 work remains. |
| `ProductionDlg` | — | Closed and promoted: E1 packed production globals, E2 direct resource-ID/order and exact geometry/control comparison, and E3 original queue-menu/editor transition, cancel return, sparse-slot mapping, accepted/canceled mutation, and USER dispatch. | Original-executable evidence opens `Customize Production Templates` from the template menu and returns on cancel; native queue/USER packets and the final review-ledger synchronization pass. |
| `DrawProductionDlg` | — | Closed and promoted: E1 packed globals, E2 formatting/resource-table and localized-string comparison, E3 full GDI/caller integration, and final ledger synchronization. | Native draw matrix with exact text/color/geometry assertions, original display evidence, and the VC++ 1.52c/Win98 E3 report. |
| `EstimateItemProdSched` | L | Closed: E1 reconciles the original-instruction-derived turn-by-turn schedule/record differential; E2 production-record/resource mutation and E3 resource/queue integration were already closed. | Fresh host/native 8-check E1 schedule replay plus preserved E2/E3 packets; final review-ledger, coverage, TODO5, and reconstruction-note promotion is synchronized. |
| `ZipProdDlg` | — | Closed: E1 recovered the 40-byte five-record queue/name layout; E2 covered the real USER control/resource loop; E3 covered the real listbox loop and original Add/Remove/Item Up/Item Down/Clear/resource-total sequence. | Host/native `ZIP_PROD_DLG_CHECKS=36`, native dialog `ZIP_PROD_DLG_USER_CHECKS=14`, native listbox `FILL_ZIP_PROD_LB_USER_CHECKS=9`, and the original Win98 mutation screenshots all pass with no Program Error dialog; Step-6 promotion is recorded. |
| `ResearchDlg` | S | Closed: the USER message-loop evidence and final review-ledger synchronization are complete. | Closed in the current review ledger; no Step-5 work remains. |
| `BrowserDlg` | — | Closed: the real USER/control/message-loop unit and direct original caller-path differential agree, and the review/coverage ledgers are synchronized. | Fresh `C:\BRDMSG` passes 9 real USER-loop checks; fresh original run covers Commands → Ship Design → Components, selection, group changes, detail updates, and no-error completion. |
| `UpdatePlayerScores` | S | Closed: the populated-score matrix and complete turn/report replay now agree with an independent oracle at the packed report boundary. | Host/native 36-check matrix covers six populated/dead/tied/single-player cases, packed report checksum, message count, dirty invalidation, and no native Program Error; Step-6 promotion is recorded. |
| `SatisfyOrders` | L | E1 corrected isolated task-5 accounting closed in host/native 19/19. Prior technology, fleet-copy, transfer, and raw-message probes plus native resource-branch 62/62 support E2; original deep-space salvage and complete live service chain remain. E3 same-input differential and promotion remain. | Complete task-5 services, reconcile the one-boranium turn gap, then replay the retained action-bearing HST input through production and compare full state/messages. |
| `FAttack` | L | E1 now has native original-table evidence for group-0x20 torpedo `dp`/`init`/`dRangeMax` and group-0x10 beam `dp`/`init`/`dRangeMax`, plus a fresh packed-token-to-callback context adapter, but complete packed weapon/hull/target wiring and raw beam/torpedo service acceptance (including hull/resource/shield/jam/capacity) remain; E2 broader original damage differentials; E3 native full-battle replay and final differential. E29/E30/E34/E35/E36/E37/E38 are supporting evidence only. | Close E1 only after complete packed beam/torpedo fields, object-kind mapping, and service behavior agree; then establish broader original damage differential (E2), full battle replay/differential (E3), and promote. |
| `FDoCoolBattle` | L | E1 close exact `_tok`/`vrgtok` binding, global compaction/production callers, and full native field map; E2 close movement geometry/speed/initiative and damage/shield/casualty/salvage/bombing records; E3 close fresh native/original full-battle replay and final differential. The 29-byte production `packed_tokens` bridge now has a fresh native attack-callback/FAttack context replay (E38), but global/production binding and the complete battle remain open. | Native battle replay with token/global snapshots, logs, original movement/damage results, and no-error evidence, separated into E1–E3, including full production-token and attack-service behavior. |
| `DoRobotoidAiTurn` | XL | E1 fleet-order repair closed: original first-order writes and fresh native three-order preservation agree. E2 colonizer/transport routing, E3 broader raw PLORD mutation/AI-table branches, and E4 complete original-turn differential remain. | Deterministic native AI-turn replay with packed before/after orders, resources, callback order, and original turn diff for E2–E4. |
| `DoTurinDroneAiTurn` | XL | E1 repair, design loops, seed/base/finalization, technology gate, minelayer, support-fleet queue, packed planet/fleet/queue decoding, and default `FPotentRobWarFleet` semantics have original-instruction and fresh host/native support; live table enumeration, growth/resource services, and actual queue/FinishProduction side effects remain. E2 routing, E3 broader raw PLORD mutation/AI-table branches, E4 original-turn differential remain. | Native AI-turn matrix over remaining production/routing states and original order/resource diff for E1–E4. |
| `DoAutomitronAiTurn` | XL | E2 packed first-order repair is closed by original-instruction audit and fresh host/native three-order replay. E1 colonizer/mineral handling; E3 route and broader packed-order integration; E4 original behavior coverage and final turn differential remain. | Native deterministic AI replay with fleet, order, and resource snapshots against the original for E1, E3, and E4. |
| `DoRototillAiTurn` | XL | E1 packed queue/census/AI markers/lifecycle now have host source support, but live table/service bindings and native/original queue comparison remain; E2 routing; E3 raw PLORD mutation and AI-table branches; E4 complete original-turn comparison. | Native AI-turn replay covering queue and route alternatives with packed mutation diff for E1–E4. |
| `DoMacintiAiTurn` | XL | E1 planet-table selection; E2 retarget/colonizer/transport predicates; E3 direct order mutation; E4 original behavior branches and final turn differential. | Native replay over target/colonizer/transport predicates with original PLORD and resource diff for E1–E4. |
| `DoCyberAiTurn` | XL | E1 packet/freighter behavior is closed by the combined E5 gate/progress, E7 accepted-composition, and E8 five-case edge matrix; E2 detailed order repair and caller order writes; E3 direct packed PLORD mutation beyond the tested first order; E4 complete AI integration and original-turn differential. | Native replay over the remaining repair/order, packed-order, AI-table, and original-turn cases, preserving before/after order, resource, callback, and original comparison evidence. |
| `ReportDlg` | — | Promoted: E1/E2/E3 packed, populated-row, live USER/GDI/menu, and native/original caller boundaries are closed and synchronized. | Native report interaction with scrollbar/menu/row state, populated report serialization, and original report selection behavior are recorded across all E-units. |
| `ExecuteReportClick` | L | E1 owner/part union closure is recorded with eight original Win98 captures and fresh native `C:\GPDI7` 49/49. E2 live USER/GDI report selection, geometry, message and refresh path; E3 all report-layout caller branches and original results remain. | Native/integrated click matrix over planet/starbase/defense/resource-exempt/modal cases plus original USER/GDI outcomes and the existing `C:\EXINT6` lookup/status packet. |
| `DumpPlanets` | — | Promoted: E1 localized normal serialization, E2 complete original normal export, E3 packed-service resolution, E4 original Win98 report-menu/file-write behavior, and Step-6 synchronization are closed. | Host/native serialization differentials, original Win98 export observation, packed-service replay, and synchronized ledgers. |
| `DumpFleets` | — | Promoted: E1 localized scanner/task serialization, E2 serialized field order, E3 packed service integration, E4 complete original output comparison, and Step-6 ledger synchronization are closed. | Native localized fleet report, six-row `GAME.fle` stream differential, production-source replay, and final ledger synchronization are recorded. |
| `DrawPopup` | — | Promoted: E1 complete packed `GlobalPD` payloads, E2 localization/hull filtering/formatting, E3 detailed GDI branches/caller integration, E4 original consumer completion, and final Step-6 synchronization are closed. | Fresh `C:\DPE6` 38-check payload matrix, `C:\DPE7` 21-check content matrix, final `C:\DPE9` 25-check geometry/style matrix, native `C:\POPINTP` 26-check caller replay, and original Win98 consumer captures are recorded; no pixel-identity claim is made. |
| `Popup` | S | Closed: all measured/fixed kinds now consume the packed content boundary and agree with an independent geometry oracle. | Host/native 148-check matrix covers localized labels, popup/fleet/planet/hull payloads, dimensions, clamping, slot updates, message/capture setup, and no native Program Error; Step-6 promotion is recorded. |
| `PopupMenu` | S | Closed: real owner/message-queue dispatch, exact `PeekMessage` arguments, and packed fleet/thing/planet/deep caller lookup agree in the native E2 boundary. | Host/native 30-check E2 matrix passed in fresh `C:\PMUINT3` with `COMMAND=15000`, `MASK=0`, real mouse selection, and no Program Error dialog; Step-6 promotion is recorded. |
| `PtDisplayPlanetStateInfo` | — | Closed and promoted: E1 packed lookup, E2 localized fixed-point formatting, E3 complete stream/wrap GDI/original comparison, and Step-6 synchronization are complete. | Native `C:\PBR3` 9-check shared branch replay plus the original Small Screen capture; review, coverage, and TODO5 ledgers are synchronized. |
| `PtDisplayPlanetPopInfo` | — | Closed and promoted: E1 packed lookup, E2 localized fixed-point formatting, E3 complete stream/wrap GDI/original comparison, and Step-6 synchronization are complete. | Native `C:\PBR3` 9-check shared branch replay plus the original Small Screen capture; review, coverage, and TODO5 ledgers are synchronized. |

### Planning view: what “forever” would mean

The evidence-unit count is now usable as a throughput measure, but it is not a
calendar estimate. The following scenarios are arithmetic planning aids only;
they assume a sustained rate of closing named Step-5 units and do not predict
new discoveries, shared-harness setup, interruptions, or the time required for
the final ledger promotions.

| Sustained closure rate | 32 Step-5 units | 42 total open items |
|---:|---:|---:|
| 2 units per week | about 16 weeks | about 21 weeks |
| 4 units per week | about 8 weeks | about 11 weeks |
| 8 units per week | about 4 weeks | about 6 weeks |

This gives us an early warning signal: if the rolling rate stays near one or
two evidence units per week, the project can indeed feel open-ended; if shared
packed-record, USER/GDI, report, turn-replay, or AI harnesses raise the rate to
four or more, the remaining queue has a visible finite path. The next status
updates should therefore report both the current `E done / E planned` row
values and the rolling Step-5 closure rate, while keeping the calendar rows
explicitly labeled as scenarios rather than promises.

### Step-5 reporting rule for every procedure

The progress narrative below preserves earlier checkpoint counts so the audit
trail remains reproducible. Use the current-state counters, scorecard, and
burn-down tables above when deciding what remains today.

Each row above is now reported as a closure packet, not as one vague “more
testing” item. Every progress update should name:

1. the specific `E` unit being closed or narrowed;
2. the host checks and the fresh Win98 report that support it;
3. the packed/UI/caller boundary that was actually exercised;
4. the original-executable or original-instruction comparison used; and
5. the remaining `E` units and the separate `P` promotion item.

For example, `UpdatePlayerScores` was an `M` row with `E1` populated-score
differential and `E2` complete turn/report replay. Its closure packet records
six scenarios, 36 host/native checks, packed report decoding, message count,
dirty invalidation, the native build artifacts, and the no-error-dialog
observation. It therefore moves from `2 + P` to `0`, then to `6/6`; a single
new native check would not have done that unless it closed one of those named
boundaries. The same packet format now applies to every remaining row, so a
passing probe can be distinguished from a genuine reduction in work left.

This expanded view makes the current risk distribution explicit. The remaining
work is not 33 interchangeable “one more test” tasks:

- The former **S** work in `FStargateJump` E2, the complete
  `LFuelUseToWaypoint` row, `AutoRouteFleet` E2, and `GenerateWorld` E2 are
  closed. `BrowserDlg`'s real USER/control/message-loop unit and direct
  original caller-path comparison are closed; its Step-6 promotion is now
  recorded.
- No S or M row remains. The L rows require a short chain of dependent
  comparisons, while the XL rows are
  broad UI, turn, AI, or full-record integrations.
- The **L** rows require a short chain of dependent comparisons and are the
  largest number of procedures.
- The **XL** rows are genuinely broad: they require a shared report,
  turn-replay, AI, or full-record harness and can consume substantial effort
  even though their count is only eight procedures.

The honest project completion measure is therefore **54 open work items**:
42 behavior/integration units plus 12 promotion units, with 12 procedures
still below `6/6`. If work proceeds one isolated procedure at a time, the XL
rows can make the pace feel nearly flat. Shared packed-record, USER/GDI,
report, turn-replay, and AI fixtures are the main way to reduce that apparent
“forever” risk: one piece of infrastructure can close the same unit type in
several rows, but each row still needs its own acceptance result before it can
be promoted.

## Phase 0 — repair the progress ledger

- [x] Reconcile `analysis/coverage.json` with
      `analysis/debug/procedures.json` and the generated TODO5 rows.
- [x] Resolve the overlapping `NewGameDlg` / `NewGameDlg2` coverage ranges and
      rerun the coverage invariants.
- [x] Reconcile the handoff count of 52 partial / 7 open with the current
      ledger count of 51 partial / 9 not-started ranges.
- [x] Regenerate [TODO5.md](TODO5.md) from the repaired authoritative records.
- [x] Confirm that `jq empty tracking/reviews.json`, the renderer, and
      `git diff --check` all pass.

## Phase 1 — procedures promoted from the stale open set

These nine procedures already had complete reconstructed sources, host probes,
and native VC1.52c evidence. Correcting their stale review keys promoted them
without inventing new source or weakening the acceptance criteria.

### Scanner and measurement

- [x] `DrawScanFleetCount` — recover drawing, fleet-count lookup, localization,
      and repaint behavior.
- [x] `ChangeScanSel` — recover selection changes, hit testing, scroll/state
      mutation, and message ordering.
- [x] `FHandleMeasuringTape` — recover measuring-tape input, coordinate
      conversion, drawing, cancellation, and focus behavior.

### Ship/order, turn, and battle support

- [x] `RenameZipDlg` — recover rename-dialog state, validation, resource text,
      and packed ship/queue record mutation.
- [x] `PlanetaryClimateChange` — recover exact climate arithmetic, bounds,
      fixed-point behavior, and planet-record mutation.
- [x] `DxyMoveTokTo` — recover battle-token movement geometry, obstruction and
      boundary handling, and movement-state updates.

### Reports and popups

- [x] `PtDisplayFactoryMineInfo` — recover packed planet display fields,
      localized formatting, and drawing geometry.
- [x] `PtDisplayResourceInfo` — recover resource display lookup, formatting,
      coordinate calculation, and GDI behavior.
- [x] `DumpUniverse` — recover report-file traversal, formatting, stream/error
      behavior, and caller-owned state handling.

## Phase 2 — partial source clusters

The current Phase 2 matrix contains 10 procedures below 6/6. Finish them as
clusters because the remaining gaps share packed records, callbacks,
resources, and native harnesses.

### Creation and new-game flow

- [x] `GenerateWorld` — **6/6**
- [x] `NewGameWizard` — **6/6**
- [x] `SimpleNewGameDlg` — **6/6**
- [x] `NewGameDlg` — **6/6**
- [x] `NewGameDlg2` — **6/6**
- [x] `DrawNewGame2` — **6/6**
- [x] `DrawNewGame3` — **6/6**
- [x] `IDropPart` — **6/6**
- [x] `FillBuildDD` — **6/6**

Close packed player/game records, race and relation fields, resource and menu
behavior, exact control geometry, ship-builder capacity rules, file naming,
and native creation-flow validation. `GenerateWorld` is the largest current
partial body and should be treated as a major milestone rather than a small
follow-up.

### Planet, fleet, production, orders, and research

- [x] `PlanetWndProc` — **6/6**
- [x] `DrawPlanetStats` — **6/6**
- [x] `DrawPlanetStarbase` — **6/6**
- [x] `DrawPlanetProduction` — **6/6**
- [x] `DrawPlanShipBitmap` — **6/6**
- [x] `DrawPlanetShipList` — **6/6**
- [x] `TransferStuff` — **6/6**
- [x] `FTrackXfer` — **6/6**
- [x] `EstFuelUse` — **6/6**
- [x] `LFuelUseToWaypoint` — **6/6**
- [x] `ZipOrderDlg` — **6/6**
- [x] `FStargateJump` — **6/6**
- [x] `KillUsedWaypoints` — **6/6**
- [x] `AutoRouteFleet` — **6/6**
- [x] `DrawMineSurvey` — **6/6**
- [x] `MineClick` — **6/6**
- [x] `EstMineralsMined` — **6/6**
- [x] `ProductionDlg` — **6/6**
- [x] `DrawProductionDlg` — **6/6**
- [x] `EstimateItemProdSched` — **6/6**
- [x] `ZipProdDlg` — **6/6**
- [x] `ResearchDlg` — **6/6**
- [x] `BrowserDlg` — **6/6**
- [x] `UpdatePlayerScores` — **6/6**
- [x] `SatisfyOrders` — **6/6**

Close packed planet/fleet/thing/cargo records, fixed-point and resource
arithmetic, route and waypoint mutation, production/research queue semantics,
score and message serialization, dialog controls, localization, and native
Win16 state transitions. `SatisfyOrders` is a major dependency for order,
movement, and turn-generation acceptance.

### Combat

- [ ] `FAttack` — **5/6**
- [ ] `FDoCoolBattle` — **5/6**

Recover the complete battle state machine: token and battle-record integration,
initiative and tie behavior, movement, target selection, damage and shields,
casualties, salvage, bombing, exact RNG consumption, and playback-visible
records. Validate resolved state, not only return values.

### AI personalities

- [ ] `DoRobotoidAiTurn` — **5/6**
- [ ] `DoTurinDroneAiTurn` — **5/6**
- [ ] `DoAutomitronAiTurn` — **5/6**
- [ ] `DoRototillAiTurn` — **5/6**
- [ ] `DoMacintiAiTurn` — **5/6**
- [ ] `DoCyberAiTurn` — **5/6**

Use the completed Macinti boundary work as the shared reconstruction pattern,
then recover each personality's exact production policy, fleet-order repair,
retargeting, colonizer and transport routing, fallback choices, and AI-table
integration. The current Macinti slice already has host and native evidence
for production gates, named fleet actions, final phases, and raw record views;
the remaining work is replacing explicit plan callbacks with recovered table
decisions and attaching the exact routing mutations.

AI acceptance must compare complete fleet/planet/player state, task and order
changes, callback order, and controlled RNG draws. A passing orchestration
probe alone is not sufficient for promotion.

### Reports, popups, and display integration

- [x] `ReportDlg` — **6/6**
- [ ] `ExecuteReportClick` — **5/6**
- [x] `DumpPlanets` — **6/6**
- [x] `DumpFleets` — **6/6**
- [x] `DrawPopup` — **6/6**
- [x] `Popup` — **6/6**
- [x] `PopupMenu` — **6/6**
- [x] `PtDisplayPlanetStateInfo` — **6/6**
- [x] `PtDisplayPlanetPopInfo` — **6/6**

Close packed `GlobalPD`, planet, fleet, and thing records; localized strings;
fixed-point formatting; scanner/minefield/battle-plan/task-name callbacks;
Win16 menu and dialog behavior; GDI drawing; hit testing; and file/report
serialization. Preserve screenshots or equivalent UI evidence where layout or
painting is part of the acceptance boundary.

## Phase 3 — shared infrastructure and dependency order

- [ ] Finish the shared packed-record and field-offset adapters needed by
      creation, simulation, AI, combat, and reports.
- [ ] Finish resource/string/menu lookup adapters before promoting UI-heavy
      procedures.
- [ ] Finish route, waypoint, fleet-task, and production queue helpers before
      promoting the AI personalities or `SatisfyOrders`.
- [ ] Keep host probes deterministic and isolate original-executable
      comparisons from generated output and native run directories.
- [ ] Reuse the running `StarDev` agent for fresh packages; preserve failed
      logs and reports as evidence rather than overwriting earlier runs.

Recommended dependency order:

1. Repair the ledger and shared layout declarations.
2. Close the small not-started scanner, popup, rename, climate, and report
   procedures that unblock callers.
3. Finish creation/new-game packed-state integration.
4. Finish planet/fleet/order/production infrastructure.
5. Complete the six AI personalities using the shared infrastructure.
6. Complete combat and battle-record integration.
7. Finish reports/popups and run integrated UI/state validation.

## Progress log

- [x] Added the `ReportDlg` packed-view load boundary. The readable source
      now accepts an optional `load_view` callback and `packed_view` input for
      the recovered 54-byte `_rpt` record. The strict host C89 probe passes
      `REPORT_DLG_VIEW_CHECKS=13 FAILURES=0 RESULT=PASS`; the existing 26-case
      source and 24-case packed integration probes remain green. A fresh native
      packet is prepared for `C:\RPTVE1`, but the guest agent has not announced,
      so this narrows the ReportDlg E1 work without reducing the authoritative
      **77 evidence units + 22 promotion units = 99 open work items**. Evidence
      is in `analysis/compat/report-dlg-view-e1.{md,json}` and
      `compat/vc152/report-dlg-view-e1/`.

- [x] Added the typed `DrawPlanetStats` value-formatting boundary. The
      readable source retains the old callback path and now optionally routes
      seven row values through paired formatter/drawing callbacks, making
      localized and fixed-point text explicit. The strict host C89 probe passes
      `DRAW_PLANET_STATS_FORMAT_CHECKS=16 FAILURES=0 RESULT=PASS`, including
      zero mineral rate and unavailable scanner/defense cases; the existing
      22-check source and 15-check packed probes also pass. A fresh native
      packet is prepared for `C:\DPSFE2`, but the guest agent is not connected,
      so this is host-complete/native-pending and does not reduce the
      authoritative **77 evidence units + 22 promotion units = 99 open work
      items**. Evidence is in
      `analysis/compat/draw-planet-stats-format-e2.{md,json}` and
      `compat/vc152/draw-planet-stats-format-e2/`.

- [x] Added the typed `DrawPlanetStats` packed lookup boundary. The optional
      `load_data` callback now populates the source's typed planet-stat record
      from the tile/object pointer before drawing, while a rejected load skips
      the tile. The strict host C89 probe passes
      `DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS` over the
      recovered 56-byte planet identity, scanner/defense word, mineral-rate
      word, and all seven row consumers. The fresh native packet is prepared
      for `C:\DPSLE1`, but the guest agent did not announce, so the
      authoritative burn-down remains **77 evidence units + 22 promotion
      units = 99 open work items**. Evidence is in
      `analysis/compat/draw-planet-stats-lookup-e1.{md,json}` and
      `compat/vc152/draw-planet-stats-lookup-e1/`.

- [x] Added the typed `PtDisplayPlanetPopInfo` segment-preparation boundary.
      The optional callback now receives every localized/value stream or wrap
      segment after branch selection, and the source probe verifies all
      emitted segments plus the negative-desirability fixed-point pair `2,5`.
      The strict host probe passes
      `PT_DISPLAY_PLANET_POP_INFO_FORMAT_CHECKS=27 FAILURES=0 RESULT=PASS`; the
      packed 19-check probe also passes. A fresh native packet is prepared for
      `C:\PPFE2`, but the guest agent did not announce, so the authoritative
      burn-down remains **77 evidence units + 22 promotion units = 99 open
      work items**. Evidence is in
      `analysis/compat/pt-display-planet-pop-info-format-e2.{md,json}` and
      `compat/vc152/pt-display-planet-pop-info-format-e2/`.

- [x] Added the typed `PtDisplayPlanetStateInfo` segment-preparation
      boundary. The optional callback now receives every selected stream/wrap
      segment, and the source probe verifies the known-limit packet plus the
      environment kind-2 fixed-point handoff. The strict host probe passes
      `PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS`;
      the packed 23-check probe also passes. A fresh native packet is prepared
      for `C:\STFE2`, but the guest agent did not announce, so the
      authoritative burn-down remains **77 evidence units + 22 promotion
      units = 99 open work items**. Evidence is in
      `analysis/compat/pt-display-planet-state-info-format-e2.{md,json}` and
      `compat/vc152/pt-display-planet-state-info-format-e2/`.

- [x] Completed the `ProductionDlg` unsigned-capacity sub-boundary in fresh
      `C:\PDGQ5`. The queue path now retains the packed capacity as unsigned
      until the recovered four-entry cap is applied; an oversized `0xffff`
      value is safely clamped and the sparse valid slots remain intact. The
      strict host and native VC++ 1.52c probes each pass 10 checks with zero
      failures; compile, link, process exit, empty redirected output, and the
      no-Program-Error window check pass. This narrows the queue-control unit
      but does not change the headline counts: ProductionDlg remains **5/6**
      with original queue differential behavior, packed production globals,
      complete resources, and promotion still open. Evidence is in
      `analysis/compat/production-dlg-queue-e5.{md,json}` and
      `runs/vmctl-vc152pdgq5/`.

- [x] Completed the `ProductionDlg` native USER/message-loop sub-boundary in
      fresh `C:\PDGU5`. An actual Win98 window and child controls route queued
      initialization, command, left-click, two right-click, paint, and erase
      messages through the reconstructed dispatcher. The native report passes
      `PRODUCTION_DLG_USER_CHECKS=12 FAILURES=0 RESULT=PASS`, including sparse
      queue selection, accepted editor mutation, two queue menus, and 74 USER
      dispatches; compile, link, process exit, empty redirected output, and no
      Program Error dialog pass. This closes only native USER dispatch:
      ProductionDlg remains **0/3** with packed globals, complete resources,
      original queue differential, and final promotion remaining. Evidence is
      recorded in `analysis/compat/production-dlg-user-e5.{md,json}` and
      `runs/vmctl-vc152pdgu5/`. The authoritative burn-down remains
      **78 evidence units + 22 promotion units = 100 open work items**, with
      **29 procedures at 6/6 and 22 at 5/6**.

- [x] Expanded the `ProductionDlg` queue-editor boundary in fresh `C:\PDGQ4`.
      The accepted-editor outcome now preserves queue mutation, while the
      canceled outcome restores the pre-dialog table; sparse valid-slot menu
      mapping and the localized editor entry remain covered in the same packet.
      Strict host C89 and VC++ 1.52c/Win98 each pass
      `PRODUCTION_DLG_QUEUE_CHECKS=9 FAILURES=0 RESULT=PASS`; compile, link,
      process exit, empty redirected output, and no Program Error dialog pass.
      This closes only the accepted/canceled mutation sub-boundary; live
      USER/message-loop behavior, original queue differential, packed globals,
      complete resources, and final promotion remain. Evidence is recorded in
      `analysis/compat/production-dlg-queue-e4.{md,json}` and
      `runs/vmctl-vc152pdgq4/`. The authoritative burn-down remains
      **78 evidence units + 22 promotion units = 100 open work items**, with
      **29 procedures at 6/6 and 22 at 5/6**.

- [x] Completed the `ProductionDlg` saved-queue menu/cancel-rollback
      sub-boundary inside E3 in fresh `C:\PDGQ3`. The typed source filters
      valid entries from the four queue slots, preserves sparse original slot
      indices when a menu row is selected, passes the localized editor entry
      after the queue names, and restores the queue table when the editor
      reports cancel. Strict host C89 and VC++ 1.52c/Win98 each pass
      `PRODUCTION_DLG_QUEUE_CHECKS=7 FAILURES=0 RESULT=PASS`; compile, link,
      process exit, empty redirected output, and no Program Error dialog pass.
      This closes only that E3 sub-boundary: ProductionDlg remains **0/3**
      named Step-5 evidence units complete, with packed production globals,
      complete resource comparison, accepted editor/live USER integration,
      original queue differential, and final promotion remaining. Evidence is
      recorded in `analysis/compat/production-dlg-queue-e3.{md,json}` and
      `runs/vmctl-vc152pdgq3/`. The authoritative burn-down is unchanged at
      **78 evidence units + 22 promotion units = 100 open work items**, with
      **29 procedures at 6/6 and 22 at 5/6**.

- [x] Completed the `ProductionDlg` exact resource-ID/callback-order
      sub-boundary inside E2 in fresh `C:\PDGRS2`. The readable source now
      preserves the original `0x00d3` copied-popup-string and `0x04c0`
      compressed-string lookup boundaries, their callback ordering, and
      suppression outside the corresponding diamond controls. Strict host C89
      and VC++ 1.52c/Win98 each pass
      `PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link,
      process exit, empty redirected output, and no Program Error dialog pass.
      This closes only the lookup-ID/order sub-boundary, not the full localized
      resource-table/string comparison: ProductionDlg remains **0/3** named
      Step-5 evidence units complete, with packed production globals, complete
      resource comparison, queue-control integration, and final promotion
      remaining. Evidence is recorded in
      `analysis/compat/production-dlg-resource-e2.{md,json}` and
      `runs/vmctl-vc152pdgrs2/`. The authoritative burn-down is unchanged at
      **78 evidence units + 22 promotion units = 100 open work items**, with
      **29 procedures at 6/6 and 22 at 5/6**.

- [x] Completed the `ProductionDlg` exact-child-geometry sub-boundary inside
      E2 in fresh `C:\PDGE2`.
      The typed WM_INITDIALOG trace covers full-width and compact-width main
      dialog sizing, research and lower control positions, five centered
      list/control windows, navigation enablement, and initialization/sticky
      callback ordering. Strict host C89 and VC++ 1.52c/Win98 each pass
      `PRODUCTION_DLG_LAYOUT_CHECKS=23 FAILURES=0 RESULT=PASS`; compile, link,
      process exit, empty redirected output, and no Program Error dialog pass.
      This does not close the full E2 geometry/resources unit: the localized
      resource comparison remains open. ProductionDlg remains **0/3** named
      Step-5 evidence units complete; packed production globals, localized
      popup resources, queue-control integration, and final promotion remain.
      Evidence is recorded in
      `analysis/compat/production-dlg-layout-e2.{md,json}` and
      `runs/vmctl-vc152pdge2/`. The authoritative burn-down is unchanged at
      **78 evidence units + 22 promotion units = 100 open work items**, with
      **29 procedures at 6/6 and 22 at 5/6**.

- [x] Closed and promoted `DrawMineSurvey` E1 detailed GDI layout in fresh
      `C:\DMSIE1`. The readable source emits typed outer-panel PATBLT records
      for the recovered 13-command fleet frame, 13-command ordinary-thing
      frame, and 7-command compact-thing frame, preserving exact offsets,
      dimensions, raster operations, and shadow/highlight brush roles. Strict
      host C89 and VC++ 1.52c/Win98 each pass
      `DRAW_MINE_SURVEY_LAYOUT_CHECKS=13 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and no Program Error dialog pass. Together with E2
      and E3, all DrawMineSurvey Step-5 evidence is closed and Step 6 is
      promoted. Evidence is recorded in
      `analysis/compat/draw-mine-survey-layout-e1.{md,json}` and
      `runs/vmctl-vc152dmslay/`. The authoritative burn-down is now **78
      evidence units + 22 promotion units = 100 open work items**, with
      **29 procedures at 6/6 and 22 at 5/6**.

- [x] Closed `DrawMineSurvey` E3 localized/caller integration in fresh
      `C:\DMSIE3`. The localized `MineWndProc` paint path now invokes the real
      typed `stars_draw_mine_survey` implementation; the packet checks the
      `0x04bd` title resource, adjusted survey rectangle, planet lookup and
      detail summary, state restoration, and paint ordering. Strict host C89
      and VC++ 1.52c/Win98 each pass
      `DRAW_MINE_SURVEY_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and no Program Error dialog pass.
      DrawMineSurvey is now **2/3** evidence; E1 detailed GDI layout and
      final promotion remain. Evidence is recorded in
      `analysis/compat/draw-mine-survey-integration-e3.{md,json}` and
      `runs/vmctl-vc152dmsint3/`. The authoritative burn-down is now **79
      evidence units + 23 promotion units = 102 open work items**, with
      **28 procedures at 6/6 and 23 at 5/6**.

- [x] Closed and promoted `MineClick` E3 in fresh `C:\MCLIE3`. The
      typed dispatcher is wired to the real `PopupMineralScanChoices`
      selector and the integration packet checks planet/fleet/thing markers,
      selected-fleet decoding, friendly-fleet redraw and main-selection
      ordering, fleet-cargo dispatch, double-click suppression, and all
      remaining popup hit families. Strict host C89 and VC++ 1.52c/Win98 each
      pass `MINE_CLICK_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and no Program Error dialog pass. All three
      MineClick Step-5 evidence units are closed and Step 6 is promoted.
      Evidence is recorded in
      `analysis/compat/mine-click-integration-e3.{md,json}` and
      `runs/vmctl-vc152mclint3/`. The authoritative burn-down is now **80
      evidence units + 23 promotion units = 103 open work items**, with
      **28 procedures at 6/6 and 23 at 5/6**.

- [x] Closed `MineClick` E2 localized/derived popup resolution in fresh
      `C:\MCLE3`. The typed source now maps recovered hit families to popup
      kinds, resolves localized resource identity, and passes selected planet
      variables, player/terraform bounds, and three derived mineral outputs
      through explicit resolver callbacks before popup construction. Strict
      host C89 and VC++ 1.52c/Win98 each pass
      `MINE_CLICK_CHECKS=36 FAILURES=0 RESULT=PASS`; compile, link, process
      exit, and no Program Error dialog pass. MineClick is now **2/3** named
      Step-5 evidence units complete; E3 complete minefield integration and
      final promotion remain. Evidence is recorded in
      `analysis/compat/mine-click-e2.{md,json}` and
      `runs/vmctl-vc152mcle3/`. The authoritative burn-down is now **81
      evidence units + 24 promotion units = 105 open work items**, with
      **27 procedures at 6/6 and 24 at 5/6**.

- [x] Closed `MineClick` E1 packed GlobalPD-facing payload integration in
      fresh `C:\MCLE2`. The typed source now decodes the recovered 56-byte
      `_planet`, 124-byte `_fleet`, and 18-byte `_thing` records into the
      popup callback. Strict host C89 and VC++ 1.52c/Win98 each pass
      `MINE_CLICK_CHECKS=30 FAILURES=0 RESULT=PASS`; compile, link, process
      exit, and no Program Error dialog pass. MineClick is now **1/3** named
      Step-5 evidence units complete; E2 localized popup/derived arithmetic
      and E3 complete minefield integration remain. Evidence is recorded in
      `analysis/compat/mine-click-packed-e1.{md,json}` and
      `runs/vmctl-vc152mcle2/`. The authoritative burn-down is now **82
      evidence units + 24 promotion units = 106 open work items**, with
      **27 procedures at 6/6 and 24 at 5/6**.

- [x] Closed `DrawMineSurvey` E2 packed-record integration in fresh
      `C:\DMSE2`. The typed source now decodes the recovered 124-byte
      `_fleet`, 56-byte `_planet`, and 18-byte `_thing` records before the
      panel callbacks. Strict host C89 and VC++ 1.52c/Win98 each pass the
      expanded raw-record matrix; the native report is
      `DRAW_MINE_SURVEY_CHECKS=12 FAILURES=0 RESULT=PASS`, with compile, link,
      process exit, and no Program Error dialog. DrawMineSurvey is now **1/3**
      named Step-5 evidence units complete; E1 detailed GDI layout and E3
      localization/complete caller integration remain. Evidence is recorded
      in `analysis/compat/draw-mine-survey-packed-e2.{md,json}` and
      `runs/vmctl-vc152dmse2/`. The authoritative burn-down is now **83
      evidence units + 24 promotion units = 107 open work items**, with
      **27 procedures at 6/6 and 24 at 5/6**.

- [x] Closed and promoted `TransferStuff` E3 in fresh `C:\TRCNE3`. The strict
      host and VC++ 1.52c/Win98 probe each report
      `TRANSFER_STUFF_E3_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link,
      process exit, and post-run window inventory passed with no Program Error
      dialog. The packet closes the corrected `grobj=4` split/jettison
      sentinel, initial packed lookup, post-dialog fleet/planet/thing update
      lookup, the real Win16 task-message transfer-dialog sequence, lookup
      rejection before dialog creation, and refresh/invalidation ordering.
      The source, review ledger, coverage, TODO5, TODO6, and handoff are now
      synchronized, so `TransferStuff` is **6/6** and promoted. Evidence is
      recorded in `analysis/compat/transfer-stuff-e3.{md,json}` and
      `runs/vmctl-vc152trcne3/`. The authoritative burn-down is now **84
      evidence units + 24 promotion units = 108 open work items**, with
      **27 procedures at 6/6 and 24 at 5/6**.

- [x] Closed and promoted `FTrackXfer` E3 in fresh `C:\FTXE3G`. The strict
      host and VC++ 1.52c/Win98 probe each report
      `F_TRACK_XFER_E3_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link, process
      exit, and post-run window inventory passed with no Program Error dialog.
      This final packet checks packed lookup/store at the raw record boundary,
      the real Win16 task message queue with capture/release, direct
      `mdXferDlg` active/opposite-record mutation separate from `XferSupply`,
      owner rejection, and refresh/redraw callbacks. The source, review ledger,
      TODO5, coverage, and handoff are synchronized, so `FTrackXfer` is now
      **6/6** and promoted. Evidence is recorded in
      `analysis/compat/f-track-xfer-e3.{md,json}` and
      `runs/vmctl-vc152ftxe3/`. The authoritative burn-down is now **85
      evidence units + 25 promotion units = 110 open work items**, with
      **26 procedures at 6/6 and 25 at 5/6**.

- [x] Closed `FTrackXfer` E1/E2 service-boundary evidence in fresh
      `C:\FTXSE2`. The strict host and VC++ 1.52c/Win98 adapters each report
      `F_TRACK_XFER_SERVICE_E12_CHECKS=12 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. The packet connects `FTrackXfer` to the
      reconstructed `XferSupply`/`ChgCargo` services and checks signed drag
      directions, source clamping, destination application, source rollback,
      mineral mutation, button/dialog bounds, conservation, and final
      refresh/redraw callbacks. Together with the earlier 22-check packed
      packet, E1 and E2 are now closed; E3 full fleet/planet/thing lookup,
      complete USER/message-loop integration, original partial/rejected/
      refresh differentials, and P promotion remain. Evidence is recorded in
      `analysis/compat/f-track-xfer-service-e12.{md,json}` and
      `runs/vmctl-vc152ftxse2/`. The authoritative burn-down is now **86
      evidence units + 26 promotion units = 112 open work items**.

- [x] Closed `TransferStuff` E2 exact cargo-mutation integration. The transfer
      orchestration now drives the reconstructed cargo-balance service at the
      raw balance boundary. The strict host and fresh Win98 `C:\TRCNE2` probe
      report `TRANSFER_STUFF_CARGO_E2_CHECKS=12 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. The packet checks prepare/split/hull/
      tutor/dialog/balance/refresh/invalidation order, conservation across
      all five cargo fields, donor-to-receiver fuel movement, packed
      per-design status mutation, and the cancel path with no balance call.
      Evidence is recorded in
      `analysis/compat/transfer-stuff-cargo-e2.{md,json}` and
      `runs/vmctl-vc152trcne2/`. `TransferStuff` is now **1/3** named
      evidence units complete; E3 live Win16 transfer/lookup/refresh and P
      promotion remain. The authoritative burn-down is now **88 evidence
      units + 26 promotion units = 114 open work items**.

- [x] Closed `PlanetWndProc` E3 message/focus/drawing integration and promoted
      it to `6/6`. The typed source now preserves the raw paint/erase,
      minimum-size, owner-draw, measure, cursor, focus, and command/message
      boundaries. The host and fresh Win98 `C:\PWTE3` probes report
      `PLANET_WND_PROC_INTEGRATION_E3_CHECKS=10 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. Together with E1 and E2, the complete
      Step-5 packet and original planet-window integration evidence are
      synchronized in the review ledger, coverage, TODO5, and reconstruction
      notes. PlanetWndProc is now **6/6**. At that checkpoint the burn-down
      was **89 evidence units + 26 promotion units = 115 open work items**.

- [x] Closed `PlanetWndProc` E2 packed-transfer lookup evidence. The raw
      `FLookupOrbitingXfer` boundary is documented through the typed callback:
      fleet records precede thing records, the selected planet is filtered,
      the packed `_xfer` header uses `id`/`grobj` at offsets 0/2, and the
      copied payload begins at offset 4. The host and fresh Win98 `C:\PWTE2`
      probes report `PLANET_WND_PROC_TRANSFER_E2_CHECKS=12 FAILURES=0
      RESULT=PASS`; compile, link, process exit, and the post-run window
      inventory passed with no Program Error dialog. Evidence is recorded in
      `analysis/compat/planet-wnd-proc-transfer-e2.{md,json}` and
      `runs/vmctl-vc152pwte2/`. `PlanetWndProc` is now **2/3** named evidence
      units closed; only E3 message/focus/drawing integration and P promotion
      remain. The authoritative burn-down is now **90 evidence units + 27
      promotion units = 117 open work items**.

- [x] Closed `PlanetWndProc` E1 exact child-layout evidence. The typed source
      now creates the recovered 23-child WM_CREATE sequence with exact styles,
      dimensions, resource IDs, fonts, edit limit/subclass operations, ship
      dropdown height capture, and handle propagation. The host and fresh
      Win98 `C:\PWLE` probes each report
      `PLANET_WND_PROC_LAYOUT_E1_CHECKS=48 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. Evidence is recorded in
      `analysis/compat/planet-wnd-proc-layout-e1.{md,json}` and
      `runs/vmctl-vc152pwle/`. `PlanetWndProc` is now **1/3** named evidence
      units closed; E2 packed transfer lookup, E3 message/focus/drawing
      integration, and P promotion remain. The authoritative burn-down is now
      **91 evidence units + 27 promotion units = 118 open work items**.

- [x] Closed `FillBuildDD` E3 complete population/control integration and
      promoted the procedure to `6/6`. The host and corrected fresh Win98
      `C:\FBDE` probe report `FILL_BUILD_DD_E3_CHECKS=14 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. The matrix covers normal, empty, hidden,
      queued, built, special-part, enemy-design, starbase-limit, selector,
      edit/delete, count-clamp, resize, and callback-order boundaries. The
      review ledger, coverage, TODO5, and reconstruction note are synchronized.
      FillBuildDD is now promoted at `6/6`. The burn-down is now **92 evidence
      units + 27 promotion units = 119 open work items**.

- [x] Closed `FillBuildDD` E2 localized-label/resource evidence. The readable
      source now exercises the recovered normal resource table at `0x52` (13
      IDs) and starbase table at `0x7c` (8 IDs) for both label modes. The host
      and fresh Win98 `C:\FBDL` probe report
      `FILL_BUILD_DD_LABELS_E2_CHECKS=58 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. The exact ordered IDs, reset/add/list-count/end
      sequence, and three nonzero-mode disable calls are recorded in
      `analysis/compat/fill-build-dd-labels-e2.md`. FillBuildDD is now `2/3`
      evidence with only E3 population/queue/resize/caller integration and
      Step-6 promotion open. The burn-down is now **93 evidence units + 28
      promotion units = 121 open work items**.

- [x] Closed `FillBuildDD` E1 packed design-record evidence. The typed source
      now accepts the recovered 147-byte `_shdef` stride and checks packed
      `fFree`, class-name, and `cExist` fields while retaining the typed
      design-array fallback. The host and fresh Win98 `C:\FBDP` probe both
      report `FILL_BUILD_DD_PACKED_E1_CHECKS=8 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. The packet deliberately conflicts the
      typed shadow records and preserves neighboring raw records. FillBuildDD
      At that earlier checkpoint FillBuildDD was at `1/3` evidence with E2
      labels/resources, E3 population/control integration, and Step-6
      promotion open; the burn-down was **94 evidence units + 28 promotion
      units = 122 open work items**.

- [x] Closed `IDropPart` E3 redraw/control integration evidence. The focused
      packet checks the raw empty-side and merge service order, client-rect
      retrieval, `yBuildInfoSum` top replacement, erase invalidation, and
      selected-component/hull arguments. The host and fresh Win98 `C:\IDPR`
      probe both report
      `I_DROP_PART_REDRAW_E3_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link,
      process exit, and the post-run window inventory passed with no Program
      Error dialog. The raw original-instruction boundary and live designer
      limitation are recorded in `analysis/compat/i-drop-part-redraw-e3.md`.
      IDropPart is now promoted at `6/6`: all named evidence and the final
      ledger synchronization are closed. The burn-down is now **95 evidence
      units + 28 promotion units = 123 open work items**, with 28 procedures
      at 5/6 and 23 at 6/6.

- [x] Closed `IDropPart` E2 drag-state mutation evidence. The focused matrix
      checks capacity-limited merges, source exhaustion, no-modify preservation,
      source-index `-1`, zero-capacity fallback, incompatible rejection, and
      the `USIA`/`SIA`/`B` callback sequences. The host and fresh Win98
      `C:\IDPM` probe both report
      `I_DROP_PART_MUTATION_E2_CHECKS=11 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. IDropPart remains at 5/6 with only E3
      redraw/control/original drop behavior and Step-6 promotion open. The
      burn-down is now **96 evidence units + 29 promotion units = 125 open
      work items**, with 29 procedures at 5/6 and 22 at 6/6.

- [x] Closed `IDropPart` E1 packed slot hit-table evidence. The source now
      consumes a packed slot-rectangle table with Win16 `PtInRect` semantics
      (left/top inclusive, right/bottom exclusive), while retaining the
      callback hit tester as a fallback. The focused host and fresh Win98
      `C:\IDPH` probe both report
      `I_DROP_PART_HIT_TABLE_E1_CHECKS=12 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. IDropPart remains at 5/6 with E2 drag-state
      mutation, E3 redraw/control/original drop behavior, and Step-6 promotion
      open. The burn-down is now **97 evidence units + 29 promotion units =
      126 open work items**, with 29 procedures at 5/6 and 22 at 6/6.

- [x] Closed `DrawNewGame3` E4 complete page integration and promoted the
      procedure to 6/6. The final packet checks background mode/color
      save-restore, client-rectangle setup, DC ownership, read-only paired
      button styles, caller-provided HDC handling, and selective draw-mode
      output. The host and fresh Win98 `C:\DNG3I` probe both report
      `DRAW_NEW_GAME3_INTEGRATION_E4_CHECKS=12 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. Combined with the original Step-3 wizard
      observation, E1–E4, the review record, regenerated coverage, TODO5,
      TODO6, and reconstruction notes are synchronized. TODO5 now reports
      818 reviewed and 28 partial procedures; the burn-down is **98 evidence
      units + 29 promotion units = 127 open work items**, with 29 procedures
      at 5/6 and 22 at 6/6.

- [x] Closed `DrawNewGame3` E3 localized-rendering evidence. The source now
      accepts a typed numeric formatter and obtains the negative-width suffix
      through resource `0x0af4`, while retaining a strict fallback formatter.
      The focused packet checks 18 formatter calls, 18 value lookups, 18
      right-aligned outputs, the localized `pct` suffix, all 27 sequential
      label IDs beginning at `0x3aa`, and the complete 36-reference boundary.
      The host and fresh Win98 `C:\DNG3L` probe both report
      `DRAW_NEW_GAME3_LOCALIZATION_E3_CHECKS=34 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. DrawNewGame3 now has one named evidence
      unit left—E4 complete page GDI/original comparison—plus Step-6
      promotion. The burn-down is now **99 evidence units + 30 promotion
      units = 129 open work items**, with 30 procedures at 5/6 and 21 at 6/6.

- [x] Closed `DrawNewGame3` E2 packed output-rectangle evidence. The focused
      packet drives all nine rows with deterministic mapped controls and checks
      every one of the 18 value rectangles plus all 18 vertically offset
      partners. It covers exact left/right coordinates, fixed button height,
      row-7/8 carry-over, all 18 value lookups, all 36 draw-button calls, and
      the negative-width percentage spacing boundary. The host and fresh
      Win98 `C:\DNG3R` probe both report
      `DRAW_NEW_GAME3_RECTANGLES_E2_CHECKS=95 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. DrawNewGame3 remains at 5/6 with E3
      localization, E4 complete page GDI/original comparison, and Step-6
      promotion open. The burn-down is now **100 evidence units + 30
      promotion units = 130 open work items**, with 30 procedures at 5/6 and
      21 at 6/6.

- [x] Closed `DrawNewGame3` E1 packed signed-width evidence. The source now
      accepts the recovered row-major 18-byte signed column table while
      retaining the named matrix as a host-adapter fallback. The focused
      packet checks negative-width percent formatting, the zero-width
      second-column label skip, all 17 resulting value lookups, the final
      value index, and 34 paired reference rectangles. The host and fresh
      Win98 `C:\DNG3P` probe both report
      `DRAW_NEW_GAME3_PACKED_E1_CHECKS=7 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. DrawNewGame3 remains at 5/6 with E2 packed
      rectangles, E3 localization, E4 complete page GDI/original comparison,
      and Step-6 promotion open. The burn-down is now **101 evidence units +
      30 promotion units = 131 open work items**, with 30 procedures at 5/6
      and 21 at 6/6.

- [x] Closed `DrawNewGame2` E3 complete page GDI integration and promoted the
      procedure to 6/6. The traced packet checks DC acquire/release ownership,
      opaque-background save/restore, bold-font and text-extent setup,
      sixteen-row diamond/label/text order, first/last row geometry and
      fourteen-pixel spacing, color reset, read-only/dead rows, and the
      caller-provided HDC path. The host and fresh Win98 `C:\DNG2I` probe both
      report
      `DRAW_NEW_GAME2_INTEGRATION_E3_CHECKS=16 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. E1, E2, E3, the original Step 2 replay,
      review record, regenerated coverage, TODO5, TODO6, and reconstruction
      notes are synchronized. TODO5 now reports 817 reviewed and 29 partial
      procedures; the burn-down is **102 evidence units + 30 promotion units =
      132 open work items**, with 30 procedures at 5/6 and 21 at 6/6.

- [x] Closed `DrawNewGame2` E2 localized-formatting evidence. The typed
      type-3 AI branch now resolves separate group and level labels from the
      packed kind byte and passes them to localized format `0x312` in the
      recovered order. The packet also covers empty, built-in race,
      special-race, generic, custom-name, custom-file, and game-file-mode
      resource paths. The host and fresh Win98 `C:\DNG2L` probe both report
      `DRAW_NEW_GAME2_LOCALIZATION_E2_CHECKS=15 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. `DrawNewGame2` is now `2/3` evidence with
      only E3 full GDI/resource integration open; the burn-down is **103
      evidence units + 31 promotion units = 134 open work items**, with 31
      procedures at 5/6 and 20 at 6/6.

- [x] Closed `DrawNewGame2` E1 packed player/type-table evidence. The typed
      renderer now consumes the recovered one-byte player kind/race table and
      thirteen-byte race-file table with the original masks, shift, and stride;
      the focused packet also checks empty-file fallback and game-file-mode
      direct filename formatting while deliberately conflicting with the typed
      shadow records. The host and fresh Win98 `C:\DNG2P` probe both report
      `DRAW_NEW_GAME2_PACKED_E1_CHECKS=13 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. The original decompilation and prior Step 2 replay
      are retained. `DrawNewGame2` remains at `1/3` evidence with E2 localized
      formatting and E3 full GDI/resource integration open; the burn-down is
      **104 evidence units + 31 promotion units = 135 open work items**, with
      31 procedures at 5/6 and 20 at 6/6.

- [x] Closed `NewGameDlg2` E1 packed race/password evidence and promoted the
      procedure to 6/6. The typed adapters commit the recovered 192-byte
      player record and 13-byte race-file slot for Create, Open, and
      password-accepted Edit actions; password rejection leaves the selected
      slot untouched, and all neighboring slots remain unchanged. The host and
      fresh Win98 `C:\ND2PACK2` probe both report
      `NEW_GAME_DLG2_PACKED_E1_CHECKS=1238 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. E1/E2/E3 evidence, original replay, review record,
      regenerated coverage, TODO5, TODO6, and reconstruction notes are
      synchronized. TODO5 now reports 815 reviewed and 31 partial procedures;
      the burn-down is **105 evidence units + 31 promotion units = 136 open
      work items**, with 31 procedures at 5/6 and 20 at 6/6.

- [x] Closed `NewGameDlg2` E3 transition/caller evidence. The focused probe
      checks accepted second-page command results at the `EndDialog` boundary,
      caller-stage dispatch, missing-first-player alert gating, read-only
      acceptance, and player-row race creation/propagation. The host and fresh
      Win98 `C:\ND2INT2` probe both report
      `NEW_GAME_DLG2_INTEGRATION_E3_CHECKS=9 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. Together with the original three-page replay, this
      leaves only E1 packed race/password evidence; the burn-down is **106
      evidence units + 32 promotion units = 138 open work items**.

- [x] Closed `NewGameDlg2` E2 resource/control evidence. The recovered
      resource `0x0187` is represented as a typed 261x210 second-page contract
      with an 8-point font and all five navigation controls, including exact
      IDs, rectangles, text lengths, and text checksums. The host and fresh
      Win98 `C:\ND2RES2` probe both report
      `NEW_GAME_DLG2_RESOURCE_E2_CHECKS=40 FAILURES=0 RESULT=PASS`; compile,
      link, process exit, and the post-run window inventory passed with no
      Program Error dialog. The first stale-header compile is preserved as
      diagnostic evidence in `runs/vmctl-vc152nd2res/`; the authoritative run
      is `runs/vmctl-vc152nd2res2/`. `NewGameDlg2` is now `1/3` evidence; E1
      packed race/password records and E3 complete wizard integration remain.
      The burn-down is **107 evidence units + 32 promotion units = 139 open
      work items**.

- [x] Closed `SimpleNewGameDlg` E3 creation/caller evidence and promoted the
      procedure to 6/6. The focused acceptance probe verifies density, map
      size, and selected race state at the `EndDialog(0x430)` caller boundary;
      the custom-race branch verifies load/wizard/save, list delete/add/limit,
      and focus restoration; Cancel preserves the accepted state. The host and
      fresh Win98 `C:\SNGDI` probe both report
      `SIMPLE_NEW_GAME_DLG_INTEGRATION_E3_CHECKS=12 FAILURES=0 RESULT=PASS`;
      compile, link, process exit, and the post-run window inventory passed
      with no Program Error dialog. The original six-page custom-race Save
      As/cancel replay, review record, regenerated coverage, TODO5, TODO6, and
      reconstruction note are synchronized. TODO5 remains at 814 reviewed and
      32 partial procedures because Phase 2 promotion is a stricter evidence
      gate than the source-level TODO5 status; the burn-down is **108 evidence units + 32
      promotion units = 140 open work items**, with 32 procedures at 5/6 and
      19 at 6/6.

- [x] Closed `SimpleNewGameDlg` E2 resource/control evidence. The recovered
      resource `0x00D1` is represented as a typed 240x190 dialog contract with
      all 16 controls, exact rectangles, text lengths, and resource-text
      checksums. The host and fresh Win98 `C:\SNGDR` probe both report
      `SIMPLE_NEW_GAME_DLG_RESOURCE_E2_CHECKS=117 FAILURES=0 RESULT=PASS`;
      compile, link, and process exit passed with no Program Error dialog. The
      procedure is now `2/3` evidence with only E3 creation mutation and
      caller integration open; the burn-down is **109 evidence units + 33
      promotion units = 142 open work items**.

- [x] Closed `SimpleNewGameDlg` E1 packed-player evidence. The optional
      0xC0-byte adapter copies the complete temporary player record into the
      selected new-player slot; the host and fresh Win98 `C:\SNGDP` probe both
      report `SIMPLE_NEW_GAME_DLG_PACKED_E1_CHECKS=3074 FAILURES=0 RESULT=PASS`
      across every byte of the selected slot and all 15 neighboring slots.
      Compile, link, and process exit passed with no Program Error dialog. The
      procedure then advanced to `2/3` when the independent resource packet
      closed; only E3 creation mutation remains.

- [x] Promoted `NewGameWizard` to 6/6. Its E1 packed-player state, E2
      resources/control geometry across all three pages, E3 complete creation
      transition/original replay, tracking record, regenerated coverage, TODO5,
      TODO6, and reconstruction note are synchronized. TODO5 now reports 814
      reviewed and 32 partial procedures; the Phase 2 burn-down is **111
      evidence units + 33 promotion units = 144 open work items**, with 33
      procedures at 5/6 and 18 at 6/6.

- [x] Closed `NewGameWizard` E2 resource/control evidence. The typed contract
      matches recovered resources 0x186, 0x187, and 0x188: all three pages are
      261x210 with 8-point fonts and all 45 controls have matching IDs,
      rectangles, text lengths, and text checksums. Host and fresh Win98
      `C:\NWRES` both report `NEW_GAME_WIZARD_RESOURCE_E2_CHECKS=366
      FAILURES=0 RESULT=PASS`; compile, link, and process exit passed with no
      Program Error dialog. Before promotion this reduced the burn-down to
      **111 evidence units + 34 promotion units = 145 open work items**.

- [x] Closed the shared creation E3 evidence unit for `NewGameDlg` and
      `NewGameWizard`. The host and fresh Win98 `C:\NGINT` probe both report
      `NEW_GAME_CREATION_E3_CHECKS=11 FAILURES=0 RESULT=PASS`; compile, link,
      process exit, and the post-run window inventory all passed with no
      Program Error dialog. The packet verifies dialog acceptance, wizard
      stages 1–3, packed player commit, game-name acceptance, bit restoration,
      world generation, and host dispatch, and is paired with the original
      creation replay in `runs/vmctl-vc152current/`. `NewGameDlg` reached
      `0 + P`; `NewGameWizard` reached `1 + P` with only E2 resources/
      geometry remaining. Before promotion this reduced the evidence burn-down
      to **112 evidence units + 35 promotion units = 147 open work items**.

- [x] Promoted `NewGameDlg` to 6/6. Its packed game record, resource/control
      geometry, shared creation transition, original replay, tracking record,
      regenerated coverage, TODO5, TODO6, and reconstruction note are now
      synchronized. TODO5 now reports 813 reviewed and 33 partial procedures;
      the Phase 2 burn-down is **112 evidence units + 34 promotion units =
      146 open work items**, with 34 procedures at 5/6 and 17 at 6/6.

- [x] Closed `NewGameDlg` E2 resource/geometry evidence. The typed resource
      contract matches the original 0x186 NE dialog: 261x210, 8-point font,
      and all 27 control IDs/rectangles. Host and fresh Win98 `C:\NGDR` both
      report `NEW_GAME_DLG_RESOURCE_E2_CHECKS=30 FAILURES=0 RESULT=PASS`;
      compile, link, and process exit passed with no Program Error dialog.
      Only the complete wizard mutation/replay unit remains; the authoritative
      burn-down is now **114 evidence units + 35 promotion units = 149 open
      work items**.

- [x] Closed `NewGameDlg` E1 packed-game evidence. The optional 64-byte
      `_game` adapter writes map size, density, start distance, option bits,
      and name at recovered offsets 4, 6, 12, 16, and 32 while preserving
      unowned bytes. The host and fresh Win98 `C:\NGDP` probe both report
      `NEW_GAME_DLG_PACKED_E1_CHECKS=7 FAILURES=0 RESULT=PASS`; compile, link,
      and process exit passed with no Program Error dialog. Two evidence units
      remain; the authoritative burn-down is now **115 evidence units + 35
      promotion units = 150 open work items**.

- [x] Closed `NewGameWizard` E1 packed-player evidence. The optional packed
      `_player` adapter now writes the wizard-owned flags, salt, dead marker,
      and normal/display names while preserving unrelated record bytes. The
      host and fresh Win98 `C:\NWP` probe both report
      `NEW_GAME_WIZARD_PACKED_E1_CHECKS=11 FAILURES=0 RESULT=PASS`; compile,
      link, and process exit passed with no Program Error dialog. Two evidence
      units remain for resources/geometry and complete creation replay; the
      authoritative burn-down is now **116 evidence units + 35 promotion units
      = 151 open work items**.

- [x] Promoted `DrawPlanetStarbase` to 6/6. Its E1 packed lookup, E2
      localized resources, and E3 exact service/GDI evidence were already
      closed in fresh native runs; the review record, coverage, TODO5, TODO6,
      and reconstruction note are now synchronized. The authoritative Phase 2
      burn-down is **117 evidence units + 35 promotion units = 152 open work
      items**, with 35 procedures at 5/6 and 16 at 6/6.

- [x] Promoted `KillUsedWaypoints` to 6/6. Its E1 live-table, E2 resource,
      and E3 complete-turn evidence were already closed in fresh native runs;
      the review record, coverage, TODO5, TODO6, and reconstruction note are
      now synchronized with the final promotion. The authoritative Phase 2
      burn-down is **117 evidence units + 36 promotion units = 153 open work
      items**, with 36 procedures at 5/6 and 15 at 6/6.

- [x] Closed `DrawPlanetProduction` E4 and completed its Step-6 promotion.
      The retained original Win98 captures show the selected `Oop Be Gone`
      planet, its empty production panel, the `Production Queue for Oop Be Gone`
      dialog with `-- Queue is Empty --`, and frame-refresh/fleet-selection
      changes without a Program Error. Together with the 14-check native
      service boundary, all four evidence units are closed; the review record,
      TODO5/coverage state, TODO6, and reconstruction note are synchronized.
      `DrawPlanetProduction` is now `6/6`. The authoritative Phase 2 burn-down
      is **117 evidence units + 37 promotion units = 154 open work items**,
      with 37 procedures at 5/6 and 14 at 6/6.

- [x] Closed `DrawPlanetProduction` E3, the exact display-service geometry and
      control boundary. The fresh host and `C:\DPSV` VC++ 1.52c probe report
      `DRAW_PLANET_PRODUCTION_SERVICE_CHECKS=14 FAILURES=0 RESULT=PASS`,
      covering pre/post list client rectangles, full/compact row heights,
      `0x14`/`0x15` resize flags, list/button geometry, localized label order,
      queue enablement, action-button kind, reference geometry, and tile reset.
      Compile/link passed, the process exited 0, and the final Win98 window
      inventory contained no Program Error dialog. `DrawPlanetProduction` is
      now `3/4` evidence units with only the original selected/empty/
      frame-refresh differential remaining. The authoritative Phase 2
      burn-down is now **118 evidence units + 38 promotion units = 156 open
      work items**, with 38 procedures at 5/6 and 13 at 6/6.

- [x] Closed `KillUsedWaypoints` E3, the complete-turn integration boundary.
      The fresh host and `C:\KUWTURN` VC++ 1.52c probe compose the real
      reconstructed `GenerateTurn`, `MoveFleets`, packed fleet-table adapter,
      and `KillUsedWaypoints`. Six checks confirm that the turn reaches
      `MoveFleets`, cleanup runs after movement, moved coordinates are written
      back to the packed fleet, stale targets are retargeted before deletion,
      and the PLORD queue compacts correctly. Compile/link passed, the process
      exited 0, and the final Win98 window inventory contained no Program Error
      dialog. The procedure is now `0 + P`; only final ledger promotion remains.
      The authoritative Phase 2 burn-down is now **119 evidence units + 38
      promotion units = 157 open work items**, with 38 procedures at 5/6 and
      13 at 6/6.

- [x] Closed `DrawPlanetStarbase` E3, the exact service and display-integration
      boundary. The fresh host and `C:\DPSBN` VC++ 1.52c probe report
      `DRAW_PLANET_STARBASE_SERVICE_CHECKS=7 FAILURES=0 RESULT=PASS`, covering
      hull-class title text, shield hull/player arguments, warp planet and
      destination-name lookup, action-button enablement, and gauge/reference
      geometry. Compile/link passed, the process exited 0, and the final
      Win98 window inventory contained no Program Error dialog. The procedure
      is now `0 + P`; only final ledger promotion remains. The authoritative
      Phase 2 burn-down is now **120 evidence units + 38 promotion units = 158
      open work items**, with 38 procedures at 5/6 and 13 at 6/6.

- [x] Closed `KillUsedWaypoints` E1, the live fleet-table enumeration
      boundary. The packed adapter now models the original four-byte
      `rglpfl` far-reference table, stops at its first null entry, loads each
      recovered 124-byte `_fleet` record and offset-100 `lpplord` reference
      into a bounded workspace, runs the genuine cleanup over the enumerated
      records, and writes the changed fleet/order fields back. The strict host
      probe and a fresh `C:\KUWPACK` VC++ 1.52c run both report
      `KILL_USED_WAYPOINTS_PACKED_CHECKS=6 FAILURES=0 RESULT=PASS`; compile and
      link passed, and the final Win98 window inventory contained no Program
      Error dialog. E2 localized resources was already closed; only E3 full
      turn integration and final promotion remain. The authoritative Phase 2
      burn-down is now **121 evidence units + 38 promotion units = 159 open
      work items**, with 38 procedures at 5/6 and 13 at 6/6.
- [x] Closed `DrawPlanetStarbase` E1, the packed design/starbase lookup
      boundary. The readable decoder now follows the recovered 56-byte
      `_planet`, 147-byte starbase `_shdef`, and 143-byte `_huldef` records:
      it extracts the starbase design slot, resolves the hull ID, reads cargo
      capacity and defense offsets, preserves the packed mass-warp word, and
      converts the destination field. It also checks no-starbase and invalid
      table bounds. Strict host C89 and a fresh `C:\SBDS` VC++ 1.52c run both
      report `DRAW_PLANET_STARBASE_DESIGN_CHECKS=10 FAILURES=0 RESULT=PASS`;
      compile/link passed, the process exited 0, and no Program Error dialog
      appeared. E2 localized resources is already closed; only E3 exact
      shield/warp services, full GDI/caller behavior, and original differential
      coverage remain before promotion. The authoritative Phase 2 burn-down is
      now **122 evidence units + 38 promotion units = 160 open work items**,
      with 38 procedures at 5/6 and 13 at 6/6.
- [x] Closed `DrawPlanetStarbase` E2, the exact localized-resource boundary.
      The original strings for row IDs `0x344` (`Dock Capacity`), `0x345`
      (`Armor`), `0x346` (`Shields`), `0x347` (`Damage`), `0x349`
      (`Mass Driver`), and `0x475` (`Destination`) were checked, along with
      special values `0x560` (`none`) and `0x2ed` (`Unlimited`). The strict
      host probe and a fresh `C:\SBRES` VC++ 1.52c run both report
      `DRAW_PLANET_STARBASE_RESOURCE_CHECKS=7 FAILURES=0 RESULT=PASS`;
      compile and link passed, the process exited 0, and the post-run window
      list contained no Program Error dialog. E1 packed records remains; E3
      exact shield/warp services, full GDI/caller behavior, and original
      differential coverage remain, followed by promotion. The authoritative
      Phase 2 burn-down is now **123 evidence units + 38 promotion units =
      161 open work items**, with 38 procedures at 5/6 and 13 at 6/6.
- [x] Closed `DrawPlanetProduction` E1, the packed production-item record and
      list-selection boundary. A 56-byte planet record, linked 4-byte
      `PLPROD` header, two variable production records, packed kind/item/count
      fields, name/schedule/format callbacks, listbox messages, and direct-item
      output pass strict host and fresh `C:\DPREC` VC++ 1.52c checks:
      `DRAW_PLANET_PRODUCTION_RECORD_CHECKS=16 FAILURES=0 RESULT=PASS`.
      Compile/link passed after the package received its proper Win16 stack
      definition, and the final window list contained no Program Error dialog.
      The remaining units are exact geometry/GDI/control integration and
      original differential coverage, followed by final promotion. The
      authoritative Phase 2 burn-down is now **124 evidence units + 38
      promotion units = 162 open work items**, with 38 procedures at 5/6 and
      13 at 6/6.
- [x] Closed `DrawPlanetProduction` E2, the exact localized production-label
      boundary. Original IDs/text `0x32e` (`Production`), `0x218`
      (`Completion: `), and `0x4e9` (`Route to`) pass strict host and fresh
      `C:\DPRES` VC++ 1.52c checks:
      `DRAW_PLANET_PRODUCTION_RESOURCE_CHECKS=3 FAILURES=0 RESULT=PASS`.
      Compile/link passed and no Program Error dialog appeared. The remaining
      units are packed production-item lookup, exact geometry/GDI/control
      integration, original differential coverage, and final promotion. The
      authoritative Phase 2 burn-down is now **125 evidence units + 38
      promotion units = 163 open work items**, with 38 procedures at 5/6 and
      13 at 6/6.
- [x] Closed `KillUsedWaypoints` E2, the exact localized message/resource
      boundary. The original compressed string table resolves `0x0137` to
      `Ultimate Recycling` and `0x004e` to `Annual resources from all planets:  `,
      preserving the original two trailing spaces. The strict host probe
      and fresh `C:\KUWRES` VC++ 1.52c run both report
      `KILL_USED_WAYPOINTS_RESOURCE_CHECKS=2 FAILURES=0 RESULT=PASS`; compile
      and link passed and no Program Error dialog appeared. Live fleet-table
      enumeration and complete turn integration remain open. The authoritative
      Phase 2 burn-down is now **126 evidence units + 38 promotion units =
      164 open work items**, with 38 procedures at 5/6 and 13 at 6/6.
- [x] Promoted `ZipOrderDlg` to 6/6 after closing E3, the real Win16
      USER/message-loop boundary. A fresh `C:\ZOD` run was extended with an
      actual parent window and child controls; VC++ 1.52c compile/link passed
      and `runs/vmctl-vc152zodmsg/ZODMSG.TXT` reports
      `ZIP_ORDER_DLG_USER_CHECKS=12 FAILURES=0 RESULT=PASS` for paint, radio
      selection, Create, Rename, Delete, control-color, Help, and OK teardown.
      The original `Rename Fleet` caller/cancel boundary remains preserved in
      `runs/vmctl-vc152current/fleet-rename.bmp`, and no Program Error dialog
      appeared. The reconstruction, native/integration notes, review ledger,
      and coverage record now agree. The authoritative Phase 2 burn-down is
      **127 evidence units + 38 promotion units = 165 open work items**, with
      38 procedures at 5/6 and 13 at 6/6.
- [x] Closed `ZipOrderDlg` E2, the localized resource-ID/text boundary. The
      updated host/native probe checks compressed title `0x231` (`Customize Zip
      Orders`), caption `0x4c1` (`Custom Orders`), empty-slot `0x4c8` (`Empty
      Custom Slot`), and action IDs `0x6d`–`0x71` (`(no action)`, `Load All
      Available`, `Unload All`, `Load Exactly...`, `Unload Exactly...`). Fresh
      VC++ 1.52c compile/link passed and `runs/vmctl-vc152zod/ZODR-RES.TXT`
      reports `ZIP_ORDER_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`; the console
      was empty and no Program Error dialog appeared. Only E3 complete live
      Win16/original differential integration remains for `ZipOrderDlg`. The
      Phase 2 burn-down is now **128 evidence units + 39 promotion units =
      167 open work items**, with 39 procedures at 5/6 and 12 at 6/6.
- [x] Closed `ZipOrderDlg` E1, the packed stored-order layout and mutation
      boundary. The fresh host/native `ZIP_ORDER_DLG_PACKED_CHECKS=11
      FAILURES=0 RESULT=PASS` matrix covers four 24-byte `_ziporder` records,
      five-word Create copying, raw name and ampersand handling, validity
      cleanup, Rename persistence, and Cancel closure. Compile and link pass
      in `C:\ZODINT`, and no Program Error dialog appeared. E2 localized
      resources and E3 complete live Win16/original differential integration
      remain open. This reduces the Phase 2 burn-down to **129 evidence units
      + 39 promotion units = 168 open work items**; the distribution remains
      39 procedures at 5/6 and 12 at 6/6.
- [x] Closed `ZipProdDlg` E3 and its Step-6 promotion. The original Win98
      Planet Summary Report path was exercised through the real Production
      Queue dialog: Add, a second Add with changed required-mineral/resource
      totals, Item Down, Item Up, Clear, Add then Remove, and Cancel back to
      the report. A fresh `C:\ZPDLAY` USER/listbox harness passed
      `FILL_ZIP_PROD_LB_USER_CHECKS=9 FAILURES=0 RESULT=PASS`, covering full
      and empty/invalid queue population, reset/add boundaries, invalidation,
      paint, and real message-loop delivery. No Program Error dialog appeared.
      The 36 packed/layout checks and 14-control USER-loop checks remain
      passing. Evidence is preserved in
      `runs/vmctl-vc152zpd-e3/`,
      `analysis/compat/zip-prod-dlg-integration.json`, and the Zip Production
      Queue reconstruction/native notes. `ZipProdDlg` is now promoted at
      `6/6`; the Phase 2 burn-down is **130 evidence units + 39 promotion
      units = 169 open work items**, with 39 procedures at 5/6 and 12 at 6/6.
- [x] Closed `ZipProdDlg` E1, the recovered packed queue/name storage unit.
      The CodeView and original initialization evidence identify five 40-byte
      `_zipprodq` records: a 13-byte name at offset 0, validity at 13,
      no-research at 14, item count at 15, and twelve 16-bit queue words at
      offset 16. The typed source and `FillZipProdLB` were corrected from the
      former 80-byte byte-array surrogate. Host strict C89 and fresh VC++
      1.52c/Win98 runs each pass
      `ZIP_PROD_DLG_CHECKS=36 FAILURES=0 RESULT=PASS`; the Win98 build and
      link pass, and no Program Error dialog appeared. Evidence is preserved
      in `analysis/debug/types.json`,
      `analysis/decompiler/acceptance/1000_124c_ReadIniSettings.c`,
      `analysis/compat/zip-prod-dlg-{native,integration}.md`,
      `analysis/compat/zip-prod-dlg-integration.json`, and
      `runs/vmctl-vc152zpdlay/`. At that historical checkpoint `ZipProdDlg`
      was `1 + P`; the current E3 closure and promotion are recorded in the
      entry above.
- [x] Closed `ZipProdDlg` E2, the real USER/control/resource integration
      unit, in fresh `C:\ZPDMSG`. The VC++ 1.52c build and link passed, and
      `runs/vmctl-vc152zpdmsg/ZPDMSG3.TXT` reports
      `ZIP_PROD_DLG_USER_CHECKS=14 FAILURES=0 RESULT=PASS`. The actual Win16
      message loop created the parent, four queue radio buttons, list box,
      Create/Rename/Delete/Help, and OK/Cancel controls; dispatched init,
      paint, erase, control-color, queue selection, delete, create, rename,
      help, and accept; and recorded two packed items, dirty bit `0x20`, two
      rename callbacks, and clean teardown. The final window list showed no
      Program Error dialog. This reduces `ZipProdDlg` from `3 + P` to
      `2 + P`: E1 packed global queue/name storage and E3 the broader
      Add/Remove/Item Up/Item Down/Clear mutation boundary remain, followed by
      the separate promotion item. Evidence is preserved in
      `analysis/compat/zip-prod-dlg-native.md`,
      `analysis/compat/zip-prod-dlg-integration.{md,json}`,
      `analysis/reconstruction/zip-prod-dlg.md`, and
      `runs/vmctl-vc152zpdmsg/`. The current Phase 2 burn-down is now 132
      evidence units plus 40 promotion units: 172 open work items across 40
      procedures still at 5/6 and 11 promoted at 6/6.
- [x] Promoted `BrowserDlg` to 6/6 after closing its direct original
      caller-path differential in the running Win98 guest. From the original
      game frame, `Commands > Ship Design...` opened the real `Ship & Starbase
      Designer`; `Components` displayed the `All` icon/name list, selection of
      `Quick Jump 5` and `Long Hump 6` changed the highlight and detail pane,
      `Armor` displayed `Tritanium`/`Crobmnium`, and `Beam Weapons` displayed
      `Laser`/`X-Ray Laser`. The fresh run ended with no Program Error dialog.
      Together with the fresh `C:\BRDMSG` native USER-loop report
      (`BROWSER_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`), this closes Step 5;
      the review record, coverage ledger, TODO5, and reconstruction notes now
      record promotion. Phase 2 is now 40 procedures at 5/6 and 11 at 6/6,
      with 132 evidence units and 40 promotion units open (172 open work
      items).
- [x] Closed the `BrowserDlg` real USER/control/message-loop unit in fresh
      `C:\BRDMSG`: the VC++ 1.52c build and link passed, and the native
      report is `BROWSER_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`. The
      harness creates the actual parent, combo, checkbox, Previous/Next,
      Done, and browser child controls; dispatches init, paint, erase, and
      command paths; adds all 17 component labels; exercises navigation and
      invalidation; and tears down cleanly. No Program Error dialog appeared.
      At that checkpoint `BrowserDlg` remained 5/6 because one focused
      original differential and Step-6 promotion remained. The later progress
      entry above closes both. At that earlier checkpoint Phase 2 was 41
      procedures at 5/6, 10 at 6/6, with 134 evidence units and 41 promotion
      units open (175 open work items); there were no M-band rows left.
- [x] Closed and promoted `GenerateWorld` E2 in fresh `C:\GWINT2`: the
      strict host and VC++ 1.52c/Win98 adapters each pass
      `GENERATE_WORLD_E2_CHECKS=13 FAILURES=0 RESULT=PASS`. The state-backed
      pipeline covers layout, 128 planets, starts/homeworld, four startup
      ships, five battle plans, file records, two data files, player status,
      and local finalization; compile/link/process pass and the guest window
      inventory has no Program Error dialog. The original Win98 generated-game
      observation and `.xy`/`.M1`/`.M2`/`.M3`/`.HST` artifacts complete the
      independent comparison. GenerateWorld is now 6/6 and promoted. At that
      checkpoint, Phase 2 was 41 procedures at 5/6, 10 at 6/6, with 135
      evidence units and 41 promotion units open (176 open work items).
- [x] Closed `GenerateWorld` E1 in fresh `C:\GWINT`: the strict host and
      VC++ 1.52c/Win98 adapters each pass
      `GENERATE_WORLD_E1_CHECKS=16 FAILURES=0 RESULT=PASS`. The new typed
      64-byte `_game` view checks `lid`, map/density/player/planet/start
      fields, dirty/world-generation flags, turn, and the separate
      `StarsSeeds` Randomize2/Random state; that state is consumed by the real
      layout phase. Compile and link pass, the process exits 0, and the final
      window inventory has no Program Error dialog. At that point GenerateWorld
      remained at 5/6 with E2 open; the current top entry records its later E2
      closure and promotion.
- [x] Closed `PopupMenu` E1/E2 and promoted it to 6/6 in fresh
      `C:\PMUINT3`: the strict host and VC++ 1.52c/Win98 adapters each pass
      `POPUP_MENU_E2_CHECKS=30 FAILURES=0 RESULT=PASS`. The native E2 path
      uses the real owner `WM_COMMAND` queue without a synthetic `PostMessage`,
      validates the exact recovered `PeekMessage` arguments, and checks packed
      fleet/thing/planet/deep caller records. A real first-item mouse selection
      yields `COMMAND=15000`, `MASK=0`; compile and link pass and the final
      window state has no Program Error dialog. Current Phase 2 is 42
      procedures at 5/6, 9 at 6/6, with 137 evidence units and 42 promotion
      units open.
- [x] Closed `Popup` E1/E2 and promoted it to 6/6 in fresh `C:\PUINT2`:
      the strict host and VC++ 1.52c/Win98 adapters each pass
      `POPUP_E2_CHECKS=148 FAILURES=0 RESULT=PASS`. The matrix covers all
      measured and fixed popup kinds, localized labels, packed popup/fleet/
      planet/hull payload checksums, independent geometry, screen clamping,
      slot updates, font/message/capture setup, and no Program Error dialog.
      Current Phase 2 is 43 procedures at 5/6, 8 at 6/6, with 139 evidence
      units and 43 promotion units open.
- [x] Closed `UpdatePlayerScores` E1/E2 and promoted it to 6/6 in fresh
      `C:\UPSINT2`: the strict host and VC++ 1.52c/Win98 adapters each pass
      `UPDATE_PLAYER_SCORES_E2_CHECKS=36 FAILURES=0 RESULT=PASS`. Six cases
      independently verify populated/dead/tied/single-player score updates,
      packed report fields, message counts, dirty invalidation, and the
      complete turn/report replay. Compile and link pass; the final window
      list and screenshot contain no Program Error dialog. Current Phase 2 is
      44 procedures at 5/6, 7 at 6/6, with 141 evidence units and 44
      promotion units open.
- [x] Closed `AutoRouteFleet` E2 and promoted it to 6/6 in fresh
      `C:\ARFINT5`: the strict host and VC++ 1.52c/Win98 adapters each pass
      `AUTO_ROUTE_FLEET_E2_CHECKS=86 FAILURES=0 RESULT=PASS`. The matrix maps
      packed design/hull/engine/cargo/mineral state into the real `EstFuelUse`
      service, invokes the real `StargateDamage` callback, and consumes the
      generated order through the real `MoveFleets` pass across four
      resource/turn cases checked by an independent oracle. Compile and link
      pass and the final window list and screenshot contain no Program Error
      dialog. Current Phase 2 is 45 procedures at 5/6, 6 at 6/6, with 143
      evidence units and 45 promotion units open.
- [x] Closed `AutoRouteFleet` E1 in fresh `C:\ARFINT4`: the strict host and
      VC++ 1.52c/Win98 adapters each pass
      `AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS`.
      The final matrix independently transcribes the raw route loop across
      twelve helper, gate-damage, mineral-slot, geometry, fuel-descent, and
      resource-capacity variations; compile and link pass and the final window
      list contains no Program Error dialog. One E2 resource/turn boundary
      remains. Current Phase 2 is 46 procedures at 5/6, 5 at 6/6, with 144
      evidence units and 46 promotion units open.
- [x] The intermediate `LFuelUseToWaypoint` E1 milestone in fresh `C:\FUTINT3`
      closed the original segment-loop differential; it is retained as the
      preceding checkpoint below.
- [x] Closed `LFuelUseToWaypoint` E2 and promoted the procedure to 6/6 in
      fresh `C:\FUTINT4`: the strict host and VC++ 1.52c/Win98 adapters each
      pass `FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=27 FAILURES=0 RESULT=PASS`.
      The final matrix loads all sixteen packed fleet/design slots, both
      cargo-hull IDs, the planet-selected hull definition, and the full
      two-segment order boundary; compile and link pass and the final window
      list contains no Program Error dialog. At that checkpoint, Phase 2 was
      46 procedures at 5/6, 5 at 6/6, with 145 evidence units and 46
      promotion units open.
- [x] Closed `LFuelUseToWaypoint` E1 in fresh `C:\FUTINT3`: the strict host
      and VC++ 1.52c/Win98 adapters each pass
      `FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=23 FAILURES=0 RESULT=PASS`.
      The added matrix independently transcribes the original segment loop
      across twelve route, warp, cargo-hull, ram-scoop, and planet-reset cases;
      compile and link pass and the final window list contains no Program Error
      dialog. One E2 ship/waypoint integration boundary remains. Current Phase
      2 was 47 procedures at 5/6, 4 at 6/6, with 146 evidence units and 47
      promotion units open.
- [x] Closed `FStargateJump` E2 and promoted the procedure to 6/6 in fresh
      `C:\SGJINT5`: the corrected reconstruction builds the raw lost-fleet
      service record, preserves the surviving source, and invokes the real
      reconstructed `FleetTransferCargoBalance` boundary. The strict host and
      VC++ 1.52c/Win98 runs each pass
      `F_STARGATE_JUMP_INTEGRATION_CHECKS=2311 FAILURES=0 RESULT=PASS`; the
      final window list contains no Program Error dialog. At that milestone,
      Phase 2 was 47 procedures at 5/6, 4 at 6/6, with 147 evidence units and
      47 promotion units open.
- [x] At the earlier Step-5 documentation snapshot, visibility was expanded
      again for all 51 procedures:
      every row now states the remaining engineering task, the expected
      host/native/original evidence package, and the exact closure condition.
      The snapshot burn-down was 148 open evidence units plus 48 promotion
      units, or 196 open work items. Relative `S`/`M`/`L`/`XL`
      labels show why the unchanged `5/6` headline is not a one-task or
      one-sixth-remaining estimate.
- [x] Phase 0 ledger repair and Phase 1 promotion completed. TODO5 now reports
      795 reviewed, 51 partial, and 0 open named procedures.
- [x] Phase 2 audit completed: all 51 partial procedures have host C89 source
      and probes; their remaining scope is recorded in the coverage ledger.
- [x] Phase 2 progress presentation expanded: every partial procedure now
      carries an explicit `4/6` or `5/6` gate count in the checklist and the
      procedure matrix, with a cluster roll-up and an explicit warning that
      the fraction counts completed gates rather than remaining effort.
- [x] Phase 2 six-step progress matrix expanded to show the gate count for
      every procedure: all 51 are now at original/integration step 5/6, with
      none at steps 3/6, 4/6, or 6/6. The
      procedure checkboxes deliberately remain open until the final promotion
      gate, and the matrix now distinguishes maturity from remaining effort.
- [x] Step-5 effort visibility expanded for all 51 procedures: the new
      burn-down ledger records 151 independently closable evidence units plus
      one explicit promotion unit per procedure, along with the next closure
      to pursue for every row. These counts are work-item counts, not time
      estimates, and are now the progress measure beneath the unchanged 5/6
      headline.
- [x] Step-5 unit definitions expanded for all 51 procedures: each remaining
      count is now split into individually named packed-record, resource/UI,
      native USER/GDI, differential/replay, or caller-integration boundaries.
      The ledger therefore shows 201 open work items in total (151 evidence
      units plus 50 promotion units), rather than presenting Step 5 as one
      undifferentiated checkbox per procedure.
- [x] `ResearchDlg` live USER message-loop checkpoint completed in isolated
      `C:\RSDM`: the native-only harness created real parent and child
      controls, ran the message pump, routed initialization/radio/Done through
      the reconstructed procedure, and reported
      `RESEARCH_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`. VC++ 1.52c compile
      and link passed, the final window list had no Program Error dialog, and
      the one remaining ResearchDlg evidence unit was removed from the ledger.
- [x] `ResearchDlg` Step-6 promotion completed: the review record is now
      `manually_reviewed`, TODO5 is regenerated with the procedure promoted,
      and the coverage/reconstruction/native evidence references are
      synchronized. Phase 2 now has 50 procedures at 5/6 and one at 6/6.
- [x] `PopupMenu` packed/Win16 integration checkpoint completed: the host
      report passes 30 structural checks, and the native wrapper passes 31
      checks including real `TrackPopupMenu` display, Enter selection of the
      first item, and decoding command 15000 as selection 0. The native run
      had no Program Error dialog. The source now preserves the raw
      caller-owned submenu header behavior; exact owner/message-queue behavior
      without the harness bridge, complete localized lookup, broader caller
      records, and promotion remain open, so the row is 5/6 rather than
      complete.
- [x] Expanded `PopupMenu` compressed-string lookup in fresh `C:\PMUINT2`:
      the flagged-name path now checks the actual original payloads for IDs
      `0x43F` through `0x44F` (`All` through `Torpedoes`), including their
      order, first/last IDs, and combined ASCII sum of 15229. The strict host
      probe passes 31 structural checks; the VC1.52c native integration run
      passes `POPUP_MENU_INTEGRATION_CHECKS=32 FAILURES=0 RESULT=PASS`, with
      real popup selection and no Program Error dialog. Exact owner/message
      queue behavior without the harness bridge, fleet/thing/planet caller
      records, and promotion remain open, so the row stays 5/6.
- [x] `ReportDlg` packed-record integration checkpoint completed: the host and
      Win98 reports each pass 20 checks over the recovered 54-byte `_rpt`
      layout, raw-to-typed round trips, initialization layout, cached widths,
      scrollbar handles, horizontal field-first mutation, vertical row-first
      mutation, and invalidation/update callbacks. Packed report unions and
      row contents, exact live USER/GDI handles, menu resources, full caller
      integration, and promotion remain open, so the row remains 5/6.
- [x] Expanded `ReportDlg` packed/caller-boundary validation in fresh
      `C:\RPTINT2`: the strict host and VC++ 1.52c/Win98 matrix now passes 24
      checks, adding the `SCROLLBAR` class and vertical/horizontal styles with
      the report window as parent, popup-state synchronization, the `300x220`
      minimum-window boundary, and report-menu selection `0x0900` through the
      frame submenu. Compile and link passed, the Win98 report was
      `REPORT_DLG_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS`, and no Program
      Error dialog appeared. The row remains at 5/6: populated report
      unions/rows, exact live USER/GDI and menu payload behavior, complete
      caller integration, and promotion remain open. The first native attempt
      caught the VC1.52c enum-width issue in the 32-bit styles; after changing
      those constants to explicit `UL` macros, the corrected report is
      `runs/vmctl-vc152rptint2/RPTINT5.TXT`.
- [x] `DoRototillAiTurn` packed integration checkpoint completed: the recovered
      56-byte `_planet` and 124-byte `_fleet` views round-trip known owner,
      visibility, mineral, planet, cord, and sixteen 16-bit ship-count fields;
      the host and fresh Win98 probe each report 12 checks with zero failures.
      The adapter keeps optimal value and desirability as service results,
      matching the original calls rather than inventing raw fields. Queue and
      colonizer/mineral routing, raw `PLORD` mutation, complete AI-table
      integration, and full original-turn differential remain open; the row is
      now 5/6, not promoted.
- [x] `DoRobotoidAiTurn` packed integration checkpoint completed: the recovered
      56-byte `_planet` and 124-byte `_fleet` views round-trip owner, flags,
      packet-strength bits, planet, cord, and sixteen 16-bit ship-count fields;
      the host and fresh Win98 probe each report 18 checks with zero failures.
      The adapter keeps production resources/costs and design selections as
      explicit service results matching the original calls. Fleet-order repair,
      colonizer/transport routing, raw `PLORD` mutation, complete AI-table
      integration, and full original-turn differential remain open; the row is
      now 5/6, not promoted.
- [x] `DoTurinDroneAiTurn` packed integration checkpoint completed: the
      recovered 56-byte `_planet` and 124-byte `_fleet` views round-trip owner,
      visibility, mineral, planet, cord, and sixteen 16-bit ship-count fields;
      the host and Win98 probe each report 20 checks with zero failures. The
      adapter keeps production resources/costs, design selections, and order
      state as explicit service results matching the original call boundary.
      Detailed production policy, fleet-order mutation, colonizer/transport
      routing, complete AI-table integration, and full original-turn
      differential remain open; the row is now 5/6, not promoted.
- [x] `DoAutomitronAiTurn` packed integration checkpoint completed: the
      recovered 56-byte `_planet` and 124-byte `_fleet` views round-trip owner,
      flags, planet, and sixteen 16-bit ship-count fields; the host and Win98
      probe each report 20 checks with zero failures. The adapter keeps planet
      service values, production resources/costs, design selections, and
      detailed order decisions as explicit service results matching the
      original call boundary. Colonizer/mineral fleet handling, order repair,
      remaining packed order integration, and complete original-behavior
      evidence remain open; the row is now 5/6, not promoted.
- [x] `DoMacintiAiTurn` packed integration checkpoint completed: the
      recovered 56-byte `_planet` and 124-byte `_fleet` views round-trip owner,
      flags, packet, planet, cord, and sixteen 16-bit ship-count fields; the
      host and Win98 probe each report 31 checks with zero failures. The
      adapter keeps mineral/service values, production costs, design choices,
      and detailed order decisions as explicit service results matching the
      original call boundary. Per-planet table selection, exact
      retarget/colonizer/transport predicates, direct packed order mutation,
      and complete original-behavior evidence remain open; the row is now
      5/6, not promoted.
- [x] The initial native Phase 2 checkpoint (`GenerateWorld`) exposed a
      package-boundary failure: the Win98 execution raised “Program Error —
      not enough space for arguments” and produced no `GWR.TXT`. That failed
      attempt is retained as diagnostic evidence and was superseded by the
      fresh-directory rerun below.
- [x] Diagnosed the first native failure: `compat/vc152/generate-world/GW.LNK`
      placed linker switches where the definitions-file field belongs, so the
      linker ignored `GW.DEF` and emitted `L4021: no stack segment`. The link
      response now matches the working VC1.52c fixtures, and
      `tests.test_vc152_package` passes.
- [x] Reconnected the Win98 guest agent, rebuilt the corrected GenerateWorld
      harness in fresh `C:\GWT`, and reran the native checkpoint. The guest
      reported `COMPILE=PASS`, `LINK=PASS`, and
      `GENERATE_WORLD_CHECKS=31 FAILURES=0 RESULT=PASS`; the host listener was
      then stopped while leaving the guest agent running.
- [x] `KillUsedWaypoints` source milestone: exposed the raw player-message
      status-word boundary (`fleet id | 0x8000`) and added an exact host probe
      assertion. Packed PLORD offsets, native execution, and full integration
      remain open, so the ledger row stays partial.
- [x] Regression after the shared-header change: the full suite passes 519
      tests with 153 expected skips and no failures; the strict-C89
      `KillUsedWaypoints` probe also reports `PASS`.
- [x] `FStargateJump` source milestone: exposed the separate raw fleet ID in
      the player-message envelope and added exact abort/empty payload checks.
      Cargo-layout integration, native execution, and differential cases remain
      open, so the ledger row stays partial.
- [x] `EstMineralsMined` source milestone: added an optional typed `rgwtMin`
      storage path with the original fixed-point concentration/percentage
      depletion arithmetic and a deterministic boundary probe. Packed planet
      integration and native validation remain open, so the ledger row stays
      partial.
- [x] `MineClick` source milestone: corrected the mineral-graph threshold
      popup boundary to pass the selected menu index, matching the original
      `PopupMenu` call, and added an exact host assertion. Packed popup records
      and native validation remain open, so the ledger row stays partial.
- [x] `EstimateItemProdSched` source milestone: corrected the special-item
      bitfield discriminator and final `0x3fc` quantity sentinel, with a probe
      observing the item passed to the build callback. Packed queue integration
      and native validation remain open, so the ledger row stays partial.
- [x] `FDoCoolBattle` source milestone: exposed the raw `SendBattleMessages`
      report summary (battle ID, masks, active counts, round count, and record
      length) and added exact host assertions. Packed battle-log integration,
      native execution, and full damage behavior remain open, so the ledger row
      stays partial.
- [x] `DoAutomitronAiTurn` source milestone: exposed the shared sixteen-byte
      design-status mask through all four status calls and into the split
      callback, with exact host assertions. Colonizer/mineral fleet handling,
      order repair, packed integration, and original-behavior evidence remain
      open; its fresh native checkpoint now passes 31/31 checks.
- [x] `DoRototillAiTurn` source milestone: exposed research-order handoff,
      mineral scoring, optimal-value classification, productive-planet
      tracking, typed fleet boundaries, and final dispatch; its recovered raw
      planet/fleet boundary now also has a separate 12-check host/native
      integration checkpoint. Queue construction, detailed routing, raw PLORD
      mutation, and full original-turn behavior remain open.
- [x] `DoCyberAiTurn` source milestone: exposed the shared sixteen-byte
      design-status mask, matched the raw clearing boundary after each status
      call, made the first late-turn split consume that mask, and corrected the
      remaining split masks to `{0:2}`, `{1:2}`, and `{2:2,3:2}`. The strict-C89
      Cyber probes pass; packet routing, freighter behavior, fleet-order repair,
      direct packed PLORD mutation, and complete original-turn evidence remain
      open. Its packed host/native integration checkpoint now passes 25/25.
- [x] `ReportDlg` source milestone: exposed report-view initialization,
      scroll/layout arithmetic, paint and mouse dispatch, cleanup, and default
      procedure boundaries through 26 strict-C89 checks; packed records and
      full USER/GDI integration remain open.
- [x] `ExecuteReportClick` source milestone: exposed fleet, enemy-fleet,
      battle, production, installation, mineral, defense, and resource-cell
      dispatch through 21 strict-C89 checks; packed rows and full caller
      integration remain open.
- [x] `DumpPlanets` source milestone: exposed active/viewer gating, normal and
      detailed header selection, visibility and class masking, row copying,
      derived turn age, and close-on-write-failure behavior through 16 strict-
      C89 checks; formatting and full integration remain open.
- [x] `DumpFleets` source milestone: exposed active/viewer gating, normal and
      detailed headers, ship and hull-class totals, destination visibility,
      detailed fields, and close-on-write-failure behavior through 10 strict-
      C89 checks; formatting and full integration remain open.
- [x] `DrawPopup` source milestone: exposed all popup-kind dispatches, client
      geometry, Win16 GDI state setup/restoration, and slot-dialog variants
      through 51 strict-C89 checks; packed records, localization, and detailed
      GDI integration remain open.
- [x] `Popup` source milestone: exposed client-to-screen conversion, all popup
      sizing branches, slot-row arithmetic, font/DC cleanup, screen clamping,
      creation, messaging, and capture through 31 strict-C89 checks; localized
      measurement and full content integration remain open.
- [x] `PtDisplayPlanetStateInfo` source milestone: exposed known/unknown
      limit branches, environment and planet rows, improvement warnings,
      signed differences, wrapping order, and returned layout through 34
      strict-C89 checks; packed lookup, formatting, and GDI integration remain
      open.
- [x] `DoRobotoidAiTurn` source milestone: matched the raw inactive `0x1d`
      design-status filter for entries 14–15 through a typed packed-record
      adapter and added an exact split-mask probe. The four strict-C89
      Robotoid probes pass; fleet-order repair, colonizer/transport routing,
      packed integration, and native validation remain open.
- [x] `DoTurinDroneAiTurn` source milestone: corrected the unowned-planet
      two-pass boundary so the optical-value service is called once for the
      viable-world count, while the second pass annotates every visible
      unowned world without a repeat service call. The Turin source probe now
      checks both callback count and annotation count; routing and native gaps
      remain open.
- [x] `DoMacintiAiTurn` source milestone: matched the raw inactive `0x1d`
      design-status filter for entries 12–13 through a typed packed-record
      adapter and added exact first-split assertions. All six focused Macinti
      probes pass; per-planet table selection, detailed routing, and native
      validation remain open.
- [x] `FAttack` source milestone: corrected complex-beam dual-mode selection
      to evaluate the raw high target-mode nibble before the low nibble, with
      an exact target-order assertion in the source probe. Packed hull/part
      damage scaling, full battle integration, and native validation remain
      open.
- [x] `FDoCoolBattle` source milestone: corrected staged-log report length so
      `SendBattleMessages` receives the serialized current-battle length rather
      than the length plus any existing log prefix. The strict-C89 battle probe
      passes its 53 checks, including a nonzero append-offset case; packed
      integration, native validation, and full damage behavior remain open.
- [x] Native retry recorded: the local controller listener was restarted and
      left ready for StarDev, but the running guest produced no `HELLO`/agent
      handshake during the bounded check. No native pass or failure result was
      inferred; guest-agent reconnection remains the next native prerequisite.
- [x] `GenerateWorld` host checkpoint rerun: the strict-C89 source probe
      reports `GENERATE_WORLD_CHECKS=44 FAILURES=0`, the typed phase-pipeline
      probe reports 7/7, and the point-ID boundary probe reports 2/2. These
      confirm the current layout, planet, start-selection, homeworld,
      startup-ship, file/finalization, wormhole, and special-ID host paths;
      the cluster remains partial because native VC1.52c execution, packed
      integration, and original-executable comparison are still open.
- [x] `IDropPart` source milestone: added the separate packed slot-capacity
      boundary, capacity-limited merge arithmetic, and actual source-slot
      decrement for both empty-side drops and partial merges. The strict-C89
      probe now reports 26 checks with zero failures; the recovered 123-byte
      `_hul`/`rghs`/`_hs`/`chs` layout now also passes a separate 24-check
      host/native integration checkpoint. Exact Win16 hit testing, drag
      initiation, build-dialog redraw/control integration, and complete
      original drop behavior remain open.
- [x] `FillBuildDD` source milestone: matched the normal-mode delete/edit
      enablement, the terminating `CBN_END` notification, the 32-row height
      clamp, and the existing font-height rounding. The strict-C89 probe now
      reports 8 checks with zero failures; exact packed design lookup,
      localization, and native build-dialog validation remain open.
- [x] `SimpleNewGameDlg` source milestone: matched custom-race list rotation
      when more than seven entries are present, the raw delete/add/limit
      boundary, focus restoration after the race wizard, and the raw
      no-selection radio sentinel. The focused strict-C89 probe passes; packed
      player-record copying, exact frame geometry, and native dialog
      validation remain open.
- [x] `DrawNewGame3` source milestone: restored the raw percent suffix for
      negative-width value columns. The focused strict-C89 probe reports 2
      checks with zero failures; exact localized formatting, packed global
      rectangles, and native GDI validation remain open.
- [x] `DrawNewGame2` source milestone: restored the raw race-6 player label
      (`0x20b`) instead of routing that case to the generic player label. The
      focused strict-C89 probe passes; exact packed player/type tables,
      localized formatting, and native GDI validation remain open.
- [x] `NewGameDlg` source milestone: matched the raw exclusive radio-scan
      sentinel for no-selection states, yielding fallback offsets 5/4/4 for
      size, density, and start distance. The focused probe reports 22 checks
      with zero failures; packed game-record integration and native dialog
      validation remain open.
- [x] Regression after the Cyber/Robotoid shared-header changes: 519 tests
      pass with 153 expected skips and no failures; generated output remains
      2241 layout assertions and 846 function declarations.
- [x] Regression after the Turin two-pass correction: 519 tests pass with 153
      expected skips and no failures; baseline/decompiler invariants, JSON
      validation, and whitespace checks also pass.
- [x] Regression after the Macinti status-filter header change: 519 tests pass
      with 153 expected skips and no failures; generated output remains 2241
      layout assertions and 846 function declarations.
- [x] Regression after the FAttack target-mode ordering correction: the
      strict-C89 combat probe reports `F_ATTACK_CHECKS=33 ... PASS`, and the
      baseline/decompiler invariants, JSON validation, and whitespace checks
      pass; the full suite then completed with 519 tests, 153 expected skips,
      and no failures.
- [x] Regression after the FDoCoolBattle staged-report correction: the full
      suite passes 519 tests with 153 expected skips and no failures; generated
      output remains 2241 layout assertions and 846 function declarations.
- [x] Regression after the IDropPart capacity/source-decrement correction: the
      full suite passes 519 tests with 153 expected skips and no failures;
      generated output remains 2241 layout assertions and 846 function
      declarations.
- [x] Regression after the FillBuildDD, SimpleNewGameDlg, and DrawNewGame3
      source corrections: the full suite passes 519 tests with 153 expected
      skips and no failures; generated output remains 2241 layout assertions
      and 846 function declarations.
- [x] Regression after the DrawNewGame2 special-race-label correction: the
      full suite passes 519 tests with 153 expected skips and no failures;
      generated output remains 2241 layout assertions and 846 function
      declarations.
- [x] Regression after the NewGameDlg and SimpleNewGameDlg radio-sentinel
      corrections: the full suite passes 519 tests with 153 expected skips and
      no failures; generated output remains 2241 layout assertions and 846
      function declarations.
- [x] `GenerateWorld` native checkpoint completed in a fresh `C:\GWT`
      directory: VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the
      Win98 executable reports `GENERATE_WORLD_CHECKS=31 FAILURES=0
      RESULT=PASS` without the prior “not enough space for arguments” dialog.
      Evidence is preserved in `runs/vmctl-vc152gwt/`; packed whole-game and
      broader file/UI integration remain open.
- [x] `GenerateWorld` step-5 creation/file/UI integration checkpoint completed:
      the original Win98 executable opened New Game, generated a game frame,
      and created the `.xy`, `.M1`, `.M2`, `.M3`, and `.HST` artifact family
      without a Program Error dialog. The original instruction sequence and
      artifact hashes are preserved in
      `analysis/compat/generate-world-integration.md`,
      `analysis/compat/generate-world-integration.json`, and
      `runs/vmctl-vc152current/`; full packed game-state and broader
      resource/UI integration remain open.
- [x] New Game flow step-5 original/UI integration checkpoint completed for
      `NewGameDlg`, `NewGameWizard`, and `NewGameDlg2`: the original Win98
      executable opened the settings dialog, traversed all three Advanced Game
      wizard pages, opened the player race/computer/no-player menu, and reached
      the Choose New Game Name finalization dialog without a Program Error.
      Evidence is preserved in
      `analysis/compat/new-game-flow-integration.md`,
      `analysis/compat/new-game-flow-integration.json`, and
      `runs/vmctl-vc152current/`; packed player/race records, localized
      resources, exact Win16 control details, and promotion remain open.
- [x] `SimpleNewGameDlg` step-5 original/UI integration checkpoint completed:
      the original Win98 executable opened all six Custom Race Wizard pages,
      reached the `Save As` dialog for `Stars! Race Files`, and returned cleanly
      through cancellation without a Program Error. Evidence is preserved in
      `analysis/compat/simple-new-game-integration.md`,
      `analysis/compat/simple-new-game-integration.json`, and
      `runs/vmctl-vc152current/`; packed player records, localized resources,
      exact Win16 geometry/control details, and promotion remain open.
- [x] `ReportDlg` and `PopupMenu` step-5 original/UI integration checkpoint
      completed: the original Win98 Reports menu displayed its six report
      choices, opened `Planet Summary Report -- 1 Planet`, and opened the
      Planet Name sort/reverse-sort popup without a Program Error. Evidence is
      preserved in `analysis/compat/report-popup-integration.md`,
      `analysis/compat/report-popup-integration.json`, and
      `runs/vmctl-vc152current/`; packed report unions, complete localized
      lookup, exact USER/GDI integration, and full caller coverage remain open.
- [x] `DumpFleets` and `DumpPlanets` step-5 original/file integration
      checkpoint completed: the original Win98 export submenu wrote and
      reported success for `GAME.fle` (527 bytes) and `GAME.pla` (334 bytes).
      Evidence and hashes are preserved in
      `analysis/compat/report-dump-integration.md`,
      `analysis/compat/report-dump-integration.json`, and
      `runs/vmctl-vc152current/`; exact localized formatting, packed fields,
      scanner/minefield/task data, and battle-plan integration remain open.
- [x] Planet/fleet frame step-5 original/UI integration checkpoint completed
      for `DrawPlanetStats`, `DrawPlanetProduction`, `DrawPlanetStarbase`,
      `DrawPlanShipBitmap`, and `DrawPlanetShipList`: the original Win98 frame
      displayed the planet summary, production, starbase, fleet bitmap,
      composition, cargo, and waypoint panels, and a map click refreshed the
      selected fleet controls without a Program Error. Evidence is preserved
      in `analysis/compat/planet-frame-integration.md`,
      `analysis/compat/planet-frame-integration.json`, and
      `runs/vmctl-vc152current/`; packed records, localized/GDI details, and
      full planet integration remain open.
- [x] `ResearchDlg` step-5 original/UI integration checkpoint completed: the
      original Win98 Commands > Research dialog displayed technology status,
      projected research, expected benefits, and resource allocation, and a
      Weapons-field selection refreshed cleanly. Evidence is preserved in
      `analysis/compat/research-integration.md`,
      `analysis/compat/research-integration.json`, and
      `runs/vmctl-vc152current/`; packed player storage, localized resources,
      exact Win16 control details, and promotion remain open.
- [x] Expanded `ResearchDlg` packed-player validation in fresh `C:\RSD2`: the
      strict host/native matrix now has 10 checks covering the exact player
      write offset `0x22`, two-byte length, ordinary packed value `0x12`, and
      zero-percent/technology-5/future-technology-6 value `0x65`. VC++ 1.52c
      compile/link passed, the Win98 report was
      `RESEARCH_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`, and no Program Error
      dialog appeared. This strengthens step 5/6; localized resources, exact
      Win16 control integration, and promotion remain open.
- [x] Expanded `ResearchDlg` resource/control validation in fresh `C:\RSD3`:
      the strict host and VC++ 1.52c/Win98 matrix now passes 11 checks over
      the six technology labels, the `Research` title, future-field resource
      IDs `0x53` through `0x5a`, and the exact radio (`0x431`–`0x436`), combo
      (`0x43b`), Done (`2`), and Help (`0x76`) control lookups. Compile and
      link passed, the Win98 report was
      `RESEARCH_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`, and no Program Error
      dialog appeared. At that point the row remained at 5/6 pending the live
      USER message-loop evidence and promotion.
- [x] `ResearchDlg` live USER message-loop validation completed in fresh
      `C:\RSDM`: the final VC++ 1.52c build/link passed and the real-control
      message pump returned `RESEARCH_DLG_USER_CHECKS=9 FAILURES=0
      RESULT=PASS`, including the expected commit and two-byte write at
      offset `0x22`; the final guest window list had no Program Error dialog.
      The Step-5 evidence unit is closed; only ledger promotion remains.
- [x] `ProductionDlg` and `DrawProductionDlg` step-5 original/UI integration
      checkpoint completed: the original Ship & Starbase Designer displayed
      ship/starbase modes, existing designs, hulls, components, images,
      statistics, and copy/delete/edit controls without a Program Error.
      Evidence is preserved in `analysis/compat/production-dialog-integration.md`,
      `analysis/compat/production-dialog-integration.json`, and
      `runs/vmctl-vc152current/`; packed production/queue records, exact
      resources and geometry, and promotion remain open.
- [x] `NewGameWizard` native checkpoint completed in fresh `C:\NWT`: the
      current source compiled and linked with VC++ 1.52c, and the Win98 probe
      reported `NEW_GAME_WIZARD_CHECKS=30 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `runs/vmctl-vc152nwt/`; packed player storage, localized resources, and
      full `GenerateWorld` integration remain open.
- [x] `NewGameDlg2` source milestone: restored the raw `WM_CONTROL_COLOR`
      control-handle scan (`0x191..0x1c0` plus the high-word class fallback),
      button-face background-color callback, and brush return. The focused
      strict-C89 probe reports `NEW_GAME_DLG2_CHECKS=29 FAILURES=0
      RESULT=PASS`; native dialog/resource validation remains open.
- [x] `NewGameDlg2` native checkpoint completed in fresh `C:\ND2`: VC++ 1.52c
      reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `NEW_GAME_DLG2_CHECKS=29 FAILURES=0 RESULT=PASS` without a runtime error
      dialog. Evidence is preserved in `runs/vmctl-vc152nd2/`; menu/resource,
      packed race-record, and exact geometry integration remain open.
- [x] `DrawNewGame2` native checkpoint completed in fresh `C:\DNG2`: VC++ 1.52c
      reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `DRAW_NEW_GAME2_CHECKS=2 FAILURES=0 RESULT=PASS` without a runtime error
      dialog. Evidence is preserved in `runs/vmctl-vc152dng2/`; packed
      player/type tables, complete localized formatting, and full GDI/game
      integration remain open.
- [x] `DrawNewGame3` native checkpoint completed in fresh `C:\DNG3`: VC++ 1.52c
      reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `DRAW_NEW_GAME3_CHECKS=2 FAILURES=0 RESULT=PASS` without a runtime error
      dialog. Evidence is preserved in `runs/vmctl-vc152dng3/`; exact
      race-creation tables, localized formatting, packed global rectangles,
      and full GDI/game integration remain open.
- [x] `SimpleNewGameDlg` native checkpoint completed in fresh `C:\SNGD`: VC++
      1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `SIMPLE_NEW_GAME_DLG_CHECKS=5 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in `runs/vmctl-vc152sngd/`; packed
      player records, localized resources, exact frame geometry, and full
      Win16 control integration remain open.
- [x] `NewGameDlg` native checkpoint completed in fresh `C:\NGD`: VC++ 1.52c
      reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `NEW_GAME_DLG_CHECKS=22 FAILURES=0 RESULT=PASS` without a runtime error
      dialog. Evidence is preserved in `runs/vmctl-vc152ngd/`; the initial
      unresolved-`_printf` linker failure is retained as `NGDLINK.LOG`, while
      `NGDLINK2.LOG` records the corrected no-CRT-safe harness. Packed game
      records, exact race-button tables, localized resources, and full Win16
      control integration remain open.
- [x] `IDropPart` native checkpoint completed in fresh `C:\IDP`: VC++ 1.52c
      reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `I_DROP_PART_CHECKS=26 FAILURES=0 RESULT=PASS` without a runtime error
      dialog. Evidence is preserved in `runs/vmctl-vc152idp/`; exact packed
      hull-slot layout, Win16 hit testing, and full build-dialog integration
      remain open.
- [x] `IDropPart` packed integration checkpoint completed in fresh
      `C:\IDPINT`: VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the
      Win98 packed adapter reports
      `I_DROP_PART_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS` without a
      Program Error dialog. Evidence is preserved in
      `analysis/compat/i-drop-part-integration.md` and
      `runs/vmctl-vc152idpint/`; exact Win16 slot hit testing and drag
      initiation, build-dialog redraw/control integration, and complete
      original-executable drop behavior remain open. The row is now 5/6, not
      promoted.
- [x] `FillBuildDD` native checkpoint completed in fresh `C:\FBD`: VC++ 1.52c
      reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `FILL_BUILD_DD_CHECKS=8 FAILURES=0 RESULT=PASS` without a runtime error
      dialog. Evidence is preserved in `runs/vmctl-vc152fbd/`; the native
      compile log retains one non-fatal signed/unsigned comparison warning.
      Exact packed design lookup, player-name formatting, queue records, and
      full build-dialog integration remain open.
- [x] `PlanetWndProc` native checkpoint completed in fresh `C:\PWP`: VC++ 1.52c
      reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `PLANET_WND_PROC_CHECKS=43 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in `runs/vmctl-vc152pwp/`; exact
      child layout, packed transfer lookup, resources, edit subclass, and
      full drawing/focus integration remain open.
- [x] `DrawPlanetStats` native checkpoint completed in fresh `C:\DPS`: VC++
      1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe reports
      `DRAW_PLANET_STATS_CHECKS=22 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in `runs/vmctl-vc152dps/`; packed
      planet/race lookups, localization, fixed-point formatting, and full GDI
      integration remain open.
- [x] `DrawPlanetStats` packed-record integration checkpoint completed in fresh
      `C:\DPSINT2`: expanded strict host C89 and native VC++ 1.52c each report
      `DRAW_PLANET_STATS_PACKED_CHECKS=15 FAILURES=0 RESULT=PASS` over the
      recovered 56-byte `_planet` status, scanner/defense, and mineral-rate
      fields. The decoded data reaches all seven value rows, including hidden
      tile suppression, references, separators, alignment, a zero mineral-rate
      boundary, and maximum scanner/defense bitfields. Compile and link passed,
      the process exited 0, and no Program Error dialog appeared. The procedure
      remains at 5/6 pending complete packed planet/race lookup, localized and
      fixed-point formatting, exact scanner/defense services, detailed GDI
      operations, original differential coverage, and final review. Evidence
      is preserved in
      `analysis/compat/draw-planet-stats-packed-integration.{md,json}` and
      `runs/vmctl-vc152dpsint2/`.
- [x] `DrawPlanetStarbase` native checkpoint completed in fresh `C:\DPSBN`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DRAW_PLANET_STARBASE_CHECKS=14 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `runs/vmctl-vc152dpsn/`; packed planet/starbase records, localized
      resources, and full GDI integration remain open.
- [x] `DrawPlanetStarbase` packed-record checkpoint completed in fresh
      `C:\DSBINT`: strict host and native adapters each passed
      `DRAW_PLANET_STARBASE_PACKED_CHECKS=17 FAILURES=0 RESULT=PASS` over the
      recovered 56-byte `_planet` identity, `fStarbase` bit, packed starbase
      word, populated detail/gauge paths, no-warp cleanup, and empty-starbase
      sentinels. Compile and link passed, the process exited 0, and no Program
      Error dialog appeared. Evidence is preserved in
      `analysis/compat/draw-planet-starbase-packed-integration.{md,json}` and
      `runs/vmctl-vc152dsbint/`; the procedure remains at 5/6 pending
      localized resources, exact design/shield/warp services, detailed GDI
      integration, original differential coverage, and final review.
- [x] `DrawPlanetProduction` native checkpoint completed in fresh `C:\DPROD`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DRAW_PLANET_PRODUCTION_CHECKS=9 FAILURES=0 RESULT=PASS` without
      a runtime error dialog. Evidence is preserved in
      `runs/vmctl-vc152dpro/`; packed planet/production records, localized ETA
      formatting, exact Win16 geometry, and full GDI integration remain open.
- [x] `DrawPlanetProduction` packed-record checkpoint completed in fresh
      `C:\DPINT`: strict host and native adapters each passed
      `DRAW_PLANET_PRODUCTION_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS` over the
      recovered 56-byte `_planet` identity, `lpplprod` pointer, 4-byte
      `PLPROD` header, ETA/name rows, queue action enablement, and empty
      cleanup path. Compile and link passed, the process exited 0, and no
      Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/draw-planet-production-packed-integration.{md,json}`
      and `runs/vmctl-vc152dpint/`; the procedure remains at 5/6 pending
      localized ETA formatting, exact Win16 geometry, full GDI/control
      integration, original differential coverage, and final review.
- [x] `DrawPlanShipBitmap` native checkpoint completed in fresh `C:\DPSBM2`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DRAW_PLAN_SHIP_BITMAP_CHECKS=9 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `runs/vmctl-vc152dpsm/`; exact Win16 GDI handles, packed selection state,
      localized names, and full planet integration remain open.
- [x] `DrawPlanShipBitmap` packed-selection checkpoint completed in fresh
      `C:\DPSBINT`: strict host and native adapters each passed
      `DRAW_PLAN_SHIP_BITMAP_PACKED_CHECKS=17 FAILURES=0 RESULT=PASS` over the
      recovered 226-byte `_sel` record, fleet and planet subrecords,
      fleet/planet dispatch, planet DIB source coordinates, and button
      cleanup/grid paths. Compile and link passed, the process exited 0, and
      no Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/draw-plan-ship-bitmap-packed-integration.{md,json}`
      and `runs/vmctl-vc152dpsbint/`; the procedure remains at 5/6 pending
      exact GDI handles, complete selection integration, localized names,
      original differential coverage, and final review.
- [x] `DrawPlanetShipList` native checkpoint completed in fresh `C:\DPSL`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DRAW_PLANET_SHIP_LIST_CHECKS=19 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `runs/vmctl-vc152dpsl/`; packed fleet/thing transfer records, exact
      Win16 control handles, localized text/gauges, and full drawing
      integration remain open.
- [x] `DrawPlanetShipList` packed-transfer checkpoint completed in fresh
      `C:\DLIST`: strict host and native adapters each passed
      `DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS` over the
      recovered 226-byte `_sel` planet view, 128-byte `_xfer` identity/kind/
      owner fields, fleet and thing-mode gating, and no-object visibility.
      Compile and link passed, the process exited 0, and no Program Error
      dialog appeared. Evidence is preserved in
      `analysis/compat/draw-planet-ship-list-packed-integration.{md,json}`
      and `runs/vmctl-vc152dlistint/`; the procedure remains at 5/6 pending
      complete transfer behavior, exact Win16 controls and geometry, localized
      text/gauges, detailed drawing, original differential coverage, and final
      review.
- [x] Expanded `DrawPlanetShipList`'s combo seam to preserve the raw cached
      width and `SetWindowPos` flags. The strict C89 probe passes
      `DRAW_PLANET_SHIP_LIST_COMBO_CHECKS=4 FAILURES=0 RESULT=PASS` for new
      width `0x14` and repeated width `0x15` behavior. This is host-only
      source-fidelity evidence; native control flags and complete transfer/
      drawing integration remain open, so no authoritative Step-5 or promotion
      count changes. Evidence is in
      `analysis/compat/draw-planet-ship-list-combo-e2.{md,json}`.
- [x] `TransferStuff` native checkpoint completed in fresh `C:\TRNS`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `TRANSFER_STUFF_CHECKS=20 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `runs/vmctl-vc152trn/`; packed fleet/planet/thing records, exact dialog
      behavior, cargo balancing integration, and Win16 lookup/refresh calls
      remain open.
- [x] `TransferStuff` packed-record checkpoint completed in fresh `C:\TRINT`:
      strict host and native adapters each passed
      `TRANSFER_STUFF_PACKED_CHECKS=14 FAILURES=0 RESULT=PASS` over the
      recovered 128-byte `_xfer` object identity/kind/fleet-owner fields,
      fleet cargo presence, split/cancel cleanup, mode-0 jettison, valid-hull
      enumeration, balance, scanner invalidation, refresh, and mode reset.
      Compile and link passed, the process exited 0, and no Program Error
      dialog appeared. Evidence is preserved in
      `analysis/compat/transfer-stuff-packed-integration.{md,json}` and
      `runs/vmctl-vc152trint/`; the procedure remains at 5/6 pending complete
      packed mutation, exact dialog behavior, cargo-balance differentials,
      Win16 lookup/refresh integration, original differential coverage, and
      final review.
- [x] `FTrackXfer` native checkpoint completed in fresh `C:\FTX`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `F_TRACK_XFER_CHECKS=24 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `runs/vmctl-vc152ftx/`; packed transfer records, Win16 capture behavior,
      cargo/supply mutation, and full integration remain open.
- [x] `FTrackXfer` packed-record checkpoint completed in fresh `C:\FTXINT`:
      strict host and native adapters each passed
      `F_TRACK_XFER_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS` over the
      recovered 128-byte `_xfer` identity/type, fleet owner and cargo fields,
      and overlapping `_thing` data. The checkpoint exercised owner gating,
      capture/release, non-zero-origin drag proportion, signed supply,
      bounded dialog-mode transfer, and mutation of both packed cargo
      records. Compile and link passed, the process exited 0, and no Program
      Error dialog appeared. Evidence is preserved in
      `analysis/compat/f-track-xfer-packed-integration.{md,json}` and
      `runs/vmctl-vc152ftxint/`; the procedure remains at 5/6 pending exact
      original `XferSupply`/`ChgCargo` semantics, complete lookup and Win16
      capture integration, original differential coverage, and final review.
- [x] `EstFuelUse` native checkpoint completed in fresh `C:\EFU`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `EST_FUEL_USE_CHECKS=6 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `runs/vmctl-vc152efu/`; exact packed design/component integration and
      full native differential coverage remain open.
- [x] Packed/integration checkpoint completed for `EstFuelUse`: the recovered
      fleet, ship-design, hull-definition, and engine-record offsets bind to
      the typed source in a fresh native harness with two checks passing. The
      original Fleet Summary Report and Fuel & Cargo surface showed per-fleet
      fuel values without a program-error dialog. Evidence is preserved in
      `analysis/compat/est-fuel-use-integration.{md,json}`,
      `tests/est_fuel_use_integration_probe.c`,
      `compat/vc152/est-fuel-use-integration/`, and
      `runs/vmctl-vc152efuint/`. `EstFuelUse` is now at step 5/6; broader
      packed-table and route-distance differential coverage remains open.
- [x] Expanded packed-table checkpoint completed for `EstFuelUse` in fresh
      `C:\EFUINT2`: strict host and native adapters each passed
      `EST_FUEL_USE_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`, covering
      engine ordering, race discount, Ram Scoop, fixed-point cost, no-cargo
      range sentinel, warp 10, invalid-warp flag clearing, order-derived
      distance, large intermediates, absent-engine fallback, range conversion,
      and raw table sizes/mapping. Compile and link passed, the process exited
      0, and no Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/est-fuel-use-integration.{md,json}` and
      `runs/vmctl-vc152efuint2/`; `EstFuelUse` remains at 5/6 pending complete
      original-versus-reconstructed packed-table and route-distance
      differential coverage and final review.
- [x] `LFuelUseToWaypoint` native checkpoint completed in fresh `C:\FUT2`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `FUEL_USE_TO_WAYPOINT_CHECKS=4 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/fuel-use-to-waypoint-native.md` and
      `runs/vmctl-vc152fut/`; exact packed hull/planet records, original fuel
      cases, and full ship integration remain open.
- [x] Packed/integration checkpoint completed for `LFuelUseToWaypoint`: the
      recovered fleet, 18-byte order, planet status, hull cargo-capacity, and
      ship-design hull fields bind to the typed source in a fresh native
      harness with two checks passing. The original Fleet Summary Report and
      Fuel & Cargo surface showed fleet fuel and cargo values without a
      program-error dialog. Evidence is preserved in
      `analysis/compat/fuel-use-to-waypoint-integration.{md,json}`,
      `tests/fuel_use_to_waypoint_integration_probe.c`,
      `compat/vc152/fuel-use-to-waypoint-integration/`, and
      `runs/vmctl-vc152futint/`. `LFuelUseToWaypoint` is now at step 5/6;
      broader route-sequence differential coverage remains open.
- [x] Expanded route-sequence checkpoint completed for `LFuelUseToWaypoint`
      in fresh `C:\FUTINT2`: strict host and native adapters each passed
      `FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=11 FAILURES=0 RESULT=PASS`,
      covering multi-segment accumulation, invalid warp, owner-mismatch versus
      owned-planet reset, ram-scoop adjustment, varying route distances, and
      preservation of three packed 18-byte order records plus fleet, planet,
      hull, and design fields. Compile and link passed, the process exited 0,
      and no Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/fuel-use-to-waypoint-integration.{md,json}` and
      `runs/vmctl-vc152futint2/`; `LFuelUseToWaypoint` remains at 5/6 pending
      complete original route-sequence differential coverage, full ship
      integration, and final review.
- [x] `ZipOrderDlg` native checkpoint completed in fresh `C:\ZOD`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `ZIP_ORDER_DLG_CHECKS=10 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/zip-order-dlg-native.md` and
      `runs/vmctl-vc152zod/`; packed order storage, localized resources, and
      full rename-dialog integration remain open.
- [x] `ZipOrderDlg` packed-record checkpoint completed in fresh `C:\ZODINT`:
      strict host and native adapters each passed
      `ZIP_ORDER_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS` over four
      recovered 24-byte `_ziporder` records, including five order words,
      names, and validity bytes. The checkpoint covered initialization,
      selection, Create copying, name/ampersand handling, Delete cleanup,
      Rename persistence, and Cancel closure. Compile and link passed, the
      process exited 0, and no Program Error dialog appeared. Evidence is
      preserved in
      `analysis/compat/zip-order-dlg-packed-integration.{md,json}` and
      `runs/vmctl-vc152zodint/`; `ZipOrderDlg` remains at 5/6 pending
      localized resources, exact original dialog behavior, complete Win16
      integration, original differential coverage, and final review.
- [x] Original dialog boundary evidence completed for `TransferStuff` and
      `ZipOrderDlg` in the running `StarDev` Win98 game. `Cargo Transfer`
      opened for `Stalwart Defender #5` and `Oop Be Gone`, showing fuel,
      cargo, and per-mineral transfer controls; the dialog was canceled without
      committing a game-state change. `Rename Fleet` opened for
      `Stalwart Defender #5`; its cancel path was exercised and the original
      name was preserved. Evidence is preserved in
      `analysis/compat/transfer-stuff-integration.{md,json}` and
      `analysis/compat/zip-order-integration.{md,json}` with screenshots under
      `runs/vmctl-vc152current/`. Both procedures are now at step 5/6; packed
      records, localized resources, and complete Win16 integration remain open.
- [x] Original transfer-control evidence completed for `FTrackXfer` in the
      running `StarDev` Win98 game. The original `Cargo Transfer` dialog stayed
      open while an Ironium transfer control was activated, then Cancel
      returned to the game without committing a state change or showing a
      program-error dialog. Evidence is preserved in
      `analysis/compat/f-track-xfer-integration.{md,json}` and
      `runs/vmctl-vc152current/f-track-xfer.bmp`. `FTrackXfer` is now at step
      5/6; packed transfer records, exact cargo/supply mutation, and complete
      Win16 capture integration remain open.
- [x] `FStargateJump` native checkpoint completed in fresh `C:\SGJ`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `F_STARGATE_JUMP_CHECKS=4 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/f-stargate-jump-native.md` and
      `runs/vmctl-vc152sgj/`; packed fleet layout, full game service tables,
      original damage cases, and cargo integration remain open.
- [x] Packed/integration checkpoint completed for `FStargateJump`: the
      recovered fleet identity/flags/planet, cargo-count and cargo-state
      words, and ship-design mass/weight fields bind to the typed source in a
      fresh native harness with two checks passing. The abort message envelope
      and surviving-cargo mutation both round-trip without a program-error
      dialog. Evidence is preserved in
      `analysis/compat/f-stargate-jump-integration.{md,json}`,
      `tests/f_stargate_jump_integration_probe.c`,
      `compat/vc152/f-stargate-jump-integration/`, and
      `runs/vmctl-vc152sgjint/`. `FStargateJump` is now at step 5/6; full
      damage-service differential coverage and complete game-service cargo
      integration remain open.
- [x] Expanded the `FStargateJump` packed/integration checkpoint in fresh
      `C:\SGJINT2`: seven native checks now cover damage failure, inactive and
      total-loss empty-fleet paths, surviving-cargo state and loss-message
      write-back, the race-stat loss exemption, and multi-slot cargo with
      independent design weights. VC1.52c compile/link passed, the Win98
      report was `F_STARGATE_JUMP_INTEGRATION_CHECKS=7 FAILURES=0 RESULT=PASS`,
      and no Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/f-stargate-jump-integration.{md,json}`,
      `analysis/reconstruction/f-stargate-jump.md`,
      `tests/f_stargate_jump_integration_probe.c`,
      `compat/vc152/f-stargate-jump-integration/`, and
      `runs/vmctl-vc152sgjint2/`. This strengthens step 5/6; full original
      damage-service differential coverage and complete game-service cargo
      integration remain open.
- [x] Expanded `FStargateJump` again in fresh `C:\SGJINT3`: the strict host
      and VC++ 1.52c/Win98 matrix now passes 10 checks, including exact
      damage-service source/destination/distance/design-mass arguments, zero
      cargo-damage state, destination transfer fields, and a 32-bit large-loss
      message amount across sixteen cargo slots. Compile and link passed, the
      Win98 report was
      `F_STARGATE_JUMP_INTEGRATION_CHECKS=10 FAILURES=0 RESULT=PASS`, and the
      final window list contained no Program Error dialog. The row remains at
      5/6: full original damage-service differential and real game-service
      cargo comparison remain open.
- [x] Closed the `FStargateJump` E1 in fresh `C:\SGJINT4`: the strict host and
      VC++ 1.52c/Win98 matrix passes
      `F_STARGATE_JUMP_INTEGRATION_CHECKS=2310 FAILURES=0 RESULT=PASS`, adding
      an independent original `MdCalcStargateDamage` differential over eight
      source/destination armor records and distance/defense-weight thresholds.
      Compile and link passed, the final process exited 0, and no Program
      Error dialog appeared. The E1 unit is closed; the row remains at 5/6
      with only E2, the real game-service cargo comparison, open. Evidence is
      preserved in `analysis/compat/f-stargate-jump-integration.{md,json}`,
      `analysis/reconstruction/f-stargate-jump.md`, and
      `runs/vmctl-vc152sgjint4/`.
- [x] `KillUsedWaypoints` native checkpoint completed in fresh `C:\KUW`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `KILL_USED_WAYPOINTS_CHECKS=6 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/kill-used-waypoints-native.md` and
      `runs/vmctl-vc152kuw/`; exact packed PLORD offsets, localized messages,
      and full fleet integration remain open.
- [x] Packed/integration checkpoint completed for `KillUsedWaypoints`: the
      recovered `_fleet` fields and four-byte PLORD header plus 18-byte order
      records bind to the typed source in a fresh native harness with two
      checks passing. The deleted-fleet message envelope, flagged status word,
      stale-fleet retargeting, and waypoint deletion/compaction all pass
      without a program-error dialog. Evidence is preserved in
      `analysis/compat/kill-used-waypoints-integration.{md,json}`,
      `tests/kill_used_waypoints_integration_probe.c`,
      `compat/vc152/kill-used-waypoints-integration/`, and
      `runs/vmctl-vc152kuwint/`. `KillUsedWaypoints` is now at step 5/6;
      localized resources, live fleet-table enumeration, and full turn
      integration remain open.
- [x] Expanded the `KillUsedWaypoints` packed/integration checkpoint in fresh
      `C:\KUWINT2`: nine native checks now cover deleted-fleet and
      empty-waypoint messages, missing-target invalidation, no-move
      preservation, both recycle decisions, location-triggered compaction
      with planet retargeting, and two-fleet enumeration. VC1.52c compile/link
      passed, the Win98 report was
      `KILL_USED_WAYPOINTS_INTEGRATION_CHECKS=9 FAILURES=0 RESULT=PASS`, and no
      Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/kill-used-waypoints-integration.{md,json}`,
      `analysis/reconstruction/kill-used-waypoints.md`,
      `tests/kill_used_waypoints_integration_probe.c`,
      `compat/vc152/kill-used-waypoints-integration/`, and
      `runs/vmctl-vc152kuwint2/`. This strengthens step 5/6; localized
      resources, live fleet-table enumeration, and full turn integration
      remain open.
- [x] `AutoRouteFleet` native checkpoint completed in fresh `C:\ARF`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `AUTO_ROUTE_FLEET_CHECKS=4 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/auto-route-fleet-native.md` and
      `runs/vmctl-vc152arf/`; exact packed order/resource offsets, original
      route cases, and full fleet integration remain open.
- [x] `DrawMineSurvey` native checkpoint completed in fresh `C:\DMS`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DRAW_MINE_SURVEY_CHECKS=5 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/draw-mine-survey-native.md` and
      `runs/vmctl-vc152dms/`; detailed GDI layout, packed records, localized
      formatting, and full mine-survey integration remain open.
- [x] Expanded the `DrawMineSurvey` native checkpoint in a fresh `C:\DMS2`:
      the repaired package includes the exact current `DMS.C` source and a
      directory-first VC1.52c wrapper, and nine native checks now cover shallow
      and unknown-owner planet summaries, non-wormhole thing decoding, invalid
      thing-selection suppression, and the null-operations guard in addition
      to the existing dispatch, ship-total, lookup, drawing-state, and packed
      field cases. VC1.52c compile/link passed, the Win98 report was
      `DRAW_MINE_SURVEY_CHECKS=9 FAILURES=0 RESULT=PASS`, redirected console
      output was empty, and no Program Error dialog appeared. Evidence is
      preserved in `analysis/compat/draw-mine-survey-native.md`,
      `analysis/reconstruction/draw-mine-survey.md`,
      `tests/draw_mine_survey_source_probe.c`,
      `compat/vc152/draw-mine-survey/`, and
      `runs/vmctl-vc152dms2/`. This strengthens step 5/6; detailed GDI
      layout, packed records/resources, localization, and complete integration
      remain open.
- [x] `MineClick` native checkpoint completed in fresh `C:\MCL`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `MINE_CLICK_CHECKS=9 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/mine-click-native.md` and `runs/vmctl-vc152mcl/`;
      packed GlobalPD/minefield records, localized popup construction, and
      full minefield integration remain open.
- [x] Repaired and reran the `MineClick` native package in fresh `C:\MCL2`:
      the exact current `MCL.C` source, directory-first wrapper, and Win32
      stub now compile and link under VC1.52c; the retained matrix
      reports `MINE_CLICK_CHECKS=9 FAILURES=0 RESULT=PASS`, redirected console
      output is empty, and no Program Error dialog appears. Evidence is
      preserved in `analysis/compat/mine-click-native.md`,
      `analysis/reconstruction/mine-click.md`,
      `compat/vc152/mine-click/`, and `runs/vmctl-vc152mcl2/`. This strengthens
      step 5/6; packed GlobalPD/minefield records, localization, derived
      mining arithmetic, and complete minefield integration remain open.
- [x] Synchronized the maintained `MineClick` probe into fresh `C:\MCL3`:
      host and native now exercise the same 25-case source matrix. VC1.52c
      compile/link passed, the Win98 report was
      `MINE_CLICK_CHECKS=25 FAILURES=0 RESULT=PASS`, redirected console output
      was empty, and no Program Error dialog appeared. Evidence is preserved
      in `analysis/compat/mine-click-native.md`,
      `analysis/reconstruction/mine-click.md`,
      `tests/mine_click_source_probe.c`,
      `compat/vc152/mine-click/`, and
      `runs/vmctl-vc152mcl3/`. This strengthens step 5/6; packed
      GlobalPD/minefield records, localization, derived mining arithmetic, and
      complete minefield integration remain open.
- [x] Mine-window caller boundary traced for the next original-behavior pass:
      `DrawMineSurvey` and `MineClick` are reached through the `MineWndProc`
      child window, while the visible toolbar magnifier opens only the zoom
      scale menu. The live probe was non-destructive and produced no program
      error; this established the route before the direct mine-window
      observation below.
- [x] Original mine-window integration checkpoint completed for
      `DrawMineSurvey` and `MineClick`: `View > Window Layout > Small Screen`
      exposed the original `Oop Be Gone` selection-summary child with
      environment and mineral rows; clicking its selection arrow advanced the
      original scan object to `Armed Probe #1` and redrew the dependent survey
      and fleet panes without a program-error dialog. Evidence is preserved in
      `analysis/compat/draw-mine-survey-integration.{md,json}`,
      `analysis/compat/mine-click-integration.{md,json}`,
      `runs/vmctl-vc152current/layout-small.bmp`, and
      `runs/vmctl-vc152current/mine-summary-arrow.bmp`. Both procedures are
      now at step 5/6; packed records, localized popup construction, detailed
      GDI/resource recovery, and complete promotion evidence remain open.
- [x] Original selection-summary integration checkpoint completed for
      `PtDisplayPlanetStateInfo` and `PtDisplayPlanetPopInfo`: the original
      small-layout `Oop Be Gone` pane showed gravity, temperature, radiation,
      and mineral state rows together with `Value: 100%`, `Population: 25,000`,
      and current-report status, without a program-error dialog. Evidence is
      preserved in `analysis/compat/pt-display-planet-state-info-integration.{md,json}`
      and `analysis/compat/pt-display-planet-pop-info-integration.{md,json}`
      with `runs/vmctl-vc152current/layout-small.bmp`. Both procedures are
      now at step 5/6; packed lookup, localization, fixed-point formatting,
      detailed GDI geometry, and complete promotion evidence remain open.
- [x] `EstMineralsMined` native checkpoint completed in fresh `C:\EMM`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `EST_MINERALS_MINED_CHECKS=5 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/est-minerals-mined-native.md` and
      `runs/vmctl-vc152emm/`; packed planet/fleet layout, exact fixed-point
      behavior, and mineral-record mutation remain open.
- [x] Packed/integration checkpoint completed for `EstMineralsMined`: the
      recovered `_planet` player, percentage, concentration, and `rgwtMin`
      offsets round-trip through the reconstructed source in a fresh native
      harness with four checks passing. The original planet surface showed
      the Minerals On Hand rows without a program-error dialog. Evidence is
      preserved in `analysis/compat/est-minerals-mined-integration.{md,json}`,
      `tests/est_minerals_mined_integration_probe.c`,
      `compat/vc152/est-minerals-mined-integration/`, and
      `runs/vmctl-vc152emmint/`. `EstMineralsMined` is now at step 5/6;
      broader mining-turn differential and complete fleet/planet mineral
      mutation remain open.
- [x] Expanded the `EstMineralsMined` packed/integration checkpoint in fresh
      `C:\EMMINT2`: nine native checks now cover invalid mining context,
      operating-mine lookup and efficiency, both random-rounding outcomes,
      concentration-floor handling, special-race fleet recursion and
      consumption, raw `rgwtMin` write-back, and the null-ops guard. VC1.52c
      compile/link passed after restoring the missing Win32 stub, the Win98
      report was
      `EST_MINERALS_MINED_INTEGRATION_CHECKS=9 FAILURES=0 RESULT=PASS`, and no
      Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/est-minerals-mined-integration.{md,json}`,
      `analysis/reconstruction/est-minerals-mined.md`,
      `tests/est_minerals_mined_integration_probe.c`,
      `compat/vc152/est-minerals-mined-integration/`, and
      `runs/vmctl-vc152emmint2/`. This strengthens step 5/6; broader original
      mining-turn differential and complete fleet/planet mineral mutation
      remain open.
- [x] Expanded `EstMineralsMined` again in fresh `C:\EMMINT3`: the strict
      host and VC++ 1.52c/Win98 matrix now passes 10 checks, binding packed
      `_planet` and `_fleet` fields for player/planet ownership, flags,
      coordinate, order-task eligibility, mineral write-back, and the original
      consumed-bit clear. Compile and link passed, the process exited 0, and
      no Program Error dialog appeared. The remaining step-5 concern is the
      broader original populated mining-turn differential; the procedure stays
      at 5/6.
- [x] Closed the remaining `EstMineralsMined` E1 in fresh `C:\EMMINT4`: the
      host and VC++ 1.52c/Win98 matrix passes 13 checks and compares two
      ordinary-race populated turns (efficiency 5/10 and 7/10) plus a
      special-race planet with two mining fleets against the original
      instruction path. It verifies all three outputs, fixed-point weights,
      concentration/percentage mutation, and both consumed fleet orders.
      The source now preserves the original pre-efficiency depletion product
      and percentage-cap reset semantics. `EMISTA-MATRIX.TXT` reports
      `COMPILE=PASS` and `LINK=PASS`; `EMIR-MATRIX.TXT` reports
      `EST_MINERALS_MINED_INTEGRATION_CHECKS=13 FAILURES=0 RESULT=PASS`, and
      the final window list contains no Program Error dialog. The E1 unit and
      Step-6 promotion are recorded in
      `analysis/compat/est-minerals-mined-integration.{md,json}`,
      `analysis/reconstruction/est-minerals-mined.md`, and
      `tracking/reviews.json`; `EstMineralsMined` is now **6/6**.
- [x] Closed the remaining `EstFuelUse` E1 in fresh `C:\EFUINT3`: the strict
      host and VC++ 1.52c/Win98 matrices each pass
      `EST_FUEL_USE_INTEGRATION_CHECKS=29 FAILURES=0 RESULT=PASS`. The matrix
      replays the independent original fuel loop across all packed design and
      hull slots, all warp rows and engine IDs, both race-discount and
      range-only modes, and route distances 1, 37, 1000, and 50000. Compile
      and link passed, the final process exited 0, and no Program Error dialog
      appeared. The E1 unit and Step-6 promotion are recorded in
      `analysis/compat/est-fuel-use-integration.{md,json}`,
      `analysis/reconstruction/est-fuel-use.md`, and
      `tracking/reviews.json`; `EstFuelUse` is now **6/6**. This is
      procedure-level differential evidence, not whole-game turn coverage.
- [x] `ProductionDlg` native checkpoint completed in fresh `C:\PDG`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `PRODUCTION_DLG_CHECKS=9 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/production-dlg-native.md` and
      `runs/vmctl-vc152pdg/`; packed production globals, exact child geometry,
      and full popup/command integration remain open.
- [x] `DrawProductionDlg` native checkpoint completed in fresh `C:\DPD`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DRAW_PRODUCTION_DLG_CHECKS=1 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/draw-production-dlg-native.md` and
      `runs/vmctl-vc152dpd/`; packed production globals, exact formatting and
      resource tables, and full GDI integration remain open.
- [x] `EstimateItemProdSched` native checkpoint completed in fresh `C:\EIPS`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `ESTIMATE_ITEM_PROD_SCHED_CHECKS=4 FAILURES=0 RESULT=PASS` without
      a runtime error dialog. Evidence is preserved in
      `analysis/compat/estimate-item-prod-sched-native.md` and
      `runs/vmctl-vc152eips/`; packed production records, CBuildProdItem
      integration, and original differential cases remain open.
- [x] `ZipProdDlg` native checkpoint completed in fresh `C:\ZPD`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `ZIP_PROD_DLG_CHECKS=21 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/zip-prod-dlg-native.md` and `runs/vmctl-vc152zpd/`;
      packed queue/name records, production bit layout, localized resources,
      rename-dialog behavior, and full Win16 control integration remain open.
- [x] `ResearchDlg` native checkpoint completed in fresh `C:\RSD`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `RESEARCH_DLG_CHECKS=9 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/research-dlg-native.md` and `runs/vmctl-vc152rsd/`;
      packed player storage, localized resources, and full Win16 control
      integration remain open.
- [x] Original wizard-page rendering evidence completed for `DrawNewGame2`
      and `DrawNewGame3`. The original `StarDev` Win98 wizard reached Step 2,
      showing all sixteen player rows, race diamonds, and player labels, then
      Step 3, showing the nine victory criteria rows, paired controls, and
      winner/minimum-year controls. No program-error dialog appeared. Evidence
      is preserved in `analysis/compat/draw-new-game-integration.{md,json}`
      and the original wizard screenshots under
      `runs/vmctl-vc152current/`. Both procedures are now at step 5/6; packed
      tables, localized resources, exact rectangles, and complete GDI
      integration remain open.
- [x] Original planet-window integration evidence completed for `PlanetWndProc`.
      The original `starsframe` displayed the planet summary, environment
      gauges, mineral bars, production queue, starbase values, selected-fleet
      bitmap, fuel/cargo, fleet composition, and waypoints. A map selection
      changed the selected fleet and refreshed the related child panels without
      a program-error dialog. Evidence is preserved in
      `analysis/compat/planet-wnd-proc-integration.{md,json}` and the existing
      planet-frame screenshots under `runs/vmctl-vc152current/`. The procedure
      is now at step 5/6; exact child layout, packed transfer lookup, resource
      handles, and complete drawing/focus integration remain open.
- [x] Original report-row integration evidence completed for
      `ExecuteReportClick`. The original Planet Summary Report displayed its
      one-row planet table, and the captured row-selection path returned with
      the selected planet/frame context visible behind the report. No
      program-error dialog appeared. Evidence is preserved in
      `analysis/compat/execute-report-click-integration.{md,json}` and
      `runs/vmctl-vc152current/report-row-click.bmp`. The procedure is now at
      step 5/6; packed report rows, the GlobalPD union, fleet/battle variants,
      and complete USER/GDI caller integration remain open.
- [x] `BrowserDlg` native checkpoint completed in fresh `C:\BRD`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `BROWSER_DLG_CHECKS=19 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/browser-dlg-native.md` and `runs/vmctl-vc152brd/`;
      packed vpartBrowser records, exact lookup tables, localized storage,
      and full USER control integration remain open.
- [x] `UpdatePlayerScores` native checkpoint completed in fresh `C:\UPS`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `UPDATE_PLAYER_SCORES_CHECKS=34 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/update-player-scores-native.md` and
      `runs/vmctl-vc152ups/`; the packed/integration follow-up is recorded
      below.
- [x] Packed/integration checkpoint completed for `UpdatePlayerScores`: the
      recovered `_scorex`/`_player` offsets, rank/validity/achievement/dead-bit
      writes, and prepended-message headers/lengths pass six host/native
      integration checks. The original `Player Scores` dialog also showed its
      headings and category grid without a Program Error dialog; the loaded
      save had blank values, so this is not claimed as a populated-value
      differential. Evidence is preserved in
      `analysis/compat/update-player-scores-integration.{md,json}`,
      `tests/update_player_scores_integration_probe.c`,
      `runs/vmctl-vc152upsint/`, and
      `runs/vmctl-vc152current/score-report.bmp`. `UpdatePlayerScores` is now
      at step 5/6; broader populated-score and complete turn/report replay
      remain open.
- [x] `SatisfyOrders` native checkpoint completed in fresh `C:\SYO`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `SATISFY_ORDERS_CHECKS=32 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/satisfy-orders-native.md` and
      `runs/vmctl-vc152syo/`; packed order battle-plan accounting, technology
      discovery, fleet/ship-design copying, salvage creation, complete message
      layouts, and full turn integration remain open.
- [x] `SatisfyOrders` packed/integration checkpoint completed in fresh
      `C:\SYINT`: the recovered 124-byte `_fleet` and 18-byte `_order` views
      were bound to the typed resource-transfer core. Host and native reports
      both returned
      `SATISFY_ORDERS_INTEGRATION_CHECKS=3 FAILURES=0 RESULT=PASS`; compile
      and link passed, and the post-run Win98 window list showed no Program
      Error. The original Fleet Orders surface is preserved in
      `runs/vmctl-vc152current/orders-before.bmp` as the consumer boundary.
      Battle-plan/resource accounting, remaining special-order branches,
      complete message layouts, and full turn replay remain open before
      promotion.
- [x] `FAttack` native checkpoint completed in fresh `C:\FA`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `F_ATTACK_CHECKS=33 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/f-attack-native.md` and `runs/vmctl-vc152fat/`; packed
      hull/part lookup, exact battle-part scaling, damage-engine integration,
      and full battle-record integration remain open.
- [x] `FAttack` packed/integration checkpoint completed in fresh `C:\FAINT`:
      the recovered 29-byte `_tok`, 123-byte `_hul`, and six-byte battle
      record views were bound to the reconstructed `FAttack` and `FDamageTok`.
      Host and native reports both returned
      `F_ATTACK_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`; compile and link
      passed, and the post-run Win98 window list showed no Program Error. The
      original battle-report surface is preserved in
      `runs/vmctl-vc152current/report-battles.bmp` as the consumer boundary.
      Exact packed part lookup, complete battle-part scaling, broader damage
      differentials, and full battle replay remain open before promotion.
- [x] `FDoCoolBattle` native checkpoint completed in fresh `C:\CB`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `F_DO_COOL_BATTLE_CHECKS=53 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/f-do-cool-battle-native.md` and
      `runs/vmctl-vc152fcb/`; packed token/global mapping, exact damage,
      full battle-log integration, and remaining record cases remain open.
- [x] `FDoCoolBattle` packed-log/original checkpoint recorded from the fresh
      `C:\CB` run: the existing strict-C89/native harness checks the raw
      14-byte battle header, six-byte movement/attack records, append-at-end
      staging, per-battle serialized length, and the `0xffff` overflow marker.
      It reports `F_DO_COOL_BATTLE_CHECKS=53 FAILURES=0 RESULT=PASS` with
      compile and link passing and no Win98 Program Error dialog. The original
      battle-report surface is preserved in
      `runs/vmctl-vc152current/report-battles.bmp`. Packed token/global
      mapping, exact movement/damage, remaining records, and full battle
      replay remain open before promotion.
- [x] Expanded `FDoCoolBattle` staged-log validation in fresh `C:\CB2`:
      the strict host/native matrix now passes 58 checks, including exact
      preservation of a five-byte existing log prefix while the current
      50-byte battle record is copied after it; the reported per-battle length
      remains 50 bytes. VC1.52c compile/link passed and the Win98 run reported
      `F_DO_COOL_BATTLE_CHECKS=58 FAILURES=0 RESULT=PASS` with no Program Error
      dialog. Packed token/global mapping, exact movement and damage,
      remaining record cases, full battle replay, and promotion remain open.
- [x] `DoRobotoidAiTurn` native checkpoint completed in fresh `C:\ROB`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `ROBOTOID_AI_TURN_CHECKS=23 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/do-robotoid-ai-turn-native.md` and
      `runs/vmctl-vc152rot/`; fleet-order repair, colonizer/transport routing,
      and full AI-table integration remain open.
- [x] `DoTurinDroneAiTurn` native checkpoint completed in fresh `C:\TUR`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `TURIN_DRONE_AI_TURN_CHECKS=35 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/do-turin-drone-ai-turn-native.md` and
      `runs/vmctl-vc152tur/`; detailed production policy, fleet-order repair,
      colonizer/transport routing, and full AI-table integration remain open.
- [x] `DoAutomitronAiTurn` native checkpoint completed in fresh `C:\AUTO`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `AUTOMITRON_AI_TURN_CHECKS=31 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/do-automitron-ai-turn-native.md` and
      `runs/vmctl-vc152auto/`; colonizer/mineral fleet handling, order repair,
      packed integration, and original-behavior evidence remain open.
- [x] `DoCyberAiTurn` native checkpoint completed in fresh `C:\CYB`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `CYBER_AI_TURN_CHECKS=32 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/do-cyber-ai-turn-native.md` and
      `runs/vmctl-vc152cyber/`; packet routing, freighter behavior, fleet-order
      repair, packed integration, and original-behavior evidence remain open.
- [x] `DoCyberAiTurn` packed integration checkpoint completed in fresh
      `C:\CYBINT`: VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the
      Win98 packed adapter reports
      `CYBER_AI_TURN_INTEGRATION_CHECKS=25 FAILURES=0 RESULT=PASS` without a
      Program Error dialog. Evidence is preserved in
      `analysis/compat/do-cyber-ai-turn-integration.md` and
      `runs/vmctl-vc152cybint/`; packet routing, freighter behavior,
      detailed fleet-order repair, direct packed PLORD mutation, complete
      AI-table integration, and original-behavior evidence remain open. The
      row is now 5/6, not promoted.
- [x] `ReportDlg` native checkpoint completed in fresh `C:\REP`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `REPORT_DLG_CHECKS=26 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/report-dlg-native.md` and `runs/vmctl-vc152report/`;
      packed report records, menu resources, USER/GDI integration, and caller
      integration remain open.
- [x] `ExecuteReportClick` native checkpoint completed in fresh `C:\EXE`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `EXECUTE_REPORT_CLICK_CHECKS=21 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/execute-report-click-native.md` and
      `runs/vmctl-vc152execute/`; packed report rows, the GlobalPD union,
      USER/GDI integration, and caller integration remain open.
- [x] `ExecuteReportClick` packed-record adapter completed its host checkpoint:
      24 strict C89 checks pass over the recovered 124-byte `_fleet`, 56-byte
      `_planet`, 14-byte `_btldata`, and 22-byte `_popupdata` layouts, including
      fleet, installation, mineral, and resource popup offsets. The first two
      native attempts exposed an omitted linker definitions-file line; after
      correction, fresh `C:\EXINT3` and `C:\EXLAY3` runs compiled and linked,
      passed 24 and 16 native checks respectively, and showed no Program Error
      dialog. The procedure remains at 5/6 pending exact GlobalPD owner/part
      cases, live USER/GDI integration, complete caller integration, and final
      promotion.
- [x] Expanded `ExecuteReportClick` packed/native coverage in fresh
      `C:\EXINT4`: the strict host and VC++ 1.52c/Win98 matrix now passes 34
      checks over the existing packed records, adding starbase and planet
      popup unions, the defense-part payload, the resource-exempt bitfield,
      and production/transfer modal guards. Compile and link passed, the
      process exited 0, and the post-run window list had no Program Error
      dialog. Exact original GlobalPD owner/part pointer cases, live USER/GDI
      behavior, complete caller/layout integration, and promotion remain open;
      `ExecuteReportClick` stays at 5/6.
- [x] `DumpPlanets` native checkpoint completed in fresh `C:\DUMP`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DUMP_PLANETS_CHECKS=16 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/dump-planets-native.md` and
      `runs/vmctl-vc152dumpplanets/`; localized formatting, packed integration,
      and original-behavior evidence remain open.
- [x] `DumpPlanets` packed-record integration checkpoint completed in fresh
      `C:\DPINT`: strict host C89 and native VC++ 1.52c each report
      `DUMP_PLANETS_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS` over the recovered
      56-byte `_planet` fields, detailed row dispatch, visibility, turn age,
      environment/mineral arrays, starbase, and wormhole values. No Program
      Error dialog appeared. The procedure remains at 5/6 pending localized
      formatting, full starbase/wormhole/production services, complete
      original differential behavior, and final promotion.
- [x] Expanded `DumpPlanets` packed validation in fresh `C:\DMPINT2`: strict
      host C89 and VC++ 1.52c/Win98 now report
      `DUMP_PLANETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`. The checkpoint
      covers all three environment/original-environment slots, all mineral
      concentration/rate slots, scanner detail, and the complete starbase,
      wormhole, and derived report-row fields. The procedure remains at 5/6
      pending localized formatting, full service callbacks, original report
      differential coverage, and promotion.
- [x] `DumpFleets` native checkpoint completed in fresh `C:\DF`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DUMP_FLEETS_CHECKS=10 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/dump-fleets-native.md` and
      `runs/vmctl-vc152dumpfleets/`; localized formatting, packed integration,
      and original-behavior evidence remain open.
- [x] `DumpFleets` packed-record integration checkpoint completed in fresh
      `C:\DFINT`: strict host C89 and native VC++ 1.52c each report
      `DUMP_FLEETS_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS` over the recovered
      124-byte `_fleet` layout, ship-class mapping, detailed fields,
      destination visibility, and write-failure path. The native build and
      link passed, the process exited 0, and no Program Error dialog appeared.
      The procedure remains at 5/6 pending localized formatting, exact
      scanner/minefield/battle-plan/task services, complete original
      differential output, and final promotion. Evidence is preserved in
      `analysis/compat/dump-fleets-packed-integration.{md,json}` and
      `runs/vmctl-vc152dfint/`.
- [x] Expanded `DumpFleets` packed validation in fresh `C:\DMPFINT2`: strict
      host C89 and VC++ 1.52c/Win98 now report
      `DUMP_FLEETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`. The checkpoint
      covers every cargo slot, all detailed scanner/cloak/mine/terraform
      fields, normal-mode suppression, and the destination-requires-detail
      gate. The procedure remains at 5/6 pending localized formatting, exact
      task/scanner/minefield/battle-plan services, original differential
      output, and promotion.
- [x] `DrawPopup` native checkpoint completed in fresh `C:\POP`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `DRAW_POPUP_CHECKS=51 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/draw-popup-native.md` and
      `runs/vmctl-vc152drawpopup/`; packed records, localization, detailed GDI
      integration, and original-behavior evidence remain open.
- [x] `DrawPopup` packed-record integration checkpoint completed in fresh
      `C:\DRINT`: strict host C89 and native VC++ 1.52c each report
      `DRAW_POPUP_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS` over the recovered
      22-byte `_popupdata`, 124-byte `_fleet`, and 56-byte `_planet` views. The
      selector-derived fleet, planet-state, slot-hull, and default dispatch
      paths, slot geometry, starbase mode, and GDI-state restoration passed.
      Compile and link passed, the process exited 0, and no Program Error
      dialog appeared. Two earlier probe-only reports are retained as
      diagnostic history because the probe initially asserted 26 instead of
      25 checks. The procedure remains at 5/6 pending complete `GlobalPD`
      union coverage, localization, hull filtering, formatting, detailed GDI
      text rendering, and final caller review. Evidence is preserved in
      `analysis/compat/draw-popup-packed-integration.{md,json}` and
      `runs/vmctl-vc152drint/`.
- [x] Expanded `DrawPopup` dispatcher-kind validation in fresh `C:\DRINT2`:
      the strict host/native matrix now passes 35 checks over all remaining
      popup callbacks, including the kind-14 slot-hull alias and GDI-state
      restoration after every dispatch. VC1.52c compile/link passed and the
      Win98 report was `DRAW_POPUP_PACKED_CHECKS=35 FAILURES=0 RESULT=PASS`
      with no Program Error dialog. Packed `GlobalPD` payloads beyond the
      dispatcher, localization, hull filtering/formatting, detailed GDI text,
      caller differential coverage, and promotion remain open.
- [x] `Popup` native checkpoint completed in fresh `C:\PUP`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `POPUP_CHECKS=31 FAILURES=0 RESULT=PASS` without a runtime error
      dialog. Evidence is preserved in
      `analysis/compat/popup-native.md` and `runs/vmctl-vc152popup/`; localized
      measurement, packed records, detailed content layout, and original-
      behavior evidence remain open.
- [x] `Popup` packed-record integration checkpoint completed in fresh
      `C:\PUINT`: strict host C89 and native VC++ 1.52c each report
      `POPUP_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS` over the recovered
      22-byte `_popupdata` selector, planet, and slot fields. The native build
      and link passed, the process exited 0, and no Program Error dialog
      appeared. Two earlier probe-only failures are retained as diagnostic
      history: one corrected the expected screen-origin clamp and one corrected
      the probe's self-count from 21 to 22. The procedure remains at 5/6
      pending localized measurement, full packed fleet/planet/hull payload
      consumption, detailed content layout, and final caller review. Evidence
      is preserved in `analysis/compat/popup-packed-integration.{md,json}` and
      `runs/vmctl-vc152puint/`.
- [x] Original popup routing probe completed in the live `StarDev` game:
      clicking a Planet Summary Report production cell opened the original
      `Production Queue for Oop Be Gone` dialog through the production-command
      route, while the targeted fleet-detail columns did not expose a
      `DrawPopup`/`Popup` window in this report layout. The dialog was closed
      cleanly and no program-error window appeared. The observation is retained
      in `runs/vmctl-vc152current/production-queue-popup.bmp`; it confirms the
      production path should not be misattributed to the popup procedures.
- [x] `DrawPopup` step-5 original/UI checkpoint recorded: the original Win98
      executable's mineral, population, report, and popup-restoration states
      are preserved without a Program Error dialog. Combined with the 51-case
      native dispatcher/state probe, this establishes the Win16 consumer
      boundary. Packed `GlobalPD` records, localization, hull filtering,
      formatting, and detailed GDI rendering remain open before promotion;
      evidence is in `analysis/compat/draw-popup-integration.{md,json}`.
- [x] `Popup` step-5 original/UI checkpoint recorded: the original Win98
      popup base, starting popup, planet popup, and restored-frame states are
      preserved without a Program Error dialog. Combined with the 31-case
      native sizing/window-lifecycle probe, this establishes the consumer
      boundary. Localized measurement, packed content records, and detailed
      layout remain open before promotion; evidence is in
      `analysis/compat/popup-integration.{md,json}`.
- [x] Original production-queue dialog evidence completed for `ZipProdDlg`.
      The live `StarDev` Win98 game opened `Production Queue for Oop Be Gone`,
      showing the available production-item list, empty selected queue,
      Add/Remove/Item Up/Item Down/Clear controls, required-mineral totals,
      template/research options, and OK/Cancel navigation. No program-error
      dialog appeared. Evidence is preserved in
      `analysis/compat/zip-prod-dlg-integration.{md,json}` and
      `runs/vmctl-vc152current/production-queue-popup.bmp`. The procedure is
      now at step 5/6; packed queue/name records, exact production bit layout,
      localized resources, rename behavior, and complete Win16 controls remain
      open.
- [x] Original component-browser evidence completed for `BrowserDlg`. The
      original `Ship & Starbase Designer` Components view displayed the group
      selector, component list with icons and scrollbar, and Done/Help controls
      without a program-error dialog. Evidence is preserved in
      `analysis/compat/browser-dlg-integration.{md,json}` and
      `runs/vmctl-vc152current/ship-design-components.bmp`. The procedure is
      now at step 5/6; packed `vpartBrowser` records, exact component lookup
      tables, localized storage, and full USER control integration remain open.
- [x] Expanded `BrowserDlg` packed-item validation in fresh `C:\BRD2`: the
      strict host/native matrix now has 20 checks covering preservation of the
      packed high byte while the low-byte component index moves
      `0x1201 -> 0x1200 -> 0x1201`. VC++ 1.52c compile/link passed, the Win98
      report was `BROWSER_DLG_CHECKS=20 FAILURES=0 RESULT=PASS`, and no Program
      Error dialog appeared. This strengthens step 5/6; exact lookup tables,
      localized storage, complete USER control/message integration, and
      promotion remain open.
- [x] Expanded `BrowserDlg` exact lookup-table validation in fresh `C:\BRD3`:
      the recovered `CS:1EB5` sentinel plus sixteen component-group values now
      drive selection mapping and empty-combo next/previous wrapping. VC++
      1.52c compile/link passed, the Win98 report was
      `BROWSER_DLG_CHECKS=24 FAILURES=0 RESULT=PASS`, and no Program Error
      dialog appeared. This closes the exact group-table concern inside step
      5/6; localized storage, complete USER control/message integration,
      packed `vpartBrowser` payload consumption, original differential
      coverage, and promotion remain open. Evidence is preserved in
      `analysis/compat/browser-dlg-integration.{md,json}` and
      `runs/vmctl-vc152brd3/`.
- [x] Expanded `BrowserDlg` localized-resource sequence validation in fresh
      `C:\BRD4`: the native probe verifies the exact compressed-string ID
      sequence `0x43F` through `0x44F` (17 entries). VC++ 1.52c compile/link
      passed, the Win98 report was
      `BROWSER_DLG_CHECKS=25 FAILURES=0 RESULT=PASS`, and no Program Error
      dialog appeared. This narrows but does not close localized payload
      recovery; complete USER control/message integration, packed
      `vpartBrowser` payload consumption, original differential coverage, and
      promotion remain open. Evidence is preserved in
      `analysis/compat/browser-dlg-integration.{md,json}` and
      `runs/vmctl-vc152brd4/`.
- [x] Expanded `BrowserDlg` packed-record validation in fresh `C:\BRD5`: the
      31-check host/native matrix round-trips the packed group/item header and
      recovered `FLookupPart` offsets for group `0x0008` (base `0x04E0`, stride
      `0x36`, 12 entries) and group `0x0010` (base `0x2450`, stride `0x3C`, 24
      entries) through initialization, navigation, and selection. VC++ 1.52c
      compile/link passed, the Win98 report was
      `BROWSER_DLG_CHECKS=31 FAILURES=0 RESULT=PASS`, and no Program Error
      dialog appeared. Actual component payloads, localized string contents,
      complete USER control/message integration, original differential
      coverage, and promotion remain open. Evidence is preserved in
      `analysis/compat/browser-dlg-integration.{md,json}` and
      `runs/vmctl-vc152brd5/`.
- [x] Expanded `BrowserDlg` component-payload validation in fresh `C:\BRD6`:
      the 33-check host/native matrix consumes the recovered 52-byte
      `_compart` layout—ID, technology bytes, name, mass, resource cost, ore
      costs, and bitmap ID—after group 8 and group `0x10` selection. VC++
      1.52c compile/link passed, the Win98 report was
      `BROWSER_DLG_CHECKS=33 FAILURES=0 RESULT=PASS`, and no Program Error
      dialog appeared. Original payload values, localized string contents,
      complete USER control/message integration, original differential
      coverage, and promotion remain open. Evidence is preserved in
      `analysis/compat/browser-dlg-integration.{md,json}` and
      `runs/vmctl-vc152brd6/`.
- [x] Direct original `Popup` caller probe attempted from the Planet Summary
      Report population cell after tracing `ReportDlg` to
      `ExecuteReportClick` and then `Popup`. The click did not yield a stable
      popup body or an enumerated popup window; the report/owner window
      disappeared during the capture and no Program Error dialog appeared.
      The captures are retained as diagnostic evidence in
      `runs/vmctl-vc152current/direct-popup-report-open.bmp`,
      `runs/vmctl-vc152current/direct-popup-population.bmp`, and
      `runs/vmctl-vc152current/direct-popup-population2.bmp`. This is not a
      step-5 acceptance checkpoint at that point in the diagnostic sequence;
      the later packed/UI checkpoints recorded above advance `DrawPopup` and
      `Popup` to 5/6. Stable original popup content and caller integration
      remain part of their promotion scope.
- [x] Original build-dialog integration checkpoint completed for `FillBuildDD`:
      the original Ship & Starbase Designer showed the populated `Armed Probe`
      design selector and component/cost summary, plus the `Small Freighter`
      Available Hull Types selector and capacity/cost summary, without a
      Program Error dialog. Evidence is preserved in
      `analysis/compat/fill-build-dd-integration.{md,json}`,
      `runs/vmctl-vc152current/ship-design.bmp`, and
      `runs/vmctl-vc152current/ship-design-hulls.bmp`. The procedure is now
      at step 5/6; exact packed lookup, localized storage, queue records, and
      complete build-dialog control integration remain open.
- [x] Live original `IDropPart` boundary diagnostic completed: copying the
      `Armed Probe` reached the editable Ship & Starbase Designer with the
      component inventory, populated slots, and selected-component summary;
      selecting `Long Hump 6` changed the inventory detail without a Program
      Error dialog. The temporary copy was cancelled. The controller cannot
      issue the required press/move/release drag sequence, so this remains
      surrounding-dialog evidence rather than a complete original drop trace.
      The later packed integration checkpoint advances `IDropPart` to 5/6.
      Evidence is preserved in `analysis/compat/i-drop-part-live-diagnostic.md`
      and `runs/vmctl-vc152idp-live/`.
- [x] `PtDisplayPlanetStateInfo` native checkpoint completed in fresh `C:\STI`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `PT_DISPLAY_PLANET_STATE_INFO_CHECKS=34 FAILURES=0 RESULT=PASS`
      without a runtime error dialog. Evidence is preserved in
      `analysis/compat/pt-display-planet-state-info-native.md` and
      `runs/vmctl-vc152stateinfo/`; packed lookup, formatting, detailed GDI
      integration, and original-behavior evidence remain open.
- [x] `PtDisplayPlanetStateInfo` packed-record integration checkpoint completed
      in fresh `C:\STINT`: strict host C89 and native VC++ 1.52c each report
      `PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=23 FAILURES=0 RESULT=PASS`
      over the recovered 56-byte `_planet` and 22-byte `_popupdata` views.
      The combined identity/limit bridge, known and unknown rows, environment
      and desirability service calls, improvement warning, wrapping, and
      returned geometry passed. Compile and link passed, the process exited 0,
      and no Program Error dialog appeared. The procedure remains at 5/6
      pending complete packed player/planet lookup, localization, fixed-point
      formatting, detailed stream/wrap GDI behavior, original differential
      coverage, and final review. Evidence is preserved in
      `analysis/compat/pt-display-planet-state-info-packed-integration.{md,json}`
      and `runs/vmctl-vc152stint/`.
- [x] `PtDisplayPlanetPopInfo` native checkpoint completed in fresh `C:\PPI`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `PT_DISPLAY_PLANET_POP_INFO_CHECKS=25 FAILURES=0 RESULT=PASS`
      without a runtime error dialog. Evidence is preserved in
      `analysis/compat/pt-display-planet-pop-info-native.md` and
      `runs/vmctl-vc152popinfo/`; packed lookup, formatting, detailed GDI
      integration, and original-behavior evidence remain open.
- [x] `PtDisplayPlanetPopInfo` packed-record integration checkpoint completed
      in fresh `C:\PPINT`: strict host C89 and native VC++ 1.52c each report
      `PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=19 FAILURES=0 RESULT=PASS` over
      the recovered 56-byte `_planet` identity, owner, mineral-rate, and
      population-change fields. The owned, unowned, foreign, and negative-
      desirability branches pass through the lookup/service boundary. Compile
      and link passed, the process exited 0, and no Program Error dialog
      appeared. An earlier probe-only report is retained as diagnostic history
      because its negative-branch label indices were incorrect. The procedure
      remains at 5/6 pending complete packed planet/player lookup, localization,
      fixed-point formatting, detailed stream/wrap GDI behavior, original
      differential coverage, and final review. Evidence is preserved in
      `analysis/compat/pt-display-planet-pop-info-packed-integration.{md,json}`
      and `runs/vmctl-vc152ppint/`.
- [x] `PopupMenu` native checkpoint completed in fresh `C:\PM`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `POPUP_MENU_CHECKS=22 FAILURES=0 RESULT=PASS` without a runtime
      error dialog. Evidence is preserved in
      `analysis/compat/popup-menu-native.md` and `runs/vmctl-vc152pmenu/`;
      localized lookup, full Win16 menu API behavior, caller-owned records,
      and original-behavior evidence remain open.
- [x] `DoRototillAiTurn` native checkpoint completed in fresh `C:\ROT`:
      VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the Win98 probe
      reports `ROTOTILL_AI_TURN_CHECKS=15 FAILURES=0 RESULT=PASS` without a
      runtime error dialog. Evidence is preserved in
      `analysis/compat/do-rototill-ai-turn-native.md` and
      `runs/vmctl-vc152rotill/`; this is the source-level native checkpoint;
      the later packed integration checkpoint is preserved separately and
      queue construction, detailed routing, raw PLORD mutation, and full
      original-turn behavior remain open.
- [x] `DoRototillAiTurn` packed integration checkpoint completed in fresh
      `C:\ROTINT3`: the recovered 56-byte `_planet` and 124-byte `_fleet`
      views round-trip known owner, visibility, mineral, planet, cord, and
      sixteen 16-bit ship-count fields. The host and Win98 reports each pass
      12 checks with zero failures, and the window inventory shows no Program
      Error dialog. Optimal value and desirability remain service results,
      matching the raw call boundary; queue construction, colonizer/wormhole
      and mineral routing, raw PLORD mutation, complete AI-table integration,
      and full original-turn differential remain open. Evidence is preserved
      in `analysis/compat/do-rototill-ai-turn-integration.{md,json}` and
      `runs/vmctl-vc152rotint3/`.
- [x] Planet/fleet fuel-and-orders source checkpoint: the focused strict-C89
      probes for `LFuelUseToWaypoint`, `EstFuelUse`, `AutoRouteFleet`, and
      `ZipOrderDlg` all report `PASS`. Their current typed boundaries cover
      waypoint fuel accumulation, fixed-point fuel/range conversion, route
      and stargate warp selection, and stored-order dialog state transitions;
      `LFuelUseToWaypoint`, `EstFuelUse`, and `ZipOrderDlg` now also have
      native compile/link checkpoints; packed integration, original-executable
      comparison, and remaining native Win16 validation remain open, so all
      four ledger rows stay partial.
- [x] `EstimateItemProdSched` packed/integration checkpoint completed: the
      raw `PLPROD` header and four-byte `_prod` queue records round-trip through
      the typed adapter, the schedule invokes reconstructed `CBuildProdItem`,
      resource/object/completion state passes two host/native checks, and the
      original Production Queue surface shows the available-item, empty-queue,
      and required-mineral boundary without a Program Error dialog. Evidence is
      preserved in `analysis/compat/estimate-item-prod-sched-integration.{md,json}`
      and `runs/vmctl-vc152eipint/`. The procedure is now at step 5/6; broader
      original turn-by-turn differential and complete production-record/resource
      mutation comparison remain open.
- [x] `AutoRouteFleet` packed/integration checkpoint completed: the recovered
      124-byte `_fleet` view, four-byte `PLORD` header, and 18-byte order
      records round-trip target/coordinates, flags, order count, stargate
      selection, fuel descent, and mineral-cargo exclusion through three
      host/native checks. The original Fleet Waypoints/Fuel & Cargo surface is
      preserved without a Program Error dialog. Evidence is preserved in
      `analysis/compat/auto-route-fleet-integration.{md,json}` and
      `runs/vmctl-vc152arfint/`. The procedure is now at step 5/6; exact
      resource/route-helper behavior and complete live fleet-turn comparison
      remain open.
- [x] Expanded the `AutoRouteFleet` packed/integration checkpoint in fresh
      `C:\ARFINT2`: eight native checks now cover source-owner mismatch,
      nonzero gate damage, missing-target early return, zero-ideal-warp
      fallback, and packed cargo-hull capacity in addition to stargate warp
      selection, fuel descent, mineral exclusion, and raw order round-trip.
      VC1.52c compile/link passed, the Win98 report was
      `AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`, and no
      Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/auto-route-fleet-integration.{md,json}`,
      `analysis/reconstruction/auto-route-fleet.md`,
      `tests/auto_route_fleet_integration_probe.c`,
      `compat/vc152/auto-route-fleet-integration/`, and
      `runs/vmctl-vc152arfint2/`. This strengthens step 5/6; exact
      resource/route-helper behavior and complete live fleet-turn comparison
      remain open.
- [x] Expanded `AutoRouteFleet` again in fresh `C:\ARFINT3`: the strict host
      and VC++ 1.52c/Win98 matrix now passes 12 checks, adding default
      Manhattan distance, absent ideal-warp, default fuel-descent, and fourth
      mineral-slot exclusion cases over the packed `_fleet`/`PLORD` boundary.
      Compile and link passed, the process exited 0, and no Program Error
      dialog appeared. Exact resource behavior and complete live fleet-turn
      route comparison remain open; the procedure stays at 5/6.

- [x] Expanded `DrawProductionDlg` formatting/resource-boundary validation in
      fresh `C:\DPD3`: the source now preserves decimal mineral costs through
      the recovered formatter instead of reducing them to one digit, routes
      mineral label colors through the recovered service boundary, and restores
      normal text color before numeric output. Strict host C89 and VC++ 1.52c
      each pass; the Win98 report is
      `DRAW_PRODUCTION_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`, redirected output
      is empty, and no Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/draw-production-dlg-native.md`,
      `analysis/reconstruction/draw-production-dlg.md`,
      `tests/draw_production_dlg_source_probe.c`,
      `compat/vc152/draw-production-dlg/`, and
      `runs/vmctl-vc152dpd3/`. This strengthens step 5/6; packed production
      globals, complete resource-table comparison, localized strings, and full
      GDI integration remain open.

- [x] Expanded `ProductionDlg` popup-result validation in fresh `C:\PDG2`:
      valid saved-queue selections now forward through the typed selection
      boundary, while the final editor entry forwards to the queue-dialog
      boundary. Strict host C89 and VC++ 1.52c each pass; the Win98 report is
      `PRODUCTION_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`, redirected output is
      empty, and no Program Error dialog appeared. Evidence is preserved in
      `analysis/compat/production-dlg-native.md`,
      `analysis/reconstruction/production-dlg.md`,
      `tests/production_dlg_source_probe.c`,
      `compat/vc152/production-dlg/`, and
      `runs/vmctl-vc152pdg2/`. This strengthens step 5/6; packed production
      globals, exact child geometry, localized popup resources, complete
      queue-control integration, and promotion remain open.

- [x] Expanded `ZipProdDlg` packed-queue validation in fresh `C:\ZPD2`: the
      strict host/native matrix now has 29 checks covering rejected production
      records, the maximum packed kind/code word (`0xffc6`), and the original
      twelve-entry cap. VC++ 1.52c compile/link passed, the Win98 report was
      `ZIP_PROD_DLG_CHECKS=29 FAILURES=0 RESULT=PASS`, and no Program Error
      dialog appeared. This strengthens step 5/6; packed global queue/name
      storage, exact localized resources, rename-dialog behavior, and complete
      Win16 control integration remain open.

- [x] Expanded `BrowserDlg` packed-payload validation in fresh `C:\BRD7`:
      the synthetic component bytes were replaced with records recovered from
      `target/stars.exe` at the original NE segment-2 table offsets. The
      strict host/native matrix now passes
      `BROWSER_DLG_CHECKS=35 FAILURES=0 RESULT=PASS` over actual technology
      masks, names, mass/resource costs, ore costs, and bitmap IDs from both
      lookup tables. VC1.52c compile/link passed, the process exited 0, and no
      Program Error dialog appeared. This closes the original component-payload
      concern inside step 5/6; localized string payloads, complete USER
      control/message integration, original differential coverage, and
      promotion remain open.

- [x] Expanded `BrowserDlg` localized-payload validation in fresh `C:\BRD8`:
      the actual compressed strings for resource IDs `0x43F` through `0x44F`
      were recovered from the original decoder/data boundary and checked in
      order (`All` through `Torpedoes`), including a combined ASCII payload
      sum of 15229. The strict host/native matrix now passes
      `BROWSER_DLG_CHECKS=36 FAILURES=0 RESULT=PASS`; VC1.52c compile/link
      passed and no Program Error dialog appeared. This closes the localized
      payload-content concern inside step 5/6; complete USER control/message
      integration, original differential coverage, and promotion remain open.

- [x] Expanded `EstimateItemProdSched` with a strict host differential
      matrix: `ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=21 FAILURES=0
      RESULT=PASS`. It covers phase-1/phase-2 completion, the 100-turn
      fallback, final and non-final special items, partial-item propagation,
      raw/research-adjusted and research-exempt resource queries, mineral
      refresh, copied-planet isolation, and population timing. The fresh
      VC++ 1.52c packet is prepared in
      `compat/vc152/estimate-item-prod-sched-diff/` for `C:\EIPD`, but the
      guest agent has not announced, so native/original acceptance and the
      three existing Step-5 units remain open.

- [x] Expanded `DumpPlanets` with a strict host boundary matrix:
      `DUMP_PLANETS_BOUNDARY_CHECKS=24 FAILURES=0 RESULT=PASS`. It covers
      active-game/viewer rejection, open/header/row failure cleanup,
      normal/detailed header counts, turn-age and owner visibility, scanned
      service fields, inhabited/unknown class clearing, and normal
      production/resource preservation. The fresh native packet is prepared
      in `compat/vc152/dump-planets-boundary/` for `C:\DPB`; native replay,
      localized serialization, packed services, original output, and
      promotion remain open.

- [x] Expanded `DumpFleets` with a strict host boundary matrix:
      `DUMP_FLEETS_BOUNDARY_CHECKS=20 FAILURES=0 RESULT=PASS`. It covers
      active-game/viewer rejection, open/header/row failure cleanup,
      normal/detailed header counts, all four ship-class buckets plus the
      other-class total, all cargo slots, detail suppression,
      destination-requires-detail behavior, and detailed scanner/task/
      mine/terraform fields. The fresh native packet is prepared in
      `compat/vc152/dump-fleets-boundary/` for `C:\DFB`; native replay,
      localized serialization, packed services, original output, and
      promotion remain open.

- [x] Added a typed `ReportDlg` paint-view boundary. The strict host probe
      passes `REPORT_DLG_PAINT_VIEW_CHECKS=6 FAILURES=0 RESULT=PASS` and
      verifies that populated report kind, row window, field count, visible
      mask, cached first-column width, and paint rectangle reach the report
      renderer while the legacy callback remains supported. This narrows the
      caller-integration concern but closes no authoritative Step-5 unit yet:
      the native replay, populated row-union payloads, live USER/GDI/menu
      behavior, original caller comparison, and promotion remain open. The
      matching native packet is prepared in
      `compat/vc152/report-dlg-paint-view-e2/` for `C:\RPTVE2`.

- [x] Expanded `EstimateItemProdSched` through the recovered production
      services. The strict host probe passes
      `ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=8 FAILURES=0 RESULT=PASS`
      over copied-planet mineral handoff, research-exempt resources, callback
      order, two queued cost records across all four lanes, completion turns,
      and caller-planet preservation. The matching native packet is prepared
      in `compat/vc152/estimate-item-prod-sched-resource-queue/` for
      `C:\EIRQ`; native replay, original comparison, and promotion remain
      open.

- [x] Expanded `ReportDlg` through the reconstructed renderer chain. The
      strict host probe passes
      `REPORT_DLG_DRAW_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS` after a
      populated `WM_PAINT` dispatch reaches `DrawReport` and
      `DrawReportItem`; the probe checks header measurement, populated planet
      lookup, text/field drawing, and selection update. This makes the host
      caller path concrete, but closes no authoritative Step-5 unit: native
      replay, packed row payloads, live USER/GDI/menu evidence, original
      comparison, and promotion remain open. Evidence is in
      `analysis/compat/report-dlg-draw-integration-e3.{md,json}`.

- [x] Added a shared host branch integration matrix for
      `PtDisplayPlanetPopInfo` and `PtDisplayPlanetStateInfo`. The strict C89
      probe passes `PLANET_INFO_BRANCH_CHECKS=9 FAILURES=0 RESULT=PASS` over
      owned/unowned/foreign population headers, zero-mineral and negative-
      desirability paths, maximum-population and growth-warning callbacks,
      unknown/known player limits, planet limits, desirability improvement,
      environment service calls, signed differences, wrapping, and returned
      layout. This closes no authoritative Step-5 unit: native replay, packed
      lookup, localized fixed-point formatting, detailed GDI comparison,
      original behavior, and promotion remain open. Evidence is in
      `analysis/compat/planet-info-branch-e3.{md,json}`.

- [x] Connected `SatisfyOrders` to the reconstructed `DoOrders` turn
      orchestrator. The strict C89 probe passes
      `SATISFY_ORDERS_TURN_CHECKS=8 FAILURES=0 RESULT=PASS` and checks both
      pre- and post-movement pass sequences, battle-id creation, battle/thing
      callbacks, planet flags and turn clearing, fleet cargo mutation, and
      order retirement. This closes no authoritative Step-5 unit: packed
      battle-plan/resource accounting, technology discovery, fleet and
      ship-design copying, salvage creation, complete message layouts,
      native replay, original behavior, and promotion remain open. Evidence
      is in `analysis/compat/satisfy-orders-turn-e3.{md,json}`; the matching
      VC++ 1.52c packet is prepared at
      `compat/vc152/satisfy-orders-turn-e3/` for `C:\SYTURN`.

- [x] Expanded `ExecuteReportClick` caller integration through the popup
      stack. The strict C89 probe passes
      `EXECUTE_REPORT_CLICK_POPUP_INTEGRATION_CHECKS=26 FAILURES=0 RESULT=PASS`
      for fleet-detail, factory-installation, and resource cells. It verifies
      semantic payload preservation from `ExecuteReportClick` into `Popup`,
      measurement and window creation, `DrawPopup` paint-kind dispatch,
      resource research-share arithmetic, factory weight extraction,
      message/capture setup, and DC release symmetry. This is host-only and
      closes no Step-5 unit; native replay, exact original `GlobalPD` owner/
      part cases, live USER/GDI behavior, original comparison, and promotion
      remain open. Evidence is in
      `analysis/compat/execute-report-click-popup-integration-e3.{md,json}`.
      The matching packet is prepared at
      `compat/vc152/execute-report-click-popup-integration-e3/` for
      `C:\POPINT`; packet source units pass the host-shape compile and the
      CRLF wrappers are ready, but native replay awaits guest-agent reconnect.

- [x] Connected the combat orchestrator to the attack service in a host E3
      dependency probe. The strict C89 result is
      `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS`.
      The deterministic two-token battle routes descending initiative through
      reconstructed `FAttack`, applies damage to the actual battle token,
      emits movement/attack events, terminates when one player remains, and
      checks the header/report/cleanup lifecycle. This closes no Step-5 unit:
      native replay, packed token/global mapping, exact part/damage behavior,
      original battle differential, and promotion remain open. Evidence is in
      `analysis/compat/f-do-cool-battle-attack-integration-e3.{md,json}` and
      the matching packet is prepared at
      `compat/vc152/f-do-cool-battle-attack-integration-e3/` for `C:\CBAT`.

- [x] Added a host paint-chain milestone for `PlanetWndProc` and
      `DrawPlanetStats`: the strict C89 probe routes `WM_PAINT` through the
      parent begin-paint/child-render/end-paint sequence and passes
      `PLANET_STATS_WNDPROC_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`.
      It checks the packed stats load, seven value rows, four references, two
      separators, and selected mineral/scanner/defense values. This closes no
      authoritative Step-5 unit; native replay and original comparison remain
      open. Evidence is in
      `analysis/compat/planet-stats-wndproc-integration-e3.{md,json}`.

- [x] A further bounded listener retry after the host paint-chain milestone
      again received no `AGENTINFO STARSAGT 0.4.2` banner and was stopped
      cleanly. Native packets remain prepared; the guest-agent endpoint still
      needs to reconnect or be restarted inside Win98 before native replay.

- [x] Added a host edge-policy matrix for `DoTurinDroneAiTurn`: the strict
      C89 probe passes `TURIN_DRONE_AI_TURN_EDGE_CHECKS=7 FAILURES=0
      RESULT=PASS` across potency thresholds/caps, merge short-circuiting, all
      eight design-status calls, rich/insufficient five-hull affordability,
      and null-input rejection. This closes no authoritative Step-5 unit;
      direct production-table mutation, fleet routing/order mutation, native
      replay, and original-turn comparison remain open. Evidence is in
      `analysis/compat/do-turin-drone-ai-turn-edge-e3.{md,json}`.

- [x] Retried the native reconnect for the combat-chain packet against the
      resumed `StarDev` 86Box window. No `AGENTINFO STARSAGT 0.4.2` banner
      arrived; the listener was stopped cleanly after the visible stale Close
      Program dialog was handled without ending `Stars!` or `Vmctl1`. Native
      replay remains pending and this retry closes no Step-5 unit.

- [x] Clarified Phase 2 gate accounting: `5/6` is a gate count, not a claim
      that one small task remains. The procedure matrix's remaining-gate cell
      is the active step-5 work breakdown; native checkpoints can close one
      concern without changing the headline number. Repaired the evidence
      ledger to reference concrete files, regenerated both coverage ledgers,
      and verified the repository with 519 tests passing and 153 host-path
      skips.

- [x] Added a host fleet-gate matrix for `DoRobotoidAiTurn`: the strict C89
      probe passes `ROBOTOID_AI_FLEET_GATE_CHECKS=10 FAILURES=0 RESULT=PASS`
      across empty, colonizer, transport, already ordered armed,
      support/attack-bin, `warship_count` fallback, and non-owned fleet cases.
      This makes the inputs to the remaining order-repair/routing seam
      explicit but closes no authoritative Step-5 unit; direct `PLORD`
      mutation, exact route choices, complete AI-table integration, native
      replay, and original-turn differential remain open. Evidence is in
      `analysis/compat/do-robotoid-ai-turn-fleet-gates-e3.{md,json}`.

- [x] Added an explicit typed `repair_order` boundary to
      `DoRobotoidAiTurn`. The strict C89 probe passes
      `ROBOTOID_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS` for
      missing-first-order dispatch, ordered/empty bypasses, policy and
      potency forwarding, callback ordering, and failure propagation. This
      closes no authoritative Step-5 unit because packed `PLORD` mutation,
      exact route selection, native replay, and original-turn comparison
      remain open. Evidence is in
      `analysis/compat/do-robotoid-ai-turn-repair-boundary-e3.{md,json}`.

- [x] Added typed `repair_order` boundaries to `DoTurinDroneAiTurn` and
      `DoCyberAiTurn`. Their strict C89 probes each pass six checks for
      missing-order dispatch, ordered/non-owned bypasses, policy/potency
      forwarding, callback ordering, and failure propagation; Cyber also
      checks its five merge masks. These host milestones close no authoritative
      Step-5 units because packed `PLORD` mutation, exact routing, native
      replay, and original-turn comparison remain open. Evidence is in
      `analysis/compat/do-turin-drone-ai-turn-repair-boundary-e3.{md,json}`
      and `analysis/compat/do-cyber-ai-turn-repair-boundary-e3.{md,json}`.

- [x] Added the shared `stars_ai_repair_first_order` packed-order helper for
      the recovered four-byte PLORD header and 18-byte order stride. The
      Robotoid repair callback exercises the mutation end-to-end; the strict
      C89 probe passes `AI_PLORD_REPAIR_SOURCE_CHECKS=9 FAILURES=0 RESULT=PASS`
      while preserving unrelated order bytes. The helper is available to the
      Turin and Cyber repair boundaries as well. This closes no authoritative
      Step-5 unit: multi-order routing, personality-specific action selection,
      native replay, and original-turn comparison remain open. Evidence is in
      `analysis/compat/ai-plord-repair-e3.{md,json}`.

- [x] Added a typed Robotoid fleet route-plan/apply boundary. The strict C89
      probe passes
      `ROBOTOID_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS` and
      verifies raw repair before planning, fleet-policy and turn forwarding,
      colonizer plan fields, the no-op path, exact apply payload, visitor
      ordering, non-owned filtering, and planner/applier failure propagation.
      The action family constants cover clear-task, move, colonize, wormhole,
      starbase movement, attack, armada targeting, transport, and freighter
      targeting. Named action callbacks now dispatch before the generic
      applier fallback, with the specialized colonize path covered by the
      probe. This is a host-only source checkpoint and closes no
      authoritative Step-5 unit: exact original route predicates, multi-order
      packed `PLORD` mutation, complete AI-table state, native replay, and
      original-turn differential remain open. Evidence is in
      `analysis/compat/do-robotoid-ai-turn-route-plan-e3.{md,json}`.

- [x] Added the shared `stars_ai_move_order_to_plord` adapter for the reviewed
      `FMoveAiFleet` 18-byte order-copy path used by colonize, wormhole, and
      personality target actions. The strict C89 probe passes
      `AI_PLORD_MOVE_ORDER_CHECKS=12 FAILURES=0 RESULT=PASS` over first-order
      replacement, append-to-tail, endpoint comparison, all nine order words,
      order-count update, and full-allocation rejection without mutation.
      Existing `F_MOVE_AI_FLEET_CHECKS=2` and
      `F_COLONIZE_AI_FLEET_CHECKS=1` probes also pass. This host-only packed
      mutation checkpoint closes no authoritative Step-5 unit: personality
      target searches, complete route decisions, native replay, and
      original-turn differential remain open. Evidence is in
      `analysis/compat/ai-plord-move-order-e3.{md,json}`.

- [x] Added the shared `stars_ai_set_first_order_kind` boundary for the raw
      Robotoid-family `0xfff0 | 5/6` writes. Its strict-C89 12-check probe
      preserves the order count, high nibble, and unrelated bytes and rejects
      invalid input; the existing repair probe remains green. This closes no
      named Step-5 unit because the complete personality route and native /
      original-turn evidence are still open. Evidence is in
      `analysis/compat/ai-plord-first-kind-e11.{md,json}`.

- [x] Connected the reviewed `FColonizeAiFleet` and
      `FGotoWormholeAiFleet` constructors to the shared packed-order adapter
      in a host integration probe. It passes
      `AI_FLEET_ACTION_PLORD_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`
      over colonizer/wormhole target and flag construction, ideal-warp nibble
      placement, first-order replacement, order-count preservation, selection
      forwarding, and movement-success fleet flags. This is a host-only
      action-to-record checkpoint and closes no authoritative Step-5 unit:
      personality target searches, exact route precedence, native replay, and
      original-turn differential remain open. Evidence is in
      `analysis/compat/ai-fleet-action-plord-integration-e3.{md,json}`.

- [x] Connected the reviewed `FMoveToNearestStarbase` constructor to the
      shared PLORD adapter. The strict host probe passes
      `AI_STARBASE_ACTION_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`
      over starbase and point lookup forwarding, replacement mode, target
      identity, exact `0x1040` flags, packed record preservation, and the
      no-target short circuit. This host-only action-to-record checkpoint
      closes no authoritative Step-5 unit: personality target searches, route
      precedence, native replay, and original-turn differential remain open.
      Evidence is in
      `analysis/compat/ai-starbase-action-plord-integration-e3.{md,json}`.

- [x] Connected the reviewed `IdTargetFreighter` typed target-selection
      boundary to the shared PLORD adapter. The strict host probe passes
      `AI_FREIGHTER_ACTION_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`
      over strict score ordering, target and point forwarding, exact
      `0x0100` move flags, first-order replacement, packed-record preservation,
      and the cargo-free rejection/sentinel path. This makes the Step-5
      freighter checkpoint more explicit, but it closes no authoritative unit:
      the raw routine's AI markings, cargo calculations, enemy/unknown
      classification, complete eligibility rules, personality integration,
      native replay, and original-turn differential remain open. Evidence is
      in `analysis/compat/ai-freighter-action-plord-integration-e3.{md,json}`.

- [x] Connected the reviewed `DoCyberFreighter` movement boundary to the
      shared PLORD adapter. The strict host probe passes
      `AI_CYBER_FREIGHTER_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`
      over the owned-planet supply branch, target lookup, ideal-warp
      forwarding, exact `0x11b0` movement flags, target/point preservation,
      first-order replacement, and the fleet `0x8000` moving flag after
      success. This host-only movement-to-record checkpoint closes no
      authoritative Step-5 unit: complete Cyber freighter behavior,
      packet/freighter interaction, repeated-turn progress semantics, native
      replay, and original-turn differential remain open. Evidence is in
      `analysis/compat/ai-cyber-freighter-plord-integration-e3.{md,json}`.

- [x] Corrected `DoCyberFreighter` so the drop-off progress field advances
      only for carrying/drop-off orders. The strict host probe passes
      `DO_CYBER_FREIGHTER_PROGRESS_CHECKS=9 FAILURES=0 RESULT=PASS` for
      pickup without drop-off-bit mutation, successful drop-off progress, and
      carrying-without-destination rollback with the expected transfer pair
      and separate planet-progress increment. This remains host
      source-fidelity evidence only: full mineral-rate/stock predicates,
      native replay, and original-turn differential remain open, so no
      authoritative Step-5 or promotion count changes. Evidence is in
      `analysis/compat/ai-cyber-freighter-progress-e4.{md,json}`.

- [x] Made the raw owned-planet mineral-rate and stock gate explicit in
      `DoCyberFreighter`. The strict host probe passes
      `DO_CYBER_FREIGHTER_OWNED_GATE_CHECKS=12 FAILURES=0 RESULT=PASS` for
      negative rate, low-rate stock `2000`, low-rate stock `2001`, and positive
      rate with empty stock, including target kind, movement, and `-1000` or
      `+1000` transfer direction. This remains host source-fidelity evidence
      only: other raw fleet-cargo predicates, native replay, and original-turn
      differential remain open, so no authoritative Step-5 or promotion count
      changes. Evidence is in
      `analysis/compat/ai-cyber-freighter-owned-gate-e5.{md,json}`.

- [x] Connected the reviewed `TargetCyberArmada` destination boundary to the
      shared PLORD adapter. The strict host probe passes
      `AI_CYBER_ARMADA_PLORD_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`
      over ordinary-planet precedence, enemy-fleet fallback, target kind and
      identity, point forwarding, planet marking, exact `0x1140` planet and
      `0x1240` fleet flags, first-order replacement, and packed-record
      preservation. This host-only action-to-record checkpoint closes no
      authoritative Step-5 unit: complete original-table state, stale-order
      and potency branches, native replay of this integration, and
      original-turn differential remain open. Evidence is in
      `analysis/compat/ai-cyber-armada-plord-integration-e3.{md,json}`.

- [x] Corrected the `TargetCyberArmada` current-order branch against the raw
      procedure and added a strict regression probe. The probe passes
      `TARGET_CYBER_ARMADA_ORDER_RETENTION_CHECKS=12 FAILURES=0 RESULT=PASS`
      for valid near-planet continuation, stale-owner rejection, nearby-fleet
      retention without lookup, and distant-fleet continuation into ordinary
      planet selection. This is a host source-fidelity checkpoint only:
      fresh native replay and original-turn comparison remain open, so no
      authoritative Step-5 or promotion count changes. Evidence is in
      `analysis/compat/ai-cyber-armada-order-retention-e4.{md,json}`.

- [x] Corrected the raw strong-hostile current-planet gate in
      `TargetCyberArmada`. The strict host probe passes
      `TARGET_CYBER_ARMADA_HOSTILE_GATE_CHECKS=4 FAILURES=0 RESULT=PASS` and
      verifies lookup without destination enumeration, movement, or task
      clearing. This remains host source-fidelity evidence only: fresh native
      replay, the complete weak-hostile/random matrix, and original-turn
      differential remain open, so no authoritative Step-5 or promotion count
      changes. Evidence is in
      `analysis/compat/ai-cyber-armada-hostile-gate-e5.{md,json}`.

- [x] Corrected the friendly-armada random gate in `TargetCyberArmada`.
      The strict host probe passes
      `TARGET_CYBER_ARMADA_FRIENDLY_RANDOM_CHECKS=9 FAILURES=0 RESULT=PASS`
      for random-roll fall-through, early return below the damage ceiling, and
      the recovered second roll above the three-times-potency threshold. This
      remains host source-fidelity evidence only: the complete native random
      matrix, weak-hostile branch matrix, and original-turn differential remain
      open, so no authoritative Step-5 or promotion count changes. Evidence is
      in `analysis/compat/ai-cyber-armada-friendly-random-e6.{md,json}`.

- [x] Corrected the weak-hostile random gate in `TargetCyberArmada` to
      preserve raw short-circuit call order. The strict host probe passes
      `TARGET_CYBER_ARMADA_HOSTILE_RANDOM_CHECKS=20 FAILURES=0 RESULT=PASS`
      for zero-roll retreat, one-roll rejection, two-roll rejection, and the
      three-roll retreat path, including starbase destination, movement, task
      clear, and ordinary-destination fall-through behavior. This remains
      host source-fidelity evidence only: fresh native replay and original-turn
      differential remain open, so no authoritative Step-5 or promotion count
      changes. Evidence is in
      `analysis/compat/ai-cyber-armada-hostile-random-e7.{md,json}`.

- [x] Added matching typed fleet route-plan/apply boundaries to
      `DoTurinDroneAiTurn` and `DoCyberAiTurn`. The strict C89 probes each pass
      14 checks:
      `TURIN_DRONE_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`
      and
      `CYBER_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`.
      They verify repair-before-plan ordering, policy/potency forwarding,
      colonizer plan payloads, no-op suppression, exact apply payloads,
      visitor ordering, non-owned filtering, and planner/applier failure
      propagation, including named-action dispatch before the generic fallback.
      These host-only checkpoints close no authoritative Step-5
      unit: exact personality route predicates, packet/freighter behavior,
      multi-order packed `PLORD` mutation, native replay, and original-turn
      differential remain open. Evidence is in
      `analysis/compat/do-turin-drone-ai-turn-route-plan-e3.{md,json}` and
      `analysis/compat/do-cyber-ai-turn-route-plan-e3.{md,json}`.

- [x] Expanded `DrawPlanShipBitmap`'s fleet-render seam to preserve the raw
      `DrawFleetBitmap` tile origin and positional tail. The strict host C89
      probe passes `DRAW_PLAN_SHIP_BITMAP_FLEET_CHECKS=6 FAILURES=0 RESULT=PASS`
      for normal/grid origins, fleet identity, and the exact
      `1,-1,0,0,-1,0` forwarded arguments. This is host source-fidelity
      evidence only; native fleet bitmap/GDI and original differential remain
      open, so no authoritative Step-5 or promotion count changes. Evidence
      is in `analysis/compat/draw-plan-ship-bitmap-fleet-e4.{md,json}`.

- [x] Added the focused `DrawPlanShipBitmap` failure/hide boundary. The strict
      host C89 probe passes
      `DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=6 FAILURES=0 RESULT=PASS` for
      failed-tile cleanup, suppression of downstream GDI and button work,
      hide-bit clearing, and no-buttons fleet rendering. This closes **0**
      authoritative Step-5 units; the native packet remains pending because
      the resumed guest did not announce and the Win98 Close Program dialog
      did not accept input. Evidence is in
      `analysis/compat/draw-plan-ship-bitmap-failure-e4.{md,json}`.

### FAttack unsigned initiative-wrap correction

- [x] Corrected the typed `FAttack` initiative derivation to preserve the raw
      unsigned-16-bit wrap before the upper `0x3f` cap. The focused strict-C89
      source probe now passes `F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS` for
      the `init_base=0`, `init_delta=-1`, `input.init=63` underflow boundary;
      the wrapped `0xffff` value is capped to 63 and dispatches as in the raw
      instruction sequence.
- [x] Re-ran the packed token/hull adapter and the actual host
      `FDoCoolBattle` → `FAttack` chain: `F_ATTACK_INTEGRATION_CHECKS=2` and
      `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18`, both with zero failures.
      This is source-fidelity evidence only and closes **0** named Step-5
      units; exact packed `FLookupPart` binding, complete battle-part scaling,
      broader original damage differentials, native chain replay, and full
      promotion remain open.
- [x] Preserved the native status accurately: the Win98 guest agent is not
      currently connected, so no native rerun or Step-5 count change is claimed.
      Evidence is in `analysis/compat/f-attack-init-wrap-e4.{md,json}`.

### FAttack packed-part resolver boundary

- [x] Exposed the raw `_hs` group/item/count boundary used by `FAttack` before
      initiative matching. The strict host probe passes
      `F_ATTACK_PACKED_PART_CHECKS=8 FAILURES=0 RESULT=PASS` for a successful
      `0x10` lookup and dispatch, the reconstructed `stars_lookup_part` gate
      and `0x00100007` component identity, caller-section preservation,
      out-of-range-item rejection, and zero-count suppression.
- [x] Kept the evidence classification precise: this closes **0** named
      Step-5 units. Native packed `FLookupPart` table/service binding, native
      replay, battle-part scaling, original damage differential, and promotion
      remain open. Evidence is in
      `analysis/compat/f-attack-packed-part-e5.{md,json}`.

### FAttack packed `_hs` slot storage boundary

- [x] Wired the exact four-byte `_hs` slot array into the readable FAttack
      input path. The strict host C89 probe passes
      `F_ATTACK_PACKED_PART_CHECKS=13 FAILURES=0 RESULT=PASS` while the raw
      `grhst`/item/count bytes override contradictory typed section fields and
      control lookup, component identity, segment/base/stride/item offset,
      dispatch, unsupported-item rejection, and zero-count suppression. The
      existing lookup source probe remains green at
      `LOOKUP_PART_CHECKS=8 FAILURES=0 RESULT=PASS`.
- [x] Kept this as a host-only checkpoint that closes **0** authoritative
      Step-5 units. Native `FLookupPart` table/service binding, complete
      battle-part scaling, broader original damage differentials, native
      replay, and promotion remain open. Evidence is in
      `analysis/compat/f-attack-packed-slot-e6.{md,json}`.

### FAttack / FLookupPart table-address contract

- [x] Exposed the fixed table metadata needed by the native-facing lookup
      seam. The host packed probe passes 13 checks for group `0x0010`, item 7:
      segment `0x10e0`, base `0x2450`, stride `0x003c`, offset `0x25f4`, and
      component identity `0x00100007`; the eight-check lookup source probe
      remains green.
- [x] Kept the evidence classification precise: this closes **0** named
      Step-5 units. Native segment/address binding, resource-relative table
      families, complete battle-part scaling, broader damage differentials,
      replay, and promotion remain open. Evidence is in
      `analysis/compat/f-attack-part-table-e7.{md,json}`.

### DrawPlanetStats service-resolution checkpoint

- [x] Added the typed post-tile `resolve_data` callback carrying the packed
      planet view, selected player, and seven-row data record. The strict host
      probe passes `DRAW_PLANET_STATS_SERVICES_CHECKS=12 FAILURES=0
      RESULT=PASS`; the existing 22-check source, 9-check lookup, and
      15-check packed probes remain green.
- [x] Kept this as host-only evidence that closes **0** authoritative Step-5
      units. Native service execution, localized resources, detailed GDI,
      original differential, and promotion remain open. Evidence is in
      `analysis/compat/draw-planet-stats-services-e3.{md,json}`.

### DrawPlanetShipList detail-dispatch checkpoint

- [x] Added a typed `draw_details` callback carrying the detail rectangle,
      resolved fleet/thing kind, and transfer index. The strict host probe
      passes `DRAW_PLANET_SHIP_LIST_DETAILS_CHECKS=9 FAILURES=0 RESULT=PASS`
      for fleet detail, unavailable-transfer suppression, and ship-mode
      suppression; the existing 19-check source and 18-check packed probes
      remain green.
- [x] Kept this as host-only evidence that closes **0** authoritative Step-5
      units. Native detail rendering, exact USER/GDI handles, localized
      text/gauges, complete transfer integration, original differential, and
      promotion remain open. Evidence is in
      `analysis/compat/draw-planet-ship-list-details-e3.{md,json}`.

### SatisfyOrders special-order dispatch checkpoint

- [x] Added a strict-C89 special-order matrix covering battle-plan pass 1,
      mine capability and race-stat gates, auto-route/no-path messaging,
      merge target/ownership/callback gates, player-transfer source/recipient
      message pairs, and mine repeat counts. The probe passes
      `SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=0 RESULT=PASS` with the
      separate mine-gate and callback-gate markers.
- [x] Kept this as host-only evidence that closes **0** authoritative Step-5
      units. Native packed battle-plan/resource accounting, technology
      discovery, fleet/ship-design copying, salvage creation, complete
      message layouts, full-turn replay, and promotion remain open. Evidence
      is in `analysis/compat/satisfy-orders-special-e4.{md,json}`.

### FAttack / FLookupPart fixed-table family expansion

- [x] Exercised all eleven fixed DGROUP/far-table families with a strict-C89
      lookup probe. It passes `LOOKUP_PART_TABLE_CHECKS=34 FAILURES=0
      RESULT=PASS`, checking segment `0x10e0`, family base, stride, item
      offset, and component identity. The resource-relative `0x0100` control
      case keeps table metadata zero.
- [x] Kept this as host-only evidence that closes **0** authoritative Step-5
      units. Native table/service binding, resource-relative families,
      complete battle-part scaling, broader damage differentials, replay, and
      promotion remain open. Evidence is in
      `analysis/compat/f-attack-part-table-families-e8.{md,json}`.

### FDoCoolBattle packed-token round-trip boundary

- [x] Added the production `StarsCoolBattleInput.packed_tokens` bridge for the
      packed 29-byte `_tok` view. The strict host C89 probe passes
      `F_DO_COOL_BATTLE_PACKED_TOKEN_CHECKS=11 FAILURES=0 RESULT=PASS` after
      loading two records, running the real coordinator, storing after
      movement/attack/reporting, preserving every raw byte on the surviving
      token, and writing only the defeated token's zero count and cleared
      active bit. It also checks invalid packed-capacity rejection.
- [x] Expanded the Step-5 row into named concerns: exact `_tok`/`vrgtok`
      global binding and production/compaction behavior; movement geometry and
      speed/initiative; damage/shield/casualty/salvage/bombing records; fresh
      VC++/Win98 no-error execution; and original full-battle replay. This
      host-only checkpoint closes **0** authoritative Step-5 units, so the
      authoritative totals remain **75/152 closed, 77 open, 22 promotion
      units**. Evidence is in
      `analysis/compat/f-do-cool-battle-packed-token-e6.{md,json}`.

### SatisfyOrders production packed bridge

- [x] Moved the recovered 124-byte `_fleet` / 18-byte `_order` bridge into
      `reconstructed/src/satisfy_orders.c` as
      `stars_satisfy_orders_packed`. It now validates record sizes, decodes
      identity, order kind/target kind, target id, and the validity bit,
      dispatches the typed transfer core, and writes turn flags and validity
      back to the raw records. The integration probe was changed to call this
      production bridge directly and still passes
      `SATISFY_ORDERS_INTEGRATION_CHECKS=3 FAILURES=0 RESULT=PASS`.
- [x] Classified the boundary accurately: it closes **0** authoritative
      Step-5 units. Battle-plan accounting, technology discovery, ship/design
      copying, salvage, complete message layouts, native special-order replay,
      and promotion remain open. Evidence is in
      `analysis/compat/satisfy-orders-integration.{md,json}`.

### DumpPlanets production packed bridge

- [x] Moved the recovered 56-byte `_planet` decoder into
      `reconstructed/src/dump_planets.c` as
      `stars_dump_planets_packed`. The bridge now owns the raw identity,
      flags, turn, environment, original-environment, mineral,
      population/production, scanner, starbase, wormhole, and mine-count
      extraction before dispatching the typed report coordinator. The packed
      probe calls the production bridge directly and still passes
      `DUMP_PLANETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. The historical native packet used the equivalent test
      adapter; a fresh native packet containing this production bridge is
      pending the guest-agent reconnect. Localized serialization, complete
      service callbacks, original report differential, and promotion remain
      open. Evidence is in
      `analysis/compat/dump-planets-packed-integration.{md,json}`.

### DumpFleets production packed bridge

- [x] Moved the recovered 124-byte `_fleet` decoder into
      `reconstructed/src/dump_fleets.c` as
      `stars_dump_fleets_packed`. The bridge now owns the raw identity, owner,
      flags, planet/coordinates, ship counts, cargo, weight, warp, and task
      extraction, while taking the live route/scanner/mine-service sidecars
      through the typed report boundary. The packed probe calls the production
      bridge directly and still passes
      `DUMP_FLEETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. The historical native packet used the equivalent test
      adapter; a fresh native packet containing this production bridge is
      pending the guest-agent reconnect. Localized serialization, complete
      scanner/minefield/task services, original fleet-report differential, and
      promotion remain open. Evidence is in
      `analysis/compat/dump-fleets-packed-integration.{md,json}`.

### ReportDlg production packed view bridge

- [x] Moved the recovered 54-byte `_rpt` view decoder into
      `reconstructed/src/report_dlg.c` as `stars_report_dlg_packed`. The
      bridge now owns the raw visible-field mask, report/field selection,
      sorting, row position, dialog geometry, cached widths, row count, and
      scrollbar state extraction before dispatching the typed window
      procedure. The focused view probe calls the production bridge directly
      and still passes `REPORT_DLG_VIEW_CHECKS=13 FAILURES=0 RESULT=PASS`.
- [x] Closed the packed-view-load unit in fresh `C:\RPTVEP`. The
      production-source packet compiled and linked under VC++ 1.52c and
      returned `REPORT_DLG_VIEW_CHECKS=13 FAILURES=0 RESULT=PASS`; the process
      exited 0 and no Program Error dialog appeared. Populated report
      unions/rows, live USER/GDI/menu payloads, complete caller integration,
      original behavior, and promotion remain open. Evidence is in
      `analysis/compat/report-dlg-view-e1.{md,json}` and
      `runs/vmctl-vc152rptvep/`.

### DrawPlanetStats production packed bridge

- [x] Moved the recovered 56-byte packed planet decoder into
      `reconstructed/src/draw_planet_stats.c` as
      `stars_draw_planet_stats_packed`. The bridge now owns the raw
      scanner/defense service word and mineral-rate extraction, accepts typed
      resource/survival sidecars, and dispatches the existing seven-row
      renderer. The packed probe calls the production bridge directly and
      still passes
      `DRAW_PLANET_STATS_PACKED_CHECKS=15 FAILURES=0 RESULT=PASS`.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. The historical native packet used the equivalent test
      loader; a fresh native packet containing this production bridge is
      pending the guest-agent reconnect. Complete planet/race lookup,
      localized/fixed-point formatting, exact service behavior, detailed GDI,
      original differential, and promotion remain open. Evidence is in
      `analysis/compat/draw-planet-stats-services-e3.{md,json}`.

### ExecuteReportClick production packed lookups

- [x] Moved the recovered 124-byte `_fleet`, 56-byte `_planet`, and 14-byte
      `_btldata` decoders into production `execute_report_click.c`. The
      optional packed fields take precedence over typed lookup callbacks and
      preserve the raw fleet identity/coordinates, planet service bitfields,
      mineral/player arrays, resource-exempt bit, and battle coordinates.
      The packed probe now exercises these production helpers directly and
      still passes `EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. The historical native packet used the equivalent adapter;
      a fresh native packet containing the production lookup path is pending
      the guest-agent reconnect. Exact original GlobalPD owner/part unions,
      live USER/GDI behavior, complete caller integration, original
      differential, and promotion remain open. Evidence is in
      `analysis/compat/execute-report-click-packed-integration.{md,json}`.

### FDoCoolBattle production packed-token bridge

- [x] Moved the recovered 29-byte `_tok` load/store into the production
      `StarsCoolBattleInput.packed_tokens` path. The bridge decodes every
      exercised token field after initialization and writes movement, attack,
      and reporting mutations back to the raw records while preserving
      unrelated bytes. The packed probe calls this path directly and passes
      `F_DO_COOL_BATTLE_PACKED_TOKEN_CHECKS=11 FAILURES=0 RESULT=PASS`.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. Exact `_tok`/`vrgtok` global binding, compaction and
      callers, movement geometry, complete damage records, fresh native
      execution, original replay, and promotion remain open; the historical
      native adapter is not a fresh production-bridge run. Evidence is in
      `analysis/compat/f-do-cool-battle-packed-token-e6.{md,json}`.

### DrawPopup production packed-selector bridge

- [x] Moved the recovered 22-byte `_popupdata` selector decode into the
      production `StarsDrawPopupInput.packed_popupdata` path. A sufficiently
      large raw view now takes precedence over the legacy typed `popup_kind`,
      while short views fall back safely. The packed probe exercises the
      production path over raw-selector precedence, every dispatcher kind,
      the kind-14 slot-hull alias, GDI-state restoration, and short-record
      fallback; it passes
      `DRAW_POPUP_PACKED_CHECKS=37 FAILURES=0 RESULT=PASS`.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. Fresh native replay through the production bridge,
      complete `GlobalPD` union payloads, localization, hull filtering and
      formatting, detailed GDI rendering, caller differential, and promotion
      remain open because the guest agent has not reconnected. Evidence is in
      `analysis/compat/draw-popup-packed-production-e4.{md,json}`.

### PtDisplayPlanetPopInfo production packed-planet bridge

- [x] Moved the recovered 56-byte `_planet` identity, owner,
      population-change, and mineral-rate decode into the production
      `StarsPlanetPopInfoInput.packed_planet_record` path. Service-derived
      selected-player, habitability, maximum-population, desirability, and
      foreign-environment values remain explicit sidecars. The packed probe
      calls production source directly and passes
      `PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS`,
      including short-record fallback to `lookup_planet`.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. Fresh native replay through the production bridge,
      complete planet/player lookup, localization/fixed-point formatting,
      detailed stream/wrap GDI behavior, original differential, and promotion
      remain open pending guest-agent reconnection. Evidence is in
      `analysis/compat/pt-display-planet-pop-info-packed-production-e4.{md,json}`.

### PtDisplayPlanetStateInfo production packed-record bridge

- [x] Moved the recovered 56-byte `_planet` and 22-byte `_popupdata` field
      decode into the production `StarsPlanetStateInfoInput` path. The bridge
      owns raw identity, environment bytes, planet/player limits and values,
      while derived environment/desirability services remain callbacks. The
      packed probe now calls production source directly and passes
      `PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS`,
      including short-record fallback to typed data.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. Fresh native replay through the production bridge,
      complete player/planet lookup, localization/fixed-point formatting,
      detailed stream/wrap GDI behavior, original differential, and promotion
      remain open pending guest-agent reconnection. Evidence is in
      `analysis/compat/pt-display-planet-state-info-packed-production-e4.{md,json}`.

### DrawPlanetShipList production packed-record bridge

- [x] Moved the recovered 226-byte selection and 128-byte transfer field
      decode into the production `StarsPlanetShipListInput` path. Complete raw
      views now supply the object word, transfer identity/kind, and fleet
      owner before the control/layout decisions; typed state and lookup remain
      the incomplete-view fallback. The packed probe passes
      `DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`, and the
      source probe remains green at 19 checks.
- [x] Classified the boundary accurately: this closes **0** authoritative
      Step-5 units. Fresh native replay through the production bridge, exact
      USER/GDI controls, localized text/gauges, complete detail rendering,
      original differential, and promotion remain open pending guest-agent
      reconnection. Evidence is in
      `analysis/compat/draw-planet-ship-list-packed-production-e4.{md,json}`.

### DrawProductionDlg native resource-table E2 closure

- [x] Fresh `C:\DPDRE2` VC++ 1.52c validation compiled and linked the
      resource-table packet and returned
      `DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=10 FAILURES=0 RESULT=PASS`.
- [x] The redirected console was empty and the post-run Win98 window list had
      no Program Error dialog. This closes one named Step-5 evidence unit;
      DrawProductionDlg is now `1 + P` with only E3 full GDI/caller
      integration and final promotion open.
- [x] Updated the authoritative counters from **75/152 closed, 77 open, 99
      practical** to **76/152 closed, 76 open, 98 practical**, leaving the
      **22** final promotion units unchanged. Evidence is in
      `analysis/compat/draw-production-dlg-resource-e2.{md,json}` and
      `runs/vmctl-vc152dpdre2/`.

### DrawPlanetShipList native control-layout E2 closure

- [x] Fresh `C:\DLIST` VC++ 1.52c validation compiled and linked the
      control-layout packet and returned
      `DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=7 FAILURES=0 RESULT=PASS`.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes E2: exact button-handle permutation,
      logical visibility order, combo geometry, transfer lookup/reference
      reset, detail fill, and enablement.
- [x] DrawPlanetShipList is now `3 + P`; E1 packed records, E3 text/gauge/
      detail rendering, E4 transfer/drawing integration, and final promotion
      remain. The authoritative counters are now **77/152 closed, 75 open,
      22 promotion units, 97 practical open work items**. Evidence is in
      `analysis/compat/draw-planet-ship-list-controls-e2.{md,json}` and
      `runs/vmctl-vc152dlist-controls-e2/`.

### DrawPlanShipBitmap native GDI-lifecycle E1 closure

- [x] Fresh `C:\DPSBINT` VC++ 1.52c validation compiled and linked the GDI
      packet and returned
      `DRAW_PLAN_SHIP_BITMAP_GDI_CHECKS=8 FAILURES=0 RESULT=PASS`.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes E1: brush lifecycle, ten frame
      `PATBLT` operations, palette/DIB setup, fleet-only dispatch, and fleet
      button geometry.
- [x] DrawPlanShipBitmap is now `3 + P`; E2 packed selection, E3 localized
      names, E4 complete planet integration, and final promotion remain. The
      authoritative counters are now **79/152 closed, 73 open, 22 promotion
      units, 95 practical open work items**. Evidence is in
      `analysis/compat/draw-plan-ship-bitmap-gdi-e1.{md,json}` and
      `runs/vmctl-vc152dpsb-gdi-e1/`.

### DrawPlanetStats native packed-lookup E1 closure

- [x] Fresh `C:\DPSLE1` VC++ 1.52c validation compiled and linked the
      packed-lookup packet and returned
      `DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS`.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes E1: typed load callback delivery,
      56-byte planet identity, scanner/defense bits, mineral rate, and all
      seven row consumers.
- [x] DrawPlanetStats is now `2 + P`; E3 service/resource behavior, E4 full
      GDI/original comparison, and final promotion remain. The authoritative
      counters are now **80/152 closed, 72 open, 22 promotion units, 94
      practical open work items**. Evidence is in
      `analysis/compat/draw-planet-stats-lookup-e1.{md,json}` and
      `runs/vmctl-vc152dpsle1/`.

### SatisfyOrders native DoOrders turn-sequencing replay

- [x] Fresh `C:\SYTURN` VC++ 1.52c validation compiled and linked the
      reconstructed `DoOrders`/`SatisfyOrders` turn packet and returned
      `SATISFY_ORDERS_TURN_CHECKS=8 FAILURES=0 RESULT=PASS` for both
      pre-movement and post-movement sequencing.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes the native turn-sequencing replay
      subcheck but **0** named Step-5 units: packed battle-plan/resource
      accounting, special-order integration, technology and ship-copy paths,
      salvage/message layouts, original behavior, full-turn differential, and
      promotion remain. The authoritative counters remain **91/152 closed,
      61 open, 20 promotion units, 81 practical open work items**. Evidence is
      in `analysis/compat/satisfy-orders-turn-e3.{md,json}`,
      `compat/vc152/satisfy-orders-turn-e3/`, and
      `runs/vmctl-vc152syturn/`.

### DumpFleets native production boundary replay

- [x] Fresh `C:\DFBP` VC++ 1.52c validation compiled and linked the
      production `reconstructed/src/dump_fleets.c` boundary packet and
      returned `DUMP_FLEETS_BOUNDARY_CHECKS=20 FAILURES=0 RESULT=PASS`.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes the native visibility/coordinator
      boundary replay but **0** named Step-5 units: localized scanner/task/
      minefield/battle-plan serialization, complete packed service integration,
      original output differential, and promotion remain open. The
      authoritative counters remain **91/152 closed, 61 open, 20 promotion
      units, 81 practical open work items**. Evidence is in
      `analysis/compat/dump-fleets-boundary-e3.{md,json}`,
      `compat/vc152/dump-fleets-boundary-production-e3/`, and
      `runs/vmctl-vc152dfbp/`.

### DumpPlanets native production boundary replay

- [x] Fresh `C:\DPBP` VC++ 1.52c validation compiled and linked the
      production `reconstructed/src/dump_planets.c` boundary packet and
      returned `DUMP_PLANETS_BOUNDARY_CHECKS=24 FAILURES=0 RESULT=PASS`.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes the native visibility/coordinator
      boundary replay but **0** named Step-5 units: localized derived-field
      serialization, complete packed service integration, original output
      differential, and promotion remain. The authoritative counters remain
      **91/152 closed, 61 open, 20 promotion units, 81 practical open work
      items**. Evidence is in
      `analysis/compat/dump-planets-boundary-e3.{md,json}`,
      `compat/vc152/dump-planets-boundary-production-e3/`, and
      `runs/vmctl-vc152dpbp/`.

### ExecuteReportClick native popup-caller replay

- [x] Fresh `C:\POPINTP` VC++ 1.52c validation compiled and linked the
      production `ExecuteReportClick`, `Popup`, and `DrawPopup` sources and
      returned
      `EXECUTE_REPORT_CLICK_POPUP_INTEGRATION_CHECKS=26 FAILURES=0 RESULT=PASS`.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes the native popup-caller replay
      subcheck but **0** named Step-5 units: exact original `GlobalPD` owner/
      part unions, localized content, live USER/GDI behavior, complete
      report-layout integration, original comparison, and promotion remain.
      The authoritative counters remain **91/152 closed, 61 open, 20
      promotion units, 81 practical open work items**. Evidence is in
      `analysis/compat/execute-report-click-popup-integration-e3.{md,json}`,
      `compat/vc152/execute-report-click-popup-production-e3/`, and
      `runs/vmctl-vc152popintp/`.

### FDoCoolBattle/FAttack native production battle-chain replay

- [x] Fresh `C:\CBATP` VC++ 1.52c validation compiled and linked the
      current production FDoCoolBattle, FAttack, movement, torpedo, board,
      and battle sources and returned
      `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS`.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes the native battle-chain replay
      subcheck but **0** named Step-5 units: packed token/global mapping,
      exact part lookup and damage scaling, complete battle records, full
      battle replay, original differential, and promotion remain. The
      authoritative counters remain **91/152 closed, 61 open, 20 promotion
      units, 81 practical open work items**. Evidence is in
      `analysis/compat/f-do-cool-battle-attack-integration-e3.{md,json}`,
      `compat/vc152/f-do-cool-battle-attack-production-e3/`, and
      `runs/vmctl-vc152cbatp/`.
- [x] The first native replay exposed one signed/unsigned warning in the
      `FAttack` section-index comparison. The production source now makes the
      unsigned 16-bit boundary explicit; the refreshed final2 compile/link
      logs are warning-free and `CBATRES-final2.TXT` still reports 18/18 with
      no Program Error dialog. This improves source quality but closes no
      additional named Step-5 unit.

### FAttack native complex target-score support

- [x] Added the first readable typed boundary for the raw complex-beam
      target-score arithmetic: resource plus ironium valuation, ship-count
      scaling, saturation, beam-defense reduction, shield/hull capacity,
      minimum-hull handling, and the weapon-flag shield-only path.
- [x] The strict host probe and fresh `C:\FAPNE10` VC++ 1.52c packet both
      pass `F_ATTACK_COMPLEX_SCORE_CHECKS=8 FAILURES=0 RESULT=PASS`; the
      native report records `MIN_HULL_ACTUAL=18181 EXPECTED=18181`.
- [x] The package-specific CRLF wrapper corrected the stale `C:\RUNVC.BAT`
      target, and the final compile/link pass has no Program Error dialog.
      This closes **0** named Step-5 units. The authoritative counters remain
      **91/152 closed, 61 open, 20 promotion units, 81 practical open work
      items**. Raw complex-beam caller integration, remaining capacity/jam/
      dispatch formulas, broader original differential, full replay, and
      promotion remain. Evidence is in
      `analysis/compat/f-attack-complex-score-e10.{md,json}`,
      `compat/vc152/f-attack-complex-native-e10/`, and
      `runs/vmctl-vc152fapne10/`.

### FAttack packed `_dv` target-score modifier support

- [x] Added the typed `_tok.dv` boundary: low seven bits are `pctSh`, upper
      nine bits are `pctDp`, and the raw truncating hull-reduction sequence is
      applied before the minimum-hull clamp.
- [x] The strict host probe and fresh `C:\FAPNE11` VC++ 1.52c packet pass
      `F_ATTACK_COMPLEX_SCORE_CHECKS=9 FAILURES=0 RESULT=PASS`; compile/link
      pass and the native window inventory contains no Program Error dialog.
- [x] This closes **0** named Step-5 units. The authoritative counters remain
      **91/152 closed, 61 open, 20 promotion units, 81 practical open work
      items**. Complete target-selection integration, weapon-part
      scaling/capacity/jam branches, post-selection dispatch, original
      differential, full replay, and promotion remain. Evidence is in
      `analysis/compat/f-attack-defense-modifier-e11.{md,json}`,
      `compat/vc152/f-attack-complex-native-e11/`, and
      `runs/vmctl-vc152fapne11/`.

### FAttack complex target-selection integration support

- [x] Connected the raw complex score to the readable `stars_f_attack`
      candidate loop behind the explicit nonzero `raw_scaling` field. The
      selected candidate is score-ranked, but the damage callback still
      receives ordinary beam damage.
- [x] Fresh `C:\FAPNE12` VC++ 1.52c validation passes
      `F_ATTACK_COMPLEX_SELECTION_CHECKS=3 FAILURES=0 RESULT=PASS` with clean
      compile/link status and no Program Error dialog.
- [x] This closes **0** named Step-5 units. The authoritative counters remain
      **91/152 closed, 61 open, 20 promotion units, 81 practical open work
      items**. Part-dependent scaling, capacity/jam behavior, distance and
      shield/hull dispatch, original differential, full replay, and promotion
      remain. Evidence is in
      `analysis/compat/f-attack-complex-selection-e12.{md,json}`,
      `compat/vc152/f-attack-complex-native-e12/`, and
      `runs/vmctl-vc152fapne12/`.

### FAttack raw beam dispatch scaling support

- [x] Added the first raw post-selection beam-damage sequence for
      `raw_scaling` sections: attacker capacity, target beam defense, and
      truncated distance/part-DP attenuation before the damage callback.
- [x] Fresh `C:\FAPNE13` VC++ 1.52c validation passes
      `F_ATTACK_COMPLEX_SELECTION_CHECKS=3 FAILURES=0 RESULT=PASS`; compile/
      link pass and no Program Error dialog were observed.
- [x] This closes **0** named Step-5 units. The authoritative counters remain
      **91/152 closed, 61 open, 20 promotion units, 81 practical open work
      items**. Part-dependent scaling, remaining capacity/jam branches,
      shield/hull follow-up dispatch, complex torpedo selection, original
      differential, full replay, and promotion remain. Evidence is in
      `analysis/compat/f-attack-beam-dispatch-e13.{md,json}`,
      `compat/vc152/f-attack-complex-native-e13/`, and
      `runs/vmctl-vc152fapne13/`.

### FAttack native packed lookup and typed-scaling support

- [x] Fresh `C:\FAPNE9` VC++ 1.52c validation compiled and linked the
      current production `lookup_part.c` and `f_attack.c` sources, with clean
      compile/link logs and no Program Error dialog.
- [x] The native reports pass the fixed-table metadata matrix, packed `_hs`
      resolver boundary, and current typed FAttack scaling matrix:
      `LOOKUP_PART_TABLE_CHECKS=34`, `F_ATTACK_PACKED_PART_CHECKS=13`, and
      `F_ATTACK_CHECKS=34`, all with zero failures.
- [x] This closes the native lookup/resolver subcheck and supplies native
      support for the current typed scaling matrix, but closes **0** named
      Step-5 units. The raw complex-beam hull/resource/shield/jam/capacity
      formulas remain to be represented and compared before E1 can close;
      E2 broader original damage differentials, E3 native/full battle replay,
      and Step-6 promotion also remain. The authoritative counters remain
      **91/152 closed, 61 open, 20 promotion units, 81 practical open work
      items**. Evidence is in
      `analysis/compat/f-attack-packed-native-e9.{md,json}`,
      `compat/vc152/f-attack-packed-native-e9/`, and
      `runs/vmctl-vc152fapne9/`.

### ReportDlg native typed paint-view replay and teardown defect

- [x] A production-source packet in `C:\RPTPVP` compiled and linked and
      reached the typed `StarsReportDlgDrawView` callback with
      `REPORT_DLG_PAINT_VIEW_CHECKS=6 FAILURES=0 RESULT=PASS` written before
      termination.
- [x] The ordinary `WinMain` return path produced a Win98 illegal-operation
      dialog and exit 130. An explicit `ExitProcess` harness termination then
      exited 0 with the same report and no Program Error dialog. This is
      native supporting evidence plus a reproducible teardown/ABI defect, not
      a named Step-5 closure; the authoritative counters remain **91/152
      closed, 61 open, 20 promotion units, 81 practical open work items**.
      The screenshot and both run variants are preserved in
      `analysis/compat/report-dlg-paint-view-e2.{md,json}` and
      `runs/vmctl-vc152rptpvpe2/`.

### ReportDlg native production packed-view E1 closure

- [x] Fresh `C:\RPTVEP` VC++ 1.52c validation compiled and linked the
      production `reconstructed/src/report_dlg.c` bridge and returned
      `REPORT_DLG_VIEW_CHECKS=13 FAILURES=0 RESULT=PASS`.
- [x] The process exited normally and the post-run Win98 window list had no
      Program Error dialog. This closes E1 packed `_rpt` view loading. The
      remaining ReportDlg Step-5 units are populated report unions/rows plus
      live USER/GDI/menu payloads, native/original caller integration, and
      final promotion. ReportDlg is now `1/3` named Step-5 units complete,
      represented as `2 + P` remaining.
- [x] The authoritative counters are now **91/152 closed, 61 open, 20
      promotion units, 81 practical open work items**. Evidence is in
      `analysis/compat/report-dlg-view-e1.{md,json}`,
      `compat/vc152/report-dlg-view-e1-production/`, and
      `runs/vmctl-vc152rptvep/`.

### FAttack residual torpedo loop — current-source native checkpoint E25/E27

- [x] Fresh E25 Win98 validation compiles and links the current production
      `FAttack` source. Native reports pass raw beam damage (9), torpedo target
      selection (5), raw shield/hull dispatch and residual-budget behavior
      (9), packed-part lookup (13), table metadata (34), complex score (9),
      and complex selection (3), all with zero failures. The native process
      exits 0 and the post-run window inventory has no Program Error.
- [x] Corrected two harness issues without overwriting failed returns: E24's
      `/NOD` link lacked a no-op `printf` shim, while E25's copied scaling
      fixture still expected pre-residual-loop hit counts. The current host
      `tests/f_attack_source_probe.c` passes natively in separate E27
      (`COMPILE=PASS`, `LINK=PASS`, process exit 0,
      `F_ATTACK_CHECKS=34 FAILURES=0`). E26 is retained as a failed reduced
      package missing `TORP.C`; E27 includes the dependency.
- [x] Full host verification passes: 519 tests, 153 expected skips, 2,241
      layout assertions, and 846 declarations; VC152 package checks pass 3/3;
      `git diff --check` passes. Evidence is in
      `analysis/compat/f-attack-torpedo-residual-native-e25.{md,json}`,
      `compat/vc152/f-attack-complex-native-e25/`,
      `compat/vc152/f-attack-source-native-e27/`, and the matching
      `runs/vmctl-vc152fapne25/` and `runs/vmctl-vc152fapne27/` returns.
- [ ] This closes **0 named Step-5 units**. `FAttack` remains at Phase-2
      step 5/6: runtime-backed group-0x20 record values, original-behavior
      differential for the split/residual sequence, and complete native
      battle replay remain before Step 5 can close. The top-level authoritative
      counters remain **120/152 named Step-5 units closed, 32 open, 10
      promotions, and 42 practical open work items**; this isolated regression
      does not change them.

### Latest checkpoint — FAttack original group-0x20 torpedo table E29/E30/E31

- [x] Resolved E22's address ambiguity against the original instruction
      sequence: `FLookupPart` forms `CS:(0x2180 + item*0x3c)`. CodeView puts
      `rgtorp` at NE 2:`0x2180`; `_torp` is 60 bytes and `dp` is at offset 54.
      The twelve original table values are `5, 12, 26, 48, 90, 169, 316, 60,
      85, 150, 280, 525`. This corrects the earlier false overlay claim that
      conflated NE segment 2 with DGROUP/autodata segment 37.
- [x] Production `stars_lookup_torpedo_dp` now reads the evidence-backed
      group-0x20 table and FAttack uses the resolved original value after the
      generic packed-part callback. Strict-C89 dispatch checks all twelve
      values, rejects item 12, and proves item 8 uses `dp=85` rather than the
      fixture's 16. Host result: `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=11
      FAILURES=0 RESULT=PASS`.
- [x] Corrected the original table metadata: `FLookupPart` stores `CS` for
      group `0x20`, so `StarsLookupPartInput` now distinguishes its dynamic
      current-CS selector from literal `0x10e0` selectors. Table-family names
      now match the CodeView evidence (`0x08` armor, `0x20` torpedo). The
      strict-C89 lookup matrix passes 38 checks for selector, offset, stride,
      and component identity.
- [x] Fresh E29 VC++ 1.52c/Win98 build and replay pass: compile/link PASS;
      raw damage 9/9, torpedo target 5/5, torpedo dispatch 11/11, packed part
      13/13, lookup metadata 34/34, complex score 9/9, and complex selection
      3/3. Fresh E30 compiles/runs current `tests/f_attack_source_probe.c`
      with current `FA.C` and `LOOKUP.C` and reports
      `F_ATTACK_CHECKS=34 FAILURES=0`. Fresh E31 compiles/links the corrected
      current sources and passes lookup 38/38, raw damage 9/9, torpedo target
      5/5, dispatch 11/11, current source 34/34, and packed part 13/13. The
      raw replay exits 0 and final window inventory has no Program Error. The
      full host suite passes: 519 tests, 153 expected
      skips, 2,241 layout assertions, and 846 declarations. E28's missing
      native lookup dependency and E29's stale copied scaling fixture failure
      remain preserved, not counted as passes.
- [ ] Supporting evidence only; this closes **0 named Step-5 units**.
      `FAttack` remains 0/3 in Step 5 and 5/6 overall: complete raw complex-
      beam battle-part scaling/table acceptance, broader original damage
      differentials, and full native battle replay remain. Counters remain
      **120/152 evidence units closed, 32 open, 10 promotion units, and 42
      practical open work items**. Evidence:
      `analysis/compat/f-attack-torpedo-table-native-e29.{md,json}` and
      `runs/vmctl-vc152fapne29/`, `runs/vmctl-vc152fapne30/`,
      `runs/vmctl-vc152fapne31/`.

### Latest checkpoint — FAttack original part-record integration E42

- [x] Added an optional `lookup_part_record` seam to `stars_f_attack`.
      When supplied, the attack path reads the selected beam/torpedo's
      `dRangeMax`, `dp`, and `init` words at original record offsets 52, 54,
      and 56; it rejects absent or undersized records. The no-record callback
      path continues to use the established typed lookup helpers.
- [x] The strict-C89 regression loads the original unrelocated NE segment-2
      image, mutates each record field to prove it drives attack dispatch, and
      rejects a 57-byte span. Host report:
      `F_ATTACK_PACKED_PART_CHECKS=20 FAILURES=0 RESULT=PASS`.
- [x] Fresh `C:\FAPNE42` VC++ 1.52c/Win98 build reports compile/link PASS;
      native execution reports the same 20/20 result and exits 0. The captured
      window inventory and screenshot contain no Program Error dialog. E40's
      unresolved link symbol and E41's duplicate stub remain preserved as
      failed attempts, not passes.
- [x] Evidence and source package are in
      `analysis/compat/f-attack-original-part-record-native-e42.{md,json}`,
      `compat/vc152/f-attack-packed-record-native-e42/`, and
      `runs/vmctl-vc152fapne42/`.
- [x] The full host suite passes 524 tests, 153 expected skips, 2,241 layout
      assertions, and 846 declarations; baseline and diff checks pass.
- [ ] Supporting evidence only; this closes **0 named Step-5 units**.
      `FAttack` remains 0/3 at Step 5 and 5/6 overall. This uses a copied
      original executable segment, not live CS mapping. Broader original
      damage differentials, complete weapon target/damage acceptance, full
      native battle replay, and promotion remain. Counters remain **120/152
      evidence units closed, 32 open, 10 promotion units, and 42 practical
      open work items**.

### Latest checkpoint — FAttack beam and torpedo part-record integration E45

- [x] Extended the record-backed FAttack regression through the group-0x20
      torpedo path. Original item 4 has `range=5`, `dp=90`, and `init=2`; with
      `initBase=7`, initiative 9, and one guaranteed hit it produces 45 hull
      damage. Mutated `dp=120` produces 60; range zero and initiative one each
      prevent dispatch. Host and native probes both report
      `F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0 RESULT=PASS`.
- [x] The host probe now links production `c_torp_hit.c`. Fresh
      `C:\FAPNE45` VC++ 1.52c compile/link pass; native process exits 0 and the
      post-run screenshot/window inventory contains no Program Error dialog.
- [x] Full host suite: 524 tests pass, 153 expected skips, 2,241 layout
      assertions, and 846 declarations; baseline and diff checks pass.
- [x] Evidence is in
      `analysis/compat/f-attack-original-part-record-native-e45.{md,json}`,
      `compat/vc152/f-attack-packed-record-native-e45/`, and
      `runs/vmctl-vc152fapne45/`. E43's drive-relative wrapper and E44's stale
      20-check report label remain preserved but are not counted as passes.
- [ ] Supporting evidence only; this closes **0 named Step-5 units**.
      `FAttack` remains 0/3 at Step 5 and 5/6 overall. The fixture is a copied
      static segment, not live CS mapping. Broader original damage
      differentials, complete scaling/target/damage acceptance, full native
      battle replay, and promotion remain. Counters remain **120/152 evidence
      units closed, 32 open, 10 promotion units, and 42 practical open work
      items**.

### Latest checkpoint — FAttack raw torpedo record and residual sequence E46

- [x] Routed the multi-pass/residual torpedo regression through the raw
      group-0x20 part-record callback and the original unrelocated NE segment
      2. Item 8 resolves at `0x2360` with `range=5`, `dp=85`, and `init=0`;
      the scripted sequence preserves the expected 85/42 damage events and
      residual packet transition 1→0. Host result:
      `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=0 RESULT=PASS`.
- [x] Fresh E46 Win98/VC++ 1.52c package compiles and links both current-source
      executables. Packed-record test passes 24/24, residual torpedo test
      passes 17/17, both processes exit 0, and no Program Error appears.
- [x] Full host suite passes 524 tests, 153 expected skips, 2,241 layout
      assertions, and 846 declarations; baseline and diff checks pass.
- [x] Evidence is in
      `analysis/compat/f-attack-torpedo-record-residual-native-e46.{md,json}`,
      `compat/vc152/f-attack-packed-record-native-e46/`, and
      `runs/vmctl-vc152fapne46/`.
- [ ] Supporting evidence only; this closes **0 named Step-5 units**.
      `FAttack` remains 0/3 at Step 5 and 5/6 overall. The fixture is a copied
      static segment and random draws are scripted; broader original-executable
      damage differentials, full native battle replay, and promotion remain.
      Counters remain **120/152 evidence units closed, 32 open, 10 promotion
      units, and 42 practical open work items**.

## Phase 4 — promotion and whole-program acceptance

- [ ] Promote each completed procedure only after its source, probe, native
      evidence, and ledger record agree.
- [ ] Re-run all affected host probes after shared-header or layout changes.
- [ ] Re-run native VC++ 1.52c package tests for each affected subsystem.
- [ ] Expand native application validation beyond START2A title/race
      acceptance to new/open/continue game, orders, turn generation, combat
      playback, reports, tutorial, and error recovery.
- [ ] Integrate every required subsystem under the recovered startup,
      message, turn, and termination paths.
- [ ] Close CRT data and common PAD/EPAD placement issues; code-member identity
      alone is not sufficient for data/link layout.
- [ ] Audit callbacks, pointer tables, entry points, indirect targets, and
      runtime dispatch.
- [ ] Reconcile every uncovered executable range as code, embedded data,
      padding, or a genuinely missing function.
- [ ] Validate all 122 resource payloads in their actual dialogs, strings,
      menus, and command paths.
- [ ] Retain no-procedure module records and prove that their data and runtime
      behavior are accounted for.
- [ ] Run whole-program original-versus-reconstructed comparisons and record
      unresolved behavior-affecting uncertainties.

## Evidence and references

- [Current reconstruction handoff](analysis/reconstruction/current-handoff.md)
- [Macinti native validation](analysis/compat/macinti-native.md)
- [Function-by-function checklist](TODO5.md)
- [Coverage ledger](analysis/coverage.json)
- [Review ledger](tracking/reviews.json)
- [TODO5 renderer](tools/render_todo5.py)

Completion of this document means the source has reached the review boundary;
it does not promise a byte-identical final executable. That remains the
separate optional milestone described in TODO.md.
