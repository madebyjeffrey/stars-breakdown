# Planet and fleet frame step-5 integration checkpoint

The original generated game was continued in the connected `StarDev` Win98
guest. The live `starsframe` displayed the `Oop Be Gone` planet summary,
environment gauges, mineral bars, production queue, and starbase values. The
same frame displayed the selected fleet bitmap, fuel/cargo, fleet composition,
and waypoints; clicking the map changed the selected fleet and refreshed those
controls. No Program Error dialog appeared. Screenshots and window details are
preserved in `runs/vmctl-vc152current/` and recorded in
`analysis/compat/planet-frame-integration.json`.

The focused native checkpoints remain the five existing VC++ 1.52c/Win98
probes listed in that JSON. The raw instruction boundaries are the corresponding
`DrawPlanetStats`, `DrawPlanetProduction`, `DrawPlanetStarbase`,
`DrawPlanShipBitmap`, and `DrawPlanetShipList` exports.

This advances the five procedures from Phase 2 step 4/6 to step 5/6 for the
original planet/fleet frame and selection-refresh boundary. Packed records,
localized names and formatting, exact Win16 GDI/control handles, and full
planet integration remain open.
