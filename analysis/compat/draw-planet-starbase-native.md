# DrawPlanetStarbase native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dpsn/`.

- `DSBSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DSBCL.LOG`: VC++ 1.52c compilation completed for the current
  `draw_planet_starbase.c` source and probe; the warnings are the expected
  signed/unsigned conversions in the recovered sentinel and rectangle code
- `DSBLINK.LOG`: segmented link completed with `DSB.DEF`
- `DPSBR.TXT`: `DRAW_PLANET_STARBASE_CHECKS=14 FAILURES=0 RESULT=PASS`
- `DSB-CONSOLE.TXT`: empty; the process exited cleanly

The corrected run used fresh `C:\DPSBN`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates the tile-reset and
empty-reference paths, title/action/detail/warp references, detail-row order,
cargo-capacity special values, defense/shield/mass-warp rendering, separator,
warp button, and packed gauge-best path. Packed planet/starbase records,
localized resource tables, and full GDI integration remain outside this
checkpoint.

## Current Step-5 status

The localized-resource unit is now closed as E2. The strict host and fresh
native package in `runs/vmctl-vc152sbr/` checked the six row-label IDs and the
two special value IDs, including exact original strings and callback order:

`DRAW_PLANET_STARBASE_RESOURCE_CHECKS=7 FAILURES=0 RESULT=PASS`

The native package reports `COMPILE=PASS` and `LINK=PASS`, exited 0, and the
post-run `windows` check showed no Program Error dialog. This is one named
evidence unit, not completion of the procedure. DrawPlanetStarbase remains at
5/6 with E1 packed planet/starbase/design-service completion and E3 exact
shield/warp services, full GDI/caller behavior, original differential
coverage, and promotion still open.

The packed design lookup unit is now also closed as E1. The fresh native
package in `runs/vmctl-vc152sbds2/` reports
`DRAW_PLANET_STARBASE_DESIGN_CHECKS=10 FAILURES=0 RESULT=PASS` for raw design
slot extraction, hull-ID resolution, cargo/defense offsets, mass-warp and
destination decoding, no-starbase behavior, and invalid-table bounds. The host
probe passes the same strict C89 matrix. The remaining E3 boundary is now
closed by the fresh `C:\DPSBN` service package in
`runs/vmctl-vc152dpsbservice/`. Its native report is
`DRAW_PLANET_STARBASE_SERVICE_CHECKS=7 FAILURES=0 RESULT=PASS`; compile/link
passed, the process exited 0, and no Program Error dialog appeared. The
procedure is therefore at `0 + P`: all named Step-5 evidence units are closed,
with only final ledger promotion and original differential note
synchronization remaining before 6/6.
