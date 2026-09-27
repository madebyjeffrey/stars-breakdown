# DrawPlanetStats packed-record integration checkpoint

`DrawPlanetStats` receives a prepared planet/player view rather than doing the
lookup itself. The packed adapter therefore decodes the direct fields used by
that caller boundary from the recovered 56-byte `_planet` record, then supplies
race/service-derived values through the existing typed `StarsPlanetStatsData`
boundary.

The checked direct fields are the planet id at offset 0, the packed status word
at offset 4, scanner/defense bits in the long at offset 24, and the mineral
rate long at offset 40 (`rgwtMin[3]`). The adapter verifies that the decoded
mineral rate, scanner range, defense capacity, resources, and survival values
reach all seven value rows, while hidden-tile label suppression, measurement,
references, separators, and alignment remain exercised by the current source.

The initial host and native checkpoint reported:

`DRAW_PLANET_STATS_PACKED_CHECKS=13 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152dpsint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This advances `DrawPlanetStats` within Phase 2 step 5/6. It does not promote
the procedure: complete packed planet/race lookup, localized and fixed-point
formatting, exact scanner/defense service behavior, detailed GDI operations,
original differential coverage, and final review remain open.

An expanded host and fresh native run in `C:\\DPSINT2` then added the raw
zero-mineral-rate boundary and the maximum scanner/defense bitfield values.
Both reports now say:

`DRAW_PLANET_STATS_PACKED_CHECKS=15 FAILURES=0 RESULT=PASS`

The expanded native build reports `COMPILE=PASS` and `LINK=PASS`; the process
exited 0 and the post-run window list again contained no Program Error dialog.
This strengthens the step-5 evidence without promoting the procedure.

Evidence:

- `tests/draw_planet_stats_packed_probe.c`
- `compat/vc152/draw-planet-stats-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1048_1716_DrawPlanetStats.c`
- `analysis/reconstruction/draw-planet-stats.md`
- `runs/vmctl-vc152dpsint/DPSINTSTA.TXT`
- `runs/vmctl-vc152dpsint/DPSINTCL.LOG`
- `runs/vmctl-vc152dpsint/DPSINTLINK.LOG`
- `runs/vmctl-vc152dpsint/DPSINT.TXT`
- `runs/vmctl-vc152dpsint2/DPSINTSTA.TXT`
- `runs/vmctl-vc152dpsint2/DPSINTCL.LOG`
- `runs/vmctl-vc152dpsint2/DPSINTLINK.LOG`
- `runs/vmctl-vc152dpsint2/DPSINT.TXT`
