# InitializeProductionDlg reconstruction

`reconstructed/src/initialize_production_dlg.c` is a typed semantic reconstruction of production-dialog initialization, preserving queue population, control state, selected item, and cost/resource display.

Validation: `tests/initialize_production_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/IPSTA.TXT` and `runs/vmctl-vc152ew/IPRES.TXT`.
