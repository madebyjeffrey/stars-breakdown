# DoRobotoidAiTurn native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152rot/`.

- `ROBSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `ROBCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `ROB.C` and `PROBE.C` source
- `ROBLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `ROBR.TXT`: `ROBOTOID_AI_TURN_CHECKS=23 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `ROB.C` is byte-identical to
`reconstructed/src/do_robotoid_ai_turn.c`, and `PROBE.C` is byte-identical to
`tests/do_robotoid_ai_turn_source_probe.c`. The probe covers late-turn setup,
design-status and split masks, production gates, threat marking, war-fleet
counting, research handoff, and final dispatch ordering.

This advances `DoRobotoidAiTurn` from Phase 2 step 3/6 to step 4/6. Fleet-order
repair, colonizer/transport routing, and full AI-table integration remain open.
