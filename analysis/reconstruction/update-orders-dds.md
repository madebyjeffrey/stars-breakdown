# UpdateOrdersDDs reconstruction

`reconstructed/src/update_orders_dds.c` is a typed semantic reconstruction of ship-order dropdown synchronization, preserving order availability, selection, enablement, and refresh behavior.

Validation: `tests/update_orders_dds_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/UODSTA2.TXT` and `runs/vmctl-vc152ew/UODRES.TXT`.
