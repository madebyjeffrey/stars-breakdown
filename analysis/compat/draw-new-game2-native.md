# DrawNewGame2 native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dng2/`.

- `DNG2STA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DNG2CL.LOG`: VC++ 1.52c compilation completed for the current
  `draw_new_game2.c` source and probe
- `DNG2LINK.LOG`: segmented link completed with `DNG2.DEF`
- `DNG2R.TXT`: `DRAW_NEW_GAME2_CHECKS=2 FAILURES=0 RESULT=PASS`
- `DNG2R-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\DNG2`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check during
execution showed no program-error dialog. This validates the recovered DC
lifecycle, editable/read-only row paths, special race-6 label, formatting
callback, opaque-background restore, bold font, multi-digit labels, and
diamond/row rendering boundary. The E1 packed-table checkpoint is preserved
separately in `analysis/compat/draw-new-game2-packed-e1.md` and
`runs/vmctl-vc152dng2pack/`; it passes 13 host/native checks over the original
one-byte player/type table and 13-byte race-file table. The E2
localized-formatting packet is preserved in
`analysis/compat/draw-new-game2-localization-e2.md` and
`runs/vmctl-vc152dng2loc/`; it passes 15 host/native checks. The final E3
page-integration packet is preserved in
`analysis/compat/draw-new-game2-integration-e3.md` and
`runs/vmctl-vc152dng2int/`; it passes 16 host/native checks. DrawNewGame2 is
now promoted at Phase 2 `6/6`.
