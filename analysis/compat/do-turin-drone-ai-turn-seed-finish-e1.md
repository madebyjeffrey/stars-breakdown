# DoTurinDroneAiTurn E1 seed and production-finalization milestone

The original Turin production path at raw lines 230–278 first inspects the
packed production list. An existing ship item calls `FinishProduction(0)`
and skips new queue construction. Otherwise, turn zero seeds design 0 one
item at a time, decrementing the `cPlanMax` counter by 100 while it is at
least 191 and by 30 below that threshold. On later turns, the base design-0
branch requires an inactive design bit, hull id 5, an existing count below
`min(cPlanMax/4, 32)`, and `cBuilt/10 < cExist`. The constructed path calls
`FinishProduction(changed)` at raw line 490.

The readable source now has typed inputs for the queue-presence and base-
design gates, a per-item turn-zero seed, and a finalization callback that
receives whether an item was queued. The strict-C89 probe covers six seed
items at `cPlanMax=240`, existing-ship bypass, later base-design acceptance
and count/built exclusions, queue rejection after an accepted prefix, and
finalization rejection. Host and fresh VC++ 1.52c/Win98 `C:\TURSF1` both
report `TURIN_SEED_FINISH_CHECKS=21 FAILURES=0 RESULT=PASS`. The native
package compiled and linked, the process exited 0, and the post-run window
list showed no Program Error. Package: `compat/vc152/turin-seed-finish-e1/`;
build status, logs, map, object, executable, report, and agent log:
`runs/vmctl-tursf1/`. The full host suite passes 535 tests, with 153 expected
skips, 2,241 layout assertions, and 846 declarations.

This is a supporting E1 subcheck, not closure. Technology/design-count and
RNG-gated queue items, support-fleet selection, live packed production and
AI/design table binding, and an original-turn differential remain. The
finalization callback models the call boundary; it does not prove that the
actual game service or all subsequent production side effects match.
