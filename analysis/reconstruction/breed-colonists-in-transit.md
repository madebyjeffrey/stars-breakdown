# BreedColonistsInTransit reconstruction

`reconstructed/src/breed_colonists_in_transit.c` is a typed semantic reconstruction of in-transit colonist growth, preserving growth limits, population updates, and transit-object state changes.

Validation: `tests/breed_colonists_in_transit_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/BTSTA.TXT` and `runs/vmctl-vc152ew/BTRES.TXT`.
