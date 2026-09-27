# DoCyberAiTurn direct DoCyberFreighter composition — E6

The complete readable `DoCyberAiTurn` caller now has a focused composition
replay through the real `stars_do_cyber_freighter` implementation. The typed
`run_freighter_ex` adapter maps the caller's owned fleet into the freighter
view, supplies the owned-planet lookup and target services, and sends the
resulting movement through the shared packed `PLORD` writer. The probe checks
that the freighter callback runs before the typed fleet visitor, transfers
`+1000` of the recovered cargo/resource pair, selects target planet 9, emits
the exact `0x11b0` movement flags, and replaces the packed first order.

The strict host C89 probe reports
`CYBER_AI_TURN_FREIGHTER_COMPOSITION_CHECKS=18 FAILURES=0 RESULT=PASS`.
A fresh VC++ 1.52c / Win98 replay in `C:\\CYBFE6` compiled and linked,
returned the same 18-check report, exited normally, and left no Program Error
dialog.

This is supporting integration evidence and closes no named Step-5 unit.
Complete packet/freighter interaction, all raw fleet-cargo predicates,
complete AI-table integration, original-turn differential, and Step-6
promotion remain open. Native artifacts are preserved in
`runs/vmctl-vc152cybfe6/`.
