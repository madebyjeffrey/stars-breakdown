# DoCyberAiTurn native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152cyber/`.

- `CYBSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `CYBCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `CYB.C` and `PROBE.C` source
- `CYBLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `CYBR.TXT`: `CYBER_AI_TURN_CHECKS=32 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `CYB.C` is byte-identical to
`reconstructed/src/do_cyber_ai_turn.c`, and `PROBE.C` is byte-identical to
`tests/do_cyber_ai_turn_source_probe.c`. The probe covers research handoff,
merge masks, packet pressure and potency ramps, shared design-status mask
consumption, late split masks, mineral and production gates, hostile threat
marking, and fleet ownership filtering.

This advances `DoCyberAiTurn` from Phase 2 step 3/6 to step 4/6. Detailed
packet routing, freighter behavior, fleet-order repair, packed integration,
and original-behavior evidence remain open.
