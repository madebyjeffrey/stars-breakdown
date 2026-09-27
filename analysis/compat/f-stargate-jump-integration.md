# FStargateJump packed/integration checkpoint

The reconstructed stargate-jump procedure was checked against a raw packed
`_fleet` and 147-byte ship-design records. The adapter uses the recovered fleet
ID, player, planet, and flags at offsets 0, 2, 6, and 4; sixteen cargo-count
words at offset 12; sixteen cargo-state words at offset 44; and ship-design
mass and ship-weight fields at offsets 40 and 56. It loads those records into
the typed procedure, then writes the resulting fleet flags, cargo counts, and
cargo-state words back to the packed byte view.

The native cases cover the abort message envelope, damage failure without
mutation, inactive and total-loss empty-fleet paths, surviving cargo with
damage-state mutation and loss classification, the race-stat loss exemption,
multiple active cargo slots with per-slot design weights, the exact
damage-service callback arguments, zero cargo-damage state, and a large
multi-slot loss whose message amount exceeds 16 bits. The expanded matrix
also independently replays the original `MdCalcStargateDamage` boundary over
eight source/destination armor records, distance limits, range-overrun
thresholds, and defense/weight boundaries. The fresh VC1.52c/Win98 report is
`F_STARGATE_JUMP_INTEGRATION_CHECKS=2311 FAILURES=0 RESULT=PASS`; compile and
link also pass, and the post-run window list contains no Program Error dialog.

The original Fleet Summary Report / Fuel & Cargo surface shows the same fleet
identity, cargo, and fuel state without an error dialog. It is recorded as the
packed consumer boundary. The expanded matrix closes the original
damage-service differential unit. The final `C:\SGJINT5` run now wires the
reconstructed `FleetTransferCargoBalance` service through the stargate
handoff. It verifies the raw lost-fleet record (`0x0407` flags, lost ship
counts, zero destination planet), preserves the surviving source fleet, and
checks resource redistribution at packed offset 76: source fuel 23 and lost
record fuel 7 from an original fuel value of 30. This closes the real
game-service cargo-transfer comparison and promotes the procedure to 6/6.

Evidence:

- `runs/vmctl-vc152sgjint3/SGIR.TXT`
- `runs/vmctl-vc152sgjint3/SGISTA2.TXT`
- `runs/vmctl-vc152sgjint3/SGICL2.LOG`
- `runs/vmctl-vc152sgjint3/SGILINK2.LOG`
- `runs/vmctl-vc152sgjint3/SGISTA.TXT` (initial missing-stub attempt)
- `runs/vmctl-vc152sgjint3/SGICL.LOG` (initial missing-stub attempt)
- `runs/vmctl-vc152sgjint3/SGILINK.LOG` (initial missing-stub attempt)
- `runs/vmctl-vc152sgjint4/SGIR2.TXT`
- `runs/vmctl-vc152sgjint4/SGISTA2.TXT`
- `runs/vmctl-vc152sgjint4/SGICL2.LOG`
- `runs/vmctl-vc152sgjint4/SGILINK2.LOG`
- `runs/vmctl-vc152sgjint4/native-screen2.bmp`
- `runs/vmctl-vc152sgjint4/SGIR3.TXT`
- `runs/vmctl-vc152sgjint4/SGISTA3.TXT`
- `runs/vmctl-vc152sgjint4/SGICL3.LOG`
- `runs/vmctl-vc152sgjint4/SGILINK3.LOG`
- `runs/vmctl-vc152sgjint4/native-screen3.bmp`
- `runs/vmctl-vc152sgjint5/SGIR.TXT`
- `runs/vmctl-vc152sgjint5/SGISTA-final.TXT`
- `runs/vmctl-vc152sgjint5/SGICL-final.LOG`
- `runs/vmctl-vc152sgjint5/SGILINK-final.LOG`
- `runs/vmctl-vc152sgjint5/SGINAT.MAP`
- `runs/vmctl-vc152sgjint5/SGINAT.EXE`
- `runs/vmctl-vc152sgjint5/SGINAT.OBJ`
- `runs/vmctl-vc152sgjint5/native-screen.bmp`
- `tests/f_stargate_jump_integration_probe.c`
- `compat/vc152/f-stargate-jump-integration/`
- `reconstructed/src/fleet_transfer_cargo_balance.c`
- `analysis/decompiler/export/raw/1080_0cfe_FStargateJump.c`
- `analysis/decompiler/export/raw/1050_ae74_FleetTransferCargoBalance.c`
- `reconstructed/include/stypes.h`
- `runs/vmctl-vc152current/report-fleet-right.bmp`

This closes both E1, the original damage-service differential, and E2, the
complete real game-service cargo-transfer comparison, for `FStargateJump`.
The procedure is now promoted to Phase 2 step 6/6.
