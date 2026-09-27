# DrawProductionDlg geometry and integration E3 validation

The host geometry probe exercises the remaining drawing-boundary details that
were not covered by the packed E1 or resource-table E2 probes:

- DC acquisition and release when the caller supplies no HDC;
- source and one-based planet list control IDs and selection traversal;
- the original 20-pixel inset for mineral labels and the measured `kT`
  separator boundary;
- right-aligned value placement two pixels left of that separator boundary;
- the two list rectangles, client-rectangle diamond placement, and ETA
  dispatch.

The source correction preserves the original horizontal sequence: the heading
uses the list rectangle's left edge, mineral labels use `left + 0x14`, the
separator uses `right - 0x14 - width("kT")`, and `RightTextOut` uses that
separator position minus two pixels.

Host strict C89 validation passes:

`DRAW_PRODUCTION_DLG_GEOMETRY_CHECKS=9 FAILURES=0 RESULT=PASS`

The existing packed and resource probes still pass, and the full regression
suite passes (`519` tests, `153` skipped). Fresh VC++ 1.52c/Win98 validation in
`C:\DPDRE2` compiled and linked the packet and returned:

`DRAW_PRODUCTION_DLG_GEOMETRY_CHECKS=9 FAILURES=0 RESULT=PASS`

The redirected console was empty and the post-run Win98 window list contained
no Program Error dialog. Together with the original Ship & Starbase Designer
observation recorded in `analysis/compat/production-dialog-integration.md`,
this closes the named E3 GDI/caller-boundary unit. Step-6 promotion is now
recorded in the review ledger and derived coverage/TODO5 ledgers.

Evidence:

- `analysis/decompiler/export/raw/10d0_35dc_DrawProductionDlg.c`
- `reconstructed/src/draw_production_dlg.c`
- `tests/draw_production_dlg_geometry_probe.c`
- `tests/draw_production_dlg_source_probe.c`
- `tests/draw_production_dlg_resource_probe.c`
- `compat/vc152/draw-production-dlg-geometry-e3/`
- `runs/vmctl-vc152dpdre3/DPDRESTA.TXT`
- `runs/vmctl-vc152dpdre3/DPDRECL.LOG`
- `runs/vmctl-vc152dpdre3/DPDRELINK.LOG`
- `runs/vmctl-vc152dpdre3/DPDGRES.TXT`
- `runs/vmctl-vc152dpdre3/DPDRE-CONSOLE.TXT`
