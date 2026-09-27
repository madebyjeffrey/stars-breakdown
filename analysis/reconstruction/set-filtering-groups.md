# SetFilteringGroups reconstruction

`reconstructed/src/set_filtering_groups.c` is a typed semantic reconstruction of scanner filtering-group setup. It preserves group membership, bitmask updates, control synchronization, and boundary behavior through explicit state and callbacks.

Validation: `tests/set_filtering_groups_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SFGSTA.TXT` and `runs/vmctl-vc152ew/SFGRES.TXT`.
