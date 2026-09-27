# SatisfyOrders foreign-fleet colonist rejection E2 native replay

The production `SatisfyOrders` source now models the original foreign-fleet
colonist permission branch. When a colonist unload targets a fleet owned by a
different player, no cargo transfer occurs; the order is cancelled and the
source player receives the three-payload `FSendPlrMsg2` message `0x0155`.
The payload is the source fleet marker (`id | 0x8000`), source fleet id, and
the source fleet's current planet id.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_FOREIGN_COLONIST_CHECKS=12 FAILURES=0 RESULT=PASS`. Fresh
VC++ 1.52c/Win98 validation in `C:\SYOE20` reports the same result; compile
and link pass, the process exits 0, and the final guest window inventory has
no Program Error dialog. The probe also proves that neither the transfer
callback nor the ordinary eight-word resource callback runs, and that cargo
and requested amount remain unchanged.

The first replay exposed a message-enum transcription error: the recovered
`idmAllowedTransferColonistsAnotherPlayer` enum is `0x0155`, not `0x0119`
(`0x0119` is `idmHasStolen`). The production constant, host evidence, and
native packet were corrected and rerun in `C:\SYOE20`; the corrected report
still passes 12/12 with clean compile/link and no dialog. The original run is
retained as superseded evidence.

This is supporting E2 evidence, not closure of the named SatisfyOrders E2
unit. Fuel/optimal-fuel, remaining permission/failure message families, the
original transfer differential, E3 full-turn comparison, and Step-6 promotion
remain open.

Evidence:

- `tests/satisfy_orders_raw_foreign_colonist_probe.c`
- `compat/vc152/satisfy-orders-raw-foreign-colonist-e2-native/`
- `runs/vmctl-vc152syoe20/SYOFOR.TXT`
- `runs/vmctl-vc152syoe20/SYORESSTA.TXT`
- `runs/vmctl-vc152syoe20/SYORESCL.LOG`
- `runs/vmctl-vc152syoe20/SYORESLINK.LOG`
- `runs/vmctl-vc152syoe20-r2/SYOFOR.TXT`
- `runs/vmctl-vc152syoe20-r2/SYORESSTA.TXT`
- `runs/vmctl-vc152syoe20-r2/SYORESCL.LOG`
- `runs/vmctl-vc152syoe20-r2/SYORESLINK.LOG`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
