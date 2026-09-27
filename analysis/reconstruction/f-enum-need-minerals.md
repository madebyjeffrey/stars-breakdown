# FEnumNeedMinerals reconstruction

`reconstructed/src/f_enum_need_minerals.c` is a typed semantic reconstruction of mineral-need enumeration, preserving candidate filtering, need classification, and callback enumeration behavior.

Validation: `tests/f_enum_need_minerals_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/ENMSTA.TXT` and `runs/vmctl-vc152ew/ENMRES.TXT`.
