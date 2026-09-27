# DrawFleetGauge reconstruction

`reconstructed/src/draw_fleet_gauge.c` is a typed semantic reconstruction of fleet-gauge drawing, preserving value scaling, labels, clipping, and drawing-resource callbacks.

Validation: `tests/draw_fleet_gauge_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DFGSTA.TXT` and `runs/vmctl-vc152ew/DFGRES.TXT`.
