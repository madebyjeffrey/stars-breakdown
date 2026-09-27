# DrawFleetBitmap reconstruction

`reconstructed/src/draw_fleet_bitmap.c` is a typed semantic reconstruction of fleet bitmap rendering, preserving design selection, bitmap placement, masking, and drawing-resource cleanup.

Validation: `tests/draw_fleet_bitmap_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DFBSTA2.TXT` and `runs/vmctl-vc152ew/DFBRES.TXT`.
