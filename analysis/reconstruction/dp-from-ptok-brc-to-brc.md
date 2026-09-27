# DpFromPtokBrcToBrc reconstruction

`reconstructed/src/dp_from_ptok_brc_to_brc.c` is a typed semantic reconstruction of token-to-token combat damage calculation, preserving range gates, beam/torpedo dispatch, defense and shield interaction, and callback-based weapon resolution.

Validation: `tests/dp_from_ptok_brc_to_brc_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DPBSTA.TXT` and `runs/vmctl-vc152ew/DPBRES.TXT`.
