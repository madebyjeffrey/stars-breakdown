# DrawFleetComp reconstruction

`reconstructed/src/draw_fleet_comp.c` is a typed semantic reconstruction of fleet-composition drawing, preserving component rows, counts, labels, and drawing-resource callbacks.

Validation: `tests/draw_fleet_comp_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DFCSTA11.TXT` and `runs/vmctl-vc152ew/DFCRES.TXT`.
