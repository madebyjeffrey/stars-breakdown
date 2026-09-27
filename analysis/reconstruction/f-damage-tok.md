# FDamageTok reconstruction

`reconstructed/src/f_damage_tok.c` is a typed semantic reconstruction of token damage resolution, preserving the damage gate, shield/defense interaction, damage accumulation, destruction handling, and affected-token callbacks.

Validation: `tests/f_damage_tok_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/FDMSTA.TXT` and `runs/vmctl-vc152ew/FDMRES.TXT`.
