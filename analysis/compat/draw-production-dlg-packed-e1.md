# DrawProductionDlg packed-production E1 checkpoint

This checkpoint closes the packed-list handoff at the `DrawProductionDlg`
acceptance boundary. The dialog now accepts the recovered source-list and
planet-list packed `_prod` arrays with their recorded item counts. For a
selected source item it passes the exact zero-based packed word; for the
planet list it preserves the original one-based UI selection normalization
before indexing the packed list. A missing list or an index at/above the
recorded count is rejected without calling the packed-cost service.

The existing decoded `get_costs` callback remains supported and takes
precedence, so this change adds the recovered packed-global path without
changing callers that already supply decoded costs.

Host strict C89 and fresh VC++ 1.52c/Win98 validation both pass:

`DRAW_PRODUCTION_DLG_CHECKS=17 FAILURES=0 RESULT=PASS`

The native packet in `C:\DPD4` reports `COMPILE=PASS` and `LINK=PASS`; the
process exited 0, redirected console output is empty, and the post-run window
list contains no Program Error dialog. Evidence is preserved in
`runs/vmctl-vc152dpd4/`.

This closes DrawProductionDlg step-5 E1, the packed production-global
boundary. Step-5 E2 remains the exact formatting/resource-table and localized
string comparison, and E3 remains full Win16 GDI/caller integration. The
procedure therefore remains at Phase 2 step 5/6 with `1/3` evidence units
closed; no promotion is claimed.

Evidence:

- `tests/draw_production_dlg_source_probe.c`
- `reconstructed/src/draw_production_dlg.c`
- `reconstructed/include/core.h`
- `compat/vc152/draw-production-dlg/DPD.C`
- `compat/vc152/draw-production-dlg/PROBE.C`
- `compat/vc152/draw-production-dlg/core.h`
- `runs/vmctl-vc152dpd4/DPDSTA.TXT`
- `runs/vmctl-vc152dpd4/DPDCL.LOG`
- `runs/vmctl-vc152dpd4/DPDLINK.LOG`
- `runs/vmctl-vc152dpd4/DPDR.TXT`
- `runs/vmctl-vc152dpd4/DPD-CONSOLE.TXT`
- `runs/vmctl-vc152dpd4/guest-after-native.bmp`
