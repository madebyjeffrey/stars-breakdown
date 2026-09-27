# EnsureISShdefs reconstruction

`reconstructed/src/ensure_is_shdefs.c` is a typed semantic reconstruction of lazy IS ship-design initialization, preserving cached success, allocation/copy behavior, and failure cleanup.

Validation: `tests/ensure_is_shdefs_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/INSSTA.TXT` and `runs/vmctl-vc152ew/INSRES.TXT`.
