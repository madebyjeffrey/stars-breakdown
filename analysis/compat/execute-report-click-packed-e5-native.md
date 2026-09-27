# ExecuteReportClick production packed bridge E5 native replay

Fresh VC++ 1.52c/Win98 validation in `C:\EXINT5` compiled and linked the
current production `reconstructed/src/execute_report_click.c`, the current
packed probe, and the current `core.h`. The native report is
`EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`; the process
exited 0 and the final guest window list contained no Program Error dialog.

The matrix covers production decoding of the 124-byte `_fleet`, 56-byte
`_planet`, and 14-byte `_btldata` records; fleet/planet/battle selection;
fleet, installation, mineral, resource, starbase, planet, and defense popup
union fields; the resource-exempt bit; and production/transfer modal guards.

This replaces the prior historical adapter-only native claim for the
production packed bridge, but it does not close a named Step-5 unit. Exact
original `GlobalPD` owner/part-pointer cases, live USER/GDI handles, complete
report caller/layout integration, original behavior, and promotion remain.

Evidence:

- `tests/execute_report_click_packed_probe.c`
- `compat/vc152/execute-report-click-packed-e5-native/`
- `runs/vmctl-vc152exint5/EXINT.TXT`
- `runs/vmctl-vc152exint5/EXINTSTA.TXT`
- `runs/vmctl-vc152exint5/EXINTCL.LOG`
- `runs/vmctl-vc152exint5/EXINTLINK.LOG`
