# ReportDlg native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152report/`.

- `REPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `REPCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `REP.C` and `PROBE.C` source; it retained two C4341 warnings for
  the original unsigned Win16 style constants
- `REPLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `REPR.TXT`: `REPORT_DLG_CHECKS=26 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `REP.C` is byte-identical to
`reconstructed/src/report_dlg.c`, and `PROBE.C` is byte-identical to
`tests/report_dlg_source_probe.c`. The probe covers initialization, cached
column widths, child scrollbars, resize and paint paths, horizontal and
vertical scrolling, header and row mouse dispatch, cleanup, and default
procedure fallthrough.

This advances `ReportDlg` from Phase 2 step 3/6 to step 4/6. Exact packed
report records, original USER/GDI handles, menu resources, and full caller
integration remain open.
