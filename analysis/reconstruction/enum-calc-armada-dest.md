# FEnumCalcArmadaDest reconstruction

`reconstructed/src/enum_calc_armada_dest.c` is a typed semantic reconstruction of armada-destination enumeration, preserving candidate filtering, range/visibility gates, and callback results.

Validation: `tests/enum_calc_armada_dest_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/EASTA.TXT` and `runs/vmctl-vc152ew/EARES.TXT`.
