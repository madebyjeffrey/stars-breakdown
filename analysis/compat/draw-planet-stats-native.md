# DrawPlanetStats native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dps/`.

- `DPSSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DPSCL.LOG`: VC++ 1.52c compilation completed for the current
  `draw_planet_stats.c` source and probe
- `DPSLINK.LOG`: segmented link completed with `DPS.DEF`
- `DPSR.TXT`: `DRAW_PLANET_STATS_CHECKS=22 FAILURES=0 RESULT=PASS`
- `DPSR-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\DPS`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates tile rejection,
label-width measurement, right-edge value alignment, hidden-tile label
suppression, all seven value rows, reference rectangles, separator placement,
and font transitions. Localization, packed planet/race lookups,
scanner/defense selection, fixed-point formatting, and full GDI integration
remain outside this checkpoint.
