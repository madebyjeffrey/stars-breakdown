# DoTurinDroneAiTurn E1 design-1 and minelayer production milestone

The original Turin planet scan first counts all viable visible unowned worlds
before it processes owned-world production (`1088_3670_DoTurinDroneAiTurn.c:
160-230`). The readable source now makes that first pass explicit, so an
owned planet before a qualifying unowned planet sees the complete count.
It retains one optical-value service call per visible unowned world; a
separate second-pass optical call and its side effects remain unresolved.

Original raw lines 295–300 queue design 1 four times when a viable world
exists and design 1's 32-bit existing count is below two. Lines 308–327
gate design 12 on the eligible bit and `Random(3)`, then scan fleets in
enumeration order for the first same-owner, same-planet fleet with a positive
slot-12 ship count. The selected count `cFr` determines whether `Random(8)`
is needed (10–16), whether the branch stops (17 or more), and the bound for
the final `Random(2*cFr+1)`. A zero final result queues the full request
`AddItemToQueue(12,3,2,1)`.

The typed source now has a full four-argument queue callback, retains signed
16-bit fleet counts, and applies those conditions before the four existing
five-hull loops. The strict-C89 matrix checks the first-pass dependency,
four exact design-1 requests, count cap, ineligible and nonzero-`Random(3)`
bypass, bounds 1/19/21, `Random(8)` rejection, 17+ cutoff, first qualifying
fleet, count 256 without byte truncation, queue rejection after a two-item
prefix, and a missing full-queue service. Host and fresh VC++ 1.52c/Win98
`C:\TURMI1` both report `TURIN_MINELAYER_CHECKS=32 FAILURES=0 RESULT=PASS`.
Build/link and process exit pass; the post-run window list shows no Program
Error. Package: `compat/vc152/turin-minelayer-e1/`; reports, logs, map,
object, executable, and agent log: `runs/vmctl-turmi1/`. The full host suite
passes 537 tests (153 expected skips), 2,241 layout assertions, and 846
declarations.

This advances, but does not close, E1. Raw `PctTrueMaxGrowth` and
`CResourcesAtPlanet` calls before the minelayer decision, support-fleet
selection and early finalization, live packed AI/design/production table
binding, actual `AddItemToQueue` and `FinishProduction` side effects, and
original-turn differential remain. E2–E4 and Step-6 promotion also remain.
