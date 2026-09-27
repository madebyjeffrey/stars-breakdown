# IBestRemoteTerra reconstruction

`reconstructed/src/best_remote_terraform.c` is a typed semantic reconstruction of best remote-terraform target selection, preserving candidate eligibility, distance/resource comparison, and fallback behavior.

Validation: `tests/best_remote_terraform_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/BRTSTA.TXT` and `runs/vmctl-vc152ew/BRTRES.TXT`.
