# EstFuelUse reconstruction

`reconstructed/src/est_fuel_use.c` is a source-level reconstruction of
`EstFuelUse` (`000b:9fe4-a6ff`, 1,819 bytes). It models the 16 ship slots with
typed engine fuel tables (the original `rgcFuelUsed` has 12 entries, including
the warp-10 entry), hull fuel weights, cargo counts, race discount, and the
ram-scoop flag. The fixed-point fuel accumulation, travel-distance lookup,
fuel-cost rounding, and range-only conversion are expressed as named C
operations rather than packed offsets or emitted bytes.

The source probe covers order-derived warp selection, distance lookup, race
discount, scoop-flag setting, ordinary fuel estimation, direct warp-10 table
selection, invalid-warp scoop-flag clearing, aggregate cargo-weight range conversion, the zero-fuel
infinite-range sentinel, and a large
intermediate-product case. The implementation keeps the fixed-point quotient
truncating while widening the intermediate through the same floating-point
boundary used by the original large-value path. Host strict C89 compilation
and `EST_FUEL_USE_SOURCE=PASS` pass. A fresh VC++ 1.52c/Win98 checkpoint also
reports `COMPILE=PASS`, `LINK=PASS`, and
`EST_FUEL_USE_CHECKS=6 FAILURES=0 RESULT=PASS` without a runtime error dialog.
Evidence is preserved in `analysis/compat/est-fuel-use-native.md` and
`runs/vmctl-vc152efu/`.

A second native harness now binds the source to the recovered packed fleet,
ship-design, hull-definition, and engine-record boundaries. It passes two
integration checks covering raw ship counts, mineral weights, component engine
selection, the Ram Scoop flag, race discount, and the fixed-point fuel result.
The original Fleet Summary Report and Fuel & Cargo surface also shows per-fleet
fuel values without a program-error dialog. This advances the procedure to
Phase 2 step 5/6. The expanded `C:\EFUINT2` native matrix now passes 12/12
checks, adding no-cargo range sentinel, warp-10, invalid-warp, order-derived
distance, absent-engine fallback, range-only, large-intermediate, record-size,
and raw engine-table cases. The final step-5 closure uses a 29-check matrix in
fresh `C:\EFUINT3`: it replays the independent original fuel loop over all
sixteen packed design and hull slots, all twelve warp rows, all engine IDs,
both race-discount modes, range-only conversions, and route distances from 1
through 50000. Host strict C89 and VC++ 1.52c/Win98 both report
`EST_FUEL_USE_INTEGRATION_CHECKS=29 FAILURES=0 RESULT=PASS`; the final window
list has no Program Error dialog. This closes the remaining packed-table and
route-distance differential unit. The procedure is promoted to Phase 2 6/6;
this is procedure-level evidence and does not claim whole-game turn coverage.
