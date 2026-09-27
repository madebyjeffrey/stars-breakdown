# FDumpCargo reconstruction

`reconstructed/src/dump_cargo.c` is a typed semantic reconstruction of cargo dumping, preserving cargo-category iteration, quantity limits, object creation/transfer, and resulting fleet state.

Validation: `tests/dump_cargo_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DCSTA.TXT` and `runs/vmctl-vc152ew/DCRES.TXT`.
