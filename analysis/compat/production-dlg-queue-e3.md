# ProductionDlg queue-menu checkpoint

The readable `ProductionDlg` source now reconstructs the saved-production
queue menu boundary instead of treating the menu as an opaque integer-only
callback. When queue records are supplied, it filters the four 40-byte queue
slots to valid entries, preserves their original slot indices, passes the
localized editor entry (`0x04c0`) after the queue names, and maps a selected
menu row back to the underlying queue slot. The editor path also snapshots the
queue table and restores it when the editor reports cancel, matching the raw
`_memcpy` rollback around `ZipProdDlg`.

The strict host C89 probe passes:

`PRODUCTION_DLG_QUEUE_CHECKS=7 FAILURES=0 RESULT=PASS`

The same source and probe compiled and linked with VC++ 1.52c in the resumed
Win98 `StarDev` guest under `C:\PDGQ3`. The native report is:

`PRODUCTION_DLG_QUEUE_CHECKS=7 FAILURES=0 RESULT=PASS`

`PDGQSTA.TXT` reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
the redirected console file is empty, and the post-run window inventory had no
Program Error dialog.

This closes the saved-queue menu construction/selection and editor-cancel
rollback sub-boundary inside E3. It does not close the full queue-control
integration: production globals, complete localized resource comparison, live
USER/message-loop behavior, accepted editor mutation, original differential
evidence, and final promotion remain.

Evidence:

- `tests/production_dlg_queue_probe.c`
- `reconstructed/src/production_dlg.c`
- `reconstructed/include/core.h`
- `compat/vc152/production-dlg-queue-e3/`
- `runs/vmctl-vc152pdgq3/`
