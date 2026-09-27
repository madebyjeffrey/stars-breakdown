# FinishProduction reconstruction

`reconstructed/src/finish_production.c` is a typed semantic reconstruction of production completion, preserving queue advancement, resource accounting, item creation, and production-state updates.

Validation: `tests/finish_production_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/FPSTA.TXT` and `runs/vmctl-vc152ev/FPRES.TXT`.
