# InitFromHuldef reconstruction

`reconstructed/src/init_from_huldef.c` is a typed semantic reconstruction of ship initialization from hull definition, preserving component fields, derived statistics, cargo/fuel state, and defaults.

Validation: `tests/init_from_huldef_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/HSTA.TXT` and `runs/vmctl-vc152ew/HRES.TXT`.
