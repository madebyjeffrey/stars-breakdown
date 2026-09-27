# FillShipDD reconstruction

`reconstructed/src/fill_ship_dd.c` is a typed semantic reconstruction of ship-design dropdown filling, preserving item enumeration, selection mapping, and control refresh behavior.

Validation: `tests/fill_ship_dd_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SDSTA.TXT` and `runs/vmctl-vc152ew/SDRES.TXT`.
