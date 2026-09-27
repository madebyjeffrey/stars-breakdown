# SatisfyOrders player-design materialization E2 native replay

The created player-transfer branch now exposes a separate design-materialization
service boundary. The production helper reconstructs the original
`IshFindSimilarDesign`/record-copy sequence: it reuses equivalent active
destination designs, allocates the first hidden destination slot for an
unmatched source design, copies the complete 0x93-byte source record, rewrites
the destination slot identity flags, clears the two accumulated design
counters, transfers the fleet design count/status, and adds the transferred
count to both 32-bit design totals.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\SYOE10`
report:

`SATISFY_ORDERS_DESIGN_MATERIALIZATION_E2_CHECKS=15 FAILURES=0 RESULT=PASS EXIT=0`

The guest build reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
and the final window inventory contained no Program Error dialog. The native
packet uses the production `common.c` and `satisfy_orders.c` implementations.
The packet uses `/O` because the added production helper otherwise pushed the
combined VC++ 1.52c translation unit over its 64K code-segment limit; no
tested helper or design operation is stubbed.

This closes the player-specific design-record materialization/copying
boundary as supporting E2 evidence, but does not close the named SatisfyOrders
E2 unit. Original technology-table/result binding, complete raw message
argument layouts, original transfer behavior differential, and E3 full-turn/
original differential remain.

Evidence:

- `tests/satisfy_orders_design_materialization_e2_probe.c`
- `tests/common_fleet_copy_stubs.c`
- `compat/vc152/satisfy-orders-design-materialization-e2-native/`
- `runs/vmctl-vc152syoe10/SYO10-design-pass.TXT`
- `runs/vmctl-vc152syoe10/SYO10-run-pass.TXT`
- `runs/vmctl-vc152syoe10/SYO10STA-opt.TXT`
- `runs/vmctl-vc152syoe10/SYO10CL-opt.LOG`
- `runs/vmctl-vc152syoe10/SYO10LINK-opt.LOG`
- `reconstructed/src/common.c`
- `reconstructed/src/satisfy_orders.c`
