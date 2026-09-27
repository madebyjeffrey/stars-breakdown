# RedrawScanSel reconstruction

`reconstructed/src/redraw_scan_selection.c` is a typed semantic reconstruction of scanner-selection redraw, preserving selected-object geometry, invalidation, and scan-window refresh behavior.

Validation: `tests/redraw_scan_selection_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/RNSTA.TXT` and `runs/vmctl-vc152ev/RNRES.TXT`.
