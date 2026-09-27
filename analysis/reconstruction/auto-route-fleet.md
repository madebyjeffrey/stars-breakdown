# AutoRouteFleet reconstruction

`reconstructed/src/auto_route_fleet.c` is a genuine C reconstruction of
`AutoRouteFleet` (`0011:1e52-227c`). It makes route initialization, destination
lookup and coordinates, order-count initialization, ideal-warp selection,
mineral-cargo stargate exclusion, same-owner stargate damage and warp-11
handling, fuel-capacity fallback, and final order-flag encoding explicit through
typed fleet/planet records and callbacks.

The host source probe covers route setup, destination resolution, order-count
setup, mineral-cargo exclusion, stargate selection, the fuel-aware fallback,
and the final descent to warp 1 when the fleet's fuel capacity rejects every
higher warp.

Native validation is complete at the focused procedure boundary. A fresh run
in `StarDev` used the exact current `reconstructed/src/auto_route_fleet.c`
source in guest directory `C:\ARF`; VC++ 1.52c reported `COMPILE=PASS` and
`LINK=PASS`, and the Win98 report was
`AUTO_ROUTE_FLEET_CHECKS=4 FAILURES=0 RESULT=PASS`. Redirected console output
was empty and the native window list showed no program-error dialog.

The packed/integration follow-up binds the recovered 124-byte `_fleet` view,
the four-byte `PLORD` header, and 18-byte order records, then round-trips
route target/coordinates, flags, order count, stargate selection, fuel descent,
and mineral-cargo exclusion through three native checks. The original Fleet
Waypoints/Fuel & Cargo surface is also preserved without a Program Error
dialog. Exact resource and route-helper behavior and a complete live
fleet-turn route comparison remain pending; the procedure is now at Phase 2
step 5/6.

The packed/integration harness was expanded to eight deterministic native
checks in a fresh `C:\ARFINT2` directory. It adds source-owner mismatch,
nonzero-gate-damage, missing-target, zero-ideal-warp, and packed cargo-hull
capacity cases while retaining stargate warp selection, fuel descent, mineral
exclusion, and raw order round-trip checks. VC1.52c compilation and linking
passed, the Win98 report was
`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`, and the
post-run window list contained no Program Error dialog. This strengthens, but
does not close, Phase 2 step 5/6; exact resource/route-helper behavior and
complete live fleet-turn comparison remain open.

The next packed/native expansion passes 12 checks in fresh `C:\ARFINT3`.
It covers the default Manhattan distance path, absent ideal-warp service,
absent fuel-estimator descent, and the fourth mineral cargo slot exclusion.
VC1.52c compilation and linking passed; the Win98 report was
`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`, with no
Program Error dialog. Exact resource behavior and the complete live
fleet-turn comparison remain the active step-5 concerns.

The final `C:\ARFINT4` matrix adds an independent transcription of the raw
route loop and twelve variations covering helper absence, gate damage,
mineral-slot selection, target geometry, fuel descent, and resource capacity.
Strict host C89 and VC++ 1.52c/Win98 both report
`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS`; compile and
link pass and the final window list contains no Program Error dialog. This
closes E1, the complete live route-comparison unit. Exact resource/helper
behavior and full live fleet-turn comparison remain E2, so the procedure stays
at Phase 2 step 5/6.

The E2 closure uses fresh `C:\ARFINT5` state. Packed design, hull, engine,
cargo, and mineral records are decoded into the real `EstFuelUse` service;
the route gate callback invokes the real `StargateDamage` service; and the
resulting order is consumed by the real `MoveFleets` pass. Four resource and
turn cases are compared with an independent fuel/gate/route/movement oracle,
including mineral-cargo exclusion, damaged-gate fallback, and a fuel-limited
route. The strict host and native reports each pass
`AUTO_ROUTE_FLEET_E2_CHECKS=86 FAILURES=0 RESULT=PASS`, with compile/link
success and no Win98 Program Error dialog. E2 is closed and the review record
is promoted; `AutoRouteFleet` is now Phase 2 step 6/6.
