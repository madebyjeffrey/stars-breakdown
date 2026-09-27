# CBattleUnits reconstruction

`reconstructed/src/c_battle_units.c` is a typed semantic reconstruction of battle-unit setup and lookup, preserving participant grouping, token linkage, and empty-unit handling.

Validation: `tests/c_battle_units_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/BSTA.TXT` and `runs/vmctl-vc152ew/BRES.TXT`.
