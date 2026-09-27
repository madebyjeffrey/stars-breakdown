# DrawPlanetStats packed lookup boundary

`DrawPlanetStats` now accepts an optional `load_data` callback. When present,
the callback receives the tile/object pointer before drawing and populates the
typed `StarsPlanetStatsData`; a false result rejects the draw. The existing
prepared-data and legacy value-drawing paths remain compatible.

The strict host C89 lookup probe reports:

`DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS`

It supplies a recovered 56-byte `_planet` view, decodes the planet identity,
the packed scanner/defense word at offset `0x18`, and the mineral-rate word at
offset `0x28`, then verifies that all seven rows see the decoded values.

Fresh VC++ 1.52c/Win98 validation under `C:\DPSLE1` compiled and linked the
packet and returned:

`DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS`

The process exited normally and the post-run Win98 window list contained no
Program Error dialog. This closes the named E1 packed lookup boundary.

Evidence:

- `runs/vmctl-vc152dpsle1/DPSLSTA.TXT`
- `runs/vmctl-vc152dpsle1/DPSLCL.LOG`
- `runs/vmctl-vc152dpsle1/DPSLLINK.LOG`
- `runs/vmctl-vc152dpsle1/DPSLE1.TXT`
