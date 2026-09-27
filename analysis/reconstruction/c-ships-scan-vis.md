# CShipsScanVis reconstruction

`reconstructed/src/c_ships_scan_vis.c` is a typed semantic reconstruction of ship scan visibility, preserving scanner range, cloak/visibility gates, player relations, and visible-result updates.

Validation: `tests/c_ships_scan_vis_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/CSSTA.TXT` and `runs/vmctl-vc152ew/CSRES.TXT`.
