# DoCyberAiTurn packet/freighter edge matrix — E8

The complete readable `DoCyberAiTurn` caller now composes the real
`stars_do_cyber_freighter` and `stars_do_cyber_packets` services across five
full-caller scenarios. The matrix exercises the high-stock owned-planet
drop-off, low-stock owned-planet pickup, enemy ordinary-planet starbase
fallback, open-space short circuit, and packet-target rejection paths.

The strict host C89 probe reports
`CYBER_AI_TURN_PACKET_FREIGHTER_EDGE_CHECKS=45 FAILURES=0 RESULT=PASS`.
It checks transfer direction and amount, exact `0x11b0`/`0x11b1` movement,
packed first-order mutation, target lookup short circuits, packet queue and
strength writeback (`501` to `389`) on the accepted case, packet suppression
on rejected cases, and final callback ordering.

A fresh VC++ 1.52c / Win98 replay in `C:\CYBFE8` compiled and linked,
returned the same 45-check report, exited normally, and left no Program Error
dialog. Combined with E5's native freighter gate/progress matrix and E7's
native accepted packet/freighter composition, this closes the named
packet/freighter behavior Step-5 unit. Detailed order-repair/order-write
coverage, complete Cyber AI-table integration, original-turn differential,
and Step-6 promotion remain open. Native artifacts are preserved in
`runs/vmctl-vc152cybfe8/`.
