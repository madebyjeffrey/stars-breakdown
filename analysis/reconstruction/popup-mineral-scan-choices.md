# PopupMineralScanChoices reconstruction

`reconstructed/src/popup_mineral_scan_choices.c` is a typed semantic reconstruction of the mineral-scan choice popup, preserving option construction, selection mapping, cancellation, and callback dispatch.

Validation: `tests/popup_mineral_scan_choices_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/PMSSTA2.TXT` and `runs/vmctl-vc152ew/PMSRES.TXT`.
