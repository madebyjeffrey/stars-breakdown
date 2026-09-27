# DrawPopup geometry and style — E3 native supporting replay

The final packet in `compat/vc152/draw-popup-geometry-e3-r2/` was compiled and
run by VC++ 1.52c in Win98 under guest directory `C:\DPE9`.

- `DRINTSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DRINTCL.LOG`: native compiler completed normally
- `DRINTLINK.LOG`: segmented link completed normally
- `DRINT.TXT`: `DRAW_POPUP_GEOMETRY_E3_CHECKS=25 FAILURES=0 RESULT=PASS`
- process exit: `0`
- post-run window inventory: no Program Error dialog

The earlier DPE8 replay passed the initial 23-check geometry probe. The final
DPE9 replay adds the separator `PatBlt` rectangle and text-color reset checks.
An initial DPE8 wrapper attempt had mixed CRLF/LF endings and produced no
result files; corrected CRLF wrappers were transferred before the authoritative
replays. This is supporting E3 evidence, not closure of the named E3 unit;
complete detailed GDI/caller differential, original completion, and Step-6
promotion remain open.

Evidence:

- `analysis/compat/draw-popup-geometry-e3-host.{md,json}`
- `compat/vc152/draw-popup-geometry-e3-r2/`
- `runs/vmctl-vc152drawpopupgeometrye3r2/DRINTSTA.TXT`
- `runs/vmctl-vc152drawpopupgeometrye3r2/DRINTCL.LOG`
- `runs/vmctl-vc152drawpopupgeometrye3r2/DRINTLINK.LOG`
- `runs/vmctl-vc152drawpopupgeometrye3r2/DRINT.MAP`
- `runs/vmctl-vc152drawpopupgeometrye3r2/DRINT.TXT`
- `runs/vmctl-vc152drawpopupgeometrye3r2/guest-screen.bmp`
