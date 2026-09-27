# MoveThings reconstruction

`MoveThings` (`0017:18f4-2ef9`, 5,637 bytes) is the turn simulator's movement
pass. The reconstruction expresses the packed runtime records as typed C
adapters and keeps the phase-sensitive behavior visible: post-production
wormhole movement, pre/post-production minefield drift, packet range and
arrival handling, packet decay, planet lookup, packet application, removal,
messages, and waypoint revalidation.

The external game services are callback operations rather than hidden byte
replay. This keeps distance, wormhole validation, warp range, survival,
planet effects, and message delivery replaceable by the surrounding game
subsystems while preserving the ordering and state transitions of the
procedure. Named constants identify the three thing classes, the wormhole
attempt count, and the race statistic used for packet movement.

The source probe covers wormhole movement, packet arrival/application and
removal, minefield movement, and the changed-waypoint validation path. The
probe compiles cleanly as host C89 and passes the native VC++ 1.52c Win98
build/link gate. The native executable report is `CHECKS=1`,
`FAILURES=0`, `RESULT=PASS` in `runs/vmctl-vc152ac/move-things/MTRES.TXT`.
