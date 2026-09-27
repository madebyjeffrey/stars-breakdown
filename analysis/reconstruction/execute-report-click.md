# ExecuteReportClick reconstruction

`ExecuteReportClick` is the report-grid action dispatcher.  The reconstructed
source keeps the original report-kind and column branches while representing
the packed fleet, planet, battle, and `GlobalPD` records with typed structures.
USER/GDI access and the surrounding report/scan/transfer/popup services are
callback boundaries.

## Recovered behavior

- Fleet rows select the fleet, invalidate the report, switch to a waypoint
  when the destination column is actionable, open transfer handling for cargo
  columns, and open the fleet popup for the detail column with its original
  variable/max fields.
- Enemy-fleet rows implement the previous/next row wrap, nearest-object scan
  selection, on-screen centering, and transfer-dialog beep behavior.
- Battle rows select and center a different battle square, or launch the VCR
  when the selected battle is already current.
- Planet rows select the planet, reject clicks while production is open, and
  dispatch starbase, planet, production, mine/factory, defense, mineral, and
  resource cells to their corresponding popup or service callbacks.  The
  installation popup preserves the packed mine/factory weight shifts, and the
  resource popup applies the typed research-share percentage unless the
  resource-exempt flag is set.
- Mineral-cell dispatch preserves the visible-column offset and three-way
  in-cell selection arithmetic before requesting estimated mineral values.
  The in-cell coordinate uses the original two-thirds column subdivision.

The source probe exercises fleet, enemy-fleet, battle, production, installa-
tion, mineral, and resource dispatches. A fresh VC++ 1.52c / Win98 checkpoint
passes all 21 native checks without a runtime error dialog. Exact packed-record
integration, the original GlobalPD union, USER/GDI integration, menu resources,
and the full caller integration remain open for promotion to formal review. The
original Planet Summary Report row-selection boundary was also observed in
Win98 with the selected planet/frame context visible behind the report; see
`analysis/compat/execute-report-click-integration.md`.

A packed adapter now passes 34 host checks and 34 native checks over the
recovered `_fleet`, `_planet`, `_btldata`, and `_popupdata` layouts. The
expanded matrix covers starbase and planet popup unions, the defense-part
payload, the resource-exempt bitfield, and production/transfer modal guards.
A separate native layout probe passes 16 checks. The first two native attempts
exposed an omitted linker definitions-file line; the corrected fresh-directory
packages compiled, linked, and ran without a Program Error dialog. Exact
original GlobalPD owner/part cases, live USER/GDI behavior, and complete caller
integration remain open; see
`analysis/compat/execute-report-click-packed-integration.md`.

The recovered raw fleet, planet, and battle lookup helpers are now owned by
production `execute_report_click.c`. The packed probe exercises those helpers
directly through the optional production record fields, while typed lookup
callbacks remain a fallback. A fresh native production replay in `C:\EXINT5`
passes `EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`; compile/
link passed, process exit was 0, and no Program Error dialog appeared. This
closes the native production-lookup subcheck but no Step-5 unit: exact
original `GlobalPD` owner/part cases, live USER/GDI behavior, and complete
caller integration remain open. See
`analysis/compat/execute-report-click-packed-e5-native.{md,json}` and
`runs/vmctl-vc152exint5/`.

A caller probe now connects three popup-producing report cells through the
reconstructed `Popup` window setup and `DrawPopup` paint path. Host and fresh
VC++ 1.52c/Win98 runs both pass 26 checks, including semantic payload
preservation, resource-share arithmetic, factory weight extraction, popup
sizing, message/capture setup, renderer-kind dispatch, and DC release
symmetry. The process exited normally and no Program Error dialog appeared.
This closes the native caller subcheck but no named Step-5 unit; exact
original `GlobalPD` owner/part payloads, localized content, live USER/GDI
behavior, complete report-layout integration, original comparison, and
promotion remain open. See
`analysis/compat/execute-report-click-popup-integration-e3.{md,json}` and
`runs/vmctl-vc152popintp/`.

The production boundary now also retains the complete 22-byte `GlobalPD`
overlay.  Fleet and starbase far-pointer words, the eight-byte defense
`_part` union, the resource part word, and the overlapping `rgi` long views
are copied as raw Win16 values without dereference.  A strict host and fresh
VC++ 1.52c/Win98 replay both pass 20 checks; compile/link and process exit
passed, and the final guest window list had no Program Error dialog.  This is
supporting E1 evidence, not a named-unit closure: exact original pointer/part
values, live USER/GDI behavior, full report-layout caller coverage, original
differential evidence, and promotion remain open.  See
`analysis/compat/execute-report-click-globalpd-e1-native.{md,json}` and
`runs/vmctl-vc152gpde2/`. The earlier `GPDE1` replay is retained as
superseded evidence because its starbase oracle predated the raw
`iPlanVal=1` correction.

## Packed mineral-status E6 native replay

The packed `_planet` bridge now preserves the original `overlap_4` status
word at offset `0x04`. The mineral popup uses bit 10 of that word for the
raw `GlobalPD.iPlanVal` field at popup offset `0x06`. The strict-C89 and fresh
VC++ 1.52c/Win98 `C:\EXINT6` matrices both pass 34/34; compile/link and
process exit pass, and no Program Error dialog appears. This is supporting
packed/native evidence, not a named-unit closure. See
`analysis/compat/execute-report-click-packed-e6-native.{md,json}` and
`runs/vmctl-vc152exint6/`.

## Resource GlobalPD union correction

The original report resource-cell branch preserves the selected planet ID
at union offset `+2`; the `PtDisplayResourceInfo` consumer reads that ID.
The part/`iPlanetVar` alias at `+4` is the just-computed resource count,
not an independent caller word. The production source now materializes
those values and the research-adjusted word at `+6`. Strict-C89 and fresh
VC++ 1.52c/Win98 `C:\GPDE3` pass 20/20; see
`analysis/compat/execute-report-click-resource-union-e1.{md,json}` and
`runs/vmctl-gpde3/`. Other live pointer/part cases and the complete E1
boundary remain open.

## Packed fleet and starbase pointer tables

The report-click source now optionally resolves fleet popup pointers from
the original row-ID/pointer-table chain and starbase popup pointers from
the per-player ship-definition far pointer plus the packed planet's
design nibble times 0x93, preserving 16-bit offset wrap. A fresh strict-C89
and VC++ 1.52c/Win98 `C:\GPDP4` matrix passes 30/30 including invalid
table bounds; see
`analysis/compat/execute-report-click-pointer-tables-e1.{md,json}` and
`runs/vmctl-gpdp4/`. These synthetic table vectors do not replace live
original pointer capture, so ER-E1 remains open.
