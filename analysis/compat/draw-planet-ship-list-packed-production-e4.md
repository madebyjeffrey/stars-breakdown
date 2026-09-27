# DrawPlanetShipList production packed-record bridge

The readable `DrawPlanetShipList` path now owns the recovered raw selection
and transfer fields when supplied with complete records. It decodes the
selection-object word at offset 152 of the 226-byte selection view, and the
transfer ID, transfer kind, and fleet owner at offsets 0, 2, and 6 of the
128-byte transfer view. Complete raw views take precedence over typed state;
incomplete views retain the callback/typed fallback boundary.

The strict-C89 packed probe now calls production source directly and passes
`DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`, covering raw
selection/transfer precedence, fleet and thing kinds, ownership gating,
visibility/layout, and the existing detail suppression paths. The source probe
also remains green with 19 checks.

This is host-only source evidence and closes no authoritative Step-5 unit.
Fresh VC++ 1.52c/Win98 replay through the production bridge, exact USER/GDI
controls, localized text/gauges, complete transfer/detail rendering, original
differential coverage, and promotion remain open pending guest-agent
reconnection.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/draw_planet_ship_list.c`
- `tests/draw_planet_ship_list_packed_probe.c`
- `analysis/compat/draw-planet-ship-list-packed-integration.{md,json}`
