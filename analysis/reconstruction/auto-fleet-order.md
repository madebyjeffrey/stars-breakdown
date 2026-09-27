# AutoFleetOrder reconstruction

`reconstructed/src/auto_fleet_order.c` is a typed semantic reconstruction of automatic fleet-order assignment, preserving route selection, order eligibility, and resulting waypoint/order updates.

Validation: `tests/auto_fleet_order_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/ASTA.TXT` and `runs/vmctl-vc152ew/ARES.TXT`.
