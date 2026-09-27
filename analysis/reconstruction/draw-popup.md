# DrawPopup

`DrawPopup` is represented as typed dispatcher logic in
`reconstructed/src/draw_popup.c`.  The reconstruction preserves the initial
GDI state setup and final restoration, popup-kind dispatch for all recorded
cases, client-rectangle-derived arguments, the adjusted slot-dialog
rectangle for popup kinds 11 and 14 including the Arial-8 line-height offset,
and their bold-font/opaque-background
state transition before hull drawing.

The packed `GlobalPD` records, localized strings, hull filtering, formatting,
and detailed GDI drawing remain behind typed per-kind callbacks. The host probe
exercises every recorded popup kind, the default path, state restoration, and
both slot-dialog variants. A fresh VC++ 1.52c / Win98 checkpoint passes all 51
native checks without a runtime error dialog. The original executable also
shows the mineral, population, report, and popup-restoration surfaces in the
preserved screenshots, establishing the Win16 consumer boundary. This
advances `DrawPopup` to Phase 2 step 5/6; packed records, localization, hull
filtering, formatting, and detailed GDI drawing remain open before promotion.

The packed-record integration checkpoint is now also complete. A strict host
adapter and a fresh VC++ 1.52c / Win98 `C:\\DRINT` run each report
`DRAW_POPUP_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS` over the recovered
`_popupdata`, `_fleet`, and `_planet` record views. The selector-derived fleet,
planet-state, slot-hull, and default dispatch paths, slot geometry, starbase
mode, and GDI-state restoration all pass. Compile and link passed, the process
exited 0, and no Program Error dialog appeared. This strengthens the
procedure's Phase 2 step-5 evidence but does not promote it: complete
`GlobalPD` union coverage, localization, hull filtering, formatting, detailed
GDI text rendering, and final caller review remain open. See
`analysis/compat/draw-popup-packed-integration.{md,json}` and
`runs/vmctl-vc152drint/`.

## Detailed GDI state E3 validation

The state-order probe now checks the ordinary and kind-14 slot-hull paths: the
initial background/text/mode/font setup, the extra bold/opaque slot transition,
the adjusted slot rectangle, callback dispatch, and reverse restoration of all
four GDI state values. Host validation passes
`DRAW_POPUP_STATE_CHECKS=7 FAILURES=0 RESULT=PASS`.

Fresh VC++ 1.52c/Win98 validation in `C:\DRINT` reports
`DRAW_POPUP_STATE_CHECKS=7 FAILURES=0 RESULT=PASS`; no Program Error dialog
appeared. This is supporting native evidence for E3, but it does not close the
named E3 unit: localized detailed rendering and caller differential coverage,
then promotion, remain open. Evidence is preserved in
`runs/vmctl-vc152drawpopupstatee3/`.

The dispatcher-kind boundary is now complete: a fresh `C:\DRINT2` matrix
passes 35 host/native checks over all remaining popup callbacks, including the
kind-14 slot-hull alias and state restoration after every dispatch. Compile
and link passed with no Program Error dialog. Packed `GlobalPD` payloads beyond
the dispatcher, localization, hull filtering, formatting, detailed GDI text,
caller differential coverage, and promotion remain open.

## Production packed-selector bridge

The readable dispatcher now owns the 22-byte `_popupdata` selector boundary.
When `StarsDrawPopupInput.packed_popupdata` is present with a 22-byte or
larger view, `DrawPopup` decodes the little-endian selector at offset 0 and
uses it in preference to the legacy typed `popup_kind`; short views retain the
typed fallback. The strict-C89 production-path probe passes 37 checks over raw
selector precedence, every dispatcher kind, the kind-14 slot-hull alias,
state restoration, and short-record fallback. A corrected fresh VC++ 1.52c/
Win98 replay in `C:\DPE5R` reports the same 37 checks with compile/link pass,
process exit 0, and no Program Error dialog. The first attempt is preserved as
a superseded compile-only failure because its packet omitted lowercase
`core.h`. This closes the native replay subcheck, not a named Step-5 unit.
Complete `GlobalPD` payloads, localization, hull filtering/formatting,
detailed GDI rendering, caller differential coverage, and promotion remain
open. Evidence is in `analysis/compat/draw-popup-packed-e5-native.{md,json}`
and `runs/vmctl-vc152dpe5r/`.

