# UpdateGuesses reconstruction

`reconstructed/src/update_guesses.c` is a typed semantic reconstruction of AI/player estimate updates, preserving guess accumulation, bounds, and resulting state fields.

Validation: `tests/update_guesses_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/UGSTA.TXT` and `runs/vmctl-vc152ew/UGRES.TXT`.
