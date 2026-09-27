# FEnumCalcArmadaHumanDest reconstruction

`reconstructed/src/enum_calc_armada_human_dest.c` is a typed semantic reconstruction of human-armada destination enumeration, preserving candidate filtering, distance/range gates, and callback results.

Validation: `tests/enum_calc_armada_human_dest_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/EHPSTA.TXT` and `runs/vmctl-vc152ew/EHPRES.TXT`.
