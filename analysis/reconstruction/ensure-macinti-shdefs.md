# EnsureMacintiShdefs reconstruction

`reconstructed/src/ensure_macinti_shdefs.c` is a typed semantic reconstruction of lazy Macinti ship-design initialization. It preserves one-time allocation, design copying, failure cleanup, and already-initialized behavior.

Validation: `tests/ensure_macinti_shdefs_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/EMSSTA.TXT` and `runs/vmctl-vc152ew/EMSRES.TXT`.
