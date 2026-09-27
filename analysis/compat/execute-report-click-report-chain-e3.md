# ExecuteReportClick ER-E3 — production report hit-test chain (host)

This is a supporting ER-E3 milestone, not a named closure. The strict-C89
`tests/execute_report_click_report_chain_probe.c` routes four original Planet
Summary Report client coordinates through production `ReportDlg` mouse
hit-testing into production `ExecuteReportClick` and its popup boundary:

| Client click | Selected field | Popup |
|---|---:|---:|
| `(265,26)` | 2 | planet kind 7 |
| `(406,26)` | 12 | resources kind 12; gross 35, adjusted 30 |
| `(160,26)` | 6 | mine kind 8; value 10, maximum 1200, operable 25 |
| `(205,26)` | 7 | factory kind 8; value 10 |

The probe also covers resource-exempt adjustment (gross 35 retained), modal
production-window suppression/beep with DC release, header-click dispatch,
out-of-range hit rejection, selection/invalidation, and matching DC
acquire/release counts. The retained original
[`report-columns-after-scroll.bmp`](../../runs/vmctl-gp-ui-r2/report-columns-after-scroll.bmp)
shows Mine immediately beside Fact in one Planet Summary Report view. The
probe also supplies a coherent eight-visible-field width array inferred from
those pixel spans and confirms the original `(160,26)` and `(205,26)` clicks
dispatch to columns 6 and 7 in that *same* layout. The widths are rounded to
the report's two-pixel grid; this is not an exact in-memory capture.

The original [`CAPTURE-STARBASE-DIFF.TXT`](../../runs/vmctl-gp-live-preflight/CAPTURE-STARBASE-DIFF.TXT)
and [`CAPTURE-DEFENSE-LIVE.TXT`](../../runs/vmctl-gp-live-preflight/CAPTURE-DEFENSE-LIVE.TXT)
also provide complete 22-byte `GlobalPD` results. Two additional production
report-to-click cases use their `(190,26)` and `(171,26)` client coordinates,
then compare the entire persistent overlay with those bytes: Starbase kind 11
with far pointer `19E7:0012`, and Defense kind 9 with the eight-byte part
`00 80 09 13 9E 18 37 11`. Each case also checks dispatch and report-DC
acquire/release. Their individual report field masks and widths are synthetic;
they do not establish the original scroll state at those clicks.

The strict host executable passes
`EXECUTE_REPORT_CLICK_REPORT_CHAIN_CHECKS=27 FAILURES=0 RESULT=PASS`;
`tests/test_execute_report_chain.py` runs it in the host suite.

The planet and resource cases still give the report one visible field at a
time with a width covering each observed click coordinate. These cases prove
the production hit-test/dispatcher seam, original Starbase/Defense payloads,
and one observed simultaneous mine/factory layout, but do **not** prove the
complete original width table and scroll transitions, all report variants,
or the original outcome for resource-exempt/modal branches. ER-E3 remains
open pending those cases and fresh native/original integration.
