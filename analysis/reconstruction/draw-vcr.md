# DrawVCR reconstruction

`DrawVCR` (`001e:1c62-3a3e`, 7,644 bytes) renders an attack playback range. The
typed C implementation filters active tokens, computes armor/displacement and
surviving-ship statistics, formats player/design labels, places fleets on the
board, emits ordered statistics, and dispatches attack overlays.

The source uses rendering callbacks and named state rather than assembly or
emitted-byte replay. Its host and native probes cover active-token filtering,
statistics and label ordering, fleet placement, ordered text, and overlay
dispatch. Native VC++ 1.52c/Win98 validation reports `CHECKS=1`,
`FAILURES=0`, `RESULT=PASS` in `runs/vmctl-vc152ew/DVRRES.TXT`.
