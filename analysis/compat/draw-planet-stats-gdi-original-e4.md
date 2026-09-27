# DrawPlanetStats GDI/original integration — E4

This checkpoint closes the final named Step-5 evidence unit for
`DrawPlanetStats`. It combines the exact source geometry/row probe with a
fresh native parent-paint replay and a live original Win98 frame capture.

## Native evidence

The fresh packet is `compat/vc152/planet-stats-wndproc-e4/`, run from
`C:\DPSWPE4`. It uses the current `reconstructed/src/draw_planet_stats.c`
and `reconstructed/src/planet_wnd_proc.c` sources.

- `INTSTA-final.TXT`: `COMPILE=PASS`, `LINK=PASS`;
- `SRCRES-final.TXT`: `DRAW_PLANET_STATS_CHECKS=22 FAILURES=0 RESULT=PASS`;
- `INTRES-final.TXT`:
  `PLANET_STATS_WNDPROC_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`;
- `INTCL-final.LOG`, `INTLINK-final.LOG`, `SRCCL-final.LOG`, and
  `SRCLINK-final.LOG` are clean VC++ 1.52c build/link evidence.

The 22-check probe covers tile acceptance, label measurement, hidden-label
behavior, seven value rows, right-edge offsets, reference rectangles,
separators, and font transitions. The 8-check integration probe drives the
actual `PlanetWndProc(WM_PAINT)` begin/paint/end sequence into the renderer
and checks packed-data loading and selected service values. The final window
inventory contains no Program Error dialog.

## Original evidence

The original `Stars!` frame was captured after dismissing the live Production
Queue dialog. `Oop Be Gone` remained selected and its frame visibly contained
the mineral/status rows, population/resources, scanner/defense rows,
production panel, fleet panel, and environment summary without a Program
Error dialog. The capture is preserved as
`runs/vmctl-vc152dps-original-e4.bmp`.

This closes E4 and permits final ledger promotion. Evidence is also preserved
in `analysis/compat/planet-stats-wndproc-integration-e3.{md,json}` and
`analysis/reconstruction/draw-planet-stats.md`.
