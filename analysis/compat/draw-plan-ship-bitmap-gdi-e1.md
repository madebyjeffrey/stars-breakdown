# DrawPlanShipBitmap GDI lifecycle E1 validation

The focused host probe covers the exact drawing boundary left open after the
packed-selection checkpoint:

- shadow-brush selection, hilite-brush selection, and restoration of the
  original brush;
- all ten planet-frame `PATBLT` operations and the first operation's exact
  coordinates/raster;
- palette selection and realization before the planet DIB blit;
- the 64-by-64 DIB source tile for object id 4; and
- fleet rendering without planet GDI work plus the three fleet button
  positions and dimensions.

Strict host C89 validation passes:

`DRAW_PLAN_SHIP_BITMAP_GDI_CHECKS=8 FAILURES=0 RESULT=PASS`

Fresh VC++ 1.52c/Win98 validation compiled and linked the packet in
`C:\DPSBINT` and returned:

`DRAW_PLAN_SHIP_BITMAP_GDI_CHECKS=8 FAILURES=0 RESULT=PASS`

The process exited normally and the post-run Win98 window list contained no
Program Error dialog. This closes the named E1 GDI lifecycle/geometry unit;
complete planet-window/original differential work and Step-6 promotion remain
open.

Evidence:

- `analysis/decompiler/export/raw/1048_3336_DrawPlanShipBitmap.c`
- `reconstructed/src/draw_plan_ship_bitmap.c`
- `tests/draw_plan_ship_bitmap_gdi_probe.c`
- `compat/vc152/draw-plan-ship-bitmap-gdi-e1/`
- `runs/vmctl-vc152dpsb-gdi-e1/DPSBINTSTA.TXT`
- `runs/vmctl-vc152dpsb-gdi-e1/DPSBINTCL.LOG`
- `runs/vmctl-vc152dpsb-gdi-e1/DPSBINTLINK.LOG`
- `runs/vmctl-vc152dpsb-gdi-e1/DPSBGE.TXT`
