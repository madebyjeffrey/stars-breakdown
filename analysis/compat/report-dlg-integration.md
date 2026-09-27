# ReportDlg packed-record integration checkpoint

The CodeView `_rpt` definition is a 54-byte packed record. The integration
probe maps its recovered offsets into the typed `StarsReportDlgState`, runs the
current `ReportDlg` source through initialization and horizontal/vertical
scroll paths, and writes the changed fields back to the raw record.

The checked offsets are `grbitVisible` 0, `irpt` 4, `cFields` 6,
`cFieldFirst` 8, `icolSort` 10, `fAscending` 12, `irowFirst` 14, `ptDlg` 16,
`ptSize` 20, `rgbdx` 26, `cRows` 42, `cRowsVis` 44, `iSubsort` 46,
`hwndVScroll` 48, `hwndHScroll` 50, and `cColScroll` 52. The probe also
checks the 54-byte size, cached header widths, scrollbar handles, field-first
mutation, row-first mutation, and callback invalidation/update order.

Host strict C89 and native VC++ 1.52c reports are:

`REPORT_DLG_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152rptint/`; the process exited 0 and the post-run window list
contained no Program Error dialog. This is a packed-record and service-boundary
checkpoint, not proof that every original USER/GDI handle or report caller is
complete.

The expanded fresh `C:\RPTINT2` run additionally checks the `SCROLLBAR` class,
vertical/horizontal styles and parent window, synchronization of the popup
sort/visibility state, the `300x220` minimum-window boundary, and the report
menu selection `0x0900` through the frame submenu. The remaining step-5
concerns are populated report unions/rows, exact live USER/GDI and menu
payload behavior, complete caller integration, and promotion.

The first expanded native attempt exposed one VC1.52c-specific issue: the
32-bit scrollbar styles were declared as enum constants, producing out-of-range
warnings and a failed style assertion. They are now explicit `UL` macros; the
corrected rebuild and rerun pass all 24 checks in
`runs/vmctl-vc152rptint2/RPTINT5.TXT`.

This advances `ReportDlg` to Phase 2 step 5/6, not promotion. Packed report
unions and row contents, exact live USER/GDI handles, menu resources, and full
caller integration remain open.

Evidence:

- `tests/report_dlg_integration_probe.c`
- `compat/vc152/report-dlg-integration/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1108_0018_ReportDlg.c`
- `analysis/reconstruction/report-dlg.md`
- `runs/vmctl-vc152rptint2/RPTISTA5.TXT`
- `runs/vmctl-vc152rptint2/RPTICL5.LOG`
- `runs/vmctl-vc152rptint2/RPTILINK5.LOG`
- `runs/vmctl-vc152rptint2/RPTINT5.TXT`
- `runs/vmctl-vc152rptint/REPIN.MAP`
