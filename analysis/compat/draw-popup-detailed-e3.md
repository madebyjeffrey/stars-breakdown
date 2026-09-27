# DrawPopup detailed GDI and caller integration — E3 closure

DrawPopup E3 is closed at the named Step-5 boundary. The final detailed text
boundary records the recovered positions, anchors, fonts, opaque background,
separator `PatBlt` rectangle/raster operation, and text-color reset. Its strict
C89 and final VC++ 1.52c/Win98 replays each pass
`DRAW_POPUP_GEOMETRY_E3_CHECKS=25 FAILURES=0 RESULT=PASS`.

The closure also includes the native 7-check GDI state-order/slot-hull probe
and the fresh 26-check native
`ExecuteReportClick -> Popup -> DrawPopup` caller replay. The original Win98
mineral, population, report, and restored-popup consumer observations remain
consistent and show no Program Error dialog.

This closes one named DrawPopup Step-5 evidence unit. E4 original completion
and Step-6 promotion remain open; this is not a claim that the full procedure
is complete.

Evidence:

- `analysis/compat/draw-popup-geometry-e3-host.{md,json}`
- `analysis/compat/draw-popup-geometry-e3-native.{md,json}`
- `analysis/compat/draw-popup-state-e3.{md,json}`
- `analysis/compat/execute-report-click-popup-integration-e3.{md,json}`
- `runs/vmctl-vc152drawpopupgeometrye3r2/`
- `runs/vmctl-vc152drawpopupstatee3/`
- `runs/vmctl-vc152popintp/`
- `analysis/compat/draw-popup-integration.json`
