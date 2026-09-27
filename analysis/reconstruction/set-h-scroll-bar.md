# SetHScrollBar reconstruction

`reconstructed/src/set_h_scroll_bar.c` is a typed semantic reconstruction of horizontal-scrollbar synchronization, preserving range, thumb position, enablement, and redraw behavior.

Validation: `tests/set_h_scroll_bar_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/HSSTA.TXT` and `runs/vmctl-vc152ev/HSRES.TXT`.
