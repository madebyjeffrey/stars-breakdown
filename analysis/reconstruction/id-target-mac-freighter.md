# IdTargetMacFreighter reconstruction

`reconstructed/src/id_target_mac_freighter.c` is a typed semantic reconstruction of Macinti freighter target selection. It preserves eligibility filtering, target ordering, range checks, and the no-target path through explicit callbacks.

Validation: `tests/id_target_mac_freighter_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/MFRSTA.TXT` and `runs/vmctl-vc152ew/MFRRES.TXT`.
