# EnsureTurinDroneShdefs reconstruction

`reconstructed/src/ensure_turin_drone_shdefs.c` is a typed semantic reconstruction of lazy Turin Drone ship-design initialization, preserving cached success, allocation/copy behavior, and failure cleanup.

Validation: `tests/ensure_turin_drone_shdefs_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/TRNSTA.TXT` and `runs/vmctl-vc152ew/TRNRES.TXT`.
