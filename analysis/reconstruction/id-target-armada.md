# IdTargetArmada reconstruction

`reconstructed/src/id_target_armada.c` is a typed semantic reconstruction of armada target selection. It preserves target eligibility, range and relation filtering, deterministic selection ordering, and the no-target result through explicit state.

Validation: `tests/id_target_armada_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/ITASTA.TXT` and `runs/vmctl-vc152ew/ITARES.TXT`.
