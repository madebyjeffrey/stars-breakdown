# KillUsedWaypoints native checkpoint

## Current Step-5 status

The localized message/resource unit is closed in fresh `C:\KUWRES` evidence.
The original compressed string table gives message `0x0137` as `Ultimate
Recycling` and message `0x004e` as `Annual resources from all planets:  `.
The native report `runs/vmctl-vc152kuwres/KWRES.TXT` passes two resource-boundary
checks with compile/link pass, and the final window list contains no Program
Error dialog. The live fleet-table enumeration follow-up is closed by the
fresh `C:\KUWPACK` run: `KWIR.TXT` reports six packed-table checks with zero
failures, and the final window list contains no Program Error dialog. The
complete-turn boundary is now closed by the fresh `C:\KUWTURN` run described
below.

The native complete-turn report is
`runs/vmctl-vc152kuwturn/KWTURN.TXT`:
`KILL_USED_WAYPOINTS_TURN_CHECKS=6 FAILURES=0 RESULT=PASS`. It compiles the
reconstructed `GenerateTurn`, `MoveFleets`, packed waypoint adapter, and
`KillUsedWaypoints` together. The probe verifies that the real turn coordinator
reaches `MoveFleets`, `MoveFleets` invokes `KillUsedWaypoints` after movement,
the moved coordinates are written back to the packed fleet, the stale fleet
target is retargeted before deletion, and the PLORD queue compacts correctly.
Compile/link passed, the process exited 0, and the final window inventory
contained no Program Error dialog. `KillUsedWaypoints` is now `0 + P`; only
final ledger promotion remains.

The final fresh Win98 guest run is preserved in `runs/vmctl-vc152kuw/`.

- `KUWSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `KUWCL.LOG`: VC++ 1.52c compiled `KUW.C` and `PROBE.C` without warnings
- `KUWLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `KUWR.TXT` and `KUWR2.TXT`: `KILL_USED_WAYPOINTS_CHECKS=6 FAILURES=0 RESULT=PASS`
- `KUW-CONSOLE.TXT`: zero bytes after redirected execution

`KUW.C` was transferred directly from the reviewed
`reconstructed/src/kill_used_waypoints.c` file. The native probe exercises
deleted-fleet reporting, live-target retargeting, missing-target invalidation,
the no-move flag, planet retargeting after queue compaction, and empty-queue
notification.

This advances `KillUsedWaypoints` from Phase 2 step 3/6 to step 4/6. Exact
packed PLORD offsets, localized message/resource integration, original
fleet-table cases, and full fleet integration remain open.
