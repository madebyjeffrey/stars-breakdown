# DrawHistoryReport reconstruction

`reconstructed/src/draw_history_report.c` is a typed semantic reconstruction of the history graph renderer. It computes the title, axes, range and tick labels, scale, grid labels, legend/player colors, and per-player series geometry, including the single-point case.

Validation: `tests/draw_history_report_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and two-case native probe pass in `runs/vmctl-vc152ew/DHSTA.TXT` and `runs/vmctl-vc152ew/DHRES.TXT`.
