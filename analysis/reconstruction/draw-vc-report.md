# DrawVCReport reconstruction

`reconstructed/src/draw_vc_report.c` is a typed semantic reconstruction of the victory-condition report renderer, preserving report sections, labels, player values, geometry, and drawing callbacks.

Validation: `tests/draw_vc_report_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DVCSTA.TXT` and `runs/vmctl-vc152ew/DVCRES.TXT`.
