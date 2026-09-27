# RenameDlg reconstruction

`reconstructed/src/rename_dlg.c` is a typed semantic reconstruction of rename-dialog behavior, preserving initialization, text transfer, validation, OK/Cancel, help, and dirty-state handling.

Validation: `tests/rename_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/RNDSTA.TXT` and `runs/vmctl-vc152ew/RNDRES.TXT`.
