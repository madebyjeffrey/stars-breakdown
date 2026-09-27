## Correction found during SO-E3 integration (2026-09-25)

The historical validation below predates a source correction and does not
validate the corrected implementation. The decompiler uses four-byte PLORD
header indexing: PLORD[5].iordMax is byte 22 (ORDER 1 x), not ORDER 5.
The nine-WORD copy is exactly 18 bytes from PLORD+22 to PLORD+4, copying
ORDER 1 into ORDER 0. PLORD[2].iordMax is ORDER 0 flags (byte 10), and
PLORD[7].wFlags is ORDER 1 flags (byte 28).

The service previously confused these header offsets with typed order
indices and deleted a waypoint outside the arrival branch. It now repairs
fleet targets, returns without deletion when the next waypoint has not been
reached, copies the arrived order into ORDER 0, retargets that order, and
performs deletion/recycle/message handling only inside the arrival branch.
The typed order and packed adapter now preserve the full ten-byte payload.

Corrected host source, integration (9 checks), packed (6 checks), and turn
(6 checks) probes pass strict C89 compilation. The turn probe now checks that
a fleet which has not reached its repaired pursuit target retains its queue.
No fresh native run has validated this correction. SO-E3 coordinator binding
and real DeleteWpFar mutation remain pending; old native fixtures must not
be treated as current behavioral proof.

# KillUsedWaypoints reconstruction

`reconstructed/src/kill_used_waypoints.c` is a genuine C reconstruction of
`KillUsedWaypoints` (`0011:189a-1d31`). It models the packed PLORD queue as
typed order records and makes the fleet-target repair, stale-target invalidation,
queue compaction, planet retargeting, waypoint deletion, recycle selection, and
owner-message branches explicit.

The host source probe covers deleted fleets, live and missing fleet targets,
missing-target invalidation, the no-auto-move target flag, queue compaction,
planet retargeting, and the empty-queue notification path.
The message boundary now also exposes and checks the raw `FSendPlrMsg`
status-word argument (`fleet id | 0x8000`) instead of retaining only the
message id and fleet id.
Native validation is complete at the focused procedure boundary. A fresh run
in `StarDev` used the exact current `reconstructed/src/kill_used_waypoints.c`
source in guest directory `C:\KUW`; VC++ 1.52c reported `COMPILE=PASS` and
`LINK=PASS`, and the Win98 report was
`KILL_USED_WAYPOINTS_CHECKS=6 FAILURES=0 RESULT=PASS`. Redirected console
output was empty and the native window list showed no program-error dialog.

Live fleet-table enumeration, original fleet-table cases, and full fleet
integration remain pending.

A second native harness now binds the source to recovered `_fleet` fields and
the four-byte PLORD header plus 18-byte order records. It passes two
integration checks for the deleted-fleet message envelope, the `fleet id |
0x8000` status word, stale-fleet coordinate retargeting, and waypoint deletion
compaction. The original Fleet Waypoints surface is visible in the captured
game window without a program-error dialog. This advances the procedure to
Phase 2 step 5/6; localized resources, live fleet-table enumeration, and full
turn integration remain open.

The packed integration harness was expanded to nine deterministic native
checks in a fresh `C:\KUWINT2` directory. It covers deleted-fleet and
empty-waypoint messages, missing-target invalidation, no-move preservation,
both recycle decisions, location-triggered compaction with planet retargeting,
and two-fleet enumeration with raw order-count write-back. VC1.52c
compilation and linking passed, the Win98 report was
`KILL_USED_WAYPOINTS_INTEGRATION_CHECKS=9 FAILURES=0 RESULT=PASS`, and the
post-run window list contained no Program Error dialog. This strengthens, but
does not close, Phase 2 step 5/6; live fleet-table integration and complete
turn behavior remain open.

The localized message/resource unit is now closed. The original compressed
string table resolves message `0x0137` to `Ultimate Recycling` and message
`0x004e` to `Annual resources from all planets:  `, including the original
two trailing spaces. A strict host probe and a fresh `C:\KUWRES` VC++ 1.52c
run each report `KILL_USED_WAYPOINTS_RESOURCE_CHECKS=2 FAILURES=0
RESULT=PASS`; compile/link passed and the final Win98 window list contained no
Program Error dialog. The remaining Phase 2 units are live fleet-table
enumeration and complete turn integration.

The live fleet-table unit is now closed. The new adapter models the original
four-byte `rglpfl` entries, stops at the first null entry, loads each fleet's
offset-100 `lpplord` reference into a bounded workspace, and writes the
mutated 124-byte fleet/18-byte order fields back. Host and fresh Win98 runs
both report `KILL_USED_WAYPOINTS_PACKED_CHECKS=6 FAILURES=0 RESULT=PASS`;
compile/link passed and the final window list contained no Program Error
dialog. The complete-turn unit is now closed. A fresh host/native `C:\KUWTURN`
probe composes `stars_generate_turn`, the genuine `stars_move_fleets` pass,
and the packed waypoint adapter. Its six checks confirm that the turn reaches
`MoveFleets`, `MoveFleets` invokes `KillUsedWaypoints` after movement, moved
coordinates are written to the packed `_fleet`, stale fleet targets are
retargeted before deletion, and the 18-byte PLORD queue compacts correctly.
The native report is `KILL_USED_WAYPOINTS_TURN_CHECKS=6 FAILURES=0 RESULT=PASS`;
compile/link passed and no Program Error dialog appeared. The original raw
`MoveFleets` call site and captured Fleet Waypoints surface provide the caller
boundary evidence. `KillUsedWaypoints` is now `0 + P`; only final review-ledger
promotion remains.
