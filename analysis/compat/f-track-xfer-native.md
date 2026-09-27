# FTrackXfer native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152ftx/`.

- `FTXSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `FTXCL.LOG`: VC++ 1.52c compilation completed for the current
  `f_track_xfer.c` source and probe
- `FTXLINK.LOG`: segmented link completed with `FTX.DEF`
- `FTXR.TXT`: `F_TRACK_XFER_CHECKS=24 FAILURES=0 RESULT=PASS`
- `FTX-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\FTX`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates hit selection, drag
eligibility, capture and mouse tracking, button-rectangle proportionality,
signed supply direction, keyboard-derived steps, bounded dialog changes,
button changes, final refresh, and redraw. Packed transfer records, Win16
capture behavior, cargo/supply mutation, and full integration remain outside
this checkpoint.
