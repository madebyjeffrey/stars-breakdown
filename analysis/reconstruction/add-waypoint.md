# FAddWayPoint reconstruction

`reconstructed/src/add_waypoint.c` is a typed semantic reconstruction of waypoint insertion, preserving coordinate validation, duplicate handling, list growth, and current-route updates.

Validation: `tests/add_waypoint_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/AWSTA.TXT` and `runs/vmctl-vc152ew/AWRES.TXT`.
