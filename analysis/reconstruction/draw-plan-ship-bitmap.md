# DrawPlanShipBitmap reconstruction

`reconstructed/src/draw_plan_ship_bitmap.c` is a genuine C reconstruction of
`DrawPlanShipBitmap` (`000a:3336-377d`). It models typed tile geometry,
hidden-button cleanup, tile-render failure handling, fleet-versus-planet image
dispatch, the framed planet DIB path, and the fleet/planet button-stack
layout.

The host source probe covers hidden and failed tile paths, fleet rendering,
planet frame/DIB rendering, and button positioning. A fresh VC++ 1.52c/Win98
checkpoint also reports `COMPILE=PASS`, `LINK=PASS`, and
`DRAW_PLAN_SHIP_BITMAP_CHECKS=9 FAILURES=0 RESULT=PASS` without a runtime error
dialog. Evidence is preserved in
`analysis/compat/draw-plan-ship-bitmap-native.md` and
`runs/vmctl-vc152dpsm/`. Exact Win16 GDI resource handles, packed selection
state, localized name integration, and full planet integration remain open, so
this remains partial semantic source.

The original Win98 game frame has now been exercised through the fleet bitmap
boundary: the selected `Stalwart Defender #5` rendered its fleet bitmap, and a
map click refreshed the selected fleet without a Program Error. Evidence is
preserved in `analysis/compat/planet-frame-integration.md` and
`runs/vmctl-vc152current/planet-click.bmp`. A packed-selection checkpoint also
passed 17 host and native checks over the 226-byte `_sel` record, its fleet and
planet subrecords, fleet/planet dispatch, planet DIB source coordinates, and
button cleanup/grid paths. Evidence is preserved in
`analysis/compat/draw-plan-ship-bitmap-packed-integration.{md,json}` and
`runs/vmctl-vc152dpsbint/`. This keeps `DrawPlanShipBitmap` at Phase 2 step
5/6; exact GDI handles, complete selection integration, localized names,
original differential coverage, and promotion remain open.

## GDI lifecycle E1 preparation

The focused host probe now checks the exact GDI boundary: shadow/hilite brush
selection and restoration, all ten planet-frame `PATBLT` calls, palette
selection/realization, the 64-by-64 DIB source tile, fleet-only dispatch, and
fleet button geometry. It passes
`DRAW_PLAN_SHIP_BITMAP_GDI_CHECKS=8 FAILURES=0 RESULT=PASS`.

Fresh VC++ 1.52c/Win98 validation in `C:\DPSBINT` reports
`DRAW_PLAN_SHIP_BITMAP_GDI_CHECKS=8 FAILURES=0 RESULT=PASS`; no Program Error
dialog appeared. This closes E1 GDI lifecycle/geometry. Complete selection and
planet-window/original differential work plus promotion remain open. Evidence
is preserved in `runs/vmctl-vc152dpsb-gdi-e1/`.

## Tile-failure and hidden-button E4 boundary

The focused strict-C89 failure-path probe now reports
`DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=6 FAILURES=0 RESULT=PASS`. It isolates
the raw `FDrawTileNC` failure cleanup, verifies that failed tiles do not enter
fleet or planet GDI work, and checks the separate hide/no-buttons path: the
hide bit clears itself, all three buttons are hidden, and the no-buttons bit
suppresses later positioning while fleet rendering still occurs. This is
host-only evidence and closes no Step-5 unit; the native GDI/resource run and
original differential remain pending.

## Native fleet and failure boundary E4

