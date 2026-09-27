# ProductionDlg accepted-editor checkpoint

The saved-queue boundary now covers both outcomes of the editor callback. The
readable `ProductionDlg` source passes the four queue records into the typed
editor service; an accepted mutation remains in the queue table, while a
canceled mutation is restored from the pre-dialog snapshot. The same packet
also verifies sparse valid-slot menu mapping and the localized editor entry.

The strict host C89 probe passes:

`PRODUCTION_DLG_QUEUE_CHECKS=9 FAILURES=0 RESULT=PASS`

The same source and probe compiled and linked with VC++ 1.52c in the resumed
Win98 `StarDev` guest under `C:\PDGQ4`. The native report is:

`PRODUCTION_DLG_QUEUE_CHECKS=9 FAILURES=0 RESULT=PASS`

`PDGQSTA.TXT` reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
the redirected console file is empty, and the post-run window inventory had no
Program Error dialog.

This closes the accepted/canceled mutation sub-boundary of the queue editor.
It does not close the full E3 unit: live USER/message-loop behavior, original
queue differential evidence, packed production globals, complete localized
resource comparison, and final promotion remain.

Evidence:

- `tests/production_dlg_queue_probe.c`
- `reconstructed/src/production_dlg.c`
- `reconstructed/include/core.h`
- `compat/vc152/production-dlg-queue-e4/`
- `runs/vmctl-vc152pdgq4/`
