# MoveFleets reconstruction

`MoveFleets` (`0017:32ce-4f3b`, 7,277 bytes) is the fleet movement pass. The
reconstruction uses typed fleet and order records with callbacks for the
shared game services. It preserves the repeated movement passes, order
filtering, dead-fleet and race-gating checks, fuel budgeting, ram-scoop gain,
minefield travel, partial coordinate interpolation, stargate jumps, arrival
and nearest-object selection, cargo changes, messages, auto-tracking cleanup,
and waypoint cleanup.

The callback boundary is deliberate: fuel formulas, fleet cargo layout,
planet/gate lookup, minefield effects, and message delivery belong to the
surrounding subsystems. The reconstructed procedure contains the control-flow
and state-transition policy in readable C rather than replaying instruction
bytes or embedding assembly.

The source probe exercises a moving fleet through the minefield and arrival
paths, including fuel gain, nearest-object selection, message delivery,
auto-track cleanup, and final waypoint cleanup. It passes strict host C89
checks and the native VC++ 1.52c Win98 build/link gate. The executable report
is `CHECKS=1`, `FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152ad/move-fleets/MFRES.TXT`.
