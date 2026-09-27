# PtDisplayPlanetPopInfo

`PtDisplayPlanetPopInfo` is represented as typed text-stream logic in
`reconstructed/src/pt_display_planet_pop_info.c`. It preserves owned,
unowned, and foreign-planet headers; habitability/desirability branches;
maximum-population output; growth warnings and population-change formatting;
foreign-environment disclosure; font transitions; wrapping; and the returned
layout point.

Localization, packed planet lookup, fixed-point formatting, stream/wrap
geometry, and GDI output remain callback-owned. The maximum-population and
desirability callbacks receive the selected player, matching the raw helper
calls. The host probe covers owned, unowned, foreign, and
negative-desirability paths. A fresh VC++ 1.52c/Win98 harness now compiles
and links the exact source and probe and reports
`PT_DISPLAY_PLANET_POP_INFO_CHECKS=25 FAILURES=0 RESULT=PASS` without a
Program Error dialog. The original Win98 small-layout selection-summary pane
was then observed for `Oop Be Gone`, showing `Value: 100%`,
`Population: 25,000`, and current-report status without a program-error
dialog. See `analysis/compat/pt-display-planet-pop-info-integration.md` and
`runs/vmctl-vc152current/layout-small.bmp`. This is the step-5 integration
checkpoint; packed lookup, localization, fixed-point formatting, detailed GDI
output, and promotion remain open.

The packed-record integration checkpoint is now complete. A strict host
adapter and a fresh VC++ 1.52c / Win98 `C:\PPINT` run each report
`PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=19 FAILURES=0 RESULT=PASS` over the
recovered 56-byte `_planet` identity, owner, mineral-rate, and
population-change fields. The owned, unowned, foreign, and negative-
desirability branches pass through the existing lookup/service boundary;
compile and link passed, the process exited 0, and no Program Error dialog
appeared. This strengthens the procedure's Phase 2 step-5 evidence but does
not promote it: complete packed planet/player lookup, localization,
fixed-point formatting, detailed stream/wrap GDI behavior, original
differential coverage, and final review remain open. See
`analysis/compat/pt-display-planet-pop-info-packed-integration.{md,json}` and
`runs/vmctl-vc152ppint/`.

The localization/fixed-point boundary is now explicit as well. The optional
`prepare_segment` callback receives every selected stream/wrap segment before
font and output dispatch, preserving the existing callback path for older
callers. The strict host source probe now passes 27 checks and
verifies all emitted segments plus the negative-desirability pair `2,5` for
`-25` tenths. The VC++ 1.52c packet at
`compat/vc152/pt-display-planet-pop-info-format-e2/` was replayed in fresh
`C:\PPFE2`; compile and link passed, and the native report is
`PT_DISPLAY_PLANET_POP_INFO_FORMAT_CHECKS=27 FAILURES=0 RESULT=PASS` with no
Program Error dialog. This closes the E2 localized/fixed-point formatting
unit. Complete packed lookup, detailed GDI/original comparison, and promotion
remain open. Evidence is in `runs/vmctl-vc152ppfe2/`.

A combined host branch harness now exercises this procedure alongside
`PtDisplayPlanetStateInfo`. The strict C89 probe passes
`PLANET_INFO_BRANCH_CHECKS=9 FAILURES=0 RESULT=PASS`, covering the owned,
unowned, foreign, zero-mineral, negative-desirability, maximum-population,
growth-warning, and shared layout/stream boundaries. This is host-only and
closes no Step-5 unit; packed lookup, localized fixed-point formatting,
detailed GDI behavior, native replay, and promotion remain open. Evidence is
in `analysis/compat/planet-info-branch-e3.{md,json}`.

## Production packed-planet bridge

The readable population-information path now owns the recovered 56-byte
`_planet` decode when `StarsPlanetPopInfoInput.packed_planet_record` is
provided. It takes the identity and owner from offsets 0 and 2, the signed
population-change byte from offset 20, and the mineral-rate word from offset
28. Selected-player, habitability, maximum-population, desirability, and
foreign-environment values remain explicit service sidecars. A short record
falls back to the typed `lookup_planet` callback.

The strict-C89 production-path probe passes
`PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS` over raw
identity precedence, owned/unowned/foreign/negative-desirability branches,
the signed population-change field, and short-record fallback. A fresh
VC++ 1.52c/Win98 replay of the same production bridge in `C:\PPPE4` records
`COMPILE=PASS`, `LINK=PASS`, and
`PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS`, with no
Program Error dialog. The native run confirmed that a valid 56-byte packed
record takes precedence over `lookup_planet`, while a 55-byte view uses lookup
fallback. This closes E1 complete packed planet/player lookup. E2 formatting
is closed separately; detailed stream/wrap GDI/original comparison and
promotion remain open. Evidence is in
`analysis/compat/pt-display-planet-pop-info-packed-production-e4.{md,json}`
and `runs/vmctl-vc152pppe4/`.

## Detailed stream/wrap GDI and original E3

The shared `PLANET_INFO_BRANCH_CHECKS=9` harness now passes both on the host
and in fresh VC++ 1.52c / Win98 directory `C:\PBR3`. It exercises owned,
unowned, and foreign headers, negative desirability, maximum-population and
growth-warning paths, foreign-environment disclosure, wrapping, and returned
layout through the complete stream/wrap seam. Compile and link pass, the
process exits 0, and no Program Error dialog appears. The original
`runs/vmctl-vc152current/layout-small.bmp` capture shows the corresponding
planet value, population, and current-report status in the Small Screen
selection-summary pane. This closes and promotes the population-information
procedure at `6/6`. Evidence is in
`analysis/compat/planet-info-branch-e3-native.{md,json}` and
`runs/vmctl-vc152pbr3/`.
