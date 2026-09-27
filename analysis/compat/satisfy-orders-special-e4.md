# SatisfyOrders special-order dispatch checkpoint

Date: 2026-09-21

The host source probe now records the special-order branches as an explicit
acceptance packet rather than leaving them implicit in the general transfer
probe. It exercises battle-plan dispatch in pass 1, mine-laying capability and
race-stat gates, same-owner starbase auto-route dispatch and its no-path
message, merge target/ownership/callback gates, and pass-4 player transfer
dispatch. The player-transfer matrix checks the source/recipient message pairs
for no-design and created results plus the invalid-target, no-ship, and blocked
source messages. It also checks mine repeat-count behavior: ordinary repeat
counts decrement, `5` remains indefinite, and zero retires the order.

The strict-C89 report is:

```text
SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=0 RESULT=PASS
SATISFY_ORDERS_SPECIAL_MINE_GATE=PASS
SATISFY_ORDERS_SPECIAL_CALLBACK_GATES=PASS
```

This is host-source evidence only and closes **0** authoritative Step-5 units.
Native packed battle-plan/resource accounting, technology discovery,
fleet/ship-design copying, salvage creation, complete message argument
layouts, full-turn replay, original behavior, and promotion remain open.

Evidence:

- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `tests/satisfy_orders_special_source_probe.c`
- `analysis/compat/satisfy-orders-turn-e3.md`
