# TbWndProc reconstruction

`reconstructed/src/tb_wnd_proc.c` is a typed semantic reconstruction of toolbar-window dispatch. It preserves creation, paint/background, cursor, command, mouse, tooltip, capture, focus-termination, and default-dispatch paths. The host probe links with the shared `tb.c` geometry helpers.

Validation: `tests/tb_wnd_proc_source_probe.c` plus `reconstructed/src/tb.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/TBWSTA3.TXT` and `runs/vmctl-vc152ew/TBWRES.TXT`.
