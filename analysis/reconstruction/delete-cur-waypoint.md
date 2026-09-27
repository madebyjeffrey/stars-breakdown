# DeleteCurWayPoint reconstruction

`reconstructed/src/delete_cur_waypoint.c` is a typed semantic reconstruction of current-waypoint deletion, preserving index adjustment, list compaction, route state, and boundary handling.

Validation: `tests/delete_cur_waypoint_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/DCURSTA.TXT` and `runs/vmctl-vc152ev/DCURRES.TXT`.
