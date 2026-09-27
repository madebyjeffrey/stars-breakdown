# DrawShipPlanet reconstruction

`reconstructed/src/draw_ship_planet.c` is a typed semantic reconstruction of ship-on-planet drawing, preserving ship/planet selection, labels, coordinates, and drawing callbacks.

Validation: `tests/draw_ship_planet_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DSPSTA2.TXT` and `runs/vmctl-vc152ew/DSPRES.TXT`.
