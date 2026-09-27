# DoRototillAiTurn RT-E3 — three-order packed state

The recovered fleet pass at NE 20:~2300–~24A0 has two bounded paths in
the current source. For a slot-7/8 fleet away from a planet with at least
two orders, the second order's ID (PLORD offset 26) selects the unowned
destination and marks `vlpbAiPlanet[id][1] |= 0x80`. For an occupied
current planet, the caller selects the fleet, writes `cord=1` and
`PLORD.iordMac=1`, refreshes, and clears AI task zero.

The [11-check strict-C89 probe](../../tests/do_rototill_multi_order_e3_probe.c)
uses three 18-byte orders. It compares all 58 PLORD bytes before/after the
marker path (unchanged), confirms only the expected AI-table marker byte
changes, then compares all 58 bytes after occupied repair (only the count
byte changes from 3 to 1; the inactive second and third bodies remain).
It verifies select → refresh → clear order and select failure before order
mutation. Planet visit markers are included in the expected AI-table diff.

Fresh VC++ 1.52c package `compat/vc152/rototill-production-e1-r10/` ran in
StarDev at `C:\RTE1N10`. The returned
[`BLDSTAT.TXT`](../../runs/rototill-production-e1-r10/BLDSTAT.TXT) says
compile/link PASS; [`RTE1RES.TXT`](../../runs/rototill-production-e1-r10/RTE1RES.TXT)
reports 20 production, 17 fleet, 12 live queue, and 11 multi-order checks,
all PASS (60/60). Process exit was 0. Logs, map, six objects, executable,
report, and agent log are retained in the same return directory.

At the r10 checkpoint, RT-E3 remained open: the transport reroute at
NE 20:~24F5, the later first-order kind change, and the colonizer AI-table
byte-15 branch had not been bound. The r10 probe also did not establish
original live pointer identity.

## Transport reroute continuation, 2026-09-26

The bounded source now follows the raw transport reroute at NE 20:~24F5
for a present foreign destination. It reads the second-order target only
when that order's object group is 1, requires AI-table byte 3 and positive
fleet cargo in `rgwtMin[3]` (packed fleet offset 88), and checks the target
race stat is not 8. It then constructs the zeroed 18-byte order with the
destination point and ID, flags `0x1101`, and word `0x2000` at order offset
14. If the first order already targets the destination, it replaces that
record; otherwise it replaces the second. The packed order count remains
unchanged. After select/write/refresh, it marks AI-table byte 3 with `0x80`.

The [14-check strict-C89 probe](../../tests/do_rototill_transport_reroute_e3_probe.c)
compares every byte of a three-order PLORD in both replacement cases and
tests race-8, zero-cargo, missing-service, and out-of-range target guards.
The last guard rejects malformed input before any PLORD, AI-table, or fleet
flag write. Fresh r12 at `C:\RTE1N12` compiled and linked under VC++ 1.52c.
Its [`RTE1RES.TXT`](../../runs/rototill-production-e1-r12/RTE1RES.TXT)
passes 20 production + 17 fleet + 12 live-queue + 11 multi-order + 14
reroute checks (74/74), with build logs, map, objects, executable, and
agent log retained. r11's earlier 72/72 report remains preserved.

The original branch also permits a target absent from the loaded planet
list, whose race-stat dereference is not safely recovered here. The
`FIsAiTransport`, `GetRaceStat`, and selected pointer identities are still
callback bindings, and no original-state differential was captured. The
later first-order kind and colonizer-marker branches were unbound at r12.

## Deep-space first-order kind continuation, 2026-09-26

The second fleet sweep at NE 20:~29D0 has a bounded deep-space fallback:
an owned fleet with no slot-7/8 ships, `cord < 2`, a ship in slot 1,
`idPlanet == -1`, zero `rgwtMin[3]`, and ship-1 first hull-slot byte at
most 1 skips `FMoveToNearestStarbase`. After selecting the fleet, it changes
only the low kind nibble of the first packed PLORD order to 5 and refreshes.
The rest of the three-order record and AI table remain byte-identical.

