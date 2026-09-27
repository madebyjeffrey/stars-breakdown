# PctCloakFromLpfl reconstruction

`reconstructed/src/pct_cloak.c` is a typed semantic reconstruction of fleet cloaking percentage calculation, preserving hull/design modifiers, fleet composition, rounding, bounds, and the empty-fleet path.

Validation: `tests/pct_cloak_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152eu/PCLKSTA.TXT` and `runs/vmctl-vc152eu/PCLKRES.TXT`.
