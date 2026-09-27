# FGetNextObjHere reconstruction

`reconstructed/src/get_next_obj_here.c` is a typed semantic reconstruction of nearby-object enumeration, preserving coordinate/range filtering, object ordering, and no-match behavior.

Validation: `tests/get_next_obj_here_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/GNSTA.TXT` and `runs/vmctl-vc152ev/GNRES.TXT`.
