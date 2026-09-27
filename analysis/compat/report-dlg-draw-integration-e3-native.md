# ReportDlg populated caller-chain E3 native replay

The populated report caller chain was replayed through the current production
sources in fresh VC++ 1.52c / Win98 directory `C:\RPTDINT`. The native report
is:

```text
REPORT_DLG_DRAW_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS
```

The six checks drive `WM_PAINT` through `ReportDlg`, forward the populated
view to `DrawReport`, and render a populated `New Eden` planet row through
`DrawReportItem`. They verify header measurement, planet lookup, text output,
field drawing, and selection update. Compile and link passed, the process
exited 0, and the final guest window inventory contained no Program Error
dialog.

Together with the original Win98 Reports-menu → `Planet Summary Report -- 1
Planet` → `Planet Name` sort-popup observation, this closes the named
`ReportDlg` E3 native/original caller-integration unit. E2's exact populated
report unions and live USER/GDI/menu payloads, plus final promotion, remain
open.

Evidence is preserved in
`analysis/compat/report-dlg-draw-integration-e3-native.{md,json}`,
`compat/vc152/report-dlg-draw-integration-e3-native/`, and
`runs/vmctl-vc152rptdint/`.
