# DrawPlanShipBitmap native fleet and failure boundaries E4

Fresh VC++ 1.52c / Win98 runs were completed after the `STARSAGT 0.4.2`
connection was restored. The current readable source and current `core.h`
were transferred into separate guest directories so the two focused probes
could be replayed without overwriting earlier evidence.

## Fleet render forwarding

Guest directory: `C:\DPSBFLE4`

The native report is:

`DRAW_PLAN_SHIP_BITMAP_FLEET_CHECKS=6 FAILURES=0 RESULT=PASS`

The build status is `COMPILE=PASS` and `LINK=PASS`. The probe validates fleet
identity, tile-derived normal and grid origins, and the recovered raw
`1,-1,0,0,-1,0` positional tail. The process exited 0 and the final `windows`
listing contained no Program Error dialog.

## Tile failure and hidden-button cleanup

Guest directory: `C:\DPSBFAI4`

The native report is:

`DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=6 FAILURES=0 RESULT=PASS`

The build status is `COMPILE=PASS` and `LINK=PASS`. The probe validates failed
tile cleanup, suppression of downstream rendering and positioning after a tile
failure, hide-bit clearing, hidden-button cleanup, and fleet rendering through
the no-buttons path. The process exited 0 and the final `windows` listing
contained no Program Error dialog.

## Acceptance effect

These runs upgrade both focused boundaries from host-only to host/native
evidence. They close no named Step-5 unit by themselves: exact Win16 resource
handles, complete packed-selection ownership, localized-name behavior,
planet-window ownership/redraw, and original differential evidence remain the
acceptance boundary for `DrawPlanShipBitmap`.

Evidence is preserved in:

- `compat/vc152/draw-plan-ship-bitmap-fleet-e4/`
- `compat/vc152/draw-plan-ship-bitmap-failure-e4/`
- `runs/vmctl-vc152dpsbfleet-e4/`
- `runs/vmctl-vc152dpsbfailure-e4/`
- `tests/draw_plan_ship_bitmap_fleet_boundary_probe.c`
- `tests/draw_plan_ship_bitmap_failure_probe.c`
