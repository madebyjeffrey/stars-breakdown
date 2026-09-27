# TransferStuff native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152trn/`.

- `TRNSSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `TRNSCL.LOG`: VC++ 1.52c compilation completed for the current
  `transfer_stuff.c` source and probe
- `TRNSLINK.LOG`: segmented link completed with `TRNS.DEF`
- `TRNSR.TXT`: `TRANSFER_STUFF_CHECKS=20 FAILURES=0 RESULT=PASS`
- `TRNS-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\TRNS`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates object preparation,
split-versus-jettison handling, valid-hull enumeration, tutor/dialog flow,
cancellation and empty-fleet cleanup, cargo balancing, selection refresh,
scanner invalidation, and transfer-mode reset. This original checkpoint remains
limited to the callback-owned balance boundary; the later E2 packet separately
binds and validates the reconstructed cargo balance service, while packed
fleet/planet/thing lookup, exact dialog behavior, and Win16 lookup/refresh
calls remain open.
