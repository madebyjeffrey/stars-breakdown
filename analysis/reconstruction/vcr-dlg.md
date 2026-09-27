# VCRDlg reconstruction

`reconstructed/src/vcr_dlg.c` is a typed semantic reconstruction of the battle playback dialog. It covers initialization, resize and paint/background dispatch, step-back/play/end controls, timer progression, progress updates, animation shutdown, popup focus, board selection, help, and close cleanup through explicit callbacks.

Validation: `tests/vcr_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and seven-case native probe pass in `runs/vmctl-vc152ew/VDLSTA.TXT` and `runs/vmctl-vc152ew/VDLRES.TXT`.
