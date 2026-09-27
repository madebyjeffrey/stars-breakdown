# DrawPlanetShipList native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dpsl/`.

- `DPSLSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DPSLCL.LOG`: VC++ 1.52c compilation completed for the current
  `draw_planet_ship_list.c` source and probe
- `DPSLLINK.LOG`: segmented link completed with `DPSL.DEF`
- `DPSLR.TXT`: `DRAW_PLANET_SHIP_LIST_CHECKS=19 FAILURES=0 RESULT=PASS`
- `DPSL-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\DPSL`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates tile-change hiding
and reference reset, title selection, failed-tile cleanup, combo placement,
transfer lookup gating, owner-based button enablement, detail rendering,
three-control layout, and final action enablement. Packed fleet/thing transfer
records, exact Win16 control handles, localized text/gauges, and full drawing
integration remain outside this checkpoint.
