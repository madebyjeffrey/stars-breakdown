# KillShips reconstruction

`reconstructed/src/kill_ships.c` is a typed semantic reconstruction of ship destruction, preserving token removal, fleet counts, cargo/salvage effects, and combat-state updates.

Validation: `tests/kill_ships_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/KSSTA.TXT` and `runs/vmctl-vc152ew/KSRES.TXT`.
