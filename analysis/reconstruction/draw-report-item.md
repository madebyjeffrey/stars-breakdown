# DrawReportItem recovery

`reconstructed/src/draw_report_item.c` is a typed C reconstruction of
`DrawReportItem` (NE `0022:3398–4EBE`, 6,950 bytes). The four original
computed-jump families are represented by explicit planet, fleet, scan-fleet,
and battle renderers. The source preserves the column-specific formatting,
selection colors, planet installation bars, mineral/resource dispatch, fuel
and primary-design indicators, fleet category totals, battle unit/kills
columns, and the original Win16 drawing-service order.

Packed far records and USER/GDI entry points are replaced by typed row records
and explicit callbacks. This is semantic source, not a byte-emitting wrapper
and not a claim that a modern host compiler emits the original instructions.

The host probe exercises every report family and all exposed column ranges,
including selection, installation bars, production dispatch, primary-design
rendering, scan-fleet totals, and battle unit/kills paths. Strict host C89
compilation and the native VC++ 1.52c StarDev build both pass; the native
probe reports `CHECKS=4 FAILURES=0 RESULT=PASS` in
`runs/vmctl-vc152ex/report-item/RPTITEMR.TXT`.
