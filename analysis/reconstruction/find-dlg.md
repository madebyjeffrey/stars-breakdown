# FindDlg reconstruction

`reconstructed/src/find_dlg.c` is a typed semantic reconstruction of the find dialog, preserving initialization, search text transfer, selection, validation, OK/Cancel, and help paths.

Validation: `tests/find_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/FDSTA.TXT` and `runs/vmctl-vc152ev/FDRES.TXT`.
