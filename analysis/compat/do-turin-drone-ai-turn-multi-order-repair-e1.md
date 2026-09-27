# DoTurinDroneAiTurn E1 first-order repair subcheck

The original Turin branch at `1088:5811–588A` requires ship slot 12
nonzero, `cord == 1`, and an empty first-order kind nibble after the preceding
zero-slot gates for slots 2, 3, 1, 8, 9, 13, 14, 0, 10, and 11. It writes
kind 6 at PLORD offset 10 and two `0x0005` words at offsets 12 and 14, then
calls `FLookupFleet`. The instruction listing does not write `iordMac` or
later orders. The branch is in
`analysis/decompiler/export/raw/1088_3670.asm`; the raw C gate is at
`analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c:675-693`.

The readable source now reads the first kind from packed PLORD when present,
instead of trusting a potentially stale typed `order_kind` field. It executes
repair in the owned-fleet loop even when no typed visitor is installed. The
strict-C89 production-source matrix passes 18/18 over exact three-order
mutation, packed-vs-typed disagreement in both directions, other-slot/cord/
owner bypasses, visitor-independent execution, and callback rejection.

Fresh VC++ 1.52c/Win98 `C:\TURRE1` compiled and linked the current
`do_turin_drone_ai_turn.c` plus `ai_plord.c`, exited 0, and reported
`TURIN_MULTI_ORDER_REPAIR_CHECKS=18 FAILURES=0 RESULT=PASS` with no Program
Error window. Logs, map, object, executable, report, and agent log are in
`runs/vmctl-turre1/`; the package is
`compat/vc152/turin-multi-order-repair-e1/`.

This completes the first-order repair subcheck of Turin E1, but **does not
close E1**. The original production phase includes multiple selected designs,
per-item queue writes, RNG gates, and population/resource thresholds not yet
represented by the current five-hull batch callback. E2 exact routing, E3
broader PLORD mutation, E4 complete AI-table/original-turn comparison, and
Step-6 promotion also remain.
