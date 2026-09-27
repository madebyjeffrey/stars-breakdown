# SatisfyOrders native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152syo/`.

- `SYOSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `SYOCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `SYO.C` and `PROBE.C` source
- `SYOLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `SYOR.TXT`: `SATISFY_ORDERS_CHECKS=32 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `SYO.C` is byte-identical to
`reconstructed/src/satisfy_orders.c`, and `PROBE.C` is byte-identical to
`tests/satisfy_orders_source_probe.c`. The probe covers the phase and fleet
gates, resource transfers, capacity and partial-transfer accounting, special
order dispatch, shortage and rejection messages, stat-percent modes, and
turn-state reset paths under the host C89 boundary.

This advances `SatisfyOrders` from Phase 2 step 3/6 to step 4/6. Packed
order battle-plan accounting, technology discovery, fleet splitting and
ship-design copying, salvage creation, complete message argument layouts,
and full turn integration remain open.
