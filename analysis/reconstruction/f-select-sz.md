# FSelectSz reconstruction

`reconstructed/src/f_select_sz.c` is a typed semantic reconstruction of size-selection filtering, preserving bounds, selection state, and the accepted/rejected result paths.

Validation: `tests/f_select_sz_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/FSSTA.TXT` and `runs/vmctl-vc152ev/FSRES.TXT`.
