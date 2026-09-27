# iAddAttackFleet reconstruction

`reconstructed/src/add_attack_fleet.c` is a typed semantic reconstruction of attack-fleet insertion, preserving fleet allocation, combatant linkage, target assignment, and list updates.

Validation: `tests/add_attack_fleet_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/IAFSTA.TXT` and `runs/vmctl-vc152ew/IAFRES.TXT`.
