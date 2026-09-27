# SpankTheCheaters reconstruction

`reconstructed/src/spank_the_cheaters.c` is a typed semantic reconstruction of the anti-cheat response path, preserving trigger checks, penalty/state changes, and notification behavior.

Validation: `tests/spank_the_cheaters_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SPKSTA.TXT` and `runs/vmctl-vc152ew/SPKRES.TXT`.
