# SatisfyOrders special-order dispatch E4 native replay

The existing 16-case strict-C89 special-order matrix was replayed through the
production `SatisfyOrders` source in fresh VC++ 1.52c / Win98 directory
`C:\SYOE4`. The native report is:

```text
SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=0 RESULT=PASS
```

The replay covers battle-plan dispatch on pass 1, mine capability and
race-stat gates, mine repeat-count handling, same-owner starbase auto-route
and no-path messaging, merge target/ownership/callback gates, and
player-transfer source/recipient message pairs. Compile and link both passed,
the process exited 0, and the final guest window inventory contained no
Program Error dialog.

This upgrades the special-order dispatch matrix from host-only to native
evidence. It is a supporting integration milestone and closes **0 named
Step-5 units**: native packed battle-plan/resource accounting, technology
discovery, fleet/ship-design copying, salvage creation, complete message
argument layouts, the full-turn/original differential, and promotion remain
open.

Evidence is preserved in
`analysis/compat/satisfy-orders-special-e4-native.{md,json}`,
`compat/vc152/satisfy-orders-special-native-e4/`, and
`runs/vmctl-vc152syoe4/`.