## Complete packed `GlobalPD` payload E1

The production callback payload now owns the complete recovered 22-byte
`_popupdata` union boundary. `StarsDrawPopupPayload` copies the raw record and
decodes every recorded overlay: all five `rgi` long values, the planet/player
and production-counter views, near/far pointer words, damage/display flags,
the eight-byte `_part` view, and token-summary fields. Raw pointer words are
forwarded without dereference so callers can resolve them in the appropriate
Win16 address space.

The strict-C89 host and fresh `C:\DPE6` VC++ 1.52c/Win98 replay both pass
`DRAW_POPUP_PACKED_CHECKS=38 FAILURES=0 RESULT=PASS`; compile/link and process
exit pass, and the final guest window inventory contains no Program Error.
This closes E1, complete packed `GlobalPD` union payload coverage. E2
localization/hull filtering/formatting, E3 detailed GDI and caller
differential, E4 original completion, and Step-6 promotion remain open. See
`analysis/compat/draw-popup-globalpd-e1.{md,json}` and
`runs/vmctl-vc152drawpopupe6/`.

## Localization, hull filtering, and formatting E2 host boundary

The readable source now emits typed semantic text segments for popup kinds
1–4. The boundary preserves the recovered resource IDs and numeric-format
choices, exposes planet-name and fleet-name seams, filters hull rows through
the optional visibility callback, extracts the hull-definition technology
bits for decoration, and emits count/damage and coordinate values through the
same typed formatter seam. Optional raw 124-byte fleet and 147-byte
hull-definition views make the recovered packed offsets explicit while the
22-byte popup union's far-pointer words remain opaque.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\DPE7` both
pass `DRAW_POPUP_CONTENT_E2_CHECKS=21 FAILURES=0 RESULT=PASS`. Compile/link
and process exit pass, and the final guest window inventory contains no
Program Error. The first compile-only link failure is retained separately;
it was caused by a host-only probe `printf` omitted under `TARGET_NATIVE` in
the corrected packet. This closes E2 localization, hull filtering, and
formatting. Detailed GDI/caller differential behavior, original completion,
and Step-6 promotion remain open. See
`analysis/compat/draw-popup-content-e2-native.{md,json}` and
`runs/vmctl-vc152drawpopupcontente2-r2/`.

## Detailed text geometry, style, and caller E3 closure

The typed text boundary now exposes the recovered x/y position, left/right/
center anchor, normal/bold font choice, and opaque background mode. Optional
measurement callbacks provide the title/label-width-dependent offsets used by
the original case-1 and case-4 layouts. Hull rows, right-aligned values,
separator placement, row-height arithmetic, and the short-client path are
covered by 23 strict-C89 checks.

The final VC++ 1.52c/Win98 replay in `C:\DPE9` reports
`DRAW_POPUP_GEOMETRY_E3_CHECKS=25 FAILURES=0 RESULT=PASS`; compile/link and
process exit passed, and no Program Error dialog appeared. Together with the
native 7-check state-order probe, the native 26-check
`ExecuteReportClick -> Popup -> DrawPopup` caller replay, and the original
consumer observations, this closes the named E3 detailed GDI/caller unit.
E4 original completion and Step-6 promotion remain open. Evidence is
preserved in `analysis/compat/draw-popup-detailed-e3.{md,json}`,
`analysis/compat/draw-popup-geometry-e3-{host,native}.{md,json}`, and
`runs/vmctl-vc152drawpopupgeometrye3r2/`.

## Original consumer completion E4

The final original-consumer boundary is now closed. The preserved Win98
captures cover the fleet-summary, starbase/base, planet mineral/status,
population/report, Planet Summary Report, and restored-frame surfaces without
a Program Error dialog. Together with the fresh packed-payload, localized
content, detailed GDI/geometry, state-order, and production caller replays,
this completes the named E4 original-behavior unit. The procedure is promoted
at Phase 2 6/6; evidence is recorded in
`analysis/compat/draw-popup-original-e4.{md,json}` and the synchronized
review/TODO6 ledgers. This records original consumer behavior at the UI
boundary and does not claim pixel identity for the callback-based renderer.
