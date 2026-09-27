# KillUsedWaypoints packed/integration checkpoint

The reconstructed waypoint cleanup procedure was checked against raw packed
`_fleet` records and a packed PLORD buffer. The adapter uses fleet ID/player,
status flags, planet ID, position, and order count at `_fleet` offsets 0, 2,
4, 6, 8, and 98. The PLORD header is four bytes, followed by 18-byte order
records whose point, target, and task/target flags are at record offsets 0,
4, and 6. The raw adapter loads those records into the typed fleet/order
view, runs cleanup and stale-fleet retargeting, then writes the changed fields
back to the packed buffers.

The native cases check the deleted-fleet message envelope (`fleet id | 0x8000`),
live and missing target handling, the no-move flag, recycle selection,
location-triggered compaction and planet retargeting, empty-waypoint messaging,
and enumeration of two packed fleet records. The fresh VC1.52c/Win98 report
is `KILL_USED_WAYPOINTS_INTEGRATION_CHECKS=9 FAILURES=0 RESULT=PASS`; compile
and link also pass, and the post-run window list contains no Program Error
dialog.

The E1 live-table follow-up adds a bounded `rglpfl` surrogate with four-byte
far-reference entries, a first-null terminator, and the recovered `_fleet`
offset-100 `lpplord` reference. The adapter enumerates the table into a caller
workspace, runs the genuine cleanup against all enumerated records, and writes
the changed fleet/order fields back. Fresh host and Win98 reports both pass
`KILL_USED_WAYPOINTS_PACKED_CHECKS=6 FAILURES=0 RESULT=PASS`; the native build
and link pass, and the final window inventory contains no Program Error dialog.

The original Fleet Waypoints surface is visible behind the captured Cargo
Transfer dialog, establishing the order/fleet UI boundary. The complete-turn
boundary is now also exercised through the real reconstructed turn coordinator
and `MoveFleets` caller. The fresh host/native turn report is
`KILL_USED_WAYPOINTS_TURN_CHECKS=6 FAILURES=0 RESULT=PASS`; it verifies caller
ordering, movement-to-packed-state write-back, stale-target retargeting before
deletion, and PLORD compaction. Compile/link passed and the final window list
contained no Program Error dialog. The raw original `MoveFleets` call site
(`10b0:4f30`) and the original Fleet Waypoints surface provide the original
caller/behavior evidence; final ledger promotion remains.

Evidence:

- `runs/vmctl-vc152kuwint2/KWIR.TXT`
- `runs/vmctl-vc152kuwint2/KWISTA.TXT`
- `runs/vmctl-vc152kuwint2/KWICL.LOG`
- `runs/vmctl-vc152kuwint2/KWILINK.LOG`
- `runs/vmctl-vc152kuwint2/native-screen.bmp`
- `runs/vmctl-vc152kuwpack/KWISTA.TXT`
- `runs/vmctl-vc152kuwpack/KWICL2.LOG`
- `runs/vmctl-vc152kuwpack/KWILINK2.LOG`
- `runs/vmctl-vc152kuwpack/KWIR.TXT`
- `runs/vmctl-vc152kuwpack/native-screen.bmp`
- `tests/kill_used_waypoints_integration_probe.c`
- `tests/kill_used_waypoints_packed_probe.c`
- `compat/vc152/kill-used-waypoints-integration/`
- `analysis/decompiler/export/raw/1080_189a_KillUsedWaypoints.c`
- `reconstructed/include/stypes.h`
- `runs/vmctl-vc152current/fleet-transfer.bmp`
- `tests/kill_used_waypoints_turn_probe.c`
- `reconstructed/src/generate_turn.c`
- `reconstructed/src/move_fleets.c`
- `compat/vc152/kill-used-waypoints-integration/KWTURN.C`
- `compat/vc152/kill-used-waypoints-integration/KWTURN.DEF`
- `compat/vc152/kill-used-waypoints-integration/KWTURN.LNK`
- `compat/vc152/kill-used-waypoints-integration/BUILDTURN.BAT`
- `compat/vc152/kill-used-waypoints-integration/RUNTURN-BUILD.BAT`
- `compat/vc152/kill-used-waypoints-integration/RUNTURN-PROBE.BAT`
- `runs/vmctl-vc152kuwturn/KWTURNSTA.TXT`
- `runs/vmctl-vc152kuwturn/KWTURNCL.LOG`
- `runs/vmctl-vc152kuwturn/KWTURNLINK.LOG`
- `runs/vmctl-vc152kuwturn/KWTURN.TXT`
- `runs/vmctl-vc152kuwturn/native-screen.bmp`

This closes the live fleet-table and complete-turn integration units. The
separate localized-resource checkpoint closes the exact message text boundary;
all named Step-5 evidence units are now closed, but the procedure is not
promoted until the review ledger and reconstruction notes are synchronized.

The resource evidence is preserved in
`compat/vc152/kill-used-waypoints-resource/` and
`runs/vmctl-vc152kuwres/`: message `0x0137` resolves to `Ultimate Recycling`,
message `0x004e` resolves to `Annual resources from all planets:  `, and
`KWRES.TXT` reports
`KILL_USED_WAYPOINTS_RESOURCE_CHECKS=2 FAILURES=0 RESULT=PASS`.
