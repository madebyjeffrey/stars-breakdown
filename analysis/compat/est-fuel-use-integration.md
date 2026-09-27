# EstFuelUse packed/integration checkpoint

The reconstructed fuel estimator was checked against a raw packed fleet and
the design/component tables used by the original procedure. The adapter uses
the recovered `_fleet` fields: player at offset 2, destination at 6, ship
counts at 12, total fleet cargo weight at 44, and five 32-bit mineral-weight
entries beginning at 76. It also walks sixteen 147-byte `_shdef` records,
using the hull/stat fields at offsets 0, 40, 58, 60, and 122, a 143-byte
`_huldef` cargo-capacity field at offset 52, and the 78-byte `_engine`
`rgcFuelUsed[12]` table at offset 54.

The packed adapter loads the raw records into the typed `EstFuelUse` input,
then checks the original engine-fuel ordering, race discount, Ram Scoop flag,
fixed-point fuel cost, raw field preservation, no-cargo range sentinel,
warp-10 selection, invalid-warp flag clearing, order-derived warp and
distance callbacks, a large intermediate product, absent-engine fallback,
range-only conversion, and record sizes. The expanded matrix independently
replays the original fuel loop over all sixteen design slots, all sixteen
hull records, all fifteen engine IDs (including a duplicate engine ID), all
twelve warp rows, both race-discount modes, range-only modes, and route
distances 1, 37, 1000, and 50000. The fresh VC1.52c/Win98 run in
`C:\EFUINT3` reports
`EST_FUEL_USE_INTEGRATION_CHECKS=29 FAILURES=0 RESULT=PASS`; compile and link
also pass, and the post-run window list contains no Program Error dialog.

The original executable's Fleet Summary Report and Fuel & Cargo panel show
per-fleet fuel values and cargo state without an error dialog. This establishes
the procedure's consumer/UI boundary, but not a complete original-versus-
reconstructed fuel differential across all hulls, engines, races, and route
distances.

Evidence:

- `runs/vmctl-vc152efuint2/EFIR.TXT`
- `runs/vmctl-vc152efuint2/EFISTA.TXT`
- `runs/vmctl-vc152efuint2/EFICL.LOG`
- `runs/vmctl-vc152efuint2/EFILINK.LOG`
- `runs/vmctl-vc152efuint2/native-screen.bmp`
- `tests/est_fuel_use_integration_probe.c`
- `compat/vc152/est-fuel-use-integration/`
- `analysis/decompiler/export/raw/1050_9fe4_EstFuelUse.c`
- `analysis/decompiler/export/raw/1050_41b2_WtMaxShdefStat.c`
- `reconstructed/include/stypes.h`
- `runs/vmctl-vc152current/report-fleet-right.bmp`

The expanded matrix closes the remaining original-instruction acceptance
boundary for `EstFuelUse`: every packed component/hull table mapping and the
route-distance cases are checked against an independent transcription of the
original loop. This is procedure-level differential evidence, not a claim that
the whole game turn generator has been reconstructed. The procedure is
promoted to Phase 2 step 6 because its remaining step-5 unit and the final
ledger synchronization are now closed.
