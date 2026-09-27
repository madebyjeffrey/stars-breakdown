# CTorpHit reconstruction

`reconstructed/src/c_torp_hit.c` is a typed semantic reconstruction of torpedo-hit resolution, preserving hit chance, shields/defense, damage, destruction, and combat-result updates.

Validation: `tests/c_torp_hit_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/THSTA.TXT` and `runs/vmctl-vc152ew/THRES.TXT`.
