# DumpFleets reconstruction (partial)

`reconstructed/src/dump_fleets.c` transfers the fleet report coordinator at
NE `0034:9530–a1c1` into typed C.  It preserves the active-game/viewer gate,
normal versus detailed header count, fleet iteration, ship-total calculation,
detailed-report destination visibility, detailed player identity and
ETA/warp/weight/scanner/stealth fields,
mine/terraform summaries, per-hull-class ship totals, and
close-on-write-failure behavior.  Text names and localized formatting remain
callback-owned.

The destination gate is now represented explicitly: a destination that the
raw routine only emits in detailed mode is not made visible by the viewer's
ownership.  The raw order/route metadata that distinguishes the always-visible
task form from the conditional coordinate form remains callback-owned.

The source probe passes those report-row, visibility, and hull-class breakdown
paths under strict host C89. A fresh VC++ 1.52c / Win98 checkpoint passes all
10 native checks without a runtime error dialog. Full destination/task-name
formatting, complete serialized stream comparison, and original-behavior
evidence remain open.

The original Win98 executable has now been exercised through the export
boundary: `Reports > Dump to Text File > Fleet Information` wrote
`C:\\STARS!\\GAME.fle` (527 bytes) and displayed its success message without a
Program Error. Evidence and the artifact hash are preserved in
`analysis/compat/report-dump-integration.md` and
`runs/vmctl-vc152current/`. This advances `DumpFleets` to Phase 2 step 5/6
for the original export/file-write boundary; full destination/task-name
formatting, scanner/cloak/minefield/battle-plan integration, packed records,
and promotion remain open.

The packed-record integration checkpoint is now also complete. A strict host
adapter and a fresh VC++ 1.52c / Win98 `C:\\DMPFINT2` run each report
`DUMP_FLEETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS` over the recovered
124-byte `_fleet` layout, all cargo slots, ship-class mapping, detailed
scanner/cloak/mine/terraform fields, destination visibility, normal-mode
suppression, and the destination-requires-detail gate. The native build
reports compile and link success, exits 0, and produces no Program Error
dialog. This strengthens the procedure's Phase 2 step-5 evidence but does not
promote it: localized formatting, exact task/scanner/minefield/battle-plan
services, complete original differential output, and final review remain open.
See `analysis/compat/dump-fleets-packed-integration.{md,json}` and
`runs/vmctl-vc152dmpfint2/`.

The packed decoder is now owned by production source in
`reconstructed/src/dump_fleets.c` as `stars_dump_fleets_packed`; the strict
host probe calls that bridge directly rather than carrying a test-only raw
record decoder. The production bridge also exposes `resolve_services`, which
runs after the 124-byte decode and before detail-mode visibility gates. A
fresh host/native 12-check matrix proves raw-record forwarding, callback
ordering, and service overrides for ETA, scanner/task, destination, battle
plan, minefield, and terraforming fields. This closes the named packed-service
integration unit; localized serialization, original output, and promotion
remain open. Evidence is in
`analysis/compat/dump-fleets-services-e3.{md,json}` and
`runs/vmctl-vc152dfse3/`.

The host boundary matrix is now expanded in
`analysis/compat/dump-fleets-boundary-e3.{md,json}`. Strict C89 and a fresh
production-source VC++ 1.52c/Win98 packet in `C:\DFBP` both pass
`DUMP_FLEETS_BOUNDARY_CHECKS=20 FAILURES=0 RESULT=PASS` over the active-game
and viewer gates, open/header/row failure cleanup, normal/detailed headers,
all four ship-class buckets plus the other-class total, all cargo slots,
detail suppression, destination-requires-detail behavior, and detailed
scanner/task/mine/terraform fields. Compile, link, process exit, and the
post-run no-Program-Error check also pass. This is a native boundary
milestone; the service-resolution unit is recorded in
`analysis/compat/dump-fleets-services-e3.{md,json}`. Localized serialization,
original output, and promotion remain open.

The production coordinator now also exposes the recovered serialized field
order through raw and formatted text callbacks. The host/native
`DUMP_FLEETS_SERIALIZATION_CHECKS=26` matrix covers the exact 12-field normal
sequence and 29-field detailed sequence, including cargo, route/scanner/task,
mine/terraform, other-hull, and all four hull-class buckets. The formatted
path checks localized fleet/planet/destination/battle-plan text, numeric
values, the preserved original `GAME.fle` first-row oracle, and detailed
task/scanner fields. Its final six-row scenario compares the complete
header/data/CRLF stream byte-for-byte with the preserved original
`runs/vmctl-vc152current/GAME.fle`; the host/native matrix reports 26/26 with
zero failures. This closes the named complete original-output E4 unit in
addition to the previously closed E1 localized scanner/task, E2 field-order,
and E3 packed service units. All named Step-5 evidence is closed for
`DumpFleets`, and Step-6 promotion is recorded in the review ledger. Evidence is in
`analysis/compat/dump-fleets-serialization-e4.{md,json}` and
`runs/vmctl-vc152dfser4/`.
