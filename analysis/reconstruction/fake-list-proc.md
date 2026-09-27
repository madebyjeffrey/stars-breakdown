# FakeListProc reconstruction

`reconstructed/src/fake_list_proc.c` is a typed semantic reconstruction of list-control dispatch, preserving selection, item data, notification, and default-message behavior.

Validation: `tests/fake_list_proc_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/FLCSTA.TXT` and `runs/vmctl-vc152ev/FLCRES.TXT`.
