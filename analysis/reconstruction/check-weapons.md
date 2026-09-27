# CheckWeapons reconstruction

`reconstructed/src/check_weapons.c` is a typed semantic reconstruction of weapon validity and combat-loadout checking, preserving weapon-class gates, capacity checks, and the resulting status decisions.

Validation: `tests/check_weapons_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152g/CWSTA.TXT` and `runs/vmctl-vc152g/CWRES.TXT`.
