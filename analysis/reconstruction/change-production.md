# ChangeProduction reconstruction

`reconstructed/src/change_production.c` is a typed semantic reconstruction of production selection changes, preserving item validation, queue replacement, resource/cost updates, and control refresh.

Validation: `tests/change_production_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/CPSTA.TXT` and `runs/vmctl-vc152ev/CPRES.TXT`.
