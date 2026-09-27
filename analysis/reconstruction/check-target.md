# CheckTarget reconstruction

`reconstructed/src/check_target.c` is a typed semantic reconstruction of combat-target validation, preserving target-class, relation, range, and eligibility gates.

Validation: `tests/check_target_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/CTSTA.TXT` and `runs/vmctl-vc152ew/CTRES.TXT`.
