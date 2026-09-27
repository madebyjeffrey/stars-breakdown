# LFuelUseToWaypoint native checkpoint

The final fresh Win98 guest run is preserved in `runs/vmctl-vc152fut/`.

- `FUTSTA3.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `FUTCL3.LOG`: VC++ 1.52c compiled `FUEL.C` and `PROBE.C` without warnings
- `FUTLINK3.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `FUTR.TXT` and `FUTR2.TXT`: `FUEL_USE_TO_WAYPOINT_CHECKS=4 FAILURES=0 RESULT=PASS`
- `FUT-CONSOLE.TXT`: zero bytes after redirected execution

The package is source-parity clean against
`reconstructed/src/fuel_use_to_waypoint.c`. The four native cases cover the
valid multi-turn path and callback order, invalid warp handling, the
cargo-hull/planet-reset path, and the ram-scoop adjustment boundary. The
guest-side build first exposed two packaging-only issues: LF-only batch input
and the missing `WINSTUB.EXE` in the fresh directory. Both were corrected;
the final run used CRLF wrappers and the retained 610-byte stub fixture.

This advances `LFuelUseToWaypoint` from Phase 2 step 3/6 to step 4/6. Exact
packed hull/planet records, original-executable differential cases, and full
ship integration remain open.
