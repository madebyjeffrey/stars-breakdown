# NewPlanNameDlg reconstruction

`reconstructed/src/new_plan_name_dlg.c` is a typed semantic reconstruction of battle-plan naming, preserving initialization, text transfer, validation, OK/Cancel, dirty state, and Help handling.

Validation: `tests/new_plan_name_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/NPNSTA.TXT` and `runs/vmctl-vc152ew/NPNRES.TXT`.
