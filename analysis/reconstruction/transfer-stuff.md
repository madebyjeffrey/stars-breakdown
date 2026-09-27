# TransferStuff

`TransferStuff` is represented as typed transfer orchestration in
`reconstructed/src/transfer_stuff.c`.  It preserves object preparation,
split-versus-jettison handling, valid-hull enumeration, tutor/dialog flow,
cancellation cleanup, empty-fleet cleanup, cargo balancing, selection refresh,
scanner invalidation, and the persistent transfer-mode reset.

Packed fleet/planet/thing lookup and exact transfer-dialog behavior remain
callback-owned. The balance callback is now bound to the reconstructed
`stars_fleet_transfer_cargo_balance` service: the E2 packet checks the original
callback order, five-resource conservation, fuel movement, packed design-status
mutation, and the no-balance cancel path. The original Win98 dialog boundary
has also been exercised: `Cargo Transfer` opened for
`Stalwart Defender #5` and `Oop Be Gone`, displaying fuel, cargo, and
per-mineral transfer controls, and was canceled without committing a game-state
change. See `analysis/compat/transfer-stuff-integration.md` and its paired JSON
evidence. The host probe covers a
successful transfer and the canceled split-fleet cleanup path. A fresh VC++
1.52c/Win98 checkpoint also reports `COMPILE=PASS`, `LINK=PASS`, and
`TRANSFER_STUFF_CHECKS=20 FAILURES=0 RESULT=PASS` without a runtime error
dialog. Evidence is preserved in
`analysis/compat/transfer-stuff-native.md` and `runs/vmctl-vc152trn/`. Packed
fleet/planet/thing lookup and mutation, exact transfer-dialog behavior, and
Win16 lookup/refresh integration remain open, so this is still partial
semantic source. A packed-record checkpoint also passed 14 host and native
checks over the 128-byte `_xfer` identity/kind/fleet-owner fields, split and
cancellation cleanup, mode-0 jettison, hull enumeration, balance, scanner
invalidation, and refresh orchestration. Evidence is preserved in
`analysis/compat/transfer-stuff-packed-integration.{md,json}` and
`runs/vmctl-vc152trint/`; complete packed lookup/mutation, exact dialog
behavior, broader differentials, and promotion remain open. The fresh E2
host/native packet is recorded in
`analysis/compat/transfer-stuff-cargo-e2.{md,json}` and
`runs/vmctl-vc152trcne2/`; it reports
`TRANSFER_STUFF_CARGO_E2_CHECKS=12 FAILURES=0 RESULT=PASS` with no Program
Error dialog.

The final E3 packet is complete in fresh `C:\TRCNE3`. The typed source resolves
initial packed objects, preserves the original `grobj=4` split/jettison
sentinel, commits fleet/planet/thing records after the dialog, and exercises
the real Win16 task queue through `transfer_dlg`. Strict host and VC++
1.52c/Win98 runs both report
`TRANSFER_STUFF_E3_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link, process
exit, and the post-run window inventory pass with no Program Error dialog.
The review ledger, coverage, TODO5, TODO6, and handoff are synchronized, so
TransferStuff is promoted at **6/6**. Evidence is recorded in
`analysis/compat/transfer-stuff-e3.{md,json}` and
`runs/vmctl-vc152trcne3/`.
