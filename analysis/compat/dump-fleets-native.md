# DumpFleets native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dumpfleets/`.

- `DFSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DFCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `DF.C` and `PROBE.C` source
- `DFLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `DFR.TXT`: `DUMP_FLEETS_CHECKS=10 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `DF.C` is byte-identical to
`reconstructed/src/dump_fleets.c`, and `PROBE.C` is byte-identical to
`tests/dump_fleets_source_probe.c`. The probe covers active/viewer gating,
normal and detailed headers, ship totals and hull classes, destination
visibility, detailed fields, and close-on-write-failure behavior.

This advances `DumpFleets` from Phase 2 step 3/6 to step 4/6. Destination and
task-name formatting, scanner/cloak/minefield/battle-plan integration, packed
records, and original-behavior evidence remain open.
