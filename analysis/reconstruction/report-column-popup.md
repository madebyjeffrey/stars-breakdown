# ReportColumnPopup reconstruction

`reconstructed/src/report_column_popup.c` is a typed semantic reconstruction of report-column popup assembly, sorting, visibility toggles, special-column mapping, cancellation, and invalidation.

Validation: `tests/report_column_popup_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/PCPSTA.TXT` and `runs/vmctl-vc152ew/PCPRES.TXT`.
