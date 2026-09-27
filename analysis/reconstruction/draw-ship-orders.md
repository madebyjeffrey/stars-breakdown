# DrawShipOrders reconstruction

`reconstructed/src/draw_ship_orders.c` is a typed semantic reconstruction of the ship-orders panel. It preserves order filtering, row geometry, labels, selected-state presentation, and the empty/disabled display path through drawing callbacks.

Validation: `tests/draw_ship_orders_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DSOSTA2.TXT` and `runs/vmctl-vc152ew/DSORES.TXT`.
