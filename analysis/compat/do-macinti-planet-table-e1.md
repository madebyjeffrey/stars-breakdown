# DoMacintiAiTurn MA-E1: bounded planet-table selection

Status: supporting source and host evidence only; **MA-E1 remains open**.

## Upstream selected-list composition (2026-09-26)

The host probe now passes **41/41** checks after composing the production
`stars_init_random_planet_list` result with Macinti's bound far-pointer input.
The [retained host report](do-macinti-planet-list-composition-e1-test.txt)
records `MACINTI_PLANET_TABLE_E1_CHECKS=41 FAILURES=0 RESULT=PASS`;
`python3 -m unittest tests.test_macinti_planet_table_e1 -q` also passes.
It verifies that the source initializer selects only current owner-2 planets:
the earlier two-ID synthetic fixture included an unowned planet and therefore
cannot stand in for the original upstream list. With one owned planet, the
bound Macinti pass marks only that record. After a second planet becomes
owned, a controlled `Random(2)=1` swaps their order; Macinti consumes the
resulting two far-pointer tokens in that order and marks both. The test uses
the source initializer in `reconstructed/src/ai.c` and the source Macinti
turn, without copying an expected selected-ID list into either service.

This is host composition evidence. The retained r27 native packet remains
37/37 and does not include these four checks. Original live pointer capture,
the rest of the planet/fleet-table lifecycle, and action-bearing downstream
original comparison still prevent MA-E1 closure.

The original `10a0:0008` body first clears byte 14 of each 16-byte
`vlpbAiPlanet` record for `game.cPlanMax` slots, then marks bit 0 for
qualifying owned fleets' second-order targets. It next clears byte 13
of each AI record and marks each selected
`vrglpplAi` planet by ID, scans the full planet table for foreign owners,
and writes threat byte 10 and visited byte 9. The threat is
`min(6, (overlap_18 & 0x0fff)/250 + 1)` plus bit 9 of `overlap_4`. Its
subsequent planet action loop follows the selected pointer list, not the
full planet table.

The optional `planet_table_bound` input now exposes 56-byte raw planets,
124-byte raw fleets, a bounded 16-byte AI table, and an ordered selected
planet list. A new opt-in byte-exact `vrglpplAi` binding reads four-byte
offset:segment tokens and resolves them against caller-supplied guest far
addresses for the planet records; the earlier ID list remains a transitional
fallback. The original loop stops at the first null far pointer, even when
`vclpplAi` is larger. It rejects truncated, unmatched, and ambiguous
non-null entries before AI-table writes. It validates record lengths, IDs, and table
bounds before the turn callbacks. After the design-status service
has returned the 10/11 mission design, it preflights any qualifying
second PLORD record before AI-table writes. The fleet marker reads raw
owner, slots 10/11, planet ID, `cord`, and cargo at offsets 2, 32/34,
6, 98, and 88/90; the second order target and MOVE-kind flags are at
PLORD offsets 26 and 28. The source then clears/marks byte 14, binds
planet owner/starbase/population/mineral fields, clears/marks byte 13,
updates foreign threat/visited bytes, and visits selected planets in
supplied order. The unbound typed path remains available.

The bound selected-planet path now also checks the original outer
`10a0:09a3–0a05` action gate before invoking its production adapter:
starbase bit 9 at offset 4, nonnegative signed high word of
`rgwtMin[3]` at offset 42, either a positive high word or an unsigned
low word of at least 200 at offset 40, nonzero `isb` low nibble at
offset 44, and a turn before 26 when `isb == 1`. The original does not
apply an explicit owner test at this point; the bound path therefore uses
this packed gate instead of the unbound typed owner gate. Subsequent
`ChangeMainObjSel`, live PLPROD and `InitProduction`/`FinishProduction`
effects are represented by the bounded lifecycle below, but the later
queue-construction branches are still incomplete.

