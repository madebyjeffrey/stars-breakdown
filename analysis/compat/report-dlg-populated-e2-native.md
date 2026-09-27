# ReportDlg populated E2 native replay

The current production `ReportDlg` and renderer sources were replayed through
a populated report matrix in fresh VC++ 1.52c/Win98 directory `C:\RPE2`. The
strict host and native reports are:

```text
REPORT_DLG_POPULATED_E2_CHECKS=43 FAILURES=0 RESULT=PASS
```

The matrix loads the production 54-byte `_rpt` view bridge and drives
`WM_CREATE`, `WM_PAINT`, and `WM_DESTROY` for planet, fleet, scan-fleet, and
battle report kinds. It carries populated typed row records through
`DrawReport` and `DrawReportItem`, including localized names, installation
status bars, production and mineral paths, fleet totals, scanner fields, and
battle unit/kills fields. It also records the live callback payloads for DC
acquisition/release, scrollbar class/style/parent creation, layout, paint,
header measurement, text/GDI drawing, report-row lookup, frame-menu lookup,
menu-item cleanup, and current-row invalidation.

VC++ 1.52c compile and link passed, the process exited 0, and the final guest
window inventory contained no Program Error dialog. This closes the named
ReportDlg E2 populated-report-union and live USER/GDI/menu-payload unit when
combined with the original Reports-menu/report/header observation already
recorded for E3. Final Step-6 promotion remains separate.

Evidence is preserved in
`tests/report_dlg_populated_e2_probe.c`,
`compat/vc152/report-dlg-populated-e2-native/`, and
`runs/vmctl-vc152rptpe2/`.
