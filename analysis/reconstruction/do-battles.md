# DoBattles reconstruction

`reconstructed/src/do_battles.c` is a typed semantic reconstruction of battle dispatch, preserving battle eligibility, participant iteration, resolution order, and resulting combat-state updates.

Validation: `tests/do_battles_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DBSTA.TXT` and `runs/vmctl-vc152ew/DBRES.TXT`.
