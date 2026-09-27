# Popup native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152popup/`.

- `POPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `POPCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `POP.C` and `PROBE.C` source
- `POPLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `POPR.TXT`: `POPUP_CHECKS=31 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `POP.C` is byte-identical to
`reconstructed/src/popup.c`, and `PROBE.C` is byte-identical to
`tests/popup_source_probe.c`. The probe covers client-to-screen conversion,
all sizing branches, slot updates and extra rows, font/DC cleanup,
screen-edge clamping, popup creation, font messaging, and capture.

This advances `Popup` from Phase 2 step 3/6 to step 4/6. Localized measurement,
packed fleet/planet/hull records, detailed content layout, and original-
behavior evidence remain open.
