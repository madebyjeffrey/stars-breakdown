# DrawProductionDlg reconstruction

`reconstructed/src/draw_production_dlg.c` is a source-level reconstruction of
`DrawProductionDlg` (`001b:35dc-3aff`, 1,315 bytes). It makes the source and
planet list selection, cost rows, optional production ETA, text measurement,
and help-diamond geometry explicit through typed drawing and production-data
callbacks.

The source probe covers DC acquisition/release, source and one-based
planet-list selection normalization, cost-row rendering, ETA dispatch, and
diamond placement. Host strict C89 compilation and
`DRAW_PRODUCTION_DLG_SOURCE=PASS` pass. A fresh VC++ 1.52c/Win98 checkpoint
now compiles, links, and runs the exact source with one grouped native
assertion passing. This advances the procedure to Phase 2 step 4/6; packed
production globals, exact formatting/resource tables, and full GDI integration
remain open.

The source was subsequently corrected to preserve decimal mineral costs rather
than a single digit, route mineral label colors through the recovered color
service, and use the original numeric-formatting boundary. A fresh `C:\DPD3`
VC++ 1.52c run reports
`DRAW_PRODUCTION_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`, with empty redirected
output and no Program Error dialog. Together with the original Ship &
Starbase Designer observation below, this strengthens the procedure's Phase 2
step-5 evidence; packed production globals, complete resource-table
comparison, localized strings, and full GDI integration remain open.

The original Win98 Ship & Starbase Designer has now been exercised through the
rendering boundary: hull, component, image, cost, statistic, and command
controls were displayed across the design views without a Program Error.
Evidence is preserved in `analysis/compat/production-dialog-integration.md`
and `runs/vmctl-vc152current/`. This advances `DrawProductionDlg` to Phase 2
step 5/6 for the original designer-rendering boundary; packed production
globals, exact resource formatting, full GDI integration, and promotion remain
open.

## E1 packed production globals

The packed-list adapter is now explicit. `source_items` and `planet_items`
represent the recovered `_prod` arrays, while their counts represent the
recorded item counts rather than capacity. The packed-cost callback receives
the list kind, normalized selected index, and exact 32-bit packed word. The
source and planet paths are both checked, including the planet list's
one-based UI placeholder and out-of-range count guards.

Fresh host/native evidence passes 17 checks with no native Program Error:
`analysis/compat/draw-production-dlg-packed-e1.{md,json}` and
`runs/vmctl-vc152dpd4/`. This closes E1 of the three named Step-5 units.
DrawProductionDlg remains at 5/6 with E2 exact formatting/resource-table
comparison and E3 full GDI/caller integration open; Step-6 promotion is not
claimed.

## E2 resource-table preparation

The next packet reconciles the original resource IDs and the DGROUP
`rgszMinerals` table rather than treating every label as a compressed-string
lookup. Host strict C89 validation passes 10 resource checks, including the
`Required Minerals:` heading, `kT` separators, ETA format, template-help
text, four mineral labels across both list renderings, and the expected lookup
order. The prepared native packet and exact scope are recorded in
`analysis/compat/draw-production-dlg-resource-e2.{md,json}`.

Fresh VC++ 1.52c/Win98 validation in `C:\DPDRE2` reports
`DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=10 FAILURES=0 RESULT=PASS`; compile/link
passed, redirected output was empty, and no Program Error dialog appeared.
This closes E2. Evidence is preserved in
`runs/vmctl-vc152dpdre2/`; the remaining named unit is E3 full GDI/caller
integration, followed by Step-6 promotion.

## E3 geometry and GDI-boundary validation

The source now preserves the original horizontal layout boundary: the heading
uses the list rectangle's left edge, mineral labels use the recovered
`left + 0x14` inset, the `kT` separator is placed at the measured text-width
boundary, and right-aligned values use the original two-pixel offset. The new
strict host probe also checks DC acquisition/release, both list controls,
selection traversal, ETA dispatch, and the client-rectangle diamond geometry.

`DRAW_PRODUCTION_DLG_GEOMETRY_CHECKS=9 FAILURES=0 RESULT=PASS`, with the
packed and resource probes still passing and the full regression suite at
`519` tests with `153` skipped. Fresh VC++ 1.52c/Win98 validation in
`C:\DPDRE2` reports the same result, with empty redirected output and no
Program Error dialog. Combined with the original designer observation, this
closes E3 full GDI/caller integration. Step-6 promotion is recorded in the
review ledger and derived coverage/TODO5 ledgers. Evidence is preserved in
`runs/vmctl-vc152dpdre3/`.
