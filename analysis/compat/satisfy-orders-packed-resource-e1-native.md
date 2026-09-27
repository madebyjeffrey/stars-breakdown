# SatisfyOrders packed resource E1 native validation

**Superseded closure claim:** SO-E1 was reopened on 2026-09-23. The
21-check host/native packet below remains supporting evidence for packed
resource handling, but its task-5 “battle-plan” callback is not the original
task-5 scrap/transfer behavior. See
`analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md` and TODO7.
SO-E1 was subsequently closed by the raw task-5 scrap accounting and
original message/planet comparison in
`analysis/compat/satisfy-orders-task5-e1.md`; the claim below remains
historical and is not the basis for that closure.

The production `stars_satisfy_orders_packed` bridge now has a focused raw
record replay for the first remaining Step-5 boundary. The probe supplies the
recovered 124-byte `_fleet` and 18-byte `_order` records and checks packed
fleet/order identity, pass-1 turn-flag clearing, active-bit write-back, source
availability, destination capacity, exact resource-channel restrictions,
stat-percent mode, exact transfer/shortage message payloads, and pass-1
battle-plan dispatch.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\SYOE6`
both report
`SATISFY_ORDERS_PACKED_RESOURCE_E1_CHECKS=21 FAILURES=0 RESULT=PASS`.
Compile/link passed, the process exited 0, and the final guest window
inventory contained no Program Error dialog.

This closes the named E1 packed battle-plan/resource-accounting unit for
`SatisfyOrders`. It does not claim E2 technology discovery, fleet/ship-design
copying, salvage creation, or complete message-layout coverage, and it does
not close the E3 full-turn/original differential.

Evidence:

- `tests/satisfy_orders_packed_resource_e1_probe.c`
- `compat/vc152/satisfy-orders-packed-resource-e1-native/`
- `runs/vmctl-vc152syoe6/SYOE6-host.TXT`
- `runs/vmctl-vc152syoe6/SYOSTA-pass2.TXT`
- `runs/vmctl-vc152syoe6/SYOCL-pass2.LOG`
- `runs/vmctl-vc152syoe6/SYOLINK-pass2.LOG`
- `runs/vmctl-vc152syoe6/SYOE6-pass.TXT`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
