# ProductionDlg resource-lookup checkpoint

The readable `ProductionDlg` source now exposes the two localized resource
boundaries present in the original procedure. A left click in the production
help diamond copies resource `0x00d3` before dispatching the popup. A right
click resolves compressed string `0x04c0` before constructing the queue popup
menu. Clicks outside the diamond do not request either resource.

The strict host C89 probe checks the exact IDs, callback ordering, copied
popup text, menu dispatch, and outside-diamond suppression:

`PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=0 RESULT=PASS`

The same source and probe were compiled and linked with VC++ 1.52c in the
resumed Win98 `StarDev` guest under `C:\PDGRS2`. The native report is:

`PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=0 RESULT=PASS`

`PDGRSSTA.TXT` reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
the redirected console file is empty, and the post-run window inventory has
no Program Error dialog.

This closes the exact resource-ID and callback-order sub-boundary inside E2.
It does not close the complete localized resource-table/string comparison,
packed production globals, queue-control integration, or final promotion.

Evidence:

- `tests/production_dlg_resource_probe.c`
- `reconstructed/src/production_dlg.c`
- `reconstructed/include/core.h`
- `compat/vc152/production-dlg-resource-e2/`
- `runs/vmctl-vc152pdgrs2/`
