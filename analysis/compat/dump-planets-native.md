# DumpPlanets native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dumpplanets/`.

- `DUMPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DUMPCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `DUMP.C` and `PROBE.C` source
- `DUMPLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `DUMPR.TXT`: `DUMP_PLANETS_CHECKS=16 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `DUMP.C` is byte-identical to
`reconstructed/src/dump_planets.c`, and `PROBE.C` is byte-identical to
`tests/dump_planets_source_probe.c`. The probe covers active-game gating,
normal and detailed headers, visibility and class gates, derived turn age,
row copying, and close-on-write-failure behavior.

This advances `DumpPlanets` from Phase 2 step 3/6 to step 4/6. Localized field
formatting, exact starbase/wormhole/production callbacks, packed integration,
and original-behavior evidence remain open.
