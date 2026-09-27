# DxReportColHdr reconstruction

`reconstructed/src/report_column_header.c` is a typed semantic reconstruction of report-column header drawing, preserving column labels, widths, alignment, and drawing callbacks.

Validation: `tests/report_column_header_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/RCHSTA.TXT` and `runs/vmctl-vc152ew/RCHRES.TXT`.
