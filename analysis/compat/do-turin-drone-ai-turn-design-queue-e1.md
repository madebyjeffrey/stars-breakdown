# DoTurinDroneAiTurn E1 four-role design queue milestone

The original Turin `CheckAiShdefStatus` outputs at raw lines 139–151 map
design-selection roles 0, 3, 5, and 7 to design groups 6–7, 4–5, 15, and
10–11. The repeated five-item production loops at lines 353–490 execute in
the order **3, 0, 7, 5**, not the first status slot alone. The prior typed
source incorrectly used role 0 for the first loop. The raw count caps are
`cPlanMax/24+4` for role 3, `cPlanMax/12+8` for roles 0 and 5, and
`cPlanMax/4+12` for role 7. Each loop requires a selected design and a
zero high word in its existing count; role 5 also requires the original
design-15 inactive bit state.

The readable source now accepts the typed plan limit, per-role existing
counts and four-channel true hull costs, and the design-15 gate. In the
table-backed path it queues in the original role order, using per-item
resource debits and carrying queued cost to the next role. A shortage stops
subsequent loops, retaining the accepted prefix. The legacy single-cost
path remains only when the richer table inputs are not supplied, and now
selects the correct role 3.

The strict-C89 production-source matrix passes 31/31: twenty per-item calls
in role order, exact cap exclusions, design-15 exclusion, stop-on-shortage,
and resource carry between roles. Existing Turin packed integration and edge
probes pass 20/20 and 7/7 after correcting the latter's expected design to
role 3. Fresh VC++ 1.52c/Win98 `C:\TURDQ1` compiles, links, exits 0, and
reports `TURIN_DESIGN_QUEUE_CHECKS=31 FAILURES=0 RESULT=PASS`; no Program
Error window appears. Package:
`compat/vc152/turin-design-queue-e1/`; logs, map, object, executable,
report, and agent log: `runs/vmctl-turdq1/`.

This advances Turin E1 but does **not** close it. Earlier production
branches still include initial turn-zero queue seeding, technology/design
count and RNG-gated choices, support-fleet checks, and exact
`FinishProduction` interaction. The typed inputs also need binding to live
packed player/design/game tables and an original-turn differential. The
preceding `C:\TURPE1` per-item packet is retained as historical evidence;
`C:\TURDQ1` is the fresh replay of the current mapped source.
