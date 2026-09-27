# ExecuteReportClick popup caller integration (host E3)

## Result

The strict host C89 probe passes:

```text
EXECUTE_REPORT_CLICK_POPUP_INTEGRATION_CHECKS=26 FAILURES=0 RESULT=PASS
```

## Scope

`tests/execute_report_click_popup_integration_probe.c` connects the typed
`ExecuteReportClick` popup callback to the reconstructed `Popup` and
`DrawPopup` procedures. It follows three real report-cell cases: fleet detail
(kind 3), factory installation (kind 8), and resources (kind 12). For each
case it verifies that the semantic popup payload survives the caller boundary,
that `Popup` measures and creates the expected window, and that the simulated
paint path reaches the matching `DrawPopup` renderer branch. It also checks the
resource research-share arithmetic, factory weight extraction, popup sizing
adjustments, message/capture setup, and report DC release symmetry.

## Status

A fresh production-source VC++ 1.52c packet in
`compat/vc152/execute-report-click-popup-production-e3/` ran in `C:\POPINTP`.
Compile and link passed, the process exited 0, and the native report is:

`EXECUTE_REPORT_CLICK_POPUP_INTEGRATION_CHECKS=26 FAILURES=0 RESULT=PASS`

The post-run Win98 window inventory contained no Program Error dialog. This
closes the native `ExecuteReportClick -> Popup -> DrawPopup` caller subcheck,
but not a named Step-5 unit: exact original `GlobalPD` owner/part payloads,
localized content, live USER/GDI behavior, complete report-layout integration,
original comparison, and promotion remain open.

## Inputs

- `reconstructed/src/execute_report_click.c`
- `reconstructed/src/popup.c`
- `reconstructed/src/draw_popup.c`
- `reconstructed/include/core.h`
- `tests/execute_report_click_popup_integration_probe.c`
