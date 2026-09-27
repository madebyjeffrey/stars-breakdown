# IdGetBestScannerDest reconstruction

`reconstructed/src/id_get_best_scanner_dest.c` is a typed semantic reconstruction of scanner-destination selection, preserving candidate filtering, distance/desirability comparison, deterministic tie handling, and no-destination behavior.

Validation: `tests/id_get_best_scanner_dest_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SGDSTA.TXT` and `runs/vmctl-vc152ew/SGDRES.TXT`.
