# FEnumPktAttack reconstruction

`reconstructed/src/f_enum_pkt_attack.c` is a typed semantic reconstruction of packet-attack enumeration, preserving attack eligibility, target matching, and callback enumeration results.

Validation: `tests/f_enum_pkt_attack_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/EPASTA3.TXT` and `runs/vmctl-vc152ew/EPARES.TXT`.
