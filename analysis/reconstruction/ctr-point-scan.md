# CtrPointScan reconstruction

`reconstructed/src/ctr_point_scan.c` is a typed semantic reconstruction of point-scan control, preserving cursor/point conversion, selected-object updates, and scanner redraw behavior.

Validation: `tests/ctr_point_scan_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/CTSTA.TXT` and `runs/vmctl-vc152ev/CTRES.TXT`.
