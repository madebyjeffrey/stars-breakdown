# EnsureRobotoidShdefs reconstruction

`reconstructed/src/ensure_robotoid_shdefs.c` is a typed semantic reconstruction of lazy Robotoid ship-design initialization. It preserves one-time allocation, design copying, failure cleanup, and the cached-success path.

Validation: `tests/ensure_robotoid_shdefs_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/RBTSTA.TXT` and `runs/vmctl-vc152ew/RBTRES.TXT`.
