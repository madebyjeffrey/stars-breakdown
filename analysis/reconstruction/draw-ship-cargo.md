# DrawShipCargo reconstruction

`reconstructed/src/draw_ship_cargo.c` is a typed semantic reconstruction of ship-cargo display, preserving cargo categories, quantities, labels, capacity formatting, and drawing callbacks.

Validation: `tests/draw_ship_cargo_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DSCSTA.TXT` and `runs/vmctl-vc152ew/DSCRES.TXT`.
