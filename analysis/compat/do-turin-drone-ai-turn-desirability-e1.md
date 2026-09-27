# DoTurinDroneAiTurn E1 negative-desirability production gate

The original second planet pass calls `PctPlanetDesirability` for an owned
planet. If its result is negative, it marks `vlpbAiPlanet[id * 16 + 2] = 1`
and does not enter the `ChangeMainObjSel`/`InitProduction` branch (original
raw export `1088_3670_DoTurinDroneAiTurn.c:187-215`). The typed source
previously recorded the negative classification but could still enter
production whenever `production_slot_open` was set.

The source now carries the actual desirability result through the second
pass and requires it to be nonnegative before entering the production
branch. The regression deliberately leaves `production_slot_open = 1`
while desirability is negative, then checks that neither queueing nor
`FinishProduction` runs and that the packed AI-planet bad-world flag is set.
This preserves the existing positive path and avoids repeating the
desirability callback.

Strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURDE1` pass **27/27**. The guest
build reports `COMPILE=PASS LINK=PASS`, the process exits zero, and the
report says `TURIN_PACKED_SERVICE_CHECKS=27 FAILURES=0 RESULT=PASS`.
The post-run window list has no Program Error. The full host suite passes
539 tests (153 expected skips). Package:
`compat/vc152/turin-desirability-e1/`; returned build logs, map, object,
executable, report, and agent log: `runs/vmctl-turde1/`.

This corrects a real instruction-level production-path mismatch but is
supporting E1 evidence only. Live table and service effects and a complete
original production/turn comparison remain; no named unit closes.
