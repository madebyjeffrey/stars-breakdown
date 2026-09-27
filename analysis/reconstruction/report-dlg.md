# ReportDlg reconstruction

`ReportDlg` is a typed C reconstruction of the 2,511-byte NE `0022:0018–09e7`
report window procedure.  The source models the report view and popup state
explicitly, including the original initialization sequence, cached column
width calculation, report sorting, child scrollbar creation, sticky dialog
position, tutor transitions, resize layout, paint and erase handling, minimum
window tracking size, report-handle cleanup, menu cleanup, and
default-procedure fallthrough.

The layout path preserves the original one-pixel scrollbar window bottom
calculation (`row_height * visible_rows + 1`).  The horizontal-scroll path
preserves command increments, thumb-position
handling, clamping, visible-field skipping, first-visible-column recomputation,
invalidation, and update.  The vertical-scroll path preserves line/page/thumb
commands, row clamping, report-body scrolling, scrollbar synchronization, and
window refresh.  Mouse handling separates header clicks from row clicks and
dispatches them to typed column-popup and report-click callbacks with the
original visible-column and row calculations.

`tests/report_dlg_source_probe.c` covers those paths with 26 strict host C89
checks. A fresh VC++ 1.52c / Win98 checkpoint passes all 26 native checks
without a runtime error dialog. Exact packed report records, the original
USER/GDI window handles, menu resources, and the full caller integration remain
callback-owned boundaries for the native pass.

The original Win98 executable has now been exercised through the report
integration boundary: the Reports menu opened `Planet Summary Report -- 1
Planet`, and the `Planet Name` header opened its sort/reverse-sort popup
without a Program Error. Evidence is preserved in
`analysis/compat/report-popup-integration.md` and
`runs/vmctl-vc152current/`. This advances `ReportDlg` to Phase 2 step 5/6 for
the original report-window/header-popup boundary; packed report records,
exact USER/GDI handles, menu resources, and full caller integration remain
open.

The CodeView `_rpt` definition is now exercised through a 54-byte packed
adapter. The host and native integration reports each pass 20 checks covering
the recovered field offsets, raw-to-typed round trip, initialization layout,
cached widths, scrollbar handle writeback, horizontal field-first mutation,
vertical row-first mutation, and invalidation/update callbacks. Evidence is in
`analysis/compat/report-dlg-integration.{md,json}` and
`runs/vmctl-vc152rptint/`. Packed report unions and row contents, exact live
USER/GDI handles, menu resources, and complete caller integration remain open;
the procedure remains at step 5/6.

The packed adapter was expanded in fresh `C:\RPTINT2` to 24 host/native
checks. It now also verifies the `SCROLLBAR` class and vertical/horizontal
styles with the report window as parent, popup-state synchronization, the
`300x220` minimum-window boundary, and cleanup of report-menu item `0x0900`
through the frame submenu. Compile and link passed and no Program Error
dialog appeared. Populated report unions/rows, exact live USER/GDI and menu
payload behavior, complete caller integration, and promotion remain open.

The initial expanded native run caught a VC1.52c enum-width issue in the
32-bit scrollbar style constants; those constants are now explicit `UL`
macros. The corrected run passes all 24 checks in
`runs/vmctl-vc152rptint2/RPTINT5.TXT`.

The host-side paint boundary now also exposes an optional typed
`StarsReportDlgDrawView` callback. A fresh strict-C89 probe passes
`REPORT_DLG_PAINT_VIEW_CHECKS=6 FAILURES=0 RESULT=PASS`; it verifies that a
paint receives the populated report kind, row window, field count, visible
field mask, cached column width, and paint rectangle while retaining the
legacy opaque callback as a fallback. This narrows the caller-integration
gap, but it is host-only until the VC1.52c packet can be replayed; populated
report unions/row payloads, exact live USER/GDI/menu payloads, original
caller behavior, and promotion remain open. The matching native packet is
prepared at `compat/vc152/report-dlg-paint-view-e2/` for `C:\RPTVE2`; its
build and report remain pending guest-agent reconnect.

