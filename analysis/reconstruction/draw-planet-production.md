# DrawPlanetProduction reconstruction

`reconstructed/src/draw_planet_production.c` is a source-level reconstruction
of `DrawPlanetProduction` (`000a:2c38-310b`, 1,235 bytes). It exposes the
planet tile, production ETA/name data, list/button windows, reference
rectangle, and drawing services as typed C records and callbacks.

The implementation covers tile-cache reset, hidden-tile cleanup, list sizing
and placement, ETA/name rows, production-button placement/visibility, the
separate queue-driven action-button enablement, and the action reference
rectangle. The source probe exercises populated and hidden paths. Host strict
C89 compilation and `DRAW_PLANET_PRODUCTION_SOURCE=PASS` pass. A fresh VC++
1.52c/Win98 checkpoint also reports `COMPILE=PASS`, `LINK=PASS`, and
`DRAW_PLANET_PRODUCTION_CHECKS=9 FAILURES=0 RESULT=PASS` without a runtime
error dialog. Evidence is preserved in
`analysis/compat/draw-planet-production-native.md` and
`runs/vmctl-vc152dpro/`. Packed planet/production records and the exact
display-service geometry are covered by the later checkpoints below; the
original differential remains open, so this remains partial semantic source.

The original Win98 game frame has now been exercised through the production
boundary: the `Oop Be Gone` panel displayed the production section and its
empty queue without a Program Error. Evidence is preserved in
`analysis/compat/planet-frame-integration.md` and
`runs/vmctl-vc152current/`. A packed-record checkpoint also passed 18 host and
native checks over the 56-byte `_planet` identity, `lpplprod` pointer, 4-byte
`PLPROD` header, ETA/name rows, queue action enablement, and empty cleanup
path. Evidence is preserved in
`analysis/compat/draw-planet-production-packed-integration.{md,json}` and
`runs/vmctl-vc152dpint/`. This keeps `DrawPlanetProduction` at Phase 2 step
5/6; exact Win16 geometry and display-service integration are now closed,
while original differential coverage and promotion remain open.

The localized production-label unit is now closed. The original compressed
string table resolves `0x32e` to `Production`, `0x218` to `Completion: `, and
`0x4e9` to `Route to`. Strict host and fresh `C:\DPRES` VC++ 1.52c probes
both report `DRAW_PLANET_PRODUCTION_RESOURCE_CHECKS=3 FAILURES=0 RESULT=PASS`;
compile/link passed and the final Win98 window list contained no Program Error
dialog. The exact geometry/display-service boundary is covered by the later
checkpoint; only the original differential and final promotion remain open.

The packed production-item unit is now closed as well. A strict host and fresh
`C:\DPREC` VC++ 1.52c probe exercised the 56-byte planet identity, linked
4-byte `PLPROD` header, two variable 4-byte production records, packed
kind/item/count fields, name and schedule callbacks, listbox messages, and the
direct-item formatting path. Both report
`DRAW_PLANET_PRODUCTION_RECORD_CHECKS=16 FAILURES=0 RESULT=PASS`; compile/link
passed after the Win16 stack definition was corrected, and no Program Error
dialog appeared. Exact Win16 geometry/GDI/control integration and original
differential coverage were then split into separate acceptance units; the
former is closed below and the latter remains, so this remains partial
semantic source.

The exact display-service boundary is now closed. The source consumes the
recovered list client rectangle before and after repositioning, preserves the
full/compact row-height calculation and `0x14`/`0x15` resize choice, and the
fresh host/native `C:\DPSV` probe reports
`DRAW_PLANET_PRODUCTION_SERVICE_CHECKS=14 FAILURES=0 RESULT=PASS`. The checks
cover list and button geometry, localized label order, queue enablement,
action-button kind, reference rectangle, and tile reset. Compile/link passed
with VC++ 1.52c, the process exited 0, and no Program Error dialog appeared.
The original executable differential is now closed as well. The retained
Win98 captures show the selected `Oop Be Gone` planet, its empty production
panel, the `Production Queue for Oop Be Gone` dialog with
`-- Queue is Empty --`, and frame-refresh/fleet-selection changes that leave
the planet summary stable. Fresh controller captures in
`runs/vmctl-vc152dpp-original/` reproduce the selected and refresh states with
no Program Error. `DrawPlanetProduction` has therefore closed all four
evidence units and is ready for final ledger promotion.
