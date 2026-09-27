# SelectAdjFleet reconstruction

`reconstructed/src/select_adj_fleet.c` is a typed semantic reconstruction of adjacent-fleet selection, preserving adjacency tests, visibility, current-selection updates, and no-match behavior.

Validation: `tests/select_adj_fleet_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SAFSTA2.TXT` and `runs/vmctl-vc152ew/SAFRES.TXT`.
