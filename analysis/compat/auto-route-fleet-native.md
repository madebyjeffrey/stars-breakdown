# AutoRouteFleet native checkpoint

The final fresh Win98 guest run is preserved in `runs/vmctl-vc152arf/`.

- `ARFSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `ARFCL.LOG`: VC++ 1.52c compiled `ARF.C` and `PROBE.C` without warnings
- `ARFLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `ARFR.TXT` and `ARFR2.TXT`: `AUTO_ROUTE_FLEET_CHECKS=4 FAILURES=0 RESULT=PASS`
- `ARF-CONSOLE.TXT`: zero bytes after redirected execution

`ARF.C` was transferred directly from the reviewed
`reconstructed/src/auto_route_fleet.c` file. The native probe exercises route
initialization and target resolution, same-owner stargate warp-11 selection,
fuel-capacity descent to warp 1, mineral-cargo stargate exclusion, and the
missing-target boundary.

This advances `AutoRouteFleet` from Phase 2 step 3/6 to step 4/6. Exact packed
order/resource offsets, floating helper constants, original route differential
cases, and full fleet integration remain open.
