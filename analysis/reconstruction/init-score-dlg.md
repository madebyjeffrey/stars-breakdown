# InitScoreDlg reconstruction

`reconstructed/src/init_score_dlg.c` is a typed semantic reconstruction of score-dialog initialization, preserving player rows, labels, values, control state, and report callbacks.

Validation: `tests/init_score_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/ISSTA.TXT` and `runs/vmctl-vc152ew/ISRES.TXT`.
