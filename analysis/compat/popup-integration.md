# Popup original/UI integration checkpoint

The native `Popup` checkpoint already covers all sizing branches, measured and
fixed geometry, slot updates and extra rows, screen-edge clamping, font/DC
cleanup, popup creation, font messaging, and capture. The original Win98
executable's popup base, planet popup, starting popup, and restored-frame
screenshots establish the corresponding client-to-screen and window-lifecycle
consumer boundary without a Program Error dialog.

This advances `Popup` to Phase 2 step 5/6. It does not claim localized
measurement, packed fleet/planet/hull content records, or detailed popup
content layout; those remain step-6 promotion gates.

Evidence:

- `tests/popup_source_probe.c`
- `analysis/compat/popup-native.md`
- `runs/vmctl-vc152popup/POPSTA.TXT`
- `runs/vmctl-vc152popup/POPCL.LOG`
- `runs/vmctl-vc152popup/POPLINK.LOG`
- `runs/vmctl-vc152popup/POPR.TXT`
- `runs/vmctl-vc152current/popup-base.bmp`
- `runs/vmctl-vc152current/popup-probe-start.bmp`
- `runs/vmctl-vc152current/popup-probe-planet.bmp`
- `runs/vmctl-vc152current/popup-probe-restored.bmp`
