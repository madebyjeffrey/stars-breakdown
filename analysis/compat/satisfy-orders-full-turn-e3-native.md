# SatisfyOrders full four-pass E3 integration replay

The new production-source harness exercises the complete `DoOrders` call
sequence for both turn halves:

- pre-movement: `SatisfyOrders(1)`, colonist drop, research update,
  `SatisfyOrders(2)`, and transfer-to-others;
- post-movement: battle-id creation, battle/thing callbacks, planet-turn
  clearing, `SatisfyOrders(3)`, colonist drop, research update, and
  `SatisfyOrders(4)`.

The matrix checks callback order, all four pass numbers, battle-plan/mine/
mine-laying/auto-route/player-transfer dispatch, exact generic and player
message order, order retirement, cargo mutation, planet ownership flags,
battle ID, turn clearing, and the absence of post-movement transfer-to-others.

Strict-C89 host validation reports
`SATISFY_ORDERS_FULL_TURN_E3_CHECKS=18 FAILURES=0 RESULT=PASS`. A fresh
VC++ 1.52c/Win98 packet in `C:\SYFTE4` reports the same result; compile and
link pass, the process exits 0, and the final guest window list contains no
Program Error dialog. The original `DoOrders` and `SatisfyOrders` raw exports
are retained as the instruction-level oracle.

This closes the native four-pass state/message integration subcheck. It does
not by itself close the named E3 unit: the original executable's complete
full-turn state/message differential and final Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_full_turn_e3_probe.c`
- `compat/vc152/satisfy-orders-full-turn-e3-r2/`
- `runs/vmctl-vc152syfte4/SYFTE3.TXT`
- `runs/vmctl-vc152syfte4/SYTSTA.TXT`
- `runs/vmctl-vc152syfte4/SYTCL.LOG`
- `runs/vmctl-vc152syfte4/SYTLINK.LOG`
- `runs/vmctl-vc152syfte4/SYTURN.MAP`
- `reconstructed/src/do_orders.c`
- `analysis/decompiler/export/raw/10b0_179a_DoOrders.c`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
