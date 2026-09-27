# UpdatePopulations reconstruction

`reconstructed/src/update_populations.c` is a typed semantic reconstruction of population growth and decline, preserving climate/owner gates, rounding, caps, and resulting planet updates.

Validation: `tests/update_populations_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/UPSTA.TXT` and `runs/vmctl-vc152ew/UPRES.TXT`.
