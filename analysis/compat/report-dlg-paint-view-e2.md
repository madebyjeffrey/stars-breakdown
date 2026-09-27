# ReportDlg typed paint-view boundary (host E2)

## Result

The strict host C89 probe passes:

```text
REPORT_DLG_PAINT_VIEW_CHECKS=6 FAILURES=0 RESULT=PASS
```

## Scope

`ReportDlg` now has an optional `StarsReportDlgDrawView` callback. On
`WM_PAINT`, the typed callback receives the report window handle, device
context, paint rectangle, and the populated `StarsReportViewState`. The probe
checks report kind, first/total row, field count, visible-field mask, cached
first-column width, and paint rectangle. The existing opaque `draw_report`
callback remains the fallback, so existing callers retain their behavior.

## Status

A production-source packet in
`compat/vc152/report-dlg-paint-view-production-e2/` compiled and linked in
fresh `C:\RPTPVP`. The paint probe wrote
`REPORT_DLG_PAINT_VIEW_CHECKS=6 FAILURES=0 RESULT=PASS`, but the normal
`WinMain` return path then produced a Win98 illegal-operation dialog and exit
130. The screenshot is preserved in
`runs/vmctl-vc152rptpvpe2/hung.png`.

The same packet was rerun with an explicit `ExitProcess` after writing the
report. That controlled termination exited 0, produced the same six-check
report, and showed no Program Error dialog. This establishes the callback
behavior as a native supporting subcheck while exposing a teardown/ABI issue
that must be resolved before the native E2 evidence can be accepted as a
clean closure. It closes no TODO6 Step-5 unit; populated report unions/rows,
exact live USER/GDI/menu behavior, original caller comparison, and promotion
remain open.

## Inputs

- `reconstructed/src/report_dlg.c`
- `reconstructed/include/core.h`
- `tests/report_dlg_paint_view_probe.c`