The [10-check strict-C89 probe](../../tests/do_rototill_deep_space_first_kind_e3_probe.c)
checks the complete 58-byte before/after PLORD, AI-table preservation,
select/refresh call order, the hull/cargo/ship/owner exclusions, and malformed
PLORD rejection before fleet-flag mutation. Fresh r13 at `C:\RTE1N13`
compiled, linked, and passed
[`RTE1RES.TXT`](../../runs/rototill-production-e1-r13/RTE1RES.TXT):
20 production + 17 fleet + 12 live queue + 11 multi-order + 14 reroute +
10 first-kind checks (84/84). Build status/logs, map, objects, executable,
report, and agent log are retained.

At the r13 checkpoint, the selected-planet, starbase-service, and colonizer
branches of the second sweep still needed binding. The transport
absent-target and selected-pointer identity questions, live services, and
original before/after state also remained. RT-E3 stayed open.

## Selected-planet and starbase continuation, 2026-09-26

The fallback now reads the selected current planet's original 56-byte record.
For owned planets, mineral word `rgwtMin[3]` must be below 50 and starbase
bit 9 clear; foreign or unowned current planets pass those two source tests.
The fleet's cargo word 3 must be zero. The source selects the fleet once
before the optional `FMoveToNearestStarbase(fleet, 0)` call; if that service
does not route it, it selects again, changes only the first PLORD kind nibble
to 5, and refreshes. The deep-space path now follows this same two-selection
sequence, correcting r13's single selection.

The expanded [19-check strict-C89 probe](../../tests/do_rototill_deep_space_first_kind_e3_probe.c)
tests whole three-order preservation, the selected-planet mineral and
starbase exclusions, foreign-planet bypass, both nearest-starbase results,
and malformed-binding rejection before a fleet flag write. Fresh r14 at
`C:\RTE1N14` compiled, linked, and ran in StarDev. Its
[`RTE1RES.TXT`](../../runs/rototill-production-e1-r14/RTE1RES.TXT)
passes 20 production + 17 fleet + 12 live queue + 11 multi-order + 14
reroute + 19 first-kind checks (93/93). Build logs, map, objects,
executable, report, and agent log are retained.

The nearest-starbase action is still a callback, and no original before/after
state has established its selected pointer identity. The colonizer marker
and other second-sweep routes remain open; RT-E3 stays open.

## Colonizer AI-byte-15 marker continuation, 2026-09-27

The recovered original `DoRototillAiTurn` listing at NE 20:~29D0 selects
the owned fleet before `IdNearestColonizablePlanet`. When the fleet is at an
owned planet, it selects again, calls `XferAiSupply`, and refreshes the fleet.
For a nonnegative target, it then calls `FColonizeAiFleet` and writes `4` to
AI-planet record byte 15 at that target. The marker write does not test the
colonization helper's result. The source caller now binds this bounded
owned-planet path through a typed callback and checks target bounds before
any supply or marker mutation.

The [9-check strict-C89 probe](../../tests/do_rototill_colonizer_marker_e3_probe.c)
checks select → nearest → select → supply → refresh → colonize order, the
marker value before and after the callback, the complete AI-table diff,
unchanged 58-byte three-order `PLORD`, absent and invalid target behavior,
missing-service preflight, and wrong-owner exclusion. Fresh VC++ 1.52c
package `compat/vc152/rototill-production-e1-r16/` ran at `C:\RTE1N16`.
Returned [build status](../../runs/rototill-production-e1-r16/BLDSTAT.TXT)
reports compile/link PASS, and the
[native report](../../runs/rototill-production-e1-r16/RTE1RES.TXT) passes
112/112 combined checks, including 9/9 marker checks. Compile logs, link
log, map, objects, executable, and agent log are retained in that unique
return directory. The focused host test and package test pass.
The retained-input full source turn regression also passes after the opt-in
branch was added (one test, 43.323 seconds).

This closes RT-E3.a's marker-write unit. Other second-sweep route conditions,
the live `FColonizeAiFleet` binding and selected-pointer identity, and an
original action-bearing internal `PLORD`/AI-table before/after comparison
remain in RT-E3.b/c/d. Parent RT-E3 stays open.
