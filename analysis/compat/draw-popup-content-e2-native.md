# DrawPopup localization, hull filtering, and formatting — E2 native replay

The production `DrawPopup` E2 content boundary was replayed with Microsoft
Visual C++ 1.52c in the Win98 guest. The packet used the current readable
`draw_popup.c`, `core.h`, and strict-C89 content probe, with the DOS wrapper
changing to `C:\DPE7` before invoking the build and executable.

The authoritative native report is:

```text
DRAW_POPUP_CONTENT_E2_CHECKS=21 FAILURES=0 RESULT=PASS
```

The build status is `COMPILE=PASS` and `LINK=PASS`; the process exited 0. The
final guest window inventory contained only the agent, desktop, and cabinet
windows, with no Program Error dialog. The final screenshot is retained as
additional evidence.

The 21-check matrix covers popup kinds 1–4, recovered localized resource IDs,
numeric format kinds, full 32-bit fleet-pointer forwarding, hull visibility
filtering, hull-definition technology extraction, hull-name decoration,
count/damage formatting, and the coordinate label/value sequence.

The first native attempt is preserved separately in
`runs/vmctl-vc152drawpopupcontente2/`; it compiled but failed at link because
the host-only probe summary `printf` was not available in the minimal Win16
packet. The probe now suppresses that console-only print under
`TARGET_NATIVE`; the WinMain wrapper writes the authoritative report instead.

Evidence:

- `analysis/compat/draw-popup-content-e2-host.{md,json}`
- `compat/vc152/draw-popup-content-e2/`
- `runs/vmctl-vc152drawpopupcontente2-r2/DRINTSTA.TXT`
- `runs/vmctl-vc152drawpopupcontente2-r2/DRINTCL.LOG`
- `runs/vmctl-vc152drawpopupcontente2-r2/DRINTLINK.LOG`
- `runs/vmctl-vc152drawpopupcontente2-r2/DRINT.TXT`
- `runs/vmctl-vc152drawpopupcontente2-r2/DRINT.MAP`
- `runs/vmctl-vc152drawpopupcontente2-r2/guest-screen.bmp`
- `runs/vmctl-vc152drawpopupcontente2/DRINTSTA.TXT`
- `runs/vmctl-vc152drawpopupcontente2/DRINTLINK.LOG`
