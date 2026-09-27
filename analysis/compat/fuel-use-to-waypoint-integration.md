# LFuelUseToWaypoint packed/integration checkpoint

The reconstructed waypoint fuel procedure was checked against raw packed fleet,
order, planet, hull-definition, and ship-design records. The adapter uses the
recovered `_fleet` owner at offset 2 and ship counts at offset 12, two 18-byte
`_order` records with coordinates at 0/2, target at 4, and task/warp flags at
6, the `_planet` owner and status word at offsets 2 and 4, the planet hull
selection at offset 44, and the `_huldef` cargo-capacity field at offset 52.
The ship-design hull identifier at offset 0 is also used for the cargo-hull
boundary.

The raw records are loaded into the typed waypoint input and the native run
checks waypoint warp extraction, distance dispatch, multi-turn fuel
decomposition, multi-segment accumulation, invalid warp, owned special-planet
reset versus owner mismatch, ram-scoop adjustment, varying route distances,
and preservation of the packed order, planet, fleet, hull, and design fields.
The expanded route matrix independently transcribes the original segment loop
and compares twelve varying route, warp, cargo-hull, ram-scoop, and planet-reset
cases against the reconstructed source. The final all-slot matrix in
`C:\FUTINT4` additionally loads all sixteen fleet/design slots and the
planet-selected hull definition. Its fresh VC1.52c/Win98 report is
`FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=27 FAILURES=0 RESULT=PASS`; compile
and link also pass, and the post-run window list contains no Program Error
dialog.

The preceding `C:\FUTINT3` report was
`FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=23 FAILURES=0 RESULT=PASS`; compile
and link also pass, and the post-run window list contains no Program Error
dialog.

The original Fleet Summary Report and Fuel & Cargo surface shows fleet fuel
values and cargo state without an error dialog. Together with the independent
segment-loop differential and all-slot packed matrix, this closes the
procedure's route and ship/waypoint acceptance boundary.

Evidence:

- `runs/vmctl-vc152futint3/FWIR2.TXT`
- `runs/vmctl-vc152futint3/FWISTA-final.TXT`
- `runs/vmctl-vc152futint3/FWICL-final.LOG`
- `runs/vmctl-vc152futint3/FWILINK-final.LOG`
- `runs/vmctl-vc152futint3/FWINAT-final.MAP`
- `runs/vmctl-vc152futint3/FWINAT-final.OBJ`
- `runs/vmctl-vc152futint3/FWINAT-final.EXE`
- `runs/vmctl-vc152futint3/native-screen2.bmp`
- `runs/vmctl-vc152futint4/FWIR.TXT`
- `runs/vmctl-vc152futint4/FWISTA-final.TXT`
- `runs/vmctl-vc152futint4/FWICL-final.LOG`
- `runs/vmctl-vc152futint4/FWILINK-final.LOG`
- `runs/vmctl-vc152futint4/FWINAT-final.MAP`
- `runs/vmctl-vc152futint4/FWINAT-final.OBJ`
- `runs/vmctl-vc152futint4/FWINAT-final.EXE`
- `runs/vmctl-vc152futint4/native-screen.bmp`
- `runs/vmctl-vc152futint2/FWIR.TXT` (prior 11-check checkpoint)
- `tests/fuel_use_to_waypoint_integration_probe.c`
- `compat/vc152/fuel-use-to-waypoint-integration/`
- `analysis/decompiler/export/raw/1050_a9f4_LFuelUseToWaypoint.c`
- `reconstructed/include/stypes.h`
- `runs/vmctl-vc152current/report-fleet-right.bmp`

The independent route-sequence differential and the complete packed
ship/waypoint integration units are now closed. The final matrix covers all
sixteen fleet count/design slots, both cargo-hull IDs, the planet-selected hull
definition, and the full two-segment order boundary. This procedure is
promoted to Phase 2 step 6/6; the 27 checks are procedure-level evidence and
do not claim whole-game turn-generator coverage.