The typed paint path is now exercised through the reconstructed report
renderers as well. `tests/report_dlg_draw_integration_probe.c` passes
`REPORT_DLG_DRAW_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS` after dispatching
`WM_PAINT`, forwarding the populated view to `DrawReport`, and forwarding a
populated `New Eden` planet row to `DrawReportItem`. The probe records header
measurement, planet lookup, text/field drawing, and selection update. This is
still host-only and closes no Step-5 unit; the native replay and original
populated-row comparison remain the next acceptance boundary. Evidence is in
`analysis/compat/report-dlg-draw-integration-e3.{md,json}`.

A production-source VC++ 1.52c paint packet in `C:\RPTPVP` also reaches the
typed callback and writes `REPORT_DLG_PAINT_VIEW_CHECKS=6 FAILURES=0
RESULT=PASS`. Its ordinary `WinMain` return path then triggers a Win98
illegal-operation dialog (exit 130), while an explicit `ExitProcess` teardown
exits 0 with the same report and no dialog. The callback semantics are
therefore native supporting evidence, but the teardown/ABI issue remains an
open acceptance defect; this closes no Step-5 unit.

The 54-byte packed `_rpt` decoder is now owned by production source as
`stars_report_dlg_packed` in `reconstructed/src/report_dlg.c`; the focused
view probe calls that bridge directly rather than carrying a test-only loader.
A fresh production-source VC++ 1.52c packet in `C:\RPTVEP` now compiles and
links successfully and reports `REPORT_DLG_VIEW_CHECKS=13 FAILURES=0
RESULT=PASS` under Win98, with process exit 0 and no Program Error dialog.
This closes the packed-view-load E1 unit. Populated report unions and row
contents, live USER/GDI/menu payloads, complete caller integration, and
promotion remain open.

The populated caller chain is now also replayed through current production
source in fresh VC++ 1.52c/Win98 directory `C:\RPTDINT`. The native six-check
report passes while carrying a populated `New Eden` row through `WM_PAINT` →
`DrawReport` → `DrawReportItem`; compile, link, and process exit pass and the
final guest window list contains no Program Error dialog. Together with the
original Reports-menu → Planet Summary Report → `Planet Name` header-popup
observation, this closes the E3 native/original caller-integration unit.
ReportDlg therefore has 2/3 named Step-5 units closed; exact populated report
unions and live USER/GDI/menu payloads remain the E2 boundary, followed by
final promotion. Evidence is in
`analysis/compat/report-dlg-draw-integration-e3-native.{md,json}`,
`compat/vc152/report-dlg-draw-integration-e3-native/`, and
`runs/vmctl-vc152rptdint/`.

The remaining populated E2 boundary is now also replayed through current
production source in fresh VC++ 1.52c/Win98 directory `C:\RPE2`. The 43-check
matrix drives packed-view loading, creation, paint, and destruction for
planet, fleet, scan-fleet, and battle rows, and records scrollbar/DC/paint,
header/text/GDI, row lookup, and frame-menu cleanup payloads. The native
report is `REPORT_DLG_POPULATED_E2_CHECKS=43 FAILURES=0 RESULT=PASS`; compile,
link, and process exit pass, with no Program Error dialog in the final guest
window list. This closes E2. Together with E1 and E3, all three named Step-5
units are closed; final Step-6 promotion remains. Evidence is in
`analysis/compat/report-dlg-populated-e2-native.{md,json}`,
`compat/vc152/report-dlg-populated-e2-native/`, and
`runs/vmctl-vc152rptpe2/`.

With E1, E2, and E3 now closed, the ReportDlg review record is synchronized
as manually reviewed and promoted. The coverage ledger, TODO5, TODO6, and
this reconstruction note now record ReportDlg at **6/6**; no ReportDlg
Step-5 or promotion unit remains.
