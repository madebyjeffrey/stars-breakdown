# PtDisplayPlanetStateInfo

`PtDisplayPlanetStateInfo` is represented as typed row-building logic in
`reconstructed/src/pt_display_planet_state_info.c`.  It preserves the
unknown/known player-limit branches, environment rows, planet-limit rows,
desirability-improvement warning, signed difference selection, wrapping order,
wrapped-width tracking, and returned layout point.

Packed planet storage, localization, fixed-point environment formatting,
temporary planet mutation/desirability lookup, and GDI output remain callback-
owned.  The host probe covers known and unknown records, the planet-limit
section, improvement warning, and signed difference paths. A fresh VC++ 1.52c
/ Win98 checkpoint passes all 34 native checks without a runtime error dialog.
The original Win98 small-layout selection-summary pane was then observed for
`Oop Be Gone`, showing the gravity, temperature, radiation, and mineral state
rows without a program-error dialog. See
`analysis/compat/pt-display-planet-state-info-integration.md` and
`runs/vmctl-vc152current/layout-small.bmp`. This is the step-5 integration
checkpoint; packed lookup, localization, fixed-point formatting, detailed GDI
output, and promotion remain open.

The packed-record integration checkpoint is now complete. A strict host
adapter and a fresh VC++ 1.52c / Win98 `C:\STINT` run each report
`PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=23 FAILURES=0 RESULT=PASS` over
the recovered 56-byte `_planet` and 22-byte `_popupdata` views. The adapter
checks the combined identity/limit bridge, known and unknown rows, environment
and desirability service calls, improvement warning, wrapping, and returned
geometry. Compile and link passed, the process exited 0, and no Program Error
dialog appeared. This strengthens the procedure's Phase 2 step-5 evidence but
does not promote it: complete packed player/planet lookup, localization,
fixed-point formatting, detailed stream/wrap GDI behavior, original
differential coverage, and final review remain open. See
`analysis/compat/pt-display-planet-state-info-packed-integration.{md,json}`
and `runs/vmctl-vc152stint/`.

The localization/fixed-point boundary is now explicit as well. The optional
`prepare_segment` callback receives every selected stream/wrap segment before
font and output dispatch; the strict host source probe passes
`PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS`,
including the kind-2 environment value after the environment service call.
The existing packed 23-check probe still passes. The VC++ 1.52c packet at
`compat/vc152/pt-display-planet-state-info-format-e2/` was replayed in fresh
`C:\STFE2`; compile and link passed, and the native report is
`PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS` with no
Program Error dialog. This closes the E2 localized/fixed-point formatting
unit. Complete packed lookup, detailed GDI/original comparison, and promotion
remain open. Evidence is in `runs/vmctl-vc152stfe2/`.

A combined host branch harness now exercises this procedure alongside
`PtDisplayPlanetPopInfo`. The strict C89 probe passes
`PLANET_INFO_BRANCH_CHECKS=9 FAILURES=0 RESULT=PASS`, covering unknown and
known player-limit rows, planet-limit rows, desirability improvement,
environment-service calls, signed differences, wrapping, and returned
layout. This is host-only and closes no Step-5 unit; packed lookup,
localization, fixed-point formatting, detailed GDI behavior, native replay,
and promotion remain open. Evidence is in
`analysis/compat/planet-info-branch-e3.{md,json}`.

## Production packed-record bridge

The readable state-information path now owns the bounded decode of the two
raw views used by the original routine: the 56-byte `_planet` record and the
22-byte `_popupdata` value block. `StarsPlanetStateInfoInput` takes the
identity and environment bytes from `_planet`, the planet/player limits and
values from `_popupdata`, and leaves environment conversion and desirability
as derived callbacks. Incomplete records leave the typed state data intact.

The strict-C89 production-path probe passes
`PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS`,
covering raw-field precedence, known and unknown limit branches, environment
and desirability callbacks, wrapping/layout, and short-record fallback. A
fresh VC++ 1.52c/Win98 replay of the same bridge in `C:\STPE4` records
`COMPILE=PASS`, `LINK=PASS`, and
`PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS`, with
no Program Error dialog. This closes E1 complete packed player/planet lookup.
E2 formatting is closed separately; detailed stream/wrap GDI/original
comparison and promotion remain open. Evidence is in
`analysis/compat/pt-display-planet-state-info-packed-production-e4.{md,json}`
and `runs/vmctl-vc152stpe4/`.

## Detailed stream/wrap GDI and original E3

The shared `PLANET_INFO_BRANCH_CHECKS=9` harness now passes both on the host
and in fresh VC++ 1.52c / Win98 directory `C:\PBR3`. It exercises the state
rows, known and unknown player-limit branches, planet limits, improvement
warning, signed difference, environment/desirability callbacks, wrapping, and
returned layout through the complete stream/wrap seam. Compile and link pass,
the process exits 0, and no Program Error dialog appears. The original
`runs/vmctl-vc152current/layout-small.bmp` capture shows the corresponding
gravity, temperature, radiation, and mineral rows in the Small Screen
selection-summary pane. This closes and promotes the state-information
procedure at `6/6`. Evidence is in
`analysis/compat/planet-info-branch-e3-native.{md,json}` and
`runs/vmctl-vc152pbr3/`.
