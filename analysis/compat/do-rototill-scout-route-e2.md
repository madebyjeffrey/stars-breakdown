# Rototill retained scout route composition

Status: **retained-input host route composition passes; RT-E2 remains open**.

The [selector-3 original witness](do-rototill-original-turn-r1.md) fixes
fleet 0 at planet 24 with 300 fuel and one design-0 ship. Its player-view M2
loads only planet 24; the 32-point XY map has planet 26 nearest to that
source among the 31 unknown-status slots. The original turn-1 compact order
targets planet 26 at `(1313,1262)`, warp 5.

`stars_rototill_target_scout` now implements the scout service
behind the Rototill caller's source gates. It prepares byte 15 in the live
16-byte AI planet table through the shared colonizable helper, calls the
shared strided nearest-unknown helper with the original RNG/wormhole
decision boundary, requests ideal warp, selects the fleet again, writes a
complete 18-byte MOVE PLORD order through the shared writer, refreshes the
packed fleet/count, and marks a planet target status 4 before warp and move.
For a selected wormhole it uses the thing point/id and `0x1800` route flags
without marking a planet. When no unknown planet or wormhole is selected,
it asks the home-planet best-target service, then draws `Random(cPlanMax)`
if no home or best target exists. An armed scout can route to a nearby enemy
fleet, subject to the human-only filter and the reserved-attack `Random(3)`
gate; this writes `0x1200` route flags without preparing or marking a planet.
The `FFleetMightHaveTeeth` callback is invoked before target search even
when the candidate list is empty. The current probe derives its result
through `stars_fleet_might_have_teeth` from retained design 0 and the
original EXE hull-4 slots; it is true for this scout.
If the human-only pass finds no enemy, it retries with the filter removed.
For a distant enemy it compares current fuel with half fuel capacity, tries
the nearest-starbase service when low, then chooses the nearest loaded planet
to that enemy unless another attack has reserved it. If that planet is the
fleet's current planet or none is eligible, it falls through to unknown
scouting. It rejects an unavailable PLORD growth service.
When a selected fleet has only one
PLORD slot, the adapter calls an explicit capacity service, uses its returned
allocation for the second order, and refreshes the fleet through that pointer.

`tests/test_rototill_scout_route_e2.py` generates a C fixture from the pinned
M2/XY and turn-1 M2 records rather than hardcoding the map. Its strict-C89
probe runs the production Rototill caller plus this adapter with an
outer `Random(100)` result of 99, and with 0 followed by a null wormhole
lookup. Both choose planet 26; the full second PLORD record has point
`(1313,1262)`, target 26, flags `0x1150`, and zero reserved bytes. It checks
the untouched first and third order bytes, fleet/PLORD count 2, target AI
status 4, the other 30 unknown statuses, and callback order. The revised
fixture derives design 0's engine 1 and its twelve fuel-use values from the
pinned original EXE, then calls the reconstructed `stars_find_ideal_warp`.
It returns warp 5 with exactly one engine lookup. It also supplies the four
retained HST wormhole bodies to `stars_lpth_worm_find`; all lie outside the
`4 * 545` distance bound, so both `Random(100)` outcomes select planet 26.
The expanded probe checks both preallocated and one-order PLORD cases for
each draw, including growth callback order, preservation of the old record,
new-pointer refresh, and the full second-order bytes. A synthetic nearby
wormhole adds two diversion cases, with and without PLORD growth. The probe
also checks that ideal-warp selection observes the planet reservation before
the order write and that a wormhole route does not reserve a planet. Three
controlled no-unknown cases cover no home, a best home target, and best-home
failure followed by a random planet. Four controlled enemy cases cover a
nearby unreserved attack, a reserved attack skipped or accepted by the draw,
and a nonhuman enemy reached by the unfiltered retry. Four distant-enemy
cases cover same-planet fallback, a nearer loaded planet, failed starbase
move followed by that planet, and successful starbase diversion without a
scout order. A fifth distant case reserves the nearer planet for another
attack and confirms fallback to an unknown planet. They check the complete
second order, AI status, and callback sequence. **180/180 host checks pass.** The actual
original RNG draw is still not captured.

The first fresh Win16 build, [r1](../../runs/rototill-scout-route-e2-r1/),
stopped at `UTILGEN.C` because its `seed_table.inc` dependency was omitted;
the partial compiler logs are retained. The corrected fresh
[r2 native return](../../runs/rototill-scout-route-e2-r2/) packages that
include as DOS `SEEDTAB.INC` with an include-name-only source conversion.
VC++ 1.52c reports `COMPILE=PASS`, `LINK=PASS`; the Win16 probe process
exited 0 and `RTE2RES.TXT` says `ROUTE_CHECKS=16 RESULT=PASS`. The manifest
pins package and then-current source hashes, the report, logs, map, objects,
executable, and agent log. No original Stars! run was made.

