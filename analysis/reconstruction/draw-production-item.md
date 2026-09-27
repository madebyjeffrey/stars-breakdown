# DrawProductionItem reconstruction

`reconstructed/src/draw_production_item.c` is a typed semantic reconstruction of production-queue item drawing, preserving item labels, counts, selected-state presentation, geometry, and drawing callbacks.

Validation: `tests/draw_production_item_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DPISTA2.TXT` and `runs/vmctl-vc152ew/DPIRES2.TXT`.
