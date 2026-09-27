# InvalidateReport reconstruction

`reconstructed/src/invalidate_report.c` is a typed semantic reconstruction of report invalidation, preserving report selection, dirty flags, redraw requests, and window-state updates.

Validation: `tests/invalidate_report_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/IRSTA.TXT` and `runs/vmctl-vc152ev/IRRES.TXT`.
