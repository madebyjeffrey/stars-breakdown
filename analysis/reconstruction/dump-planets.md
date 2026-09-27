# DumpPlanets reconstruction (partial)

`reconstructed/src/dump_planets.c` transfers the report coordinator at NE
`0034:86e4–952f` into typed C.  It preserves the active-game/viewer gate,
normal versus detailed header count, planet iteration, packed visibility-bit
owner rule,
turn-age calculation, copied environment/mineral fields, detailed-report
gating, planet-class gates for production/resources/mineral fields, scan
visibility for scanner/starbase/wormhole fields, derived report fields,
close-on-write-failure behavior, and
callback-owned text formatting.

The source probe passes those coordinator, visibility, and normal-versus-
detailed field paths under strict host C89. A fresh VC++ 1.52c / Win98
checkpoint passes all 16 native checks without a runtime error dialog. The
remaining work is exact localized field formatting, complete serialized
stream comparison, and original-behavior evidence.

The original Win98 executable has now been exercised through the export
boundary: `Reports > Dump to Text File > Planet Information` wrote
`C:\\STARS!\\GAME.pla` (334 bytes) and displayed its success message without a
Program Error. Evidence and the artifact hash are preserved in
`analysis/compat/report-dump-integration.md` and
`runs/vmctl-vc152current/`. This advances `DumpPlanets` to Phase 2 step 5/6
for the original export/file-write boundary; exact localized formatting,
starbase/wormhole/production callbacks, packed integration, and promotion
remain open.

A packed-record adapter now passes 22 host and native checks over the
recovered 56-byte `_planet` layout and the detailed report-row boundary. The
fresh `C:\DMPINT2` run had `COMPILE=PASS`, `LINK=PASS`, and no Program Error
dialog; the expanded run covers every environment and mineral array slot plus
the complete starbase, wormhole, scanner, and row-derived fields. Localized
formatting, complete starbase/wormhole/production services, full original
differential behavior, and promotion remain open; see
`analysis/compat/dump-planets-packed-integration.md`.

The packed decoder is now owned by production source in
`reconstructed/src/dump_planets.c` as `stars_dump_planets_packed`; the strict
host probe calls that bridge directly rather than carrying a test-only raw
record decoder. The production bridge also exposes `resolve_services`, which
runs after the 56-byte decode and before the ordinary report gates. A fresh
host/native 12-check matrix proves raw-record forwarding, callback ordering,
and service overrides for the derived report fields. This closes the named
packed-service integration unit; localized serialization, original output,
and promotion remain open. Evidence is in
`analysis/compat/dump-planets-services-e3.{md,json}` and
`runs/vmctl-vc152dpse3/`.

The host boundary matrix is now expanded in
`analysis/compat/dump-planets-boundary-e3.{md,json}`. Strict C89 and a fresh
production-source VC++ 1.52c/Win98 packet in `C:\DPBP` both pass
`DUMP_PLANETS_BOUNDARY_CHECKS=24 FAILURES=0 RESULT=PASS` over the active-game
and viewer gates, open/header/row failure cleanup, normal/detailed headers,
turn-age and owner visibility, scanned service fields, planet-class clearing,
and normal production/resource preservation. Compile, link, process exit, and
the post-run no-Program-Error check also pass. This is a native boundary
milestone; the service-resolution unit is recorded in
`analysis/compat/dump-planets-services-e3.{md,json}`. Localized serialization,
original output, and promotion remain open.

The production coordinator now also exposes the recovered twenty-column normal
serialization boundary through formatter callbacks. The strict-C89 host probe
and fresh VC++ 1.52c/Win98 replay in `C:\DPSER1` both report
`DUMP_PLANETS_SERIALIZATION_CHECKS=12 FAILURES=0 RESULT=PASS`. The matrix checks
raw and formatted field order over two rows, localized names/queue/value/defence
text, the preserved tab/CRLF `GAME.pla` row oracle, and invalid callback
rejection. Compile/link and process exit passed, with no Program Error dialog.
This closes the named localized normal-serialization E1 unit; detailed/service
columns, complete original differential output, and promotion remain open.
Evidence is in `analysis/compat/dump-planets-serialization-e1.{md,json}`,
`compat/vc152/dump-planets-serialization-e1/`, and
`runs/vmctl-vc152dpser1/`.

The complete normal export now has an independent E2 differential. The
strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\DPSOR2` both
report `DUMP_PLANETS_SERIALIZATION_CHECKS=14 FAILURES=0 RESULT=PASS`; the
header, both rows, tab separators, and CRLF terminators compare byte-for-byte
with `runs/vmctl-vc152current/GAME.pla`. Compile/link and process exit passed,
and the final guest window inventory contained no Program Error dialog. This
closes the named complete serialized-output E2 unit. The original executable
behavior boundary beyond the preserved export and final promotion remain open.
Evidence is in `analysis/compat/dump-planets-original-output-e2.{md,json}`,
`compat/vc152/dump-planets-original-output-e2/`, and
`runs/vmctl-vc152dpsor2/`.

The original Win98 executable behavior boundary is now closed as E4. The
actual `Reports > Dump to Text File > Planet Information` path wrote the
334-byte `GAME.pla`, displayed its success message, and completed without a
Program Error; the artifact hash and screenshots are preserved in
`analysis/compat/report-dump-integration.json`. Combined with E1, E2, and E3,
all four named Step-5 units are closed and the final review-ledger promotion is
recorded. `DumpPlanets` is now Phase 2 **6/6**. Evidence is in
`analysis/compat/dump-planets-original-behavior-e4.{md,json}`.
