# SatisfyOrders raw mining message E2 native replay

The production `SatisfyOrders` source now binds the mining failure outcomes to
the original three-payload `FSendPlrMsg2` ABI. The no-planet branch emits
`0x0077` with a zero planet payload; empty-source emits `0x0075`; and a
hostile-planet rejection emits `0x0076`. The latter two preserve the target
planet id in the final payload. Every case carries the source fleet identity
with the original `0x8000` marker and the raw fleet id.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_MINE_CHECKS=12 FAILURES=0 RESULT=PASS`. Fresh VC++ 1.52c/
Win98 validation in `C:\SYOE14` reports the same result; compile/link pass,
the process exits 0, and the final guest window inventory contains no Program
Error dialog. The success result is also checked to remain message-free.

This is supporting E2 evidence, not closure of the named SatisfyOrders E2
unit. Ordinary resource-transfer layouts, the remaining shortage/merge/
colonist/technology families, the original transfer differential, E3 full-turn
comparison, and Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_mine_probe.c`
- `compat/vc152/satisfy-orders-raw-mine-e2-native/`
- `runs/vmctl-vc152syoe14/SYOMINE.TXT`
- `runs/vmctl-vc152syoe14/SYOMINESTA.TXT`
- `runs/vmctl-vc152syoe14/SYOMINECL.LOG`
- `runs/vmctl-vc152syoe14/SYOMINELINK.LOG`
- `runs/vmctl-vc152syoe14/SYOMINE.MAP`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
