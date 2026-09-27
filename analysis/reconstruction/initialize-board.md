# InitializeBoard reconstruction

`reconstructed/src/initialize_board.c` is a typed semantic reconstruction of battle-board initialization, preserving board dimensions, token placement, viewer state, initial rendering/setup callbacks, and the produced token count used by the optional FDoCoolBattle board path.

Validation: `tests/initialize_board_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152h/IBDSTA.TXT` and `runs/vmctl-vc152h/IBDRES.TXT`.
