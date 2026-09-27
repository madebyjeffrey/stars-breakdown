# DoAutomitronAiTurn native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152auto/`.

- `AUTOSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `AUTOCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `AUTO.C` and `PROBE.C` source
- `AUTOLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `AUTOR.TXT`: `AUTOMITRON_AI_TURN_CHECKS=31 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `AUTO.C` is byte-identical to
`reconstructed/src/do_automitron_ai_turn.c`, and `PROBE.C` is byte-identical
to `tests/do_automitron_ai_turn_source_probe.c`. The probe covers the late
merge masks, shared design-status mask, turn budget and potency ramps,
ownership filtering, typed fleet-slot aggregation, and classification paths.

This advances `DoAutomitronAiTurn` from Phase 2 step 3/6 to step 4/6.
Colonizer/mineral fleet handling, order repair, packed integration, and
original-behavior evidence remain open.
