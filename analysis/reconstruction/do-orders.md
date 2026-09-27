# DoOrders reconstruction

`reconstructed/src/do_orders.c` is a typed semantic reconstruction of fleet-order execution, preserving order dispatch, waypoint/routing updates, and resulting fleet state.

Validation: `tests/do_orders_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DOSTA.TXT` and `runs/vmctl-vc152ew/DORES.TXT`.
