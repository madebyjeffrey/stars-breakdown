# DoCyberAiTurn freighter-service bridge — E5

The readable Cyber turn now exposes the next typed service boundary for the
owned-fleet pass. `run_freighter_ex` receives the converted fleet, the exact
typed fleet policy already derived by `DoCyberAiTurn`, and the current armada
potency. It runs after raw first-order repair and before fleet-action planning,
which preserves the recovered phase ordering while leaving the actual
`DoCyberFreighter` policy in an explicit callback.

The strict host C89 integration probe passes
`CYBER_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS`. The added check
verifies the owned fleet identity, the packed transport count of two, typed
policy transport count of two, and potency four. The existing 30 checks remain
in place, including packed planet/fleet conversion, packet-strength writeback,
and final callback ordering. A fresh VC++ 1.52c / Win98 replay in `C:\\CYBPE5`
compiled and linked with `COMPILE=PASS` and `LINK=PASS`, returned the same
31-check report, exited normally, and left no Program Error dialog.

This is supporting evidence and closes no named Step-5 unit. Direct
`DoCyberFreighter` composition, complete packed `PLORD` mutation, AI-table
integration, original-turn differential, and Step-6 promotion remain open.
Native artifacts are preserved in `runs/vmctl-vc152cybe5/`.
