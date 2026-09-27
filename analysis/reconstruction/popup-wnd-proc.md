# PopupWndProc reconstruction

`reconstructed/src/popup_wnd_proc.c` is a typed semantic reconstruction of popup-window dispatch, preserving creation, paint, command, close, and default-message behavior.

Validation: `tests/popup_wnd_proc_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/PWSTA.TXT` and `runs/vmctl-vc152ev/PWRES.TXT`.
