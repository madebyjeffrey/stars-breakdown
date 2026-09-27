# DrawNewGame3 reconstruction

`reconstructed/src/draw_new_game3.c` is a source-level reconstruction of
`DrawNewGame3` (`0010:9d5e-a20f`, 1,201 bytes). It exposes the compact signed
18-byte column table, VC-value lookup, nine-row label/value ordering, read-only button
style, paired vertically offset button rectangles, and reference rectangles
through typed C callbacks.

The source probe covers DC lifecycle, row labels, value rendering including
the raw percent suffix for negative-width columns, paired button creation,
row-7/8 geometry carry-over, and reference-count output. Host strict C89
compilation and the focused 7-check packed-table probe pass. The separate
rectangle packet covers all 18 output rectangles and 36 paired references
with 95/95 host and native checks. The localized packet covers the formatter,
resource suffix, all 27 label IDs, and the complete reference boundary with
34/34 host and native checks. The final integration packet covers the complete
GDI service boundary, read-only styles, caller-owned HDC, and selective draw
mode with 12/12 host and native checks. Fresh VC1.52c/Win98 checkpoints
compile, link, and run without a runtime error dialog. The procedure is ready
for Step-6 ledger promotion.
The
original wizard Step 3 was also observed in Win98 with the nine victory
criteria rows, paired controls, winner threshold, and minimum-year control;
see `analysis/compat/draw-new-game-integration.md`.
