# FillBuildDD original Win98 integration checkpoint

This is the step-5 original-executable boundary for `FillBuildDD`. In the
original `StarDev` Win98 Ship & Starbase Designer, the Existing Designs view
showed a populated design selector (`Armed Probe`), the selected hull bitmap,
component tiles with counts, and the design summary with mineral/resource,
mass, fuel, armor, shield, rating, initiative, and scanner values. The
Available Hull Types view also showed a populated hull selector
(`Small Freighter`), capacity tiles, cost, and hull statistics.

These are the live build-dialog dropdown/list and resize/population surfaces
owned by `FillBuildDD`; no Program Error dialog appeared.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Dialog: `Ship & Starbase Designer`.
- Original screenshots: `runs/vmctl-vc152current/ship-design.bmp` and
  `runs/vmctl-vc152current/ship-design-hulls.bmp`.
- Recovered boundary: `analysis/decompiler/export/raw/10c8_5e80_FillBuildDD.c`.
- Native checkpoint: `runs/vmctl-vc152fbd/` and
  `analysis/compat/fill-build-dd-native.md`.

The E1 packed-design-record packet and E2 localized-label/resource packet were
closed in the preceding checkpoints. The E2 report was
`FILL_BUILD_DD_LABELS_E2_CHECKS=58 FAILURES=0 RESULT=PASS` in both strict host
C89 and fresh VC1.52c/Win98 runs; exact normal/starbase resource-ID order,
both label modes, and the surrounding control-message sequence are recorded
in `analysis/compat/fill-build-dd-labels-e2.md` and
`runs/vmctl-vc152fbdl/`. The E3 packet below subsequently closes the remaining
queue-state and complete build-dialog population/control integration boundary.

The E3 complete-population/control packet is now closed and `FillBuildDD` is
promoted at `6/6`. The host and corrected fresh
VC1.52c/Win98 run report
`FILL_BUILD_DD_E3_CHECKS=14 FAILURES=0 RESULT=PASS`; the matrix covers empty,
hidden, queued, built, special-part, enemy-design, starbase-limit, selector,
edit/delete, count-clamp, resize, and callback-order boundaries. Evidence is
recorded in `analysis/compat/fill-build-dd-e3.md` and
`runs/vmctl-vc152fbde2/`. The first no-CRT linker diagnostic is retained in
`runs/vmctl-vc152fbde/`; the review ledger, coverage, TODO5, and reconstruction
note are synchronized.
