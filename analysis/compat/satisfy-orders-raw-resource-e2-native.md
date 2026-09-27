# SatisfyOrders raw resource-transfer message E2 native replay

The production `SatisfyOrders` source now preserves the standard successful
resource-transfer `FSendPlrMsg` frame. Planet transfers use `0x002d` for
minerals and `0x002e` for colonists; same-owner fleet transfers use `0x002b`
for minerals and `0x002c` for colonists. The caller supplies the packed target
word and target x coordinate because those values are not owned by the reduced
typed order record. The eight forwarded payloads are:

`fleet.id | 0x8000`, fleet id, accepted amount, packed target word, resource
index, target x, raw target id, zero.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_RESOURCE_CHECKS=24 FAILURES=0 RESULT=PASS`. Fresh VC++
1.52c/Win98 validation in `C:\SYOE19` reports the same result; compile/link
pass, the process exits 0, and the final guest window inventory contains no
Program Error dialog. The matrix covers planet mineral/colonist and same-owner
fleet mineral/colonist frames.

The payload mapping agrees with the recovered successful load/unload calls in
`turn3.c`, which preserve the fleet marker/id, accepted amount, packed target,
resource, x coordinate, raw target id, and zero tail. The native compile log
contains one non-fatal VC1.52c C4018 signed/unsigned warning from the combined
legacy translation unit; it produced a successful executable and did not
affect the report.

This is supporting E2 evidence, not closure of the named SatisfyOrders E2
unit. Foreign-fleet colonist message `0x0119`, fuel/optimal-fuel variants,
permission/failure messages, the original transfer differential, E3 full-turn
comparison, and Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_resource_probe.c`
- `compat/vc152/satisfy-orders-raw-resource-e2-native/`
- `runs/vmctl-vc152syoe19/SYORES.TXT`
- `runs/vmctl-vc152syoe19/SYORESSTA.TXT`
- `runs/vmctl-vc152syoe19/SYORESCL.LOG`
- `runs/vmctl-vc152syoe19/SYORESLINK.LOG`
- `runs/vmctl-vc152syoe19/SYORES.MAP`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
