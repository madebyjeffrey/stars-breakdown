# DoRototillAiTurn native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152rotill/`.

- `ROTSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `ROTCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `ROT.C` and `PROBE.C` source
- `ROTLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `ROTR.TXT`: `ROTOTILL_AI_TURN_CHECKS=15 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `ROT.C` is byte-identical to
`reconstructed/src/do_rototill_ai_turn.c`, and `PROBE.C` is byte-identical to
`tests/do_rototill_ai_turn_source_probe.c`. The probe covers research-order
handoff, mineral scoring, positive optimal-value classification, productive
planet tracking, ownership filtering, and final dispatch.

This advances `DoRototillAiTurn` from Phase 2 step 3/6 to step 4/6.
Queue construction, colonizer/wormhole and mineral-fleet routing, order repair,
packed integration, and original-behavior evidence remain open.
