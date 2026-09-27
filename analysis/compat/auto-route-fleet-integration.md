# AutoRouteFleet packed/integration checkpoint

The reconstructed route planner was exercised through recovered packed fleet
and order storage in a fresh VC1.52c/Win98 harness. The adapter binds the
124-byte `_fleet` view, including player/position, mineral-weight, `cord`, and
the external `PLORD` buffer with its four-byte header and 18-byte order
records. It writes the route target, coordinates, flags, and order count back
to the raw records.

The checkpoint covers same-owner stargate selection, fuel-capacity descent,
mineral-cargo exclusion, source-owner mismatch, nonzero gate damage, missing
targets, zero ideal-warp fallback, and the packed cargo-hull capacity passed to
the gate service. Host and native reports are:

`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`

The original Fleet Waypoints/Fuel & Cargo surface is preserved in
`runs/vmctl-vc152current/fleet-transfer.bmp`; it confirms the consumer
boundary without claiming a full original route differential. Exact resource
and route-helper behavior and complete live fleet-turn comparison remain open,
so this advances `AutoRouteFleet` to Phase 2 step 5/6, not promotion.

The follow-up fresh `C:\ARFINT3` package adds four fallback/helper cases:
default Manhattan distance when the distance service is absent, stargate warp
selection when the ideal-warp service is absent, default fuel descent when the
fuel service is absent, and exclusion when only the fourth mineral cargo slot
is populated. VC++ 1.52c compile/link passed and the native report returned
`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`; no Program
Error dialog appeared.

Evidence:

- `tests/auto_route_fleet_integration_probe.c`
- `compat/vc152/auto-route-fleet-integration/`
- `runs/vmctl-vc152arfint/ARFISTA.TXT`
- `runs/vmctl-vc152arfint/ARFICL.LOG`
- `runs/vmctl-vc152arfint/ARFILINK.LOG`
- `runs/vmctl-vc152arfint/ARFIR.TXT`
- `runs/vmctl-vc152arfint/native-screen.bmp`
- `runs/vmctl-vc152arfint2/ARFISTA.TXT`
- `runs/vmctl-vc152arfint2/ARFICL.LOG`
- `runs/vmctl-vc152arfint2/ARFILINK.LOG`
- `runs/vmctl-vc152arfint2/ARFIR.TXT`
- `runs/vmctl-vc152arfint2/native-screen.bmp`
- `runs/vmctl-vc152arfint3/ARFISTA.TXT`
- `runs/vmctl-vc152arfint3/ARFICL.LOG`
- `runs/vmctl-vc152arfint3/ARFILINK.LOG`
- `runs/vmctl-vc152arfint3/ARFIR.TXT`
- `runs/vmctl-vc152arfint4/ARFISTA-final.TXT`
- `runs/vmctl-vc152arfint4/ARFICL-final.LOG`
- `runs/vmctl-vc152arfint4/ARFILINK-final.LOG`
- `runs/vmctl-vc152arfint4/ARFIR.TXT`
- `runs/vmctl-vc152arfint4/ARFINT-final.MAP`
- `runs/vmctl-vc152arfint4/ARFINT-final.OBJ`
- `runs/vmctl-vc152arfint4/ARFINT-final.EXE`
- `runs/vmctl-vc152arfint4/native-screen.bmp`
- `runs/vmctl-vc152current/fleet-transfer.bmp`

The final fresh `C:\ARFINT4` package adds an independent transcription of the
raw route loop and compares twelve variations covering helper absence, gate
damage, mineral-slot selection, target geometry, fuel descent, and resource
capacity. The strict host and VC++ 1.52c/Win98 adapters each report
`AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS`; compile and
link pass and the final window list contains no Program Error dialog. This
closes the live route-comparison E1 unit. Exact resource/helper behavior and
the complete live fleet-turn comparison remain the single E2 boundary.

The fresh `C:\ARFINT5` E2 package closes that boundary. It maps the packed
design, hull, engine, cargo, and mineral records into the real reconstructed
`EstFuelUse` service, invokes the real `StargateDamage` service through the
route callback, and hands the generated order to the reconstructed
`MoveFleets` pass. Four cases cover gate success, mineral-cargo exclusion,
damaged-gate fallback, and a fuel-limited route. An independent fuel, gate,
route, and movement oracle checks the packed state and the complete turn
result. Strict host C89 and VC++ 1.52c/Win98 each report
`AUTO_ROUTE_FLEET_E2_CHECKS=86 FAILURES=0 RESULT=PASS`; compile and link pass,
and the final Win98 window list and screenshot contain no Program Error dialog.
E2 and the final ledger promotion are closed, so `AutoRouteFleet` is promoted
to Phase 2 step 6/6.

Additional E2 evidence:

- `tests/auto_route_fleet_e2_probe.c`
- `compat/vc152/auto-route-fleet-integration/E2INT.C`
- `compat/vc152/auto-route-fleet-integration/E2INT.LNK`
- `compat/vc152/auto-route-fleet-integration/E2INT.DEF`
- `compat/vc152/auto-route-fleet-integration/BUILDE2.BAT`
- `compat/vc152/auto-route-fleet-integration/RUNVC-E2.BAT`
- `compat/vc152/auto-route-fleet-integration/RUNPROBE-E2.BAT`
- `reconstructed/src/est_fuel_use.c`
- `reconstructed/src/stargate_damage.c`
- `reconstructed/src/move_fleets.c`
- `runs/vmctl-vc152arfint5/E2ISTA-final2.TXT`
- `runs/vmctl-vc152arfint5/E2ICL-final2.LOG`
- `runs/vmctl-vc152arfint5/E2ILINK-final2.LOG`
- `runs/vmctl-vc152arfint5/E2IR-final2.TXT`
- `runs/vmctl-vc152arfint5/E2INT-final2.MAP`
- `runs/vmctl-vc152arfint5/E2INT-final2.OBJ`
- `runs/vmctl-vc152arfint5/E2INT-final2.EXE`
- `runs/vmctl-vc152arfint5/native-screen-final2.bmp`
