# ProductionDlg oversized-capacity checkpoint

The saved-queue boundary now also guards the width of the packed queue
capacity. The readable source keeps the capacity unsigned until the recovered
four-entry table cap is applied; an oversized `0xffff` value therefore clamps
to four entries instead of becoming a negative signed value.

The strict host C89 probe passes:

`PRODUCTION_DLG_QUEUE_CHECKS=10 FAILURES=0 RESULT=PASS`

The same source and probe compiled and linked with VC++ 1.52c in the resumed
Win98 `StarDev` guest under `C:\PDGQ5`. The native report is:

`PRODUCTION_DLG_QUEUE_CHECKS=10 FAILURES=0 RESULT=PASS`

`PDGQSTA.TXT` reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
the redirected console file is empty, and the post-run window inventory had
no Program Error dialog.

This closes one packed-capacity safety sub-boundary inside the still-open
queue-control evidence unit. It does not close the full E3 unit: original
queue differential behavior, complete localized resource comparison, packed
production globals, and final promotion remain open.

Evidence:

- `tests/production_dlg_queue_probe.c`
- `reconstructed/src/production_dlg.c`
- `reconstructed/include/core.h`
- `compat/vc152/production-dlg-queue-e5/`
- `runs/vmctl-vc152pdgq5/`
