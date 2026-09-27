# SatisfyOrders raw colonist rejection E2 native replay

The production `SatisfyOrders` source now binds the three colonist-drop
rejection outcomes to the original `FSendPlrMsg2` ABI. An unowned, uninhabited
destination emits `0x0055`; the race-specific rejection emits `0x0056`; and a
planet-blocked destination emits `0x0135`. Each message is sent to the source
fleet owner with payloads `fleet.id | 0x8000`, the raw fleet id, and the target
planet id.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_COLONIST_CHECKS=12 FAILURES=0 RESULT=PASS`. Fresh VC++
1.52c/Win98 validation in `C:\SYOE17` reports the same result; compile/link
pass, the process exits 0, and the final guest window inventory contains no
Program Error dialog.

The mapping agrees with the recovered original colonist branch: each rejection
calls `FSendPlrMsg2` with the source fleet marker, source fleet id, and planet
id after the unowned, race, or planet gate rejects the drop. The typed generic
message callback remains available; the raw callback records this exact ABI
boundary.

This is supporting E2 evidence, not closure of the named SatisfyOrders E2
unit. Successful transfer layouts, the remaining technology message family,
the original transfer differential, E3 full-turn comparison, and Step-6
promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_colonist_probe.c`
- `compat/vc152/satisfy-orders-raw-colonist-e2-native/`
- `runs/vmctl-vc152syoe17/SYOCOLON.TXT`
- `runs/vmctl-vc152syoe17/SYOCOLONSTA.TXT`
- `runs/vmctl-vc152syoe17/SYOCOLONCL.LOG`
- `runs/vmctl-vc152syoe17/SYOCOLONLINK.LOG`
- `runs/vmctl-vc152syoe17/SYOCOLON.MAP`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
