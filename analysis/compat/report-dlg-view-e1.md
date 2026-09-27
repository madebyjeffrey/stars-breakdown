# ReportDlg packed `_rpt` view-load boundary

The readable `ReportDlg` source now exposes the recovered 54-byte `_rpt`
storage boundary through the production `stars_report_dlg_packed` bridge. The
bridge populates the typed report kind, visible-field mask,
field-first position, sort direction, row position/count, dialog position and
size, cached column widths, and scroll state before the original initialization
sequence creates child scrollbars and synchronizes popup state.

The strict host C89 probe passes:

`REPORT_DLG_VIEW_CHECKS=13 FAILURES=0 RESULT=PASS`

The strict host probe now calls the production bridge directly. The existing
26-check source probe, 24-check packed/integration probe, 6-check paint-view
probe, and 6-check draw integration probe still pass. A fresh production-source
VC++ 1.52c packet in
`compat/vc152/report-dlg-view-e1-production/` was replayed in `C:\RPTVEP`.
The build records `COMPILE=PASS` and `LINK=PASS`, and the native report is:

`REPORT_DLG_VIEW_CHECKS=13 FAILURES=0 RESULT=PASS`

The process exited 0 and the post-run Win98 window inventory contained no
Program Error dialog. This closes the E1 packed-view load unit. Populated
report unions/rows, exact live USER/GDI and menu payloads, complete caller
integration, and final promotion remain open.
