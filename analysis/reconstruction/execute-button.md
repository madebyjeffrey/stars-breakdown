# ExecuteButton reconstruction

`reconstructed/src/execute_button.c` is a typed semantic reconstruction of toolbar/button command execution. It preserves command lookup, enablement and dispatch decisions, UI refresh callbacks, and invalid-command handling.

Validation: `tests/execute_button_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/EXBSTA.TXT` and `runs/vmctl-vc152ew/EXBRES.TXT`.
