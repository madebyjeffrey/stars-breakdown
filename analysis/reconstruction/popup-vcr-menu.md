# PopupVCRMenu reconstruction

`reconstructed/src/popup_vcr_menu.c` is a typed semantic reconstruction of the battle-playback popup menu, preserving token filtering, player/design labels, current-step/loss annotations, row mapping, and empty-menu behavior.

Validation: `tests/popup_vcr_menu_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/PVMSTA.TXT` and `runs/vmctl-vc152ew/PVMRES.TXT`.
