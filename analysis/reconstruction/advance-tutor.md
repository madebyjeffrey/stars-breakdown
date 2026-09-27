# AdvanceTutor reconstruction

`reconstructed/src/advance_tutor.c` is a typed semantic reconstruction of tutorial progression, preserving step bounds, prerequisite checks, state advancement, and notification callbacks.

Validation: `tests/advance_tutor_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/ATSTA.TXT` and `runs/vmctl-vc152ev/ATRES.TXT`.
