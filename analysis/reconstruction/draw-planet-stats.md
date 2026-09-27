# DrawPlanetStats

`DrawPlanetStats` is represented as typed row-oriented rendering code in
`reconstructed/src/draw_planet_stats.c`. It preserves tile rejection,
label-width measurement, right-edge alignment, hidden-tile label suppression,
the mineral, resources, scanner, defense, best-defense, and survival rows,
reference rectangles, and separator placement.

Localization, text measurement, tile drawing, packed planet/race lookups,
scanner/defense selection, fixed-point formatting, and GDI operations remain
callback-owned. The value callback now receives the raw routine's
`available-width - widest-label-width` offset for `RightTextOut` alignment.
The host probe covers normal and hidden-tile layouts and all
row/reference boundaries. A fresh VC++ 1.52c/Win98 checkpoint now compiles,
links, and reports 22 checks without a runtime error dialog. Localization,
packed planet/race lookups, scanner/defense selection, fixed-point formatting,
and full GDI operations remain callback-owned, so this is still a partial
semantic source.

The original Win98 game frame has now been exercised through the planet
statistics boundary: `Oop Be Gone` displayed value/population, environment
gauges, and mineral bars without a Program Error. Evidence is preserved in
`analysis/compat/planet-frame-integration.md` and
`runs/vmctl-vc152current/`. This advances `DrawPlanetStats` to Phase 2 step
5/6 for the original planet-frame boundary; packed lookups, localized and
fixed-point formatting, exact GDI operations, and promotion remain open.

The packed-record integration checkpoint is now complete. An expanded strict
host adapter and a fresh VC++ 1.52c / Win98 `C:\\DPSINT2` run each report
`DRAW_PLANET_STATS_PACKED_CHECKS=15 FAILURES=0 RESULT=PASS` over the recovered
56-byte `_planet` status, scanner/defense, and mineral-rate fields. The
decoded data reaches all seven value rows, including hidden-tile suppression,
references, separators, alignment, a zero mineral-rate boundary, and maximum
scanner/defense bitfields. Compile and link passed, the process exited 0, and
no Program Error dialog appeared. This strengthens the
procedure's Phase 2 step-5 evidence but does not promote it: complete packed
planet/race lookup, localized and fixed-point formatting, exact scanner/defense
service behavior, detailed GDI operations, original differential coverage,
and final review remain open. See
`analysis/compat/draw-planet-stats-packed-integration.{md,json}` and
`runs/vmctl-vc152dpsint2/`.

The value text boundary is now explicit as well. `StarsPlanetStatsOps` keeps
the existing `draw_value` path and adds an optional paired
`format_value`/`draw_formatted_value` path, so the seven rows can carry
localized and fixed-point text without hiding that work inside an untyped
callback. The strict host formatter probe reports
`DRAW_PLANET_STATS_FORMAT_CHECKS=16 FAILURES=0 RESULT=PASS`, including zero
mineral rate and unavailable scanner/defense text. Fresh VC++ 1.52c/Win98
validation in `C:\DPSFE2` reports the same result; no Program Error dialog
appeared. The packet corrected a probe accounting omission by explicitly
checking the unavailable defense text `77/0`, making the 16-check count
complete. This closes E2; E1 lookup, E3 service/resource behavior, E4 detailed
GDI/original comparison, and promotion remain open. Evidence is preserved in
`runs/vmctl-vc152dpsfe2/`.

Packed lookup is now explicit as a second typed seam. `StarsPlanetStatsOps`
may provide `load_data`, which receives the tile/object pointer and populates
the typed planet-stat record before the tile is drawn; a rejected load skips
the draw. The strict host lookup probe reports
`DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS` over the recovered
56-byte planet identity, scanner/defense word, and mineral-rate word. Fresh
VC++ 1.52c/Win98 validation in `C:\DPSLE1` reports
`DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS`; no Program Error
dialog appeared. This closes E1 packed lookup. Race/resource service lookup,
localized resources, detailed GDI/original comparison, and promotion remain
open. Evidence is preserved in `runs/vmctl-vc152dpsle1/`.

The service-resolution boundary is now explicit as well. The optional typed
`resolve_data` callback receives the accepted packed planet view and selected
player after tile drawing and before the seven value rows; it owns the
race/resource, scanner, defense, and survival service calls that the original
performs at this point. The strict host and fresh VC++ 1.52c/Win98 probes both
report `DRAW_PLANET_STATS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS`; compile,
link, empty redirected output, and the no-Program-Error check pass. This closes
E3. Detailed GDI/original differential and promotion remain open. Evidence is
in `analysis/compat/draw-planet-stats-services-e3.{md,json}` and
`runs/vmctl-vc152dpsse3/`.

The host paint-chain boundary is now explicit as well. A strict C89 integration
probe routes `PlanetWndProc(WM_PAINT)` through its real begin-paint,
`paint_planet_ship`, and end-paint callbacks into `DrawPlanetStats`, then
checks the packed-data load, seven value rows, four references, two
separators, and selected mineral/scanner/defense values. It reports
`PLANET_STATS_WNDPROC_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`. This is
host-only evidence and closes no authoritative unit; native replay, exact
Win16 GDI behavior, and original comparison remain open. See
`analysis/compat/planet-stats-wndproc-integration-e3.{md,json}`.

The recovered 56-byte packed planet decoder is now also in production source
as `stars_draw_planet_stats_packed`; the packed probe calls it directly and
the 15-check report remains green. The fresh E3 packet above replays the
service seam with the selected player and all seven value rows; the remaining
acceptance boundary is detailed GDI/original differential followed by
promotion.

The final E4 boundary is now closed. A fresh VC++ 1.52c/Win98 packet under
`C:\DPSWPE4` passes both the 22-check source geometry/row probe and the
8-check `PlanetWndProc(WM_PAINT)` integration replay, with clean compile/link
logs and no Program Error dialog. The resumed original frame was captured
after dismissing the Production Queue dialog; `Oop Be Gone` visibly retained
the mineral/status, population/resources, scanner/defense, production/fleet,
and environment panels. This closes the detailed GDI/original unit and leaves
only final ledger promotion. Evidence is in
`analysis/compat/draw-planet-stats-gdi-original-e4.{md,json}`,
`compat/vc152/planet-stats-wndproc-e4/`, and
`runs/vmctl-vc152dpswpe4/` plus
`runs/vmctl-vc152dps-original-e4.bmp`.
