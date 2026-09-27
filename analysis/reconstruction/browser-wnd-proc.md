# BrowserWndProc reconstruction

`reconstructed/src/browser_wnd_proc.c` is a typed semantic reconstruction of the browser window procedure, preserving creation, navigation, paint, command, close, and default-message dispatch.

Validation: `tests/browser_wnd_proc_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/BRWSTA.TXT` and `runs/vmctl-vc152ew/BRWRES.TXT`.
