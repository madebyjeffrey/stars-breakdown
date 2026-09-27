# PlanetWndProc to DrawPlanetStats integration checkpoint

The host integration probe connects the reconstructed `PlanetWndProc` paint
dispatch to the reconstructed `DrawPlanetStats` renderer. A `WM_PAINT`
message now exercises the actual parent begin-paint/end-paint lifecycle and
the child stats path rather than testing the two procedures independently.

The strict C89 probe reports:

`PLANET_STATS_WNDPROC_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`

The checks cover the `BPE` paint sequence, the returned paint DC, one packed
planet-data load, tile drawing, all seven value rows, four reference
rectangles, two separators, and the mineral/scanner/defense fields reaching
the renderer. This is host-only evidence. It does not close the authoritative
Step-5 units because native replay, localized resources, exact Win16 GDI
behavior, and original-executable comparison remain open.

Evidence source: `tests/planet_stats_wndproc_integration_probe.c`.
