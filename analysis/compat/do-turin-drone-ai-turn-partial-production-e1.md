# DoTurinDroneAiTurn E1 per-item production milestone

The original Turin production loop in
`analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c:353-385`
subtracts four channels of existing queue cost, then debits the selected
hull's four costs up to five times. After each affordable debit it calls
`AddItemToQueue(design, 1, 2, 1)`. If a later debit becomes negative, it jumps
to `FinishProduction` with earlier queued items retained. The previous typed
source required all five hulls to be affordable before issuing one
quantity-five callback, losing that partial prefix.

The readable source now performs 32-bit unsigned debit/sign checks and calls
the production callback once per affordable hull with quantity one. It
updates queued-planet and queued-hull counts as each callback succeeds. The
strict-C89 matrix passes 11/11 over five affordable items, three- and
four-item prefixes, insufficient preexisting queue resources, and a callback
rejection after two accepted items. The packed Turin integration and edge
probes also pass 20/20 and 7/7 with per-item expectations.

Fresh VC++ 1.52c/Win98 `C:\TURPE1` compiled, linked, exited 0, and reported
`TURIN_DRONE_AI_PRODUCTION_CHECKS=11 FAILURES=0 RESULT=PASS`; the final window
inventory showed no Program Error. Build logs, map, object, executable,
report, and agent log are retained in `runs/vmctl-turpe1/`; package:
`compat/vc152/turin-partial-production-e1/`.
The full host suite passes 533 tests (153 expected skips), 2,241 layout
assertions, and 846 declarations.

This advances, but does not close, Turin Step-5 E1. The current source still
does not implement the other selected-design production loops, their
population/design-count predicates, RNG-gated items, and exact
`FinishProduction` service interaction. The new per-item behavior is
instruction-derived for the one currently modeled design path, not a claim
of complete original-turn production agreement.
