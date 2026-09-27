# MeteorStrike reconstruction

`reconstructed/src/meteor_strike.c` is a typed semantic reconstruction of meteor-strike resolution, preserving target selection, impact damage, planetary effects, and resulting state updates.

Validation: `tests/meteor_strike_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/MSSTA2.TXT` and `runs/vmctl-vc152ew/MSRES2.TXT`.
