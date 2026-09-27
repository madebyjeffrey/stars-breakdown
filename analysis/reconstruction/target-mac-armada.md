# TargetMacArmada reconstruction

`reconstructed/src/target_mac_armada.c` is a typed semantic reconstruction of Macinti armada target selection, preserving eligibility, relation/range gates, deterministic target choice, and the no-target result.

Validation: `tests/target_mac_armada_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/TMASTA.TXT` and `runs/vmctl-vc152ew/TMARES.TXT`.
