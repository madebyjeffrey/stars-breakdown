# PlanetWndProc E3 message, focus, and drawing integration

This packet closes the final named Step-5 evidence unit for `PlanetWndProc`.
The typed source now exposes the raw USER/GDI service sequence instead of
collapsing it into generic callbacks:

- `WM_PAINT` begins painting, draws the selected planet ship, and ends paint
  in the original order;
- `WM_ERASEBKGND` obtains the client rectangle and fills it with the button-face
  brush;
- `WM_GETMINMAXINFO` writes the recovered frame/title-bar minimum dimensions;
- `WM_DRAWITEM` distinguishes focus action `-1` from entire-item actions 1,
  2, and 4 and passes the original semantic modes `-2` and `-4`;
- `WM_MEASUREITEM` supplies `dyArial8 + 2`; and
- `WM_SETCURSOR` performs screen-to-client conversion, containment, ship-order
  precedence, planet-order fallback, and cursor-handle selection.

The existing 43-check dispatcher packet continues to cover child creation
fallback, color, keyboard navigation, command routing, focus sequencing,
production/tutor refresh, mouse clicks, activation, and the default-procedure
boundary. Together with E1 and E2, this closes the complete named Step-5
acceptance packet for the procedure.

Evidence:

- Host: `tests/planet_wnd_proc_integration_e3_probe.c` reports
  `PLANET_WND_PROC_INTEGRATION_E3_CHECKS=10 FAILURES=0 RESULT=PASS`.
- Native package: `compat/vc152/planet-wnd-proc-integration-e3/`.
- Native build: `runs/vmctl-vc152pwte3/PWTE3STA.TXT`, with
  `COMPILE=PASS` and `LINK=PASS`.
- Native compile/link logs: `runs/vmctl-vc152pwte3/PWTE3CL.LOG` and
  `runs/vmctl-vc152pwte3/PWTE3LINK.LOG`.
- Native report: `runs/vmctl-vc152pwte3/PWTE3.TXT` reports
  `PLANET_WND_PROC_INTEGRATION_E3_CHECKS=10 FAILURES=0 RESULT=PASS`.
- Original Win98 planet/fleet panel and map-selection evidence remains in
  `analysis/compat/planet-wnd-proc-integration.{md,json}`.

The Win98 process exited 0 and no Program Error dialog appeared. E1, E2, and
E3 are now closed; only the final Step-6 ledger promotion remains.
