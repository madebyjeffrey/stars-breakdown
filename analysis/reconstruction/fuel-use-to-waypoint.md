# LFuelUseToWaypoint reconstruction

`reconstructed/src/fuel_use_to_waypoint.c` is a genuine C reconstruction of
`LFuelUseToWaypoint` (`000b:a9f4-ae73`). It models typed waypoint records and
the original segment loop: warp validation, distance and multi-turn fuel
decomposition, ram-scoop/cargo-hull adjustment, cumulative maximum tracking,
and the owned-planet cargo reset condition.

The host source probe covers valid and invalid warp segments, fuel-estimator
dispatch with the raw zero range-only argument, cargo-hull handling, cumulative
maximum behavior, the planet reset path, and the ram-scoop adjustment boundary
where the recovered code compares the adjusted multi-turn cost against the
already-maximized segment cost.

A fresh native checkpoint is complete in `StarDev` using the package in
`compat/vc152/fuel-use-to-waypoint/` and guest directory `C:\FUT2`. VC++ 1.52c
reported `COMPILE=PASS` and `LINK=PASS`; the Win98 report was
`FUEL_USE_TO_WAYPOINT_CHECKS=4 FAILURES=0 RESULT=PASS`, and the redirected
console file was empty. The failed LF-only batch attempt and the missing-stub
link attempt are retained in the same run directory as diagnostics; the final
run used CRLF wrappers and the retained `WINSTUB.EXE` fixture.

Exact packed hull/planet records, original-executable fuel cases, and full ship
integration remain pending.

A second native harness now binds the source to the recovered packed fleet,
18-byte waypoint orders, planet status, hull cargo capacity, and ship-design
hull boundary. It passes two integration checks covering warp/task extraction,
distance and multi-turn fuel dispatch, the owned special-planet reset boundary,
and raw field preservation. The original Fleet Summary Report and Fuel & Cargo
surface shows fleet fuel and cargo values without a program-error dialog. The
expanded `C:\FUTINT2` native matrix now passes 11/11 checks, adding
multi-segment accumulation, invalid warp, owner-mismatch versus owned-planet
reset, ram-scoop adjustment, varying route distances, and three-order packed
preservation. The final `C:\FUTINT3` matrix adds an independent transcription
of the original segment loop and twelve route cases varying warp sequence,
cargo-hull IDs, ram-scoop gain, and planet-reset state. Strict host C89 and
VC++ 1.52c/Win98 both report
`FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=23 FAILURES=0 RESULT=PASS`; compile
and link pass and the final window list contains no Program Error dialog. This
closes E1 and E2 in Phase 2. The final matrix covers all sixteen fleet
count/design slots, both cargo-hull IDs, the planet-selected hull definition,
and the full two-segment order boundary. The final `C:\FUTINT4` matrix passes
27/27 after adding all sixteen fleet/design slots, both cargo-hull IDs, and
the planet-selected hull definition. This closes E2 and the Step-6 promotion;
`LFuelUseToWaypoint` is promoted to Phase 2 step 6/6. The evidence is
procedure-level and does not claim whole-game turn-generator coverage.


## Original-instruction correction (2026-09-25)

The SO-E3 instruction audit supersedes the earlier arithmetic-equivalence
claims above. The old tests/native matrices used distance/warp for travel years
and missed the sufficient-fuel-generation branch; those passes did not prove
original equivalence. Source now implements the retained x87 formula with
0.99999, two warp divisions and 0.9999, plus the one-year fuel cap when generation
covers a year's use. Optional precise distance preserves fractional inputs.
The expanded strict-C89 probe passes; the revised code has not been run in
Win98. See `analysis/compat/satisfy-orders-e3-fuel-waypoint-disassembly.txt` for
the complete original instructions and constant bytes. The active SO-E3 speed
binding remains incomplete, with an explicitly failing integrated replay.
