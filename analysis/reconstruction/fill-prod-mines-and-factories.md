# FFillProdMinesAndFactories reconstruction

`reconstructed/src/fill_prod_mines_and_factories.c` is a typed semantic reconstruction of production-site filling. It accounts for factories and mines separately, applies population and mineral constraints, and routes the resulting production-state changes through explicit callbacks.

Validation: `tests/fill_prod_mines_and_factories_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/FPMSTA.TXT` and `runs/vmctl-vc152ew/FPMRES.TXT`.
