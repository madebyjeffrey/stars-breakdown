# PtDisplayPlanetPopInfo native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152popinfo/`.

- `PPISTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `PPICL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `PPI.C` and `PROBE.C` source
- `PPILINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `PPIR.TXT`: `PT_DISPLAY_PLANET_POP_INFO_CHECKS=25 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `PPI.C` is byte-identical to
`reconstructed/src/pt_display_planet_pop_info.c`, and `PROBE.C` is
byte-identical to `tests/pt_display_planet_pop_info_source_probe.c`. The probe
covers owned, unowned, foreign, negative-desirability, maximum-population,
growth-warning, population-change, font, wrapping, and returned-layout paths.

This advances `PtDisplayPlanetPopInfo` from Phase 2 step 3/6 to step 4/6.
Packed planet storage, localization, fixed-point formatting, detailed GDI
output, and original-behavior evidence remain open.
