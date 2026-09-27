# DrawPopup production packed-selector E5 native replay

Fresh VC++ 1.52c/Win98 validation in `C:\DPE5R` compiled and linked the
current production `reconstructed/src/draw_popup.c` with the current 22-byte
`_popupdata` probe and header. The native report is
`DRAW_POPUP_PACKED_CHECKS=37 FAILURES=0 RESULT=PASS`; the process exited 0 and
the final guest window list contained no Program Error dialog.

The 37 checks cover selector precedence, the full dispatcher-kind matrix,
kind-14 slot/hull aliasing, client/slot geometry, GDI-state callback ordering,
and short-record fallback through the production bridge. The first attempt is
preserved separately in `runs/vmctl-vc152dpe5/`; it failed before linking only
because the packet transfer omitted the lowercase `core.h`. The corrected
fresh replay is `runs/vmctl-vc152dpe5r/`.

This closes the native replay subcheck for the packed selector bridge, not a
named Step-5 unit. Complete `GlobalPD` union payloads, localization, hull
filtering/formatting, detailed GDI rendering, original caller differential,
and Step-6 promotion remain open.

Evidence:

- `tests/draw_popup_packed_probe.c`
- `compat/vc152/draw-popup-packed-e5r-native/`
- `runs/vmctl-vc152dpe5r/DRINT.TXT`
- `runs/vmctl-vc152dpe5r/DRINTSTA.TXT`
- `runs/vmctl-vc152dpe5r/DRINTCL.LOG`
- `runs/vmctl-vc152dpe5r/DRINTLINK.LOG`
- `runs/vmctl-vc152dpe5/DRINTSTA.TXT`
- `runs/vmctl-vc152dpe5/DRINTCL.LOG`
