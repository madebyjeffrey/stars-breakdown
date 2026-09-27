# DoCyberAiTurn action dispatch matrix — E10

The complete readable `DoCyberAiTurn` caller now has a native action-family
matrix over all nine named Cyber fleet actions: clear task, move, colonize,
wormhole, move to starbase, attack, target armada, transport, and target
freighter. Each action is selected by the full caller, dispatched through its
named callback, then reaches the typed visitor. The movement families also
route their recovered `StarsMoveOrder` through the shared packed `PLORD`
writer using a seeded first-order record, preserving the original
replacement-style behavior.

The strict host C89 probe reports
`CYBER_AI_TURN_ACTION_MATRIX_CHECKS=34 FAILURES=0 RESULT=PASS`. A fresh VC++
1.52c / Win98 replay in `C:\CYBAME10` compiled and linked, returned the same
34-check report, exited normally, and left no Program Error dialog.

This is supporting evidence rather than closure of the remaining Cyber unit:
raw personality branch predicates, broader original-turn state comparison,
and complete AI-table integration remain open. Native artifacts are preserved
in `runs/vmctl-vc152cybame10/`.
