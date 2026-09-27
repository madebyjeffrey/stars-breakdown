# SatisfyOrders through DoOrders turn sequencing (host E3)

## Result

The strict host C89 probe passes:

```text
SATISFY_ORDERS_TURN_CHECKS=8 FAILURES=0 RESULT=PASS
```

## Scope

`tests/satisfy_orders_turn_probe.c` connects the typed `SatisfyOrders`
resource-transfer core to the real reconstructed `DoOrders` orchestrator. It
checks both turn modes: pre-movement sequencing (`thing interactions`, passes
1/2, colonist drop, research, transfer-to-others) and post-movement
sequencing (battle-id creation, battle callback, thing interactions, passes
3/4, colonist drop, research). The probe verifies phase ordering, pass masks,
fleet cargo mutation, order retirement, planet turn clearing, owned/unowned
planet flags, and battle id `0x0b01`.

## Status

A fresh VC++ 1.52c packet in `compat/vc152/satisfy-orders-turn-e3/` ran in
`C:\SYTURN`. Compile and link passed, the process exited 0, and the native
report is:

`SATISFY_ORDERS_TURN_CHECKS=8 FAILURES=0 RESULT=PASS`

The post-run Win98 window inventory contained no Program Error dialog. This
closes the native turn-sequencing replay subcheck but does not close the
authoritative Step-5 unit: packed battle-plan/resource accounting, technology
discovery, fleet/ship-design copying, salvage creation, complete message
layouts, original behavior, full-turn differential, and promotion remain open.

## Inputs

- `reconstructed/src/satisfy_orders.c`
- `reconstructed/src/do_orders.c`
- `reconstructed/include/core.h`
- `tests/satisfy_orders_turn_probe.c`
