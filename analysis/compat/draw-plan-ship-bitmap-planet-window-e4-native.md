# DrawPlanShipBitmap planet-window E4 native validation

The production `PlanetWndProc` WM_PAINT path now has a focused integration
replay through `DrawPlanShipBitmap`. The probe drives the recovered
begin-paint/planet-paint/end-paint ownership sequence, forwards the packed
226-byte `_sel` record, and exercises both selected-fleet and selected-planet
rendering through the production sources.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\DPSBE4`
both report
`DRAW_PLAN_SHIP_BITMAP_PLANET_WINDOW_E4_CHECKS=12 FAILURES=0 RESULT=PASS`.
The matrix checks WM_PAINT ordering, exact packed kind/ID forwarding, the
localized fleet and planet tile names, fleet dispatch, the ten planet-frame
pattern operations, the planet DIB source coordinates, and the no-buttons
positioning boundary. Compile/link and process exit passed, and the final
guest window inventory contained no Program Error dialog.

Together with the existing original Win98 planet/fleet frame evidence—the
`Oop Be Gone` planet panels and `Stalwart Defender #5` fleet bitmap refreshing
after map selection—this closes the named E4 planet-window/original
differential integration unit. The procedure still requires the separate
Step-6 ledger promotion until the synchronized review records are regenerated.

Evidence:

- `tests/draw_plan_ship_bitmap_planet_window_e4_probe.c`
- `compat/vc152/draw-plan-ship-bitmap-planet-window-e4-native/`
- `runs/vmctl-vc152dpsbe4/DPSBE4-host.TXT`
- `runs/vmctl-vc152dpsbe4/DPSBINTSTA-pass.TXT`
- `runs/vmctl-vc152dpsbe4/DPSBINTCL-pass.LOG`
- `runs/vmctl-vc152dpsbe4/DPSBINTLINK-pass.LOG`
- `runs/vmctl-vc152dpsbe4/DPSBE4-pass.TXT`
- `analysis/compat/planet-frame-integration.md`
- `analysis/compat/planet-frame-integration.json`
