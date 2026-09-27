# Report drawing recovery

`reconstructed/src/draw_report.c` is a genuine C reconstruction of DrawReport
(NE 22:0BAE–0F65). It preserves the transparent background mode, visible-field
mask and first-column rules, report-header width caching, title/frame drawing,
row clipping, cell borders/fills, and report-item dispatch. The report state,
Win16 drawing services, and report-specific callbacks are explicit typed C
boundaries; the implementation does not replay emitted bytes.

The source probe covers header and row passes, cached widths, full clipping,
and the no-row repaint region in 3 checks. A signed rectangle-boundary issue
was caught by the native run and corrected explicitly. VC1.52c under the
Win98 StarDev environment then compiled and linked the corrected source, and
the native probe passed all 3 checks with zero failures
(`runs/vmctl-vc152ew/draw-report/DRPRES.TXT`). This is formally reviewed
against the current raw body; the dependent DrawReportItem remains a separate
open procedure.
