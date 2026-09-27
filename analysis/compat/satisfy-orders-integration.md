# SatisfyOrders packed/integration checkpoint

The reconstructed order-service core was exercised through recovered packed
Win16 storage in a fresh VC1.52c/Win98 harness. The production bridge binds the
124-byte `_fleet` view's id, owner, flag word, and turn-state word, together
with the 18-byte `_order` view's point, target id, task/object nibbles, and
validity bit. It routes the raw transfer order into the typed resource
service, writes the completed-order validity bit and pass-1 turn-flag changes
back to the raw views, and checks the inactive-fleet gate.

Host and native reports are:

`SATISFY_ORDERS_INTEGRATION_CHECKS=3 FAILURES=0 RESULT=PASS`

The earlier native package compiled and linked with VC++ 1.52c in `StarDev` at
`C:\SYINT`; the post-run window list contained no Program Error dialog. That
historical packet exercised the equivalent typed integration boundary. A fresh
native package containing the new production bridge is still pending the guest
agent reconnect. The
original Fleet Orders surface is preserved in
`runs/vmctl-vc152current/orders-before.bmp` as the consumer boundary. This is
not claimed as a complete original turn replay.

The packed bridge is now implemented in `reconstructed/src/satisfy_orders.c`;
the host integration probe calls `stars_satisfy_orders_packed` directly rather
than carrying a test-only decoder. Strict host C89 and the existing native
report remain green for the three integration checks.

This advances `SatisfyOrders` to Phase 2 step 5/6, not promotion. Battle-plan
resource accounting, technology discovery, fleet splitting and ship-design
copying, salvage creation, complete message layouts, and full turn
integration remain open.

Evidence:

- `tests/satisfy_orders_integration_probe.c`
- `compat/vc152/satisfy-orders-integration/SYINT.C`
- `compat/vc152/satisfy-orders-integration/SYINTS.C`
- `compat/vc152/satisfy-orders-integration/PROBE.C`
- `runs/vmctl-vc152syint/SYISTA.TXT`
- `runs/vmctl-vc152syint/SYICL.LOG`
- `runs/vmctl-vc152syint/SYILINK.LOG`
- `runs/vmctl-vc152syint/SYIRES.TXT`
- `runs/vmctl-vc152syint/native-screen.bmp`
- `runs/vmctl-vc152current/orders-before.bmp`
