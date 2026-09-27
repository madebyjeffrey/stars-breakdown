# DoRobotoidAiTurn E1 multi-order repair closure

The original Robotoid repair branch at `1088:130D–136F` tests the low kind
nibble of the first order, copies the selected fleet, writes kind 6 and the
two `0x0005` words at PLORD offsets 10, 12 and 14, then calls `FLookupFleet`.
It does **not** write header byte 3 (`iordMac`). The corresponding raw C is
`analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c:575-588`.
The analogous repair branches in Cyber, Automitron, Turin Drone and Macinti
also do not write the count. A prior shared helper set it to one, which could
truncate an otherwise valid multi-order queue. The corrected helper preserves
both header counts and every byte outside the three repaired words.

The new production-source probe starts with a three-record PLORD and an
eligible owned Robotoid colonizer at turn 41. It checks exact whole-buffer
mutation, count preservation, planner-before-repair callback order, route
bypass, noneligible `cord`, and foreign ownership. Strict host C89 reports
`ROBOTOID_MULTI_ORDER_REPAIR_CHECKS=14 FAILURES=0 RESULT=PASS`.
The existing shared-helper and Robotoid/Cyber/Turin/Automitron/Macinti repair
or route probes pass 9, 7, 18, 14, 14, 8 and 5 checks, respectively, with
realistic initially occupied first-order records.

Fresh Win98 directory `C:\ROBRE1` compiled and linked the current
`do_robotoid_ai_turn.c` and `ai_plord.c` under VC++ 1.52c. `BLDSTAT.TXT`
reports `COMPILE=PASS` and `LINK=PASS`; the probe process exited 0, the
native `ROBREP.TXT` reports the same 14/14 result, and the final window
inventory has no Program Error. Build logs, map, object, executable, report,
and agent log are retained in `runs/vmctl-robre1/`; the package is
`compat/vc152/robotoid-multi-order-repair-e1/`.
The full host suite passes 530 tests (153 expected skips), with 2,241
generated layout assertions and 846 declarations.

This closes the named Robotoid Step-5 E1 first-order repair unit. E2 exact
route target predicates and order writes, E3 broader raw PLORD mutation, E4
complete AI-table/original-turn comparison, and Step-6 promotion remain.
It does not claim those larger route or original-turn boundaries.
