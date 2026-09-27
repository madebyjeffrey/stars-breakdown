# Popup

`Popup` is represented as typed sizing and window-placement logic in
`reconstructed/src/popup.c`.  It preserves client-to-screen conversion, the
popup-kind sizing split, fixed geometry for component/build/simple cases,
slot-update and extra-row handling, font/DC cleanup, screen-edge clamping,
popup creation, font message, and capture.

Localized measurement, packed fleet/planet/hull records, and detailed content
layout remain callback-owned.  The typed measurement boundary now carries the
original `200` width argument for factory/resource popups and the selected
planet ID for string popups.  The host probe covers measured, fixed-size,
slot-update, edge-clamped, and simple-popup paths. A fresh VC++ 1.52c / Win98
checkpoint passes all 31 native checks without a runtime error dialog. The
original executable's popup base, planet popup, and restored-frame screenshots
establish the client-to-screen, window-creation, and capture/restoration
consumer boundary. The E2 closure now exercises localized measurement and the
packed content boundary through all measured and fixed popup kinds. A
148-check strict host and fresh Win98 run agree with an independent geometry
oracle over popup, fleet, planet, and hull payloads, including slot updates,
clamping, capture, and message setup. `Popup` is promoted to Phase 2 step
6/6; see `analysis/compat/popup-packed-integration.md` and
`runs/vmctl-vc152puint2/`.

The packed-record integration checkpoint is now also complete. A strict host
adapter and a fresh VC++ 1.52c / Win98 `C:\PUINT` run each report
`POPUP_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS` over the recovered 22-byte
`_popupdata` layout, including the popup selector, planet-id measurement
bridge, slot extra-row bridge, and oversized component-popup screen clamp.
Compile and link passed, the process exited 0, and no Program Error dialog
appeared. This was the initial packed-record checkpoint; the E2 closure above
completes the remaining procedure boundary. See
`analysis/compat/popup-packed-integration.{md,json}` and
`runs/vmctl-vc152puint/`.

The later live-original report-click capture identified a placement error in
the previously promoted source: original `Popup` subtracts measured width and
height from the screen click before clamping, so the click is the popup's
bottom-right anchor. The production source and its geometry oracle are now
corrected, and the 32-bit `CreateWindow` style argument is preserved. Fresh
host/native `C:\PUIE4` 148/148 and `C:\ERUI2` 86/86 revalidate the Popup
boundary and the report-click caller chain. See
`analysis/compat/execute-report-click-live-ui-geometry-e2.md` and
`runs/vmctl-gp-ui-r2/`. This does not itself close ExecuteReportClick ER-E2's
remaining real USER/GDI lifecycle.
