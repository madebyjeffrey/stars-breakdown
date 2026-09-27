# SatisfyOrders raw shortage message E2 native replay

The production `SatisfyOrders` source now binds pass-four resource shortages
to the original three-payload `FSendPlrMsg2` ABI. Planet destinations emit
`0x011f`, fleet destinations emit `0x0120`, and point destinations emit
`0x0123`. Every message is sent to the source fleet owner with payloads
`fleet.id | 0x8000`, the raw fleet id, and the resource index.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_SHORTAGE_CHECKS=12 FAILURES=0 RESULT=PASS`. Fresh VC++
1.52c/Win98 validation in `C:\SYOE16` reports the same result; compile/link
pass, the process exits 0, and the final guest window inventory contains no
Program Error dialog.

The mapping agrees with the recovered original branch, which selects `0x11f`,
`0x120`, or `0x123` from the packed destination kind and calls
`FSendPlrMsg2(..., fleet marker, fleet id, resource index)` only on pass four.
The typed resource path still retains its existing generic message callback;
the raw callback is an exact ABI evidence boundary rather than a replacement
for the higher-level service interface.

This is supporting E2 evidence, not closure of the named SatisfyOrders E2
unit. Ordinary successful-transfer layouts, the remaining colonist/technology
families, the original transfer differential, E3 full-turn comparison, and
Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_shortage_probe.c`
- `compat/vc152/satisfy-orders-raw-shortage-e2-native/`
- `runs/vmctl-vc152syoe16/SYOSHORT.TXT`
- `runs/vmctl-vc152syoe16/SYOSHORTSTA.TXT`
- `runs/vmctl-vc152syoe16/SYOSHORTCL.LOG`
- `runs/vmctl-vc152syoe16/SYOSHORTLINK.LOG`
- `runs/vmctl-vc152syoe16/SYOSHORT.MAP`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
