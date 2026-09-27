# ProductionDlg exact child-layout checkpoint

`ProductionDlg` now has a dedicated E2 geometry packet for the recovered
`WM_INITDIALOG` boundary. The strict host C89 probe records the complete
`SetWindowPos` trace rather than checking only the final outer-window size:
the full-width and compact-width modes, the research control, four lower
controls, five centered list/control windows, navigation enablement, and the
initialization/sticky-position callbacks.

The host report is:

`PRODUCTION_DLG_LAYOUT_CHECKS=23 FAILURES=0 RESULT=PASS`

The same source and probe were compiled and linked with VC++ 1.52c in the
resumed Win98 `StarDev` guest under `C:\PDGE2`. The native report is:

`PRODUCTION_DLG_LAYOUT_CHECKS=23 FAILURES=0 RESULT=PASS`

`PDGESTA.TXT` reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
the redirected console file is empty, and the post-run window inventory has
no Program Error dialog.

This closes `ProductionDlg`'s exact child-geometry sub-boundary inside the E2
geometry/resources unit, but does not close that full unit or claim the
procedure complete. Packed production globals, localized popup resources,
full queue-control integration, and final ledger promotion remain.

Evidence:

- `tests/production_dlg_layout_probe.c`
- `reconstructed/src/production_dlg.c`
- `compat/vc152/production-dlg-e2/`
- `runs/vmctl-vc152pdge2/`
