# DoTurinDroneAiTurn native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152tur/`.

- `TURSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `TURCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `TUR.C` and `PROBE.C` source
- `TURLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `TURR.TXT`: `TURIN_DRONE_AI_TURN_CHECKS=35 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `TUR.C` is byte-identical to
`reconstructed/src/do_turin_drone_ai_turn.c`, and `PROBE.C` is byte-identical
to `tests/do_turin_drone_ai_turn_source_probe.c`. The probe covers research
handoff, conditional merge masks, design-status and technology gating,
production affordability, planet classification, fleet-role summaries, and
ordered finalization.

This advances `DoTurinDroneAiTurn` from Phase 2 step 3/6 to step 4/6.
Detailed production policy, fleet-order repair, colonizer/transport routing,
and full AI-table integration remain open.
