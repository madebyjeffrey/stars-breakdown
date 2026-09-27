# SatisfyOrders packed special-order E5 native replay

The production packed bridge was extended to carry the recovered special-order
source coordinate and auto-route message variant, then replayed in fresh
VC++ 1.52c / Win98 directory `C:\SYOP5`. The native report is:

```text
SATISFY_ORDERS_PACKED_SPECIAL_CHECKS=10 FAILURES=0 RESULT=PASS
```

The 10 checks exercise the production `stars_satisfy_orders_packed` entry
through raw 124-byte `_fleet` and 18-byte `_order` records for battle-plan
pass-1 dispatch, mine success, same-owner merge, pass-4 auto-route, and
player-transfer creation/message pairs. Compile and link passed, the process
exited 0, and the final guest window inventory contained no Program Error
dialog.

This is stronger packed/native integration evidence, but it closes **0 named
Step-5 units**. The readable source still needs the deeper battle-plan
resource accounting, technology discovery, fleet/ship-design copying, salvage
creation, complete message argument layouts, and full-turn/original
differential before SatisfyOrders can be promoted.

Evidence is preserved in
`analysis/compat/satisfy-orders-packed-special-e5-native.{md,json}`,
`compat/vc152/satisfy-orders-packed-special-e5/`, and
`runs/vmctl-vc152syop5/`.
