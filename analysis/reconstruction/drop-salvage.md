# DropSalvage reconstruction

`reconstructed/src/drop_salvage.c` is a typed semantic reconstruction of salvage creation and merging, preserving duplicate suppression, fallback cargo, scaled three-thousand-unit chunking, map marking, and remainder handling.

Validation: `tests/drop_salvage_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152f/SLVSTA.TXT` and `runs/vmctl-vc152f/SLVRES.TXT`.
