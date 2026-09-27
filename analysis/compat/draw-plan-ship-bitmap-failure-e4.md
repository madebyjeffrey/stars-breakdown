# DrawPlanShipBitmap tile-failure and hidden-button boundary

The focused strict-C89 probe now exercises the two cleanup branches that were
described in the reconstruction but were not previously isolated in a report:

- a failed `FDrawTileNC` result hides all three planet/fleet buttons and does
  not enter fleet, planet-frame, DIB, or button-positioning work;
- the hide flag performs the initial three-button cleanup, clears its raw bit,
  and the no-buttons flag then suppresses the later positioning/show pass
  while still rendering a fleet bitmap.

The probe also checks that the selected fleet identity reaches the tile and
name callbacks. It reports:

`DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=6 FAILURES=0 RESULT=PASS`

This is host-only source-fidelity evidence and closes no authoritative Step-5
unit. Native GDI/resource execution, packed selection integration, localized
name comparison, original differential coverage, and promotion remain open.

Evidence:

- `reconstructed/src/draw_plan_ship_bitmap.c`
- `tests/draw_plan_ship_bitmap_failure_probe.c`
- `analysis/decompiler/export/raw/1048_3336_DrawPlanShipBitmap.c`