At original `10a0:0a08–0b00`, a planet that passes the outer gate is
selected first. A non-null original `lpplprod` pointer (planet offsets
52–55) with `iprodMac >= 24` skips `InitProduction`. Otherwise the
source calls `InitProduction`, obtains the **post-init** live PLPROD
allocation, and scans its four-byte entries. A type-2 item with design
index below 16 calls `FinishProduction(0)` and bypasses further
construction. The caller supplies a bounded existing allocation and a
current-queue callback rather than dereferencing guest far-pointer bytes
on the host. Missing lifecycle callbacks and malformed existing queues
are rejected before AI-table writes; malformed post-init results fail
after the select/init callbacks, without claiming rollback of their
effects. The normal no-bypass path still delegates the many subsequent
resource/RNG/add-item decisions to the typed production adapter. It now
calls `FinishProduction(0 or 1)` after that normal path, with 1 only when
the represented plan queued an item; an explicit early-finish plan is not
finished twice. The packed selected-planet path no longer uses generic
typed queue-limit, resource-affordability, or hull-batch-affordability fields
as extra entry/queue predicates. The original live-queue 24-item and
packed starbase/population gates still apply. The unbound typed path retains
its earlier policy and affordability checks.
The bound selected-planet loop now runs independently of the optional
`visit_planet_typed` observer. The observer still sees the packed view
when installed, but its absence no longer suppresses selection, the
live-queue scan, or production dispatch. The unbound compatibility path
keeps its prior callback-driven behavior.

The strict-C89 host probe `tests/do_macinti_planet_table_e1_probe.c`
passes **37/37** checks: the prior selected-ID/foreign-threat cases,
positive second-order marking, no-design/no-ships/no-cargo/non-MOVE/
foreign-owner exclusions, short/out-of-range packed-input rejection,
and all five outer production-gate exclusions plus the turn-25
kind-1 inclusion. It also checks select/init/current/finish call order,
the pre-existing 24-item skip, design-15 bypass versus design-16
continuation, malformed/missing queue bindings, and normal/bypass
behavior with no observation callback, and raw far-pointer order and
malformed/ambiguous-pointer preflight and first-null termination.
The earlier typed Macinti probe still passes 9/9. The DOS-8.3/CRLF
VC1.52c packet at `compat/vc152/macinti-planet-table-e1-r24/`
ran in `C:\MAE1N24` with separate compile/link logs, status, and the 31-check
E1 report combined with a 19-check pressure queue, 39-check attack gate,
12-check E2 fleet,
10-check first-transport, 16-check buddy/miner, 8-check E3 waypoint,
and 14-check E3 order probe. The r23 packet compiled but failed to link
because the Win16 Windows CRT did not resolve probe-only `printf`. The r24
driver discards those console summaries; the individual probe return values
and file report still determine pass/fail. The live Win98 agent compiled and
linked r24, and `runs/vmctl-mae1n24/MAE1RES.TXT` reports all eight probes
passing. Build status, logs, executable and map are preserved alongside it.
The subsequent r25 transfer lost `MAE1.C` during an agent reconnect and
recorded `COMPILE=FAIL`, `LINK=SKIP`; its failure logs are retained in
`runs/vmctl-mae1n25/`. Fresh r26 explicitly listed all 18 guest files
before build; `runs/vmctl-mae1n26/BLDSTAT.TXT` records compile/link PASS,
and `MAE1RES.TXT` records all eight probes passing, including 36/36
planet-pointer checks. The executable, map, compiler logs, linker log, and
agent log are retained in that return directory.
Inspection of the original loop at `10a0:0008` revealed that r26's
null-pointer rejection was too strict. Fresh r27 implements the original
first-null break, lists all 18 guest files before build, and passes both
compile/link (`runs/vmctl-mae1n27/BLDSTAT.TXT`) and the eight-probe
report (`MAE1RES.TXT`, planet 37/37). Executable, map, and logs are retained.
The packet layout/CRLF/command-length/overwrite-guard test passes. The
full host suite passed **575 tests, 153 expected skips, zero failures**;
baseline and diff checks are clean.

Remaining MA-E1 acceptance: bind the actual original pointer-list and
complete planet/fleet table lifecycle, verify downstream
production/selection predicates and writes, and compare meaningful action-bearing state
against the original. The pointer adapter is caller-supplied; no live
original `vrglpplAi` memory capture or action-bearing differential has yet
proved its source identity. Focused r27 native checks do not establish full
live Win16 behavior.
