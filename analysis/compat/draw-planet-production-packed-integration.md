# DrawPlanetProduction packed-record integration checkpoint

`DrawPlanetProduction` receives the selected planet object and production
queue through the planet-frame boundary. The packed adapter decodes the
recovered direct record fields, then supplies localized planet text and ETA
through the existing typed `StarsPlanetProductionData` boundary.

The checked direct fields are the 56-byte `_planet` identity at offset 0 and
the far `lpplprod` pointer at offset 52. The pointed-to 4-byte `PLPROD`
header is decoded as `cbItem`, `fMark`, `ht`, and `cAlloc` in the low word,
with `iprodMax` at byte 2 and `iprodMac` at byte 3. The adapter verifies the
populated ETA/name rows, list and button placement, queue-driven action
enablement, and the empty-production cleanup path.

Host strict C89 and native VC++ 1.52c reports are:

`DRAW_PLANET_PRODUCTION_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152dpint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This advances `DrawPlanetProduction` within Phase 2 step 5/6. The packed
production-item record/list-selection unit is now closed by the separate
16-check host/native record probe in `runs/vmctl-vc152dprec/`. The exact
display-service boundary is also closed by the 14-check host/native
`C:\DPSV` probe, which covers client-rectangle sizing, resize flags, button
geometry, label order, queue enablement, and reset handling. Only the original
differential coverage and final review remain open.

## E4 original-executable differential

The retained original Win98 captures close the final behavior boundary. The
selected `Oop Be Gone` planet shows the production panel with an empty queue;
the original `Production Queue for Oop Be Gone` dialog independently shows
`-- Queue is Empty --`. The before/cycle and map-click captures show the frame
refresh and fleet-selection changes while the planet summary remains stable.
The fresh controller captures in `runs/vmctl-vc152dpp-original/` reproduce the
same selected-planet and frame-refresh observations without a Program Error.

The original instruction boundary is
`analysis/decompiler/export/raw/1048_2c38_DrawPlanetProduction.c`. Together
with the 14-check native service boundary, this closes E4. The procedure is
ready for final ledger promotion.

The separate localized-label checkpoint closes the exact resource boundary:
`0x32e` is `Production`, `0x218` is `Completion: `, and `0x4e9` is `Route to`.
Fresh host/native evidence in `tests/draw_planet_production_resource_probe.c`
and `runs/vmctl-vc152dpres/DPRES.TXT` passes three checks with no Program Error
dialog; it does not claim the remaining GDI, geometry, or original differential
boundaries.

Evidence:

- `tests/draw_planet_production_packed_probe.c`
- `compat/vc152/draw-planet-production-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1048_2c38_DrawPlanetProduction.c`
- `analysis/reconstruction/draw-planet-production.md`
- `runs/vmctl-vc152dpint/DPINTSTA.TXT`
- `runs/vmctl-vc152dpint/DPINTCL.LOG`
- `runs/vmctl-vc152dpint/DPINTLINK.LOG`
- `runs/vmctl-vc152dpint/DPINT.TXT`
