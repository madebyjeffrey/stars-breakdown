# DrawMineSurvey reconstruction

`reconstructed/src/draw_mine_survey.c` is a bounded genuine C reconstruction
of `DrawMineSurvey` (`0006:065a-37ab`). It preserves the selected-object mode
normalization, compatible-DC and drawing-state lifecycle, fleet/planet/thing
panel dispatch, typed 16-slot fleet ship-total calculation and status summary
handoff, typed planet lookup success/rejection, unknown-planet fallback,
no-selection suppression, and restoration of the prior background mode and
colors through typed callbacks.  The planet path now decodes a typed survey
summary (owner visibility, detail-row availability, survey age, environment,
concentration, and mineral values), while the thing path decodes the packed
thing kind/owner/id and wormhole destination/range fields into a typed detail
summary before invoking its renderer.  For wormholes, the destination-known
state follows the original nonzero encoded range nibble rather than an
unrelated high-bit sentinel.

The host source probe covers the normalized fleet path, successful and rejected
planet lookup, planet fallback, thing dispatch and packed-field decoding,
no-selection, background-mode/color restoration, and DC cleanup.

Native validation is complete at the focused procedure boundary. A fresh run
in `StarDev` used the exact current `reconstructed/src/draw_mine_survey.c`
source in guest directory `C:\DMS`; VC++ 1.52c reported `COMPILE=PASS` and
`LINK=PASS`, and the Win98 report was
`DRAW_MINE_SURVEY_CHECKS=5 FAILURES=0 RESULT=PASS`. Redirected console output
was empty and the native window list showed no program-error dialog.

The original Win98 selection-summary child was also observed in the small
window layout, showing the `Oop Be Gone` summary, environment bars, and
mineral rows; it redrew for `Armed Probe #1` after a selection-arrow action.
See `analysis/compat/draw-mine-survey-integration.md` and the preserved
screenshots in `runs/vmctl-vc152current/`. This is the step-5 integration
checkpoint. Detailed GDI layout, localized formatting, packed
record/resource integration, fleet/planet derived arithmetic, and full
mine-survey promotion remain open.

The native source harness was expanded to nine deterministic checks in a fresh
`C:\DMS2` directory. It adds shallow and unknown-owner planet summaries,
non-wormhole thing decoding, invalid thing-selection suppression, and the
null-operations guard. The repaired package includes the missing current
`DMS.C` source and a directory-first wrapper; VC1.52c compilation and linking
passed, the Win98 report was
`DRAW_MINE_SURVEY_CHECKS=9 FAILURES=0 RESULT=PASS`, and the post-run window
list contained no Program Error dialog. This strengthens, but does not close,
Phase 2 step 5/6; detailed GDI layout, packed records/resources,
localization, and complete mine-survey integration remain open.

The packed-record E2 boundary is now closed. `DrawMineSurvey` accepts the
recovered 124-byte `_fleet`, 56-byte `_planet`, and 18-byte `_thing` records
directly and decodes the fleet ship-count array, planet owner/environment/
concentration/mineral fields, and thing identity/point/wormhole range fields
before invoking the typed render callbacks. Strict host C89 and a fresh
VC++ 1.52c/Win98 `C:\DMSE2` run pass the raw-record matrix; the native report
is `DRAW_MINE_SURVEY_CHECKS=12 FAILURES=0 RESULT=PASS`, with compile/link and
process exit 0 and no Program Error dialog. DrawMineSurvey is now **1/3** at
Step 5. E1 detailed GDI layout and E3 localization/complete caller
integration remain. Evidence is recorded in
`analysis/compat/draw-mine-survey-packed-e2.{md,json}` and
`runs/vmctl-vc152dmse2/`.

The E3 localized/caller boundary is now closed. A fresh `C:\DMSIE3` packet
wires the localized `MineWndProc` paint path to the real typed
`stars_draw_mine_survey` implementation and checks the adjusted survey
rectangle, planet lookup/details, state restoration, and paint ordering.
Strict host C89 and VC++ 1.52c/Win98 each report
`DRAW_MINE_SURVEY_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`; compile,
link, process exit, and no Program Error dialog pass. DrawMineSurvey is now
**2/3** at Step 5. E1 detailed GDI layout and final promotion remain. Evidence
is recorded in `analysis/compat/draw-mine-survey-integration-e3.{md,json}` and
`runs/vmctl-vc152dmsint3/`.

The E1 detailed GDI boundary is now closed. The readable source emits the
recovered outer-panel PATBLT layout through typed pattern records, preserving
the 13-command fleet frame, 13-command ordinary-thing frame, and 7-command
compact-thing frame with exact offsets, dimensions, raster operations, and
brush roles. Strict host C89 and VC++ 1.52c/Win98 each report
`DRAW_MINE_SURVEY_LAYOUT_CHECKS=13 FAILURES=0 RESULT=PASS`; compile, link,
process exit, and no Program Error dialog pass. E1, E2, and E3 are closed and
DrawMineSurvey is promoted at **6/6**. Evidence is recorded in
`analysis/compat/draw-mine-survey-layout-e1.{md,json}` and
`runs/vmctl-vc152dmslay/`.
