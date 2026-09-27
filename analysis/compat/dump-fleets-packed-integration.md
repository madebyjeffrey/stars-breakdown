# DumpFleets packed-record integration checkpoint

The CodeView `_fleet` definition is a 124-byte packed record. The production
bridge maps its recovered offsets into the typed `StarsDumpFleet` and
`StarsDumpFleetRow` views, runs the current `DumpFleets` coordinator through
normal and detailed report paths, and checks the report callbacks and write
failure behavior.

The checked record fields are the fleet id/bitfield at offset 0, player at 2,
flags at 4, planet id at 6, coordinates at 8, the 16 two-byte ship counts at
12, fleet weight at 44, five four-byte cargo values at 76, planet index at 96,
route/order metadata at 98, and direction at 118. The adapter also checks the
typed ship-class service mapping, ETA/warp/weight/cloak/scanner/task and
destination fields used by the detailed report.

The expanded probe also checks every cargo slot, all detailed scanner/cloak/
mine/terraform fields, normal-mode detail suppression, and the
destination-requires-detail gate.

Host strict C89 and native VC++ 1.52c reports are:

`DUMP_FLEETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`

The historical native build in `runs/vmctl-vc152dmpfint2/` reports
`COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog. That packet used the earlier equivalent test adapter.
The host probe now calls the production `stars_dump_fleets_packed` bridge
directly. A fresh native packet containing this production bridge is pending
the guest-agent reconnect. This is a packed-record and service-boundary
checkpoint, not proof that localized strings, every scanner/minefield/battle-
plan service, or the complete original output format is finished.

This advances `DumpFleets` within Phase 2 step 5/6. The procedure is not
promoted: localized formatting, exact task/scanner/minefield/battle-plan
service behavior, complete original differential output, and final ledger
review remain open.

Evidence:

- `tests/dump_fleets_packed_probe.c`
- `reconstructed/src/dump_fleets.c`
- `reconstructed/include/core.h`
- `compat/vc152/dump-fleets-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1108_9530_DumpFleets.c`
- `analysis/reconstruction/dump-fleets.md`
- `runs/vmctl-vc152dmpfint2/DFINTSTA.TXT`
- `runs/vmctl-vc152dmpfint2/DFINTCL.LOG`
- `runs/vmctl-vc152dmpfint2/DFINTLINK.LOG`
- `runs/vmctl-vc152dmpfint2/DFINT.TXT`
