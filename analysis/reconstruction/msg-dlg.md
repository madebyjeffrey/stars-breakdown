# MsgDlg reconstruction

`reconstructed/src/msg_dlg.c` is a typed semantic reconstruction of message-dialog dispatch, preserving initialization, text population, button handling, help, close, and default-message paths.

Validation: `tests/msg_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/MDLSTA.TXT` and `runs/vmctl-vc152ew/MDLRES.TXT`.
