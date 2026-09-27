# DrawReport reconstruction

`reconstructed/src/draw_report.c` is a typed semantic reconstruction of report rendering, preserving report selection, title/column setup, row formatting, and drawing callbacks.

Validation: `tests/draw_report_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/draw-report/DRPSTA.TXT` and `runs/vmctl-vc152ew/draw-report/DRPRES.TXT`.
