# CplrBattle reconstruction

`reconstructed/src/cplr_battle.c` is a typed semantic reconstruction of combatant preparation. It expands the attacker's plan target relation transitively, folds allied players into the combatant mask, separates spectators, marks participating fleets, and preserves the native execution path used to stage the battle.

Validation: `tests/cplr_battle_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and two-case native probe pass in `runs/vmctl-vc152ew/CBTSTA.TXT` and `runs/vmctl-vc152ew/CBTRES.TXT`.
