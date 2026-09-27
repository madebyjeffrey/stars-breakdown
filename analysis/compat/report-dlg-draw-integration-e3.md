# ReportDlg draw integration boundary (host E3)

## Result

The strict host C89 probe passes:

```text
REPORT_DLG_DRAW_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS
```

## Scope

This probe exercises the reconstructed caller chain rather than only the
`ReportDlg` callback payload. A `WM_PAINT` dispatch enters `ReportDlg`, the
typed paint callback forwards the populated view state to `DrawReport`, and
the report renderer forwards a populated planet row to `DrawReportItem`.
The callbacks record the report kind, row/field selection, header width,
planet lookup, text output, field drawing, and selection update. The row is a
populated `New Eden` planet rather than an empty or failure-only case.

## Status

This is host-only evidence. It closes no authoritative TODO6 Step-5 unit:
native VC++ 1.52c replay, exact USER/GDI/menu payloads, packed report-row
unions, original caller behavior, and final promotion remain open. It does,
however, replace an opaque caller-integration concern with a concrete tested
host chain and identifies the remaining native work as replay and comparison
of the same populated path. The guest agent has not announced `AGENTINFO`, so
no native claim is made.

## Inputs

- `reconstructed/src/report_dlg.c`
- `reconstructed/src/draw_report.c`
- `reconstructed/src/draw_report_item.c`
- `reconstructed/include/core.h`
- `tests/report_dlg_draw_integration_probe.c`
