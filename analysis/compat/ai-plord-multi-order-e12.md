# Cyber AI multi-order PLORD mutation E12

E12 broadens the packed movement-order evidence beyond queues of one or two
records. The executable reference is `FMoveAiFleet`, NE `19:3D0A`, especially
its `fAppend` index choice, endpoint comparison, `cord=1` truncation when a
non-append endpoint matches the first order, record copy, and final `cord` /
`iordMac` write. The probe drives the readable `stars_ai_move_order_to_plord`
helper across those multi-record branches.

## Cases and verification

- Append a third distinct endpoint to a two-order queue; preserve the first
  two complete 18-byte records.
- Append at the existing last endpoint; replace that record without changing
  the three-order count or earlier records.
- Non-append at the first endpoint; replace the first record and truncate the
  active count to one.
- Non-append at a new endpoint; preserve the first record, write slot two,
  and set the count to two.
- At the helper boundary, reject append when the supplied four-record capacity
  is full, with no bytes changed. This is not a claim about FMoveAiFleet's
  allocator: the original routine first requests `LpplReAlloc` when `cord`
  reaches `iordMax`; allocator integration remains outside this probe.
- Strict-C89 host runner passes
  `AI_PLORD_MULTI_ORDER_CHECKS=15 FAILURES=0 RESULT=PASS`.
- Fresh `C:\PLOE12` VC++ 1.52c / Win98 package compiles and links; native
  report passes 15/15, process exits 0, and no Program Error appears. Returns
  include report, build/link logs, map, object, executable, and screenshot in
  `runs/vmctl-ai-plord-multi-e12/`.
- Full host suite passes 526 tests, 153 expected skips, 2,241 generated
  layout assertions, and 846 function declarations.

This adds supporting evidence for the Cyber AI Step-5 E3 packed-order scope,
but closes **0 named Step-5 units**. Cyber remains 2/4 at Step 5 with its raw
personality predicates and complete AI-table/original-turn evidence open.
Project counters remain 120/152 evidence units closed, 32 open, 10 promotion
units open, and 42 practical open work items.
