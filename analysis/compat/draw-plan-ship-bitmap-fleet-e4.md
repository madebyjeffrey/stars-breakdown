# DrawPlanShipBitmap fleet-render forwarding boundary

The typed `DrawPlanShipBitmap` view now preserves the raw fleet-render call's
geometry and positional tail. `draw_fleet_ex` receives the selected fleet id,
the tile-derived x/y origin, and the recovered `1, -1, 0, 0, -1, 0` tail
arguments used by the original `DrawFleetBitmap` call. The existing identity
only `draw_fleet` callback remains as a compatibility fallback.

The strict host C89 probe reports:

`DRAW_PLAN_SHIP_BITMAP_FLEET_CHECKS=6 FAILURES=0 RESULT=PASS`

It covers both normal and grid positioning, fleet identity, and every
forwarded tail argument. This is a host source-fidelity checkpoint only: the
Win16 fleet bitmap implementation, exact handles, localized name behavior,
full planet-frame ownership, native replay, and original differential remain
open. No authoritative Step-5 or promotion count changes.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/draw_plan_ship_bitmap.c`
- `tests/draw_plan_ship_bitmap_fleet_boundary_probe.c`
- `analysis/decompiler/export/raw/1048_3336_DrawPlanShipBitmap.c`
