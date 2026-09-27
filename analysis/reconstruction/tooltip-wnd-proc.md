# TooltipWndProc reconstruction

`reconstructed/src/tooltip_wnd_proc.c` is a typed semantic reconstruction of tooltip-window dispatch, preserving creation, paint, text, timer, cursor, and default-message paths.

Validation: `tests/tooltip_wnd_proc_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/TWPSTA.TXT` and `runs/vmctl-vc152ew/TWPRES.TXT`.
