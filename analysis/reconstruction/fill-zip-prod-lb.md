# FillZipProdLB reconstruction

`reconstructed/src/fill_zip_prod_lb.c` is a typed semantic reconstruction of production-list filling, preserving item ordering, labels, costs, capacity, selection, and control refresh.

Validation: `tests/fill_zip_prod_lb_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/FZPSTA.TXT` and `runs/vmctl-vc152ew/FZPRES.TXT`.
