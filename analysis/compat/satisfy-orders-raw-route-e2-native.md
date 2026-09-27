# SatisfyOrders raw auto-route message E2 native replay

The production `SatisfyOrders` source now exposes an explicit `raw_message`
service boundary for the original eight-payload `FSendPlrMsg` auto-route
messages. The typed fleet view carries the raw `idPlanet`, while the order
view carries the original starbase word used to derive the displayed
starbase index. This preserves the source's identity fields instead of using
the logical order target as a substitute.

The strict-C89 probe checks both original message variants: success
`0x0127` and the no-path/fuel message `0x0128`. For each it checks the
recipient, all eight payloads, the `0x8000` fleet marker, fleet id, source
planet id, `(lStarbase & 0x3ff) - 1`, and the four trailing zero fields.

The host report is
`SATISFY_ORDERS_RAW_ROUTE_CHECKS=20 FAILURES=0 RESULT=PASS`. Fresh VC++
1.52c/Win98 validation in `C:\SYOE13` reports the same result; compile and
link pass, the process exits 0, and the final guest window inventory contains
no Program Error dialog.

This is a supporting E2 milestone, not closure of the named SatisfyOrders E2
unit. Ordinary resource-transfer layouts, the remaining special message
families, the original transfer differential, E3 full-turn comparison, and
Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_route_probe.c`
- `compat/vc152/satisfy-orders-raw-route-e2-native/`
- `runs/vmctl-vc152syoe13/SYOROUTE.TXT`
- `runs/vmctl-vc152syoe13/SYOROUTESTA.TXT`
- `runs/vmctl-vc152syoe13/SYOROUTECL.LOG`
- `runs/vmctl-vc152syoe13/SYOROUTELINK.LOG`
- `runs/vmctl-vc152syoe13/SYOROUTE.MAP`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
