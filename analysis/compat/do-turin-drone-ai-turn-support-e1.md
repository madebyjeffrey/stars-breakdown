# DoTurinDroneAiTurn E1 support-fleet production milestone

Original Turin raw lines 328–352 select the status result for design group
13–14, then scan fleets in enumeration order at the current planet. The
first same-owner fleet for which `FPotentRobWarFleet(fleet,2)` is nonzero is
decisive: if its slot-13 plus slot-14 ship count meets the support potency
threshold, the AI queues `AddItemToQueue(selected,4,2,1)` and jumps straight
to `FinishProduction(changed)`. If that first potent fleet is too weak, the
scan stops without considering a later stronger fleet, and the normal
five-hull loops follow.

The readable source now models the potent-fleet service as a typed callback,
passes its literal argument 2, applies the existing support-potency value,
and uses the full four-argument queue callback. A successful support queue
skips the four ordinary design loops while preserving finalization. The
strict-C89 probe covers the threshold edge, first-potent stop, earlier
nonpotent bypass, owner/planet filtering, unavailable service, queue
rejection, and existing-ship queue bypass. Host and fresh VC++ 1.52c/Win98
`C:\TURSP1` report `TURIN_SUPPORT_CHECKS=23 FAILURES=0 RESULT=PASS`.
Native compile/link and process exit pass; no Program Error window appeared.
Package: `compat/vc152/turin-support-e1/`; build logs, map, object,
executable, report, and agent log: `runs/vmctl-tursp1/`. The full host
suite passes 538 tests (153 expected skips), 2,241 layout assertions, and
846 declarations.

This is supporting E1 evidence, not closure. The callback has not yet been
bound to the actual game `FPotentRobWarFleet` service; live AI/design/
production table values, the preceding growth/resource service effects,
actual queue/finalization side effects, and original-turn differential
remain. E2–E4 and Step-6 promotion remain separate.
