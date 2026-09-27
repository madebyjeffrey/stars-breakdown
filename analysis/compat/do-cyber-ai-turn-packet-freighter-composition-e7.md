# DoCyberAiTurn packet/freighter composition — E7

The complete readable `DoCyberAiTurn` caller now composes both recovered
service routines in one turn: `run_freighter_ex` invokes the real
`stars_do_cyber_freighter`, and `run_packets_ex` invokes the real
`stars_do_cyber_packets`. The adapter preserves the recovered phase order:
freighter transfer and packed movement occur before the typed fleet visitor;
basic tasks precede packet queueing; packet strength is written back before
production finalization.

The strict host C89 probe reports
`CYBER_AI_TURN_PACKET_FREIGHTER_COMPOSITION_CHECKS=20 FAILURES=0 RESULT=PASS`.
It checks the `+1000` owned-planet transfer, target planet 9, exact `0x11b0`
movement and packed first-order replacement, packet target 9, best-mineral
queue item 15, seven packets at pressure 16, packet-strength writeback from
501 to 389, and final callback ordering.

A fresh VC++ 1.52c / Win98 replay in `C:\\CYBFE7` compiled and linked,
returned the same 20-check report, exited normally, and left no Program Error
dialog. This is supporting integration evidence and closes no named Step-5
unit. Complete raw fleet-cargo predicates, packet/freighter edge branches,
complete AI-table integration, original-turn differential, and Step-6
promotion remain open. Native artifacts are preserved in
`runs/vmctl-vc152cybfe7/`.
