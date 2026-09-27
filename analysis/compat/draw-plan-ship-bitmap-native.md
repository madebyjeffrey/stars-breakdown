# DrawPlanShipBitmap native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dpsm/`.

- `DPSBSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DPSBCL.LOG`: VC++ 1.52c compilation completed for the current
  `draw_plan_ship_bitmap.c` source and probe
- `DPSBLINK.LOG`: segmented link completed with `DPSB.DEF`
- `DPSMR.TXT`: `DRAW_PLAN_SHIP_BITMAP_CHECKS=9 FAILURES=0 RESULT=PASS`
- `DPSB-CONSOLE.TXT`: empty; the process exited cleanly

The corrected run used fresh `C:\DPSBM2`, directory-first CRLF wrappers, a
local `WINSTUB.EXE`, and a native-only report path. The guest `windows` check
after execution showed no program-error dialog. This validates hidden-button
cleanup, tile rendering, fleet dispatch, planet frame drawing, palette/DIB
operations, and fleet/planet button-stack placement. Exact Win16 GDI resource
handles, packed selection state, localized names, and full planet integration
remain outside this checkpoint.
