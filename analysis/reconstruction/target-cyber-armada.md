# TargetCyberArmada reconstruction

`reconstructed/src/target_cyber_armada.c` is a typed semantic reconstruction of Cyber armada target selection, preserving eligibility, relation/range gates, deterministic target choice, and the no-target result.

Validation: `tests/target_cyber_armada_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/TCASTA3.TXT` and `runs/vmctl-vc152ew/TCARES2.TXT`.
