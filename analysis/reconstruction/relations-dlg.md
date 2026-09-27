# RelationsDlg reconstruction

`reconstructed/src/relations_dlg.c` is a typed semantic reconstruction of the relations dialog, preserving initialization, current-player exclusion, radio selection, dirty logging, scanner invalidation, paint/background dispatch, OK, and Help handling.

Validation: `tests/relations_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152d/RLSTA2.TXT` and `runs/vmctl-vc152d/RLRES.TXT`.
