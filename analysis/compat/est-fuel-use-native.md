# EstFuelUse native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152efu/`.

- `EFUSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `EFUCL.LOG`: VC++ 1.52c compilation completed for the current
  `est_fuel_use.c` source and probe, including its widened floating-point
  intermediate path
- `EFULINK.LOG`: segmented link completed with `EFU.DEF`
- `EFUR.TXT`: `EST_FUEL_USE_CHECKS=6 FAILURES=0 RESULT=PASS`
- `EFU-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\EFU`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates order-derived warp
selection, distance lookup, race discount, ram-scoop flagging, fuel ordering,
fixed-point cost/range conversion, invalid-warp handling, the zero-fuel range
sentinel, and the large intermediate-product case. Exact packed
design/component integration and full original differential coverage remain
outside this checkpoint.
