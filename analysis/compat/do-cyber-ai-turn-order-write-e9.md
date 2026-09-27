# DoCyberAiTurn repair and packed order write — E9

The complete readable `DoCyberAiTurn` caller now has a combined repair,
route-plan, action-dispatch, and packed-order integration checkpoint. The
matrix runs an owned colonizer fleet through first-order repair and a
same-endpoint replacement, then runs an owned armed fleet through an appended
movement order. A foreign fleet is bypassed. The callback trace verifies
repair before planning, action application before the typed visitor, and
failure propagation from both the planner and packed-order applier.

The strict host C89 probe reports
`CYBER_AI_TURN_ORDER_WRITE_CHECKS=24 FAILURES=0 RESULT=PASS`. It checks the
recovered first-order repair bytes, policy and potency forwarding, exact
action payloads, replacement and append record counts, complete 18-byte order
copies, foreign-fleet filtering, callback order, and both failure exits.

A fresh VC++ 1.52c / Win98 replay in `C:\CYBOWE9` compiled and linked,
returned the same 24-check report, exited normally, and left no Program Error
dialog. This closes the named detailed repair/order-write Step-5 unit when
combined with the existing route-plan and shared PLORD source checkpoints.
Direct raw personality branch coverage beyond these typed action families,
complete Cyber AI-table integration, the original-turn differential, and
Step-6 promotion remain open. Native artifacts are preserved in
`runs/vmctl-vc152cybowe9/`.
