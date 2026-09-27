# FRetargetMiner reconstruction

`reconstructed/src/f_retarget_miner.c` is a typed semantic reconstruction of miner retargeting, preserving candidate selection, target validity, route/order updates, and no-target behavior.

Validation: `tests/f_retarget_miner_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/RSTA.TXT` and `runs/vmctl-vc152ew/RRES.TXT`.
