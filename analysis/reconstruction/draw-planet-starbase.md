# DrawPlanetStarbase reconstruction

`reconstructed/src/draw_planet_starbase.c` is a source-level reconstruction of
`DrawPlanetStarbase` (`000a:22cc-2afa`, 2,094 bytes). It uses typed tile and
starbase records and explicit drawing, localization, shield, warp, and gauge
callbacks instead of embedding assembly or emitted bytes.

The source preserves the cached-reference reset (including the title/action
sentinels), common tile frame and fill,
empty-starbase sentinel rectangles, four detail rows, cargo-capacity special
values, defense and shield values, mass-warp highlighting, separator, warp
button, optional mass-warp gauge, and the original no-warp reference cleanup.
The focused probe covers populated, no-warp, and empty tile paths.

Host validation: strict C89 compilation and
`DRAW_PLANET_STARBASE_SOURCE=PASS`. A fresh VC++ 1.52c/Win98 checkpoint also
reports `COMPILE=PASS`, `LINK=PASS`, and
`DRAW_PLANET_STARBASE_CHECKS=14 FAILURES=0 RESULT=PASS` without a runtime error
dialog. Evidence is preserved in
`analysis/compat/draw-planet-starbase-native.md` and
`runs/vmctl-vc152dpsn/`. Packed planet/starbase records, localized resource
tables, and full GDI integration remain callback-owned or otherwise open, so
this remains partial semantic source.

The original Win98 game frame has now been exercised through the starbase
boundary: the `Oop Be Gone` panel displayed dock capacity, armor, and shields
without a Program Error. Evidence is preserved in
`analysis/compat/planet-frame-integration.md` and
`runs/vmctl-vc152current/current-continue.bmp`. A packed-record checkpoint
also passed 17 host and native checks over the 56-byte `_planet` identity,
starbase flag, packed starbase word, populated detail/gauge paths, and empty
sentinel cleanup. Evidence is preserved in
`analysis/compat/draw-planet-starbase-packed-integration.{md,json}` and
`runs/vmctl-vc152dsbint/`. This keeps `DrawPlanetStarbase` at Phase 2 step
5/6; localized resources, exact shield/warp and GDI integration, original
differential coverage, and promotion remain open.

The localized-resource boundary is now closed as E2. The original resource
IDs and strings were checked for the six displayed row labels—`0x344` Dock
Capacity, `0x345` Armor, `0x346` Shields, `0x347` Damage, `0x349` Mass Driver,
and `0x475` Destination—and for the special `0x560` `none` and `0x2ed`
`Unlimited` values. The host and fresh VC++ 1.52c/Win98 probe both report
`DRAW_PLANET_STARBASE_RESOURCE_CHECKS=7 FAILURES=0 RESULT=PASS`; compile/link
passed and no Program Error dialog appeared. Evidence is preserved in
`analysis/compat/draw-planet-starbase-packed-integration.{md,json}` and
`runs/vmctl-vc152sbr/`. The remaining Step-5 units are E1 packed
planet/starbase/design-service completion and E3 exact shield/warp services,
full GDI/caller behavior, and original differential coverage.

The packed design boundary is now closed as E1. `stars_decode_planet_starbase`
follows the recovered raw offsets and strides for the 56-byte planet record,
147-byte starbase design records, and 143-byte hull records. It extracts the
low-nibble starbase design slot, resolves the full hull ID, reads cargo and
defense from the hull record, preserves the packed mass-warp word, and decodes
the ten-bit destination field. The strict host and fresh native `C:\SBDS`
probe both report `DRAW_PLANET_STARBASE_DESIGN_CHECKS=10 FAILURES=0 RESULT=PASS`;
compile/link passed and no Program Error dialog appeared. With E2 resources
also closed, the remaining Step-5 boundary was E3: exact shield/warp services,
full GDI/caller behavior, and original differential coverage. A fresh
host/native service probe now closes that boundary with seven checks covering
title/class data, shield and warp callback arguments, destination lookup/text,
button enablement, and gauge/reference geometry. The native `C:\DPSBN` report
is `DRAW_PLANET_STARBASE_SERVICE_CHECKS=7 FAILURES=0 RESULT=PASS`, with clean
compile/link and no Program Error dialog. `DrawPlanetStarbase` is now `0 + P`;
only final review-ledger promotion remains.