Fresh VC++ 1.52c / Win98 replays now cover both focused host boundaries. The
fleet-forwarding packet in `C:\DPSBFLE4` reports
`DRAW_PLAN_SHIP_BITMAP_FLEET_CHECKS=6 FAILURES=0 RESULT=PASS`; the tile-failure
and hidden-button packet in `C:\DPSBFAI4` reports
`DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=6 FAILURES=0 RESULT=PASS`. Both packets
compiled and linked, exited normally, and left no Program Error dialog in the
final guest window list. This upgrades the two boundaries to native evidence
but closes no named Step-5 unit: exact resource handles, complete packed
selection ownership, localized names, planet-window ownership/redraw, and the
original differential remain open. Evidence is preserved in
`analysis/compat/draw-plan-ship-bitmap-native-boundaries-e4.{md,json}`,
`compat/vc152/draw-plan-ship-bitmap-{fleet-e4,failure-e4}/`, and
`runs/vmctl-vc152dpsbfleet-e4/` plus
`runs/vmctl-vc152dpsbfailure-e4/`.

## Packed-selection E2 production bridge

The production source now accepts a complete 226-byte `_sel` view directly.
It decodes `grobj` and `id` at offsets 4 and 8 before the normal tile/name,
fleet-bitmap, planet-frame, and button paths, and refuses to dispatch an
undersized packed record. The strict-C89 and fresh VC++ 1.52c/Win98 replay in
`C:\DPSBE2` both report
`DRAW_PLAN_SHIP_BITMAP_PACKED_E2_CHECKS=11 FAILURES=0 RESULT=PASS`.
Compile/link and process exit passed, and the final guest window list had no
Program Error dialog. This closes the named E2 packed-selection unit while E3
localized-name behavior, E4 complete planet-window/original differential
integration, and final promotion remain open. Evidence is preserved in
`analysis/compat/draw-plan-ship-bitmap-packed-e2-native.{md,json}`,
`compat/vc152/draw-plan-ship-bitmap-packed-e2-native/`, and
`runs/vmctl-vc152dpsbe2/`.

## Localized-name E3 production replay

The localized name boundary is now exercised through the production source
and packed selection view. Fleet selection `grobj=2, id=4` forwards the exact
`Stalwart Defender #5` bytes through the fleet-name service; planet selection
`grobj=1, id=7` forwards `Oop Be Gone` through the planet-name service. Both
services are selected before the tile renderer and preserve the selected ID.
Strict host C89 and fresh VC++ 1.52c/Win98 validation in `C:\DPSBE3` report
`DRAW_PLAN_SHIP_BITMAP_LOCALIZED_E3_CHECKS=11 FAILURES=0 RESULT=PASS`.
Compile/link and process exit passed, with no Program Error dialog in the
final guest window list. This closes E3; E4 complete planet-window/original
differential integration and final promotion remain open. Evidence is in
`analysis/compat/draw-plan-ship-bitmap-localized-e3-native.{md,json}`,
`compat/vc152/draw-plan-ship-bitmap-localized-e3-native/`, and
`runs/vmctl-vc152dpsbe3/`.

## Planet-window E4 production integration replay

The production `PlanetWndProc` WM_PAINT path is now exercised directly into
`DrawPlanShipBitmap` through the recovered begin-paint/planet-paint/end-paint
ownership sequence. The packed 226-byte `_sel` view selects both the fleet
`Stalwart Defender #5` case and the planet `Oop Be Gone` case. The focused
matrix checks callback ordering, exact kind/ID forwarding, fleet dispatch,
the ten planet-frame `PATBLT` operations, the planet DIB source tile, and
the no-buttons positioning boundary.

Strict host C89 and fresh VC++ 1.52c/Win98 validation in `C:\DPSBE4` report
`DRAW_PLAN_SHIP_BITMAP_PLANET_WINDOW_E4_CHECKS=12 FAILURES=0 RESULT=PASS`.
Compile and link passed, the process exited 0, and the final guest window
inventory contained no Program Error dialog. Combined with the existing
original Win98 planet/fleet frame and map-selection refresh observation,
this closes the named E4 planet-window/original differential integration
unit. The procedure is ready for the separate Step-6 ledger promotion.

Evidence is preserved in
`analysis/compat/draw-plan-ship-bitmap-planet-window-e4-native.{md,json}`,
`compat/vc152/draw-plan-ship-bitmap-planet-window-e4-native/`, and
`runs/vmctl-vc152dpsbe4/`.
