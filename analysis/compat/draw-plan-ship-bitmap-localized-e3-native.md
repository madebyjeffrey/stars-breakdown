# DrawPlanShipBitmap localized-name E3 native validation

The current production `DrawPlanShipBitmap` source preserves the localized
name service boundary from the original routine. Through the packed 226-byte
`_sel` view, it selects the fleet-name callback for `grobj=2` and the
planet-name callback for `grobj=1`, forwards the selected ID, and passes the
returned byte string to `FDrawTileNC`-equivalent tile rendering before the
remaining fleet/planet drawing work.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\DPSBE3`
both report
`DRAW_PLAN_SHIP_BITMAP_LOCALIZED_E3_CHECKS=11 FAILURES=0 RESULT=PASS`.
The matrix checks the exact fleet and planet strings, callback selection and
IDs, packed kind/ID decode, callback-before-tile ordering, and the tile
boundary's null-terminated text. Compile/link and process exit passed, and the
final guest window inventory contained no Program Error dialog.

This closes DrawPlanShipBitmap E3. E4 complete planet-window/original
differential integration and final promotion remain open, so the procedure
remains at Phase 2 step 5/6.

Evidence:

- `tests/draw_plan_ship_bitmap_localized_e3_probe.c`
- `compat/vc152/draw-plan-ship-bitmap-localized-e3-native/`
- `analysis/decompiler/export/raw/1048_3336_DrawPlanShipBitmap.c`
- `analysis/debug/types.json`
- `runs/vmctl-vc152dpsbe3/DPSBINTSTA.TXT`
- `runs/vmctl-vc152dpsbe3/DPSBINTCL.LOG`
- `runs/vmctl-vc152dpsbe3/DPSBINTLINK.LOG`
- `runs/vmctl-vc152dpsbe3/DPSBE3.TXT`
