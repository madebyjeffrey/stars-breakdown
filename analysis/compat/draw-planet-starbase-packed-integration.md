# DrawPlanetStarbase packed-record integration checkpoint

`DrawPlanetStarbase` receives a prepared planet/starbase view from the planet
frame. The packed adapter therefore decodes the direct fields at the caller
boundary and supplies design, cargo/defense, shield, and warp-service values
through the existing typed `StarsPlanetStarbaseData` and callback boundary.

The checked direct fields are the recovered 56-byte `_planet` identity at
offset 0, the status word at offset 4 including `fStarbase` bit 9, and the
packed starbase word at offset 44. The adapter verifies the populated title,
detail rows, references, warp gauge, and mass-warp gauge-best calculation,
then exercises the no-warp and empty-starbase sentinel paths.

Host strict C89 and native VC++ 1.52c reports are:

`DRAW_PLANET_STARBASE_PACKED_CHECKS=17 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152dsbint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This advances `DrawPlanetStarbase` within Phase 2 step 5/6. The localized
resource unit is now closed separately: the fresh `C:\SBRES` package checks
the exact six row labels, the `none` and `Unlimited` special values, and the
resource callback order. The native report is
`DRAW_PLANET_STARBASE_RESOURCE_CHECKS=7 FAILURES=0 RESULT=PASS`, with compile,
link, clean process exit, and no Program Error dialog. The remaining named
unit was E3 exact shield/warp service behavior, detailed GDI/caller operations,
and original differential coverage, followed by final promotion. The packed
design lookup unit is now closed separately: fresh `C:\SBDS` host/native
checks report `DRAW_PLANET_STARBASE_DESIGN_CHECKS=10 FAILURES=0 RESULT=PASS`
over the raw design slot, hull lookup, cargo/defense offsets, mass-warp and
destination fields, no-starbase path, and invalid-table bounds.

The E3 service boundary is now closed. The strict host and fresh native
`C:\DPSBN` service probe report
`DRAW_PLANET_STARBASE_SERVICE_CHECKS=7 FAILURES=0 RESULT=PASS`. The matrix
checks that the populated title comes from the starbase class, shield lookup
receives hull and player IDs, warp lookup receives the planet ID, destination
lookup receives the destination ID and renders the returned name, the action
button receives its enabled state, and the gauge/reference rectangles match
the recovered geometry. Compile/link passed, the process exited 0, and the
post-run window list contained no Program Error dialog. This closes the named
Step-5 evidence units for the procedure; final ledger promotion remains.

Evidence:

- `tests/draw_planet_starbase_packed_probe.c`
- `compat/vc152/draw-planet-starbase-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1048_22cc_DrawPlanetStarbase.c`
- `analysis/reconstruction/draw-planet-starbase.md`
- `runs/vmctl-vc152dsbint/DSBINTSTA.TXT`
- `runs/vmctl-vc152dsbint/DSBINTCL.LOG`
- `runs/vmctl-vc152dsbint/DSBINTLINK.LOG`
- `runs/vmctl-vc152dsbint/DSBINT.TXT`
- `tests/draw_planet_starbase_resource_probe.c`
- `compat/vc152/draw-planet-starbase-resource/`
- `runs/vmctl-vc152sbr/SBRESSTA.TXT`
- `runs/vmctl-vc152sbr/SBRESCL.LOG`
- `runs/vmctl-vc152sbr/SBRESLINK.LOG`
- `runs/vmctl-vc152sbr/SBRES.TXT`
- `runs/vmctl-vc152sbr/native-screen.bmp`
- `tests/draw_planet_starbase_design_probe.c`
- `compat/vc152/draw-planet-starbase-design/`
- `runs/vmctl-vc152sbds2/SBDSSTA.TXT`
- `runs/vmctl-vc152sbds2/SBDSCL.LOG`
- `runs/vmctl-vc152sbds2/SBDSLINK.LOG`
- `runs/vmctl-vc152sbds2/SBDS.TXT`
- `runs/vmctl-vc152sbds2/native-screen.bmp`
- `tests/draw_planet_starbase_service_probe.c`
- `compat/vc152/draw-planet-starbase/DSB.C`
- `compat/vc152/draw-planet-starbase/core.h`
- `compat/vc152/draw-planet-starbase/DSBSV.C`
- `compat/vc152/draw-planet-starbase/DSBSV.DEF`
- `compat/vc152/draw-planet-starbase/DSBSV.LNK`
- `compat/vc152/draw-planet-starbase/BUILD-SV.BAT`
- `compat/vc152/draw-planet-starbase/RUN-SV-BUILD.BAT`
- `compat/vc152/draw-planet-starbase/RUN-SV-PROBE.BAT`
- `runs/vmctl-vc152dpsbservice/DSBSVSTA.TXT`
- `runs/vmctl-vc152dpsbservice/DSBSVCL.LOG`
- `runs/vmctl-vc152dpsbservice/DSBSVLINK.LOG`
- `runs/vmctl-vc152dpsbservice/DSBSVR.TXT`
- `runs/vmctl-vc152dpsbservice/native-screen.bmp`