The revised fresh [r3 native return](../../runs/rototill-scout-route-e2-r3/)
adds the source-derived engine/wormhole fixture and `find_ideal_warp.c`.
VC++ 1.52c again reports `COMPILE=PASS`, `LINK=PASS`; the Win16 probe exits
0 and `RTE2RES.TXT` says `ROUTE_CHECKS=18 RESULT=PASS`. Its manifest pins
its source revision and all returned build artifacts.

The fresh [r4 native return](../../runs/rototill-scout-route-e2-r4/)
covers PLORD growth and relocated-pointer refresh. VC++ 1.52c reports
`COMPILE=PASS`, `LINK=PASS`; the Win16 probe exits 0 and `RTE2RES.TXT`
says `ROUTE_CHECKS=40 RESULT=PASS`. Its manifest pins the package,
source, report, build logs, map, objects, executable, and agent log.
The three related host test modules pass 6/6. No new original Stars! run
was made.

The fresh [r5 native return](../../runs/rototill-scout-route-e2-r5/)
adds the synthetic wormhole diversion and source-order planet reservation.
VC++ 1.52c reports `COMPILE=PASS`, `LINK=PASS`; the Win16 probe exits 0
and `RTE2RES.TXT` says `ROUTE_CHECKS=60 RESULT=PASS`. The manifest pins
all package and returned artifacts and current source hashes. The retained
original HST has no wormhole within this route's limit, so r5 establishes
the branch's native behavior, not an original wormhole outcome.

The fresh [r6 native return](../../runs/rototill-scout-route-e2-r6/)
adds the source-order no-unknown fallback and the three controlled outcomes.
VC++ 1.52c reports `COMPILE=PASS`, `LINK=PASS`; the Win16 probe exits 0
and `RTE2RES.TXT` says `ROUTE_CHECKS=90 RESULT=PASS`. The manifest pins
package/source hashes and all returned artifacts. These fallback inputs are
controlled branch fixtures, not retained original turn outcomes.

The fresh [r7 native return](../../runs/rototill-scout-route-e2-r7/)
adds the bounded nearby-enemy route. VC++ 1.52c reports `COMPILE=PASS`,
`LINK=PASS`; the Win16 probe exits 0 and `RTE2RES.TXT` says
`ROUTE_CHECKS=130 RESULT=PASS`. The manifest pins package/source hashes
and all returned artifacts. These enemy inputs are controlled fixtures;
the retained original scout input has no enemy candidates. The r7
human-only case fell through to planet scouting; source review showed it
should retry enemy search without the filter, so r7 is superseded for that
case by r8.

The fresh [r8 native return](../../runs/rototill-scout-route-e2-r8/)
corrects that retry and checks distant-enemy fuel, starbase, nearest-planet,
and fallback decisions. The adapter has a reserved-planet exclusion input;
that exclusion still needs a direct branch probe. VC++ 1.52c reports `COMPILE=PASS`,
`LINK=PASS`; the Win16 probe exits 0 and `RTE2RES.TXT` says
`ROUTE_CHECKS=170 RESULT=PASS`. The manifest pins package/source hashes
and all returned artifacts. These are still controlled branches; no new
original Stars! run was made.

The fresh [r9 native return](../../runs/rototill-scout-route-e2-r9/)
checks reserved-planet exclusion and moves the tooth callback before every
route decision, including an empty enemy list. VC++ 1.52c reports
`COMPILE=PASS`, `LINK=PASS`; the Win16 probe exits 0 and `RTE2RES.TXT`
says `ROUTE_CHECKS=180 RESULT=PASS`. The manifest pins package/source
hashes and all returned artifacts. The retained fleet's tooth outcome is
still injected in r9; r10 supersedes that input.

The fresh [r10 native return](../../runs/rototill-scout-route-e2-r10/)
packages the retained design-0 hull components and calls the reconstructed
tooth predicate. It checks the callback count and result before route
search. VC++ 1.52c reports `COMPILE=PASS`, `LINK=PASS`; the Win16 probe
exits 0 and `RTE2RES.TXT` says `ROUTE_CHECKS=180 RESULT=PASS`. Its manifest
pins package/source hashes and all returned build artifacts.

The original scout helper has a second-order planet-route guard, but the
Rototill caller reaches this helper only for `cord < 2`. That guard is
unreachable in this caller path; the separate RT-E3 multi-order work remains.

Original wormhole and enemy outcomes, live global and allocator identity,
original RNG-state provenance,
original fallback outcomes, later movement, and a complete
original turn comparison remain necessary for RT-E2/E4.
