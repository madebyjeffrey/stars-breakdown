# CreateSalvage reconstruction

`reconstructed/src/create_salvage.c` is a typed semantic reconstruction of salvage-object creation, preserving object allocation, cargo/type fields, map placement, and linked-list updates.

Validation: `tests/create_salvage_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/CSSTA.TXT` and `runs/vmctl-vc152ew/CSRES.TXT`.
