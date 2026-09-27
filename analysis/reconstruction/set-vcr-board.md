# SetVCRBoard reconstruction

`reconstructed/src/set_vcr_board.c` is a typed semantic reconstruction of battle-playback board selection, preserving live-token filtering, board coordinate setup, player/design labels, and empty-board handling.

Validation: `tests/set_vcr_board_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SVBSTA.TXT` and `runs/vmctl-vc152ew/SVBRES.TXT`.
