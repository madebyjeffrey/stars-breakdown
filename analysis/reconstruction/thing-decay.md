# ThingDecay reconstruction

`reconstructed/src/thing_decay.c` is a typed semantic reconstruction of transient-object decay, preserving age/count thresholds, removal, and affected-object updates.

Validation: `tests/thing_decay_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/TDSTA.TXT` and `runs/vmctl-vc152ew/TDRES.TXT`.
