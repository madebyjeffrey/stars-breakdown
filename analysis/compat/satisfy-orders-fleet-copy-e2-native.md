# SatisfyOrders fleet/design copy E2 native replay

The created player-transfer branch now has a distinct production service
boundary for copying the resulting fleet state. The callback is exercised
through `stars_satisfy_orders`, then invokes the existing
`stars_find_similar_design` and `stars_duplicate_fleet` implementations from
`common.c`. The probe verifies a matching and a no-match design candidate,
the complete fixed 124-byte fleet record, and the variable two-record tail of
18-byte order records, including destination allocation and source
preservation.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\SYOE9`
report:

`SATISFY_ORDERS_FLEET_COPY_E2_CHECKS=10 FAILURES=0 RESULT=PASS EXIT=0`

The guest build reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
and the final window inventory contained no Program Error dialog. The native
packet uses the production `common.c` implementation. Small runtime-helper
stubs are present only for unrelated `common.c` call sites so the VC++ 1.52c
code segment remains within its 64K limit; the tested fleet-copy function is
not stubbed.

This closes the design-selection helper and a concrete fleet-state-copy
boundary but does not close the named SatisfyOrders E2 unit. Player-specific
ship-design record materialization/copying, original technology-table/result
binding, complete raw message argument layouts, original transfer behavior,
and E3 full-turn/original differential remain.

Evidence:

- `tests/satisfy_orders_fleet_copy_e2_probe.c`
- `tests/common_fleet_copy_stubs.c`
- `compat/vc152/satisfy-orders-fleet-copy-e2-native/`
- `runs/vmctl-vc152syoe9/SYO9-fleet-design-pass.TXT`
- `runs/vmctl-vc152syoe9/SYO9STA-design.TXT`
- `runs/vmctl-vc152syoe9/SYO9CL-design.LOG`
- `runs/vmctl-vc152syoe9/SYO9LINK-design.LOG`
- `runs/vmctl-vc152syoe9/SYO9-fleet-copy-pass.TXT`
- `runs/vmctl-vc152syoe9/SYO9STA-pass2.TXT`
- `runs/vmctl-vc152syoe9/SYO9CL-pass2.LOG`
- `runs/vmctl-vc152syoe9/SYO9LINK-pass2.LOG`
- `reconstructed/src/common.c`
